/*
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
 *
 * This file is part of Edje Theme Editor.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; If not, see www.gnu.org/licenses/lgpl.html.
 */

#include "animator.h"
#include "animator_private.h"
#include "sound_editor.h"

static const Edje_Action_Type combobox_action[] =
{
   EDJE_ACTION_TYPE_NONE,
   EDJE_ACTION_TYPE_STATE_SET,
   EDJE_ACTION_TYPE_ACTION_STOP,
   EDJE_ACTION_TYPE_SIGNAL_EMIT,
   EDJE_ACTION_TYPE_DRAG_VAL_SET,
   EDJE_ACTION_TYPE_DRAG_VAL_STEP,
   EDJE_ACTION_TYPE_DRAG_VAL_PAGE,
   EDJE_ACTION_TYPE_SCRIPT,
   EDJE_ACTION_TYPE_FOCUS_SET,
   EDJE_ACTION_TYPE_FOCUS_OBJECT,
   EDJE_ACTION_TYPE_SOUND_SAMPLE,
   EDJE_ACTION_TYPE_SOUND_TONE,

   EDJE_ACTION_TYPE_LAST
};

typedef struct _Program_Editor Program_Editor;
struct _Program_Editor
{
   Evas_Object *editor_layout;
   Evas_Object *parent;
   Live_View *live;
   Evas_Object *slider;
   struct {
      Style *style;
      const char *program;
      Evas_Object *name;
      Evas_Object *signal;
      Evas_Object *source;
      struct {
         Evas_Object *item;
         Evas_Object *item_from, *entry_from;
         Evas_Object *item_range, *entry_range;
      } in;
      struct {
         Evas_Object *item;
         Evas_Object *box;
         Evas_Object *combobox;
         Evas_Object *layout1, *entry1, *btn1;
         Evas_Object *layout2, *entry2;
         Evas_Object *layout3, *combobox3;
      } action;
      Evas_Object *script;
      struct {
         Evas_Object *item;
         Evas_Object *combobox;
         Evas_Object *layout0, *checkbox;
         Evas_Object *layout1, *entry1;
         Evas_Object *layout2, *entry2;
         Evas_Object *layout3, *entry3;
      } transition;
      Evas_Object *targets;
      Evas_Object *afters;
      Edje_Action_Type act_type;
      Edje_Tween_Mode trans_type;
      Evas_Object *prop_scroller;
      Evas_Object *prop_box;
   } prop_view;
   struct {
      Ecore_Timer *timer;
      double last_callback_time;
      double program_time;
      double start_delay;
      double total_time;
      Eina_Bool is_played : 1;
      Eina_Bool is_paused : 1;
      Eina_Bool is_in_seek : 1;
      Eina_Bool is_cycled : 1;
   } playback;
};

static const char *transition_type[] = {
                                    N_("NONE"),
                                    N_("LINEAR"),
                                    N_("SINUSOIDAL"),
                                    N_("ACCELERATE"),
                                    N_("DECELERATE"),
                                    N_("ACCELERATE FACTOR"),
                                    N_("DECELERATE FACTOR"),
                                    N_("SINUSOIDAL FACTOR"),
                                    N_("DIVISOR INTERP"),
                                    N_("BOUNCE"),
                                    N_("SPRING")};


static const char *action_type[] = {N_("NONE"),
                                    N_("STATE_SET"),
                                    N_("ACTION_STOP"),
                                    N_("SIGNAL_EMIT"),
                                    N_("DRAG_VAL_SET"),
                                    N_("DRAG_VAL_STEP"),
                                    N_("DRAG_VAL_PAGE"),
                                    N_("SCRIPT"),
                                    N_("FOCUS_SET"),
                                    N_("[NOT SUPPORTED] RESERVED00"),
                                    N_("FOCUS_OBJECT"),
                                    N_("[NOT SUPPORTED] PARAM_COPY"),
                                    N_("[NOT SUPPORTED] PARAM_SET"),
                                    N_("SOUND_SAMPLE"),
                                    N_("SOUND_TONE"),
                                    N_("[NOT SUPPORTED] PHYSICS_IMPULSE"),
                                    N_("[NOT SUPPORTED] PHYSICS_TORQUE_IMPULSE"),
                                    N_("[NOT SUPPORTED] PHYSICS_FORCE"),
                                    N_("[NOT SUPPORTED] PHYSICS_TORQUE"),
                                    N_("[NOT SUPPORTED] PHYSICS_FORCES_CLEAR"),
                                    N_("[NOT SUPPORTED] PHYSICS_VEL_SET"),
                                    N_("[NOT SUPPORTED] PHYSICS_ANG_VEL_SET"),
                                    N_("[NOT SUPPORTED] PHYSICS_STOP"),
                                    N_("[NOT SUPPORTED] PHYSICS_ROT_SET"),
                                    N_("[NOT SUPPORTED] VIBRATION_SAMPLE")};


static const char *sound_channel[] = {N_("EFFECT"),
                                      N_("BACKGROUND"),
                                      N_("MUSIC"),
                                      N_("FOREGROUND"),
                                      N_("INTERFACE"),
                                      N_("INPUT"),
                                      N_("ALERT"),
                                      N_("ALL")};

static void
_prop_item_program_targets_update(Program_Editor *prog_edit);
static Evas_Object *
_prop_item_program_target_add(Evas_Object *parent, Program_Editor *prog_edit, const char *tooltip);

#define CALLBACK_KEY "callback.key"
#define DATA_KEY "prog_edit_data"
#define LITTLE_TIME 0.0005

#define prop prog_edit->prop_view
#define action prog_edit->prop_view.action
#define transition prog_edit->prop_view.transition

/********************* playback ***********************************************/
static void
_object_state_reset(Program_Editor *prog_edit)
{
   Part *part;

   edje_edit_program_stop_all(prog_edit->live->object);

   EINA_INLIST_FOREACH(prop.style->parts, part)
     edje_edit_part_selected_state_set(prog_edit->live->object,
                                       part->name,
                                       part->curr_state,
                                       part->curr_state_value);
}

static Eina_Bool
_timer_cb(void *data)
{
   Program_Editor *prog_edit = data;

   double pos;
   double time = ecore_loop_time_get();
   double transition_time = edje_edit_program_transition_time_get(prop.style->obj,
                                                                  prop.program);
   double delta = time - prog_edit->playback.last_callback_time;
   prog_edit->playback.last_callback_time = time;
   if (prog_edit->playback.is_in_seek)
     return prog_edit->playback.is_played;

   prog_edit->playback.program_time += delta;
   if (prog_edit->playback.program_time > prog_edit->playback.total_time)
     {
        if (!prog_edit->playback.is_cycled)
          {
             prog_edit->playback.is_played = false;
             /* timer will be deleted after returning false so pointer is invalid */
             prog_edit->playback.timer = NULL;
             evas_object_smart_callback_call(prog_edit->editor_layout, PAUSE_CB, NULL);
             prog_edit->playback.program_time = prog_edit->playback.total_time;
          }
        else
          {
             prog_edit->playback.program_time = 0;
             _object_state_reset(prog_edit);
          }
     }
   pos = (prog_edit->playback.program_time - prog_edit->playback.start_delay) /
      (prog_edit->playback.total_time - prog_edit->playback.start_delay);
   if (transition_time == 0) pos = pos > 0;
   elm_slider_value_set(prog_edit->slider, prog_edit->playback.program_time);

   edje_edit_program_transition_state_set(prog_edit->live->object, prop.program, pos);

   return prog_edit->playback.is_played;
}

static void
_program_reset(Program_Editor *prog_edit)
{
   _object_state_reset(prog_edit);
   prog_edit->playback.program_time = 0;
   elm_slider_value_set(prog_edit->slider, 0);
   prog_edit->playback.is_played = false;
   prog_edit->playback.is_paused = false;
   if (prog_edit->playback.timer)
     ecore_timer_del(prog_edit->playback.timer);
   prog_edit->playback.timer = NULL;
   prog_edit->playback.start_delay = edje_edit_program_in_from_get(prop.style->obj, prop.program);
   double transition_time = edje_edit_program_transition_time_get(prop.style->obj, prop.program);
   prog_edit->playback.total_time = prog_edit->playback.start_delay + transition_time;
   prog_edit->playback.last_callback_time = ecore_loop_time_get();
   if (transition_time == 0)
     elm_slider_min_max_set(prog_edit->slider, 0.0, prog_edit->playback.total_time + LITTLE_TIME);
   else
     elm_slider_min_max_set(prog_edit->slider, 0.0, prog_edit->playback.total_time);
   evas_object_smart_callback_call(prog_edit->editor_layout, PAUSE_CB, NULL);
}

void
program_editor_program_reset(Evas_Object *obj)
{
   Program_Editor *prog_edit = evas_object_data_get(obj, DATA_KEY);

   _program_reset(prog_edit);
}

void
program_editor_program_stop(Evas_Object *obj)
{
   Program_Editor *prog_edit = evas_object_data_get(obj, DATA_KEY);

   if (prog_edit->playback.timer)
     ecore_timer_del(prog_edit->playback.timer);
   prog_edit->playback.timer = NULL;
}

void
program_editor_program_play(Evas_Object *obj)
{
   Program_Editor *prog_edit = evas_object_data_get(obj, DATA_KEY);

   if (!prog_edit->playback.is_played)
     {
        if (!prog_edit->playback.is_paused)
          _program_reset(prog_edit);
        prog_edit->playback.is_played = true;
        prog_edit->playback.is_paused = false;
        if (!prog_edit->playback.timer)
          prog_edit->playback.timer = ecore_timer_add(1.0/60.0, _timer_cb, prog_edit);
        evas_object_smart_callback_call(prog_edit->editor_layout, PLAY_CB, NULL);
     }
   else
     {
        if (prog_edit->playback.timer)
          ecore_timer_del(prog_edit->playback.timer);
        prog_edit->playback.timer = NULL;
        prog_edit->playback.is_played = false;
        prog_edit->playback.is_paused = true;
        evas_object_smart_callback_call(prog_edit->editor_layout, PAUSE_CB, NULL);
     }
}

void
program_editor_cycled_set(Evas_Object *obj, Eina_Bool cycled)
{
   Program_Editor *prog_edit = evas_object_data_get(obj, DATA_KEY);

   prog_edit->playback.is_cycled = cycled;
}

static void
_slider_seek_start_cb(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   Program_Editor *prog_edit = data;
   prog_edit->playback.is_in_seek = true;
}

static void
_slider_seek_stop_cb(void *data,
                     Evas_Object *obj __UNUSED__,
                     void *event_info __UNUSED__)
{
   Program_Editor *prog_edit = data;
   prog_edit->playback.is_in_seek = false;
}

static void
_slider_changed_cb(void *data,
                   Evas_Object *obj,
                   void *event_info __UNUSED__)
{
   double pos, transition_time;
   Program_Editor *prog_edit = data;

   prog_edit->playback.program_time = elm_slider_value_get(obj);
   transition_time = prog_edit->playback.total_time - prog_edit->playback.start_delay;
   if (transition_time == 0)
     pos = prog_edit->playback.program_time > prog_edit->playback.start_delay;
   else
     pos = (prog_edit->playback.program_time - prog_edit->playback.start_delay) / transition_time;
   edje_edit_program_transition_state_set(prog_edit->live->object, prop.program, pos);
}

/********************* editor *************************************************/
#define ITEM_ADD_(parent, item, text, style) \
   item = elm_layout_add(parent); \
   evas_object_size_hint_weight_set(item, EVAS_HINT_EXPAND, 0.0); \
   evas_object_size_hint_align_set(item, EVAS_HINT_FILL, 0.0); \
   elm_layout_file_set(item, EFLETE_EDJ, "property/item/"style); \
   elm_object_part_text_set(item, "elm.text", text); \
   evas_object_show(item);


#define ITEM_STRING_CALLBACK(sub, value) \
static void \
_on_##sub##_##value##_change(void *data, \
                              Evas_Object *obj, \
                              void *ei __UNUSED__) \
{ \
   Program_Editor *prog_edit = (Program_Editor *)data; \
   const char *value = elm_entry_entry_get(obj); \
   Eina_Bool res = edje_edit_##sub##_##value##_set(prop.style->obj, \
                                                   prop.program, \
                                                   value); \
   if (!res) \
     { \
        NOTIFY_WARNING(_("The entered data is not valid!")) \
        return; \
     } \
}

#define ITEM_1ENTRY_ADD(text, sub, value, regex) \
static Evas_Object * \
_prop_item_##sub##_##value##_add(Evas_Object *parent, \
                                 const char *tooltip, \
                                 Evas_Smart_Cb callback, \
                                 void* cb_data) \
{ \
   Evas_Object *item, *entry; \
   ITEM_ADD_(parent, item, text, "editor") \
   EWE_ENTRY_ADD(parent, entry, true, DEFAULT_STYLE) \
   REGEX_SET(entry, regex); \
   elm_object_tooltip_text_set(entry, tooltip); \
   evas_object_smart_callback_add(entry, "changed,user", callback, cb_data); \
   elm_object_part_content_set(item, "elm.swallow.content", entry); \
   return item; \
}

#define ITEM_1ENTRY_UPDATE(sub, value) \
static void \
_prop_item_##sub##_##value##_update(Evas_Object *item, Program_Editor *prog_edit) \
{ \
   Evas_Object *entry; \
   const char *value; \
   entry = elm_object_part_content_get(item, "elm.swallow.content"); \
   value = edje_edit_##sub##_##value##_get(prop.style->obj, prop.program); \
   ewe_entry_entry_set(entry, value); \
}

#define ITEM_1ENTRY_PROG_CREATE(text, sub, value, regex) \
   ITEM_STRING_CALLBACK(sub, value) \
   ITEM_1ENTRY_ADD(text, sub, value, regex) \
   ITEM_1ENTRY_UPDATE(sub, value)

#define REGEX_SET(entry, regex) \
   ewe_entry_regex_set(entry, regex, EWE_REG_EXTENDED); \
   ewe_entry_regex_autocheck_set(entry, true); \
   ewe_entry_regex_glow_set(entry, true);

static void _on_v1_active(void *data, Evas_Object *obj, void *ei);
static void _on_v2_active(void *data, Evas_Object *obj, void *ei);
static void _on_state_active(void *data, Evas_Object *obj, void *ei);
static void _on_value_active(void *data, Evas_Object *obj, void *ei);
static void _on_value2_active(void *data, Evas_Object *obj, void *ei);
static void _prop_progs_update(Program_Editor *prog_edit);

ITEM_1ENTRY_PROG_CREATE(_("signal"), program, signal, EDJE_NAME_REGEX)
ITEM_1ENTRY_PROG_CREATE(_("source"), program, source, EDJE_NAME_REGEX)
ITEM_1ENTRY_ADD(_("name"), program, name, EDJE_NAME_REGEX)

static int
_sort_cb(const void *d1, const void *d2)
{
   const char *txt = d1;
   const char *txt2 = d2;

   if (!txt) return(1);
   if (!txt2) return(-1);

   return(strcmp(txt, txt2));
}

static void
_special_properties_hide(Program_Editor *prog_edit)
{
   if (prop.script)
     {
        elm_box_unpack(prop.prop_box, prop.script);
        evas_object_del(prop.script);
        prop.script = NULL;
     }
   if (prop.targets)
     {
        elm_box_unpack(prop.prop_box, prop.targets);
        evas_object_del(prop.targets);
        prop.targets = NULL;
     }
   if (action.btn1)
     {
        evas_object_del(action.btn1);
        action.btn1 = NULL;
     }
   if (action.layout3)
     {
        evas_object_del(action.layout3);
        action.layout3 = NULL;
     }
}

static void
_on_program_name_change(void *data,
                        Evas_Object *obj,
                        void *ei __UNUSED__)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   const char *value = elm_entry_entry_get(obj);
   Eina_Bool res = edje_edit_program_name_set(prop.style->obj, prop.program,
                                              value);
   if (!res)
     {
        NOTIFY_WARNING(_("The entered data is not valid!"))
           return;
     }
   evas_object_smart_callback_call(prog_edit->editor_layout,
                                   NAME_CHANGED_CB,
                                   (void *) value);
   prop.program = value;
}

static void
_on_in_from_change(void *data,
                   Evas_Object *obj,
                   void *ei __UNUSED__)
{
   Eina_Bool was_playing;
   Program_Editor *prog_edit = (Program_Editor*)data;
   const char *value = elm_entry_entry_get(obj);
   Eina_Bool res = edje_edit_program_in_from_set(prop.style->obj, prop.program,
                                                 atof(value));
   if (!res)
     {
        NOTIFY_WARNING(_("The entered data is not valid!"));
        return;
     }

   was_playing = prog_edit->playback.is_played;
   _program_reset(prog_edit);
   if (was_playing) program_editor_program_play(prog_edit->editor_layout);
}

static void
_on_in_range_change(void *data,
                    Evas_Object *obj,
                    void *ei __UNUSED__)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   const char *value = elm_entry_entry_get(obj);
   Eina_Bool res = edje_edit_program_in_range_set(prop.style->obj, prop.program,
                                                  atof(value));
   if (!res)
     {
        NOTIFY_WARNING(_("The entered data is not valid!"));
        return;
     }
}

static void
_on_target_name_change(void *data,
                       Evas_Object *obj,
                       void *ei)
{
   const char *value;
   const char *old_value;
   Evas_Object *del_button;
   Program_Editor *prog_edit = (Program_Editor *) data;
   Ewe_Combobox_Item *item = ei;

   value = ewe_combobox_item_title_get(obj, item->index);
   old_value = evas_object_data_get(obj, TARGET_NAME_KEY);

   edje_edit_program_target_del(prop.style->obj, prop.program, old_value);
   edje_edit_program_target_add(prop.style->obj, prop.program, value);

   del_button = evas_object_data_get(obj, DEL_BUTTON_KEY);
   evas_object_data_set(del_button, TARGET_NAME_KEY, value);
   evas_object_data_set(obj, TARGET_NAME_KEY, value);
}

static Evas_Object *
_prop_item_program_script_add(Evas_Object *parent,
                              const char *tooltip __UNUSED__)
{
   Evas_Object *item = NULL;
   Evas_Object *box = NULL;
   Evas_Object *entry = NULL;

   if (!parent) return NULL;

   ITEM_ADD_(parent, item, _("script"), "script");

   BOX_ADD(item, box, true, false);
   EWE_ENTRY_ADD(item, entry, false, DEFAULT_STYLE);
   elm_scroller_policy_set(entry, ELM_SCROLLER_POLICY_OFF,
                           ELM_SCROLLER_POLICY_AUTO);
   elm_entry_editable_set(entry, false);
   elm_box_pack_end(box, entry);
   elm_object_part_content_set(item, "elm.swallow.content", box);
   return item;
}

static void
_prop_item_program_script_update(Program_Editor *prog_edit)
{
   Evas_Object *box = NULL;
   char *script = NULL;
   Evas_Object *entry = NULL;

   box = elm_object_part_content_get(prop.script, "elm.swallow.content");
   Eina_List *childs = elm_box_children_get(box);

   script = edje_edit_script_program_get(prop.style->obj, prop.program);
   entry = eina_list_nth(childs, 0);
   script = elm_entry_utf8_to_markup(script);
   ewe_entry_entry_set(entry, script);
   free(script);
   eina_list_free(childs);
}

#define ENTRY_UPDATE(ENTRY, IS_DISABLED, LAYOUT, TEXT) \
   elm_entry_editable_set(ENTRY, true); \
   elm_object_disabled_set(ENTRY, IS_DISABLED); \
   if (TEXT) elm_object_part_text_set(LAYOUT, "elm.text", TEXT);

#define TRANS_ENTRIES_DEFAULT_SET(IS_DISABLED) \
   elm_object_disabled_set(transition.checkbox, IS_DISABLED); \
   elm_object_disabled_set(transition.entry1, IS_DISABLED); \
   elm_object_disabled_set(transition.entry2, IS_DISABLED); \
   elm_object_disabled_set(transition.entry3, IS_DISABLED); \
   elm_check_state_set(transition.checkbox, false); \
   elm_object_part_text_set(transition.layout2, "elm.text", _("param1")); \
   elm_object_part_text_set(transition.layout3, "elm.text", _("param2")); \
   ewe_entry_entry_set(transition.entry1, ""); \
   ewe_entry_entry_set(transition.entry2, ""); \
   ewe_entry_entry_set(transition.entry3, "");

#define TRANS_VAL_UPDATE(_val_, _entry) \
   value = edje_edit_program_transition_##_val_##_get(prop.style->obj, \
                                                      prop.program); \
   buff = eina_stringshare_printf("%1.2f", value); \
   ewe_entry_entry_set(_entry, buff); \
   eina_stringshare_del(buff);

#define CALLBACK_UPDATE(_activated_cb, _entry) \
        evas_object_smart_callback_del(_entry, "changed,user", \
                                       evas_object_data_get(_entry, CALLBACK_KEY)); \
        evas_object_data_set(_entry, CALLBACK_KEY, _activated_cb); \
        evas_object_smart_callback_add(_entry, "changed,user", _activated_cb, \
                                       prog_edit);

static void
_trans_entries_set(Program_Editor *prog_edit)
{
   Eina_Stringshare *buff = NULL;
   double value;

   switch (prop.trans_type & EDJE_TWEEN_MODE_MASK)
     {

      case EDJE_TWEEN_MODE_LINEAR:
      case EDJE_TWEEN_MODE_SINUSOIDAL:
      case EDJE_TWEEN_MODE_ACCELERATE:
      case EDJE_TWEEN_MODE_DECELERATE:
        {
           ENTRY_UPDATE(transition.entry2, true, transition.layout2, NULL);
           ENTRY_UPDATE(transition.entry3, true, transition.layout3, NULL);
           TRANS_VAL_UPDATE(time, transition.entry1);
           break;
        }
      case EDJE_TWEEN_MODE_ACCELERATE_FACTOR:
      case EDJE_TWEEN_MODE_DECELERATE_FACTOR:
      case EDJE_TWEEN_MODE_SINUSOIDAL_FACTOR:
        {
           ENTRY_UPDATE(transition.entry2, false, transition.layout2, _("factor"));
           ENTRY_UPDATE(transition.entry3, true, transition.layout3, NULL);
           CALLBACK_UPDATE(_on_v1_active, transition.entry2);
           TRANS_VAL_UPDATE(time, transition.entry1);
           TRANS_VAL_UPDATE(value1, transition.entry2);
           break;
        }
      case EDJE_TWEEN_MODE_DIVISOR_INTERP:
        {
           ENTRY_UPDATE(transition.entry2, false, transition.layout2, _("gradient"));
           ENTRY_UPDATE(transition.entry3, false, transition.layout3, _("factor"));
           CALLBACK_UPDATE(_on_v1_active, transition.entry2);
           CALLBACK_UPDATE(_on_v2_active, transition.entry3);
           TRANS_VAL_UPDATE(time, transition.entry1);
           TRANS_VAL_UPDATE(value1, transition.entry2);
           TRANS_VAL_UPDATE(value2, transition.entry3);
           break;
        }
      case EDJE_TWEEN_MODE_BOUNCE:
        {
           ENTRY_UPDATE(transition.entry2, false, transition.layout2, _("decay"));
           ENTRY_UPDATE(transition.entry3, false, transition.layout3, _("bounces"));
           CALLBACK_UPDATE(_on_v1_active, transition.entry2);
           CALLBACK_UPDATE(_on_v2_active, transition.entry3);
           TRANS_VAL_UPDATE(time, transition.entry1);
           TRANS_VAL_UPDATE(value1, transition.entry2);
           TRANS_VAL_UPDATE(value2, transition.entry3);
           break;
        }
      case EDJE_TWEEN_MODE_SPRING:
        {
           ENTRY_UPDATE(transition.entry2, false, transition.layout2, _("decay"));
           ENTRY_UPDATE(transition.entry3, false, transition.layout3, _("swings"));
           CALLBACK_UPDATE(_on_v1_active, transition.entry2);
           CALLBACK_UPDATE(_on_v2_active, transition.entry3);
           TRANS_VAL_UPDATE(time, transition.entry1);
           TRANS_VAL_UPDATE(value1, transition.entry2);
           TRANS_VAL_UPDATE(value2, transition.entry3);
           break;
        }
      case EDJE_TWEEN_MODE_CUBIC_BEZIER: // TODO: implement
      case EDJE_TWEEN_MODE_NONE:
      default:
        {
           TRANS_ENTRIES_DEFAULT_SET(true);
        }
     }
}

#define ACTION_STATE_GET(_state_get, _entry) \
         str = _state_get(prop.style->obj, prop.program); \
         ewe_entry_entry_set(_entry, str); \
         edje_edit_string_free(str);

#define ACTION_VAL_GET(_val_get, _entry) \
         value = _val_get(prop.style->obj, prop.program); \
         buff = eina_stringshare_printf("%1.2f", value); \
         ewe_entry_entry_set(_entry, buff); \
         eina_stringshare_del(buff);

static void
_on_sample_select_done(void *data,
                       Evas_Object *obj __UNUSED__,
                       void *event_info)
{
   Program_Editor *prog_edit = data;
   const char *value;
   const char *selected = (const char *)event_info;

   value = elm_entry_entry_get(action.entry1);
   if ((strcmp(selected, "")) && (strcmp(value, selected)))
   {
      elm_entry_entry_set(action.entry1, selected);
      edje_edit_program_sample_name_set(prop.style->obj, prop.program, selected);
   }
}

static void
_on_sample_select(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *ei __UNUSED__)
{
   Program_Editor *prog_edit = data;
   App_Data *ap = app_data_get();
   Evas_Object *snd_edit;
   const char *selected = elm_entry_entry_get(action.entry1);

   snd_edit = sound_editor_window_add(ap->project, SOUND_EDITOR_SAMPLE_SELECT);
   sound_editor_file_choose(snd_edit, selected);
   evas_object_smart_callback_add(snd_edit, SIG_SOUND_SELECTED, _on_sample_select_done, data);
}

static void
_on_combobox_channel_sel(void *data,
                         Evas_Object *obj __UNUSED__,
                         void *ei)
{
   Program_Editor *prog_edit = (Program_Editor *) data;
   Ewe_Combobox_Item *combitem = ei;

   edje_edit_program_channel_set(prop.style->obj, prop.program,
                                 (Edje_Channel)(combitem->index));
}

static void
_on_tone_select_done(void *data,
                     Evas_Object *obj __UNUSED__,
                     void *event_info)
{
   Program_Editor *prog_edit = data;
   const char *value;
   const char *selected = (const char *)event_info;

   value = elm_entry_entry_get(action.entry1);
   if ((strcmp(selected, "")) && (strcmp(value, selected)))
   {
      elm_entry_entry_set(action.entry1, selected);
      edje_edit_program_tone_name_set(prop.style->obj, prop.program, selected);
   }
}

static void
_on_tone_select(void *data,
                Evas_Object *obj __UNUSED__,
                void *ei __UNUSED__)
{
   Program_Editor *prog_edit = data;
   App_Data *ap = app_data_get();
   Evas_Object *snd_edit;
   const char *selected = elm_entry_entry_get(action.entry1);

   snd_edit = sound_editor_window_add(ap->project, SOUND_EDITOR_TONE_SELECT);
   sound_editor_file_choose(snd_edit, selected);
   evas_object_smart_callback_add(snd_edit, SIG_SOUND_SELECTED, _on_tone_select_done, data);
}

static void
_action_entries_set(Program_Editor *prog_edit, Eina_Bool is_update)
{
   Eina_Stringshare *str = NULL;
   double value;
   Eina_Stringshare *buff = NULL;

   evas_object_smart_callback_del(action.entry1, "clicked", _on_sample_select);
   evas_object_smart_callback_del(action.entry1, "clicked", _on_tone_select);

   switch (prop.act_type)
     {
      case EDJE_ACTION_TYPE_NONE:
        {
           ENTRY_UPDATE(action.entry1, true, action.layout1, NULL);
           ENTRY_UPDATE(action.entry2, true, action.layout2, NULL);
           break;
        }
      case EDJE_ACTION_TYPE_ACTION_STOP:
      case EDJE_ACTION_TYPE_FOCUS_SET:
      case EDJE_ACTION_TYPE_FOCUS_OBJECT:
        {
           prop.targets = _prop_item_program_target_add(prop.prop_box, prog_edit, _("targets"));
           _prop_item_program_targets_update(prog_edit);
           elm_box_pack_after(prop.prop_box, prop.targets, action.item);

           ENTRY_UPDATE(action.entry1, true, action.layout1, NULL);
           ENTRY_UPDATE(action.entry2, true, action.layout2, NULL);
           break;
        }
      case EDJE_ACTION_TYPE_SCRIPT:
        {
           prop.script = _prop_item_program_script_add(prop.prop_box, _("script"));
           _prop_item_program_script_update(prog_edit);
           elm_box_pack_after(prop.prop_box, prop.script, action.item);
           TRANS_ENTRIES_DEFAULT_SET(true);
           break;
        }
      case EDJE_ACTION_TYPE_STATE_SET:
        {
           prop.targets = _prop_item_program_target_add(prop.prop_box, prog_edit, _("targets"));
           _prop_item_program_targets_update(prog_edit);
           elm_box_pack_after(prop.prop_box, prop.targets, action.item);

           ENTRY_UPDATE(action.entry1, false, action.layout1, _("state name"));
           REGEX_SET(action.entry1, EDJE_NAME_REGEX);
           ENTRY_UPDATE(action.entry2, false, action.layout2, _("state value"));
           REGEX_SET(action.entry2, FLOAT_NUMBER_0_1_REGEX);
           CALLBACK_UPDATE(_on_state_active, action.entry1);
           CALLBACK_UPDATE(_on_value_active, action.entry2);
           if (is_update)
             {
                ACTION_STATE_GET(edje_edit_program_state_get, action.entry1);
                ACTION_VAL_GET(edje_edit_program_value_get, action.entry2);
             }
           break;
        }
      case EDJE_ACTION_TYPE_SIGNAL_EMIT:
        {
           prop.targets = _prop_item_program_target_add(prop.prop_box, prog_edit, _("targets"));
           _prop_item_program_targets_update(prog_edit);
           elm_box_pack_after(prop.prop_box, prop.targets, action.item);

           ENTRY_UPDATE(action.entry1, false, action.layout1, _("signal name"));
           REGEX_SET(action.entry1, EDJE_NAME_REGEX);
           ENTRY_UPDATE(action.entry2, false, action.layout2, _("emitter"));
           REGEX_SET(action.entry2, EDJE_NAME_REGEX);
           CALLBACK_UPDATE(_on_state_active, action.entry1);
           CALLBACK_UPDATE(_on_value_active, action.entry2);
           if (is_update)
             {
                ACTION_STATE_GET(edje_edit_program_state_get, action.entry1);
                ACTION_STATE_GET(edje_edit_program_state2_get, action.entry2);
             }
           break;
        }
      case EDJE_ACTION_TYPE_DRAG_VAL_SET:
      case EDJE_ACTION_TYPE_DRAG_VAL_STEP:
      case EDJE_ACTION_TYPE_DRAG_VAL_PAGE:
        {
           prop.targets = _prop_item_program_target_add(prop.prop_box, prog_edit, _("targets"));
           _prop_item_program_targets_update(prog_edit);
           elm_box_pack_after(prop.prop_box, prop.targets, action.item);

           ENTRY_UPDATE(action.entry1, false, action.layout1, _("x"));
           REGEX_SET(action.entry1, FLOAT_NUMBER_REGEX);
           ENTRY_UPDATE(action.entry2, false, action.layout2, _("y"));
           REGEX_SET(action.entry2, FLOAT_NUMBER_REGEX);
           CALLBACK_UPDATE(_on_value_active, action.entry1);
           CALLBACK_UPDATE(_on_value2_active, action.entry2);
           if (is_update)
             {
                ACTION_VAL_GET(edje_edit_program_value_get, action.entry1);
                ACTION_VAL_GET(edje_edit_program_value2_get, action.entry2);
             }
           break;
        }
      case EDJE_ACTION_TYPE_SOUND_SAMPLE:
        {
           int i;
           ENTRY_UPDATE(action.entry1, false, action.layout1, _("sample name"));
           elm_entry_editable_set(action.entry1, false);
           action.btn1 = elm_button_add(action.entry1);
           elm_object_style_set(action.btn1, "eflete/elipsis");
           evas_object_show(action.btn1);
           evas_object_smart_callback_add(action.btn1, "clicked", _on_sample_select, prog_edit);
           evas_object_smart_callback_add(action.entry1, "clicked", _on_sample_select, prog_edit);
           elm_object_part_content_set(action.entry1, "elm.swallow.end", action.btn1);
           elm_object_tooltip_text_set(action.btn1, _("Select sample"));

           ENTRY_UPDATE(action.entry2, false, action.layout2, _("speed"));
           REGEX_SET(action.entry2, FLOAT_NUMBER_REGEX);
           CALLBACK_UPDATE(_on_value2_active, action.entry2);

           ITEM_ADD_(action.box, action.layout3, _("channel"), "editor");
           EWE_COMBOBOX_ADD(action.box, action.combobox3);
           elm_object_part_content_set(action.layout3,
                                       "elm.swallow.content",
                                       action.combobox3);
           elm_box_pack_end(action.box, action.layout3);
           evas_object_smart_callback_add(action.combobox3, "selected",
                                          _on_combobox_channel_sel, prog_edit);

           /* EDJE_CHANNEL_ALL is absent at doc so we skip it */
           for (i = 0; i < EDJE_CHANNEL_ALL; i++)
             ewe_combobox_item_add(action.combobox3, sound_channel[i]);

           if (is_update)
             {
                Eina_Stringshare *sample_name =
                  edje_edit_program_sample_name_get(prop.style->obj, prop.program);
                ewe_entry_entry_set(action.entry1, sample_name);
                eina_stringshare_del(sample_name);

                ACTION_VAL_GET(edje_edit_program_sample_speed_get, action.entry2);

                i = edje_edit_program_channel_get(prop.style->obj, prop.program);
                ewe_combobox_select_item_set(action.combobox3, i);
             }
           break;
        }
      case EDJE_ACTION_TYPE_SOUND_TONE:
        {
           ENTRY_UPDATE(action.entry1, false, action.layout1, _("tone name"));
           elm_entry_editable_set(action.entry1, false);
           action.btn1 = elm_button_add(action.entry1);
           elm_object_style_set(action.btn1, "eflete/elipsis");
           evas_object_show(action.btn1);
           evas_object_smart_callback_add(action.btn1, "clicked", _on_tone_select, prog_edit);
           evas_object_smart_callback_add(action.entry1, "clicked", _on_tone_select, prog_edit);
           elm_object_part_content_set(action.entry1, "elm.swallow.end", action.btn1);
           elm_object_tooltip_text_set(action.btn1, _("Select tone"));

           ENTRY_UPDATE(action.entry2, false, action.layout2, _("duration"));
           REGEX_SET(action.entry2, FLOAT_NUMBER_REGEX);
           CALLBACK_UPDATE(_on_value2_active, action.entry2);

           if (is_update)
             {
                Eina_Stringshare *tone_name =
                  edje_edit_program_tone_name_get(prop.style->obj, prop.program);
                ewe_entry_entry_set(action.entry1, tone_name);
                eina_stringshare_del(tone_name);

                ACTION_VAL_GET(edje_edit_program_tone_duration_get, action.entry2);
             }
           break;
        }
      default:
         break;
     }
}

static void
_on_combobox_trans_sel(void *data,
                       Evas_Object *obj __UNUSED__,
                       void *ei)
{
   Program_Editor *prog_edit = (Program_Editor *) data;
   Ewe_Combobox_Item *combitem = ei;
   ewe_entry_entry_set(transition.entry1, "");
   ewe_entry_entry_set(transition.entry2, "");
   ewe_entry_entry_set(transition.entry3, "");

   edje_edit_program_transition_set(prop.style->obj, prop.program,
                                    (Edje_Tween_Mode)(combitem->index));
   elm_check_state_set(transition.checkbox, false);
   if (prop.act_type != EDJE_ACTION_TYPE_STATE_SET)
     {
        prop.trans_type = EDJE_TWEEN_MODE_NONE;
        NOTIFY_INFO(5, _("Transition block can used only with STATE_SET action"));
        TRANS_ENTRIES_DEFAULT_SET(true);
        ewe_combobox_select_item_set(combitem->owner, 0);
        return;
     }

   TRANS_ENTRIES_DEFAULT_SET(false);
   prop.trans_type = (Edje_Tween_Mode)combitem->index;
   _trans_entries_set(prog_edit);
}

static void
_cancel_cb(void *data,
           Evas_Object *obj __UNUSED__,
           void *ei __UNUSED__)
{
   Eina_Bool *res = data;
   *res = false;
   ecore_main_loop_quit();
}

static void
_continue_cb(void *data,
             Evas_Object *obj __UNUSED__,
             void *ei __UNUSED__)
{
   Eina_Bool *res = data;
   *res = true;
   ecore_main_loop_quit();
}

static void
_on_combobox_action_sel(void *data,
                        Evas_Object *obj __UNUSED__,
                        void *ei)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   Ewe_Combobox_Item *combitem = ei;
   Eina_List *targets_list;
   Eina_Bool result = false;
   Evas_Object *popup, *label, *btn;

   targets_list = edje_edit_program_targets_get(prop.style->obj, prop.program);
   if (targets_list)
     {
        edje_edit_string_list_free(targets_list);

        popup = elm_popup_add(prog_edit->parent);
        elm_object_style_set(popup, "eflete");
        elm_object_part_text_set(popup, "title,text", _("Warning"));
        LABEL_ADD(popup, label, _("This program has targets. If you change action"
                                  "type all target would be deleted"));
        elm_object_content_set(popup, label);
        BUTTON_ADD(popup, btn, _("Continue"));
        evas_object_smart_callback_add(btn, "clicked", _continue_cb, &result);
        elm_object_part_content_set(popup, "button1", btn);
        BUTTON_ADD(popup, btn, _("Cancel"));
        evas_object_smart_callback_add(btn, "clicked", _cancel_cb, &result);
        elm_object_part_content_set(popup, "button2", btn);
        evas_object_show(popup);

        ecore_main_loop_begin();

        evas_object_del(popup);

        if (!result)
          {
             ewe_combobox_select_item_set(action.combobox, (int)prop.act_type);
             return;
          }
     }

   ewe_entry_entry_set(action.entry1, "");
   ewe_entry_entry_set(action.entry2, "");

   edje_edit_program_action_set(prop.style->obj, prop.program,
                                combobox_action[combitem->index]);
   prop.act_type = combobox_action[combitem->index];

   if (prop.act_type != EDJE_ACTION_TYPE_STATE_SET)
     {
        ewe_combobox_select_item_set(transition.combobox, 0);
        ENTRY_UPDATE(transition.entry1, false, transition.layout1, NULL);
        ENTRY_UPDATE(transition.entry2, false, transition.layout2, _("param1"));
        ENTRY_UPDATE(transition.entry3, false, transition.layout3, _("param2"));
        TRANS_ENTRIES_DEFAULT_SET(true);
     }

   _special_properties_hide(prog_edit);
   _action_entries_set(prog_edit, false);
   _prop_progs_update(prog_edit);
}

static void
_on_transition_time_active(void *data,
                           Evas_Object *obj,
                           void *ei __UNUSED__)
{
   Eina_Bool was_playing;
   Program_Editor *prog_edit = (Program_Editor*)data;
   const char *value;
   value = elm_entry_entry_get(obj);
   Eina_Bool res = edje_edit_program_transition_time_set(prop.style->obj,
                                                         prop.program, atof(value));
   if (!res)
     {
        NOTIFY_WARNING(_("The entered data is not valid!"));
        return;
     }

   was_playing = prog_edit->playback.is_played;
   _program_reset(prog_edit);
   if (was_playing) program_editor_program_play(prog_edit->editor_layout);
}

static void
_on_transition_opt_current_changed(void *data,
                                   Evas_Object *obj,
                                   void *ei __UNUSED__)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   Ewe_Combobox_Item *combitem = ewe_combobox_select_item_get(transition.combobox);
   Eina_Bool value;
   Edje_Tween_Mode mode;
   value = elm_check_state_get(obj);
   if (value)
     mode = (Edje_Tween_Mode)(combitem->index) | EDJE_TWEEN_MODE_OPT_FROM_CURRENT;
   else
     mode = (Edje_Tween_Mode)(combitem->index);
   edje_edit_program_transition_set(prop.style->obj, prop.program, mode);
}

static void
_on_state_active(void *data,
                 Evas_Object *obj,
                 void *ei __UNUSED__)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   const char *state;
   state = elm_entry_entry_get(obj);
   if (!edje_edit_program_state_set(prop.style->obj, prop.program, state))
     NOTIFY_WARNING(_("The entered data is not valid!"));
}

static void
_on_value2_active(void *data,
                  Evas_Object *obj,
                  void *ei __UNUSED__)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   const char *value;
   value = elm_entry_entry_get(obj);
   switch (prop.act_type)
     {
      case EDJE_ACTION_TYPE_NONE:
      case EDJE_ACTION_TYPE_ACTION_STOP:
      case EDJE_ACTION_TYPE_FOCUS_SET:
      case EDJE_ACTION_TYPE_FOCUS_OBJECT:
      case EDJE_ACTION_TYPE_STATE_SET:
      case EDJE_ACTION_TYPE_SIGNAL_EMIT:
      case EDJE_ACTION_TYPE_SCRIPT:
        break;
      case EDJE_ACTION_TYPE_DRAG_VAL_SET:
      case EDJE_ACTION_TYPE_DRAG_VAL_STEP:
      case EDJE_ACTION_TYPE_DRAG_VAL_PAGE:
        {
           if (!edje_edit_program_value2_set(prop.style->obj, prop.program,
                                             atof(value)))
             NOTIFY_WARNING(_("The entered data is not valid!"));
           break;
        }
      case EDJE_ACTION_TYPE_SOUND_SAMPLE:
        {
           if (!edje_edit_program_sample_speed_set(prop.style->obj, prop.program,
                                                   atof(value)))
             NOTIFY_WARNING(_("The entered data is not valid!"));
           break;
        }
      case EDJE_ACTION_TYPE_SOUND_TONE:
        {
           if (!edje_edit_program_tone_duration_set(prop.style->obj, prop.program,
                                             atof(value)))
             NOTIFY_WARNING(_("The entered data is not valid!"));
           break;
        }
      default:
        break;
     }
}

static void
_on_v1_active(void *data,
              Evas_Object *obj,
              void *ei __UNUSED__)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   const char *value;
   value = elm_entry_entry_get(obj);
   if (!edje_edit_program_transition_value1_set(prop.style->obj, prop.program,
                                                atof(value)))
     NOTIFY_WARNING(_("The entered data is not valid!"));
}

static void
_on_v2_active(void *data,
              Evas_Object *obj,
              void *ei __UNUSED__)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   const char *value;
   value = elm_entry_entry_get(obj);
   if (!edje_edit_program_transition_value2_set(prop.style->obj, prop.program,
                                                atof(value)))
     NOTIFY_WARNING(_("The entered data is not valid!"));
}

static void
_on_value_active(void *data,
                 Evas_Object *obj,
                 void *ei __UNUSED__)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   const char *value;

   value = elm_entry_entry_get(obj);
   switch (prop.act_type)
     {
      case EDJE_ACTION_TYPE_NONE:
      case EDJE_ACTION_TYPE_ACTION_STOP:
      case EDJE_ACTION_TYPE_FOCUS_SET:
      case EDJE_ACTION_TYPE_FOCUS_OBJECT:
      case EDJE_ACTION_TYPE_SCRIPT:
        break;
      case EDJE_ACTION_TYPE_DRAG_VAL_SET:
      case EDJE_ACTION_TYPE_DRAG_VAL_STEP:
      case EDJE_ACTION_TYPE_DRAG_VAL_PAGE:
      case EDJE_ACTION_TYPE_STATE_SET:
        {
           if (!edje_edit_program_value_set(prop.style->obj, prop.program,
                                            atof(value)))
             NOTIFY_WARNING(_("The entered data is not valid!"));
           break;
        }
      case EDJE_ACTION_TYPE_SIGNAL_EMIT:
        {
           if (!edje_edit_program_state2_set(prop.style->obj, prop.program,
                                             value))
             NOTIFY_WARNING(_("the entered data is not valid!"));
           break;
        }
      default:
        break;
     }
}

static void
_on_after_name_change(void *data,
                      Evas_Object *obj,
                      void *ei)
{
   const char *value;
   const char *old_value;
   Evas_Object *del_button;
   Program_Editor *prog_edit = (Program_Editor*)data;
   Ewe_Combobox_Item *item = ei;

   value = ewe_combobox_item_title_get(obj, item->index);
   old_value = evas_object_data_get(obj, AFTER_NAME_KEY);

   edje_edit_program_after_del(prop.style->obj, prop.program, old_value);
   edje_edit_program_after_add(prop.style->obj, prop.program, value);

   del_button = evas_object_data_get(obj, DEL_BUTTON_KEY);
   evas_object_data_set(del_button, AFTER_NAME_KEY, value);
   evas_object_data_set(obj, AFTER_NAME_KEY, value);
}

static void
_after_remove_button_cb(void *data,
                        Evas_Object *obj,
                        void *event_info __UNUSED__)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   const char *after = evas_object_data_get(obj, AFTER_NAME_KEY);
   Evas_Object *item = evas_object_data_get(obj, AFTER_ITEM_KEY);
   edje_edit_program_after_del(prop.style->obj, prop.program, after);
   evas_object_del(item);
}

static void
_after_item_add(Program_Editor *prog_edit, const char *name)
{
   Eina_List *posible_afters_list = NULL;
   Eina_List *l;
   const char *after_name;
   Evas_Object *element_box = NULL;
   Evas_Object *button = NULL;
   Evas_Object *combobox = NULL;
   Evas_Object *item_box = NULL;
   Eina_List *childs = NULL;

   childs = elm_box_children_get(elm_object_part_content_get(prop.afters,
                                                             "elm.swallow.content"));
   item_box = eina_list_nth(childs, 0);

   BOX_ADD(item_box, element_box, true, false);
   BUTTON_ADD(element_box, button, _("Del"));
   evas_object_size_hint_weight_set(button, 0.0, 0.0);

   posible_afters_list = edje_edit_programs_list_get(prop.style->obj);

   posible_afters_list = eina_list_sort(posible_afters_list,
                                        eina_list_count(posible_afters_list),
                                        _sort_cb);
   EWE_COMBOBOX_ADD(element_box, combobox);
   EINA_LIST_FOREACH(posible_afters_list, l, after_name)
     ewe_combobox_item_add(combobox, after_name);
   edje_edit_string_list_free(posible_afters_list);
   ewe_combobox_text_set(combobox, name);

   evas_object_data_set(combobox, AFTER_NAME_KEY, name);
   evas_object_data_set(combobox, DEL_BUTTON_KEY, button);
   evas_object_data_set(button, AFTER_NAME_KEY, name);
   evas_object_data_set(button, AFTER_ITEM_KEY, element_box);

   evas_object_smart_callback_add(combobox, "selected", _on_after_name_change,
                                  prog_edit);
   evas_object_smart_callback_add(button, "clicked", _after_remove_button_cb,
                                  prog_edit);

   elm_box_pack_end(element_box, combobox);
   elm_box_pack_end(element_box, button);
   elm_box_pack_end(item_box, element_box);
   eina_list_free(childs);
}

static void
_after_add_button_cb(void *data,
                     Evas_Object *obj __UNUSED__,
                     void *event_info __UNUSED__)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   _after_item_add(prog_edit, "");
}

static void
_target_remove_button_cb(void *data,
                         Evas_Object *obj,
                         void *event_info __UNUSED__)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   const char *target = evas_object_data_get(obj, TARGET_NAME_KEY);
   Evas_Object *item = evas_object_data_get(obj, TARGET_ITEM_KEY);
   edje_edit_program_target_del(prop.style->obj, prop.program, target);
   evas_object_del(item);
}

static void
_target_item_add(Program_Editor *prog_edit, const char *name)
{
   Eina_List *posible_targets_list = NULL;
   Eina_List *l;
   const char *target_name;
   Evas_Object *element_box = NULL;
   Evas_Object *button = NULL;
   Evas_Object *combobox = NULL;
   Evas_Object *item_box = NULL;
   Eina_List *childs = NULL;

   childs = elm_box_children_get(elm_object_part_content_get(prop.targets,
                                                             "elm.swallow.content"));
   item_box = eina_list_nth(childs, 0);

   BOX_ADD(item_box, element_box, true, false);
   BUTTON_ADD(element_box, button, _("Del"));
   evas_object_size_hint_weight_set(button, 0.0, 0.0);

   if (prop.act_type == EDJE_ACTION_TYPE_ACTION_STOP)
     posible_targets_list = edje_edit_programs_list_get(prop.style->obj);
   else
     posible_targets_list = edje_edit_parts_list_get(prop.style->obj);

   posible_targets_list = eina_list_sort(posible_targets_list,
                                         eina_list_count(posible_targets_list),
                                         _sort_cb);
   EWE_COMBOBOX_ADD(element_box, combobox);
   EINA_LIST_FOREACH(posible_targets_list, l, target_name)
     ewe_combobox_item_add(combobox, target_name);
   ewe_combobox_text_set(combobox, name);

   edje_edit_string_list_free(posible_targets_list);

   evas_object_data_set(combobox, TARGET_NAME_KEY, name);
   evas_object_data_set(combobox, DEL_BUTTON_KEY, button);
   evas_object_data_set(button, TARGET_NAME_KEY, name);
   evas_object_data_set(button, TARGET_ITEM_KEY, element_box);

   evas_object_smart_callback_add(combobox, "selected", _on_target_name_change,
                                  prog_edit);
   evas_object_smart_callback_add(button, "clicked", _target_remove_button_cb,
                                  prog_edit);

   elm_box_pack_end(element_box, combobox);
   elm_box_pack_end(element_box, button);
   elm_box_pack_end(item_box, element_box);
   eina_list_free(childs);
}

static void
_target_add_button_cb(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   _target_item_add(prog_edit, "");
}

static Evas_Object *
_prop_item_program_transition_add(Evas_Object *parent,
                                  Program_Editor *prog_edit,
                                  const char *tooltip)
{
   Evas_Object *item, *box;
   int i = 0;

   ITEM_ADD_(parent, item, _("transition"), "editor");
   BOX_ADD(item, box, false, true);
   EWE_COMBOBOX_ADD(item, transition.combobox);

   ITEM_ADD_(box, transition.layout0, _("from current"), "editor");
   CHECK_ADD(transition.layout0, transition.checkbox, DEFAULT_STYLE);
   elm_object_part_content_set(transition.layout0,
                               "elm.swallow.content",
                               transition.checkbox);
   evas_object_smart_callback_add(transition.checkbox, "changed",
                                  _on_transition_opt_current_changed, prog_edit);

   ITEM_ADD_(box, transition.layout1, _("length"), "editor");
   EWE_ENTRY_ADD(transition.layout1, transition.entry1, true, DEFAULT_STYLE);
   REGEX_SET(transition.entry1, FLOAT_NUMBER_REGEX);
   elm_object_part_content_set(transition.layout1,
                               "elm.swallow.content",
                               transition.entry1);
   evas_object_smart_callback_add(transition.entry1, "changed,user",
                                  _on_transition_time_active, prog_edit);

   ITEM_ADD_(box, transition.layout2, _("param1"), "editor");
   EWE_ENTRY_ADD(transition.layout2, transition.entry2, true, DEFAULT_STYLE);
   REGEX_SET(transition.entry2, FLOAT_NUMBER_REGEX);
   elm_object_part_content_set(transition.layout2,
                               "elm.swallow.content",
                               transition.entry2);

   ITEM_ADD_(box, transition.layout3, _("param2"), "editor");
   EWE_ENTRY_ADD(transition.layout3, transition.entry3, true, DEFAULT_STYLE);
   REGEX_SET(transition.entry3, FLOAT_NUMBER_REGEX);
   elm_object_part_content_set(transition.layout3,
                               "elm.swallow.content",
                               transition.entry3);

   for (i = 0; i < TRANSITIONS_COUNT; i++)
     ewe_combobox_item_add(transition.combobox, transition_type[i]);
   evas_object_smart_callback_add(transition.combobox, "selected",
                                  _on_combobox_trans_sel, prog_edit);

   elm_box_pack_end(box, transition.combobox);
   elm_box_pack_end(box, transition.layout0);
   elm_box_pack_end(box, transition.layout1);
   elm_box_pack_end(box, transition.layout2);
   elm_box_pack_end(box, transition.layout3);

   elm_object_tooltip_text_set(item, tooltip);
   elm_object_part_content_set(item, "elm.swallow.content", box);
   return item;
}

static void
_prop_item_program_transition_update(Program_Editor *prog_edit)
{
   Eina_Stringshare *buff = NULL;
   double value;
   if (!transition.item) return;
   prop.trans_type = edje_edit_program_transition_get(prop.style->obj, prop.program);

   if (prop.act_type != EDJE_ACTION_TYPE_STATE_SET)
     prop.trans_type = EDJE_TWEEN_MODE_NONE;
   ewe_combobox_select_item_set(transition.combobox, (int)(prop.trans_type & EDJE_TWEEN_MODE_MASK));
   elm_check_state_set(transition.checkbox, !!(prop.trans_type & EDJE_TWEEN_MODE_OPT_FROM_CURRENT));
   if (prop.trans_type == EDJE_TWEEN_MODE_NONE)
     {
        TRANS_ENTRIES_DEFAULT_SET(true);
        return;
     }

   value = edje_edit_program_transition_time_get(prop.style->obj, prop.program);

   buff = eina_stringshare_printf("%1.2f", value);
   ewe_entry_entry_set(transition.entry1, buff);
   eina_stringshare_del(buff);
   TRANS_ENTRIES_DEFAULT_SET(false);

   _trans_entries_set(prog_edit);
}

static Evas_Object *
_prop_item_program_action_add(Evas_Object *parent,
                              Program_Editor *prog_edit,
                              const char *tooltip)
{
   Evas_Object *item, *box;
   int i = 0;

   ITEM_ADD_(parent, item, _("action"), "editor");
   BOX_ADD(item, box, false, true);
   EWE_COMBOBOX_ADD(box, action.combobox);
   ITEM_ADD_(box, action.layout1, _("param1"), "editor");
   EWE_ENTRY_ADD(action.layout1, action.entry1, true, DEFAULT_STYLE);
   elm_object_part_content_set(action.layout1,
                               "elm.swallow.content",
                               action.entry1);

   ITEM_ADD_(box, action.layout2, _("param2"), "editor");
   EWE_ENTRY_ADD(action.layout2, action.entry2, true, DEFAULT_STYLE);
   elm_object_part_content_set(action.layout2,
                               "elm.swallow.content",
                               action.entry2);

   for (i = 0; combobox_action[i] != EDJE_ACTION_TYPE_LAST; i++)
     ewe_combobox_item_add(action.combobox, _(action_type[combobox_action[i]]));
   evas_object_smart_callback_add(action.combobox, "selected",
                                  _on_combobox_action_sel, prog_edit);

   elm_object_tooltip_text_set(item, tooltip);
   elm_box_pack_end(box, action.combobox);
   elm_box_pack_end(box, action.layout1);
   elm_box_pack_end(box, action.layout2);

   elm_object_part_content_set(item, "elm.swallow.content", box);
   action.box = box;

   return item;
}

static void
_prop_item_program_action_update(Program_Editor *prog_edit)
{
   int act_type_item;
   prop.act_type = edje_edit_program_action_get(prop.style->obj, prop.program);
   ewe_entry_entry_set(action.entry1, "");
   ewe_entry_entry_set(action.entry2, "");
   elm_object_disabled_set(action.entry1, true);
   elm_object_disabled_set(action.entry2, true);

   for (act_type_item = 0; combobox_action[act_type_item] != EDJE_ACTION_TYPE_LAST; act_type_item++)
     if (combobox_action[act_type_item] == prop.act_type)
       break;

   if (combobox_action[act_type_item] == EDJE_ACTION_TYPE_LAST)
     {
        ERR("Oops, not supported action type");
        ewe_combobox_text_set(action.combobox, action_type[prop.act_type]);
        elm_object_disabled_set(action.combobox, true);
     }
   else
     {
        ewe_combobox_select_item_set(action.combobox, act_type_item);
        elm_object_disabled_set(action.combobox, false);
     }
   _special_properties_hide(prog_edit);

   _action_entries_set(prog_edit, true);
}

static Evas_Object *
_prop_item_program_after_add(Evas_Object *parent,
                             Program_Editor *prog_edit,
                             const char *tooltip __UNUSED__)
{
   Evas_Object *item = NULL;
   Evas_Object *box = NULL;
   Evas_Object *entrys_box = NULL;
   Evas_Object *button = NULL;

   if (!parent) return NULL;

   ITEM_ADD_(parent, item, _("after"), "editor");
   BOX_ADD(item, box, true, false);
   BOX_ADD(box, entrys_box, false, false);

   BUTTON_ADD(box, button, _("Add"));
   evas_object_size_hint_weight_set(button, 0.0, 0.0);
   evas_object_smart_callback_add(button, "clicked", _after_add_button_cb,
                                  prog_edit);

   elm_box_pack_end(box, entrys_box);
   elm_box_pack_end(box, button);
   elm_object_part_content_set(item, "elm.swallow.content", box);
   return item;
}

static void
_prop_item_program_after_update(Program_Editor *prog_edit)
{
   Eina_List *afters_list = NULL;
   Eina_List *l;
   const char *after_name;
   Eina_List *childs = NULL;

   Evas_Object *box = NULL;
   Evas_Object *entrys_box = NULL;

   if (!prop.afters) return;

   afters_list = edje_edit_program_afters_get(prop.style->obj, prop.program);

   box = elm_object_part_content_get(prop.afters, "elm.swallow.content");
   childs = elm_box_children_get(box);
   entrys_box = eina_list_nth(childs, 0);
   elm_box_clear(entrys_box);

   afters_list = eina_list_sort(afters_list, eina_list_count(afters_list), _sort_cb);
   EINA_LIST_FOREACH(afters_list, l, after_name)
     _after_item_add(prog_edit, after_name);
   eina_list_free(childs);
   eina_list_free(afters_list);
}

static Evas_Object *
_prop_item_program_target_add(Evas_Object *parent,
                              Program_Editor *prog_edit,
                              const char *tooltip __UNUSED__)
{
   Evas_Object *item = NULL;
   Evas_Object *box = NULL;
   Evas_Object *entries_box = NULL;
   Evas_Object *button = NULL;

   if (!parent) return NULL;

   ITEM_ADD_(parent, item, _("targets"), "editor");
   BOX_ADD(item, box, true, false);
   BOX_ADD(box, entries_box, false, false);

   BUTTON_ADD(box, button, _("Add"));
   evas_object_size_hint_weight_set(button, 0.0, 0.0);

   elm_box_pack_end(box, entries_box);
   elm_box_pack_end(box, button);
   elm_object_part_content_set(item, "elm.swallow.content", box);

   evas_object_smart_callback_add(button, "clicked", _target_add_button_cb,
                                  prog_edit);
   return item;
}

static void
_prop_item_program_targets_update(Program_Editor *prog_edit)
{
   Eina_List *targets_list = NULL;
   Eina_List *l;
   const char *target_name;
   Eina_List *childs = NULL;

   Evas_Object *box = NULL;
   Evas_Object *entrys_box = NULL;

   if (!prop.targets) return;

   targets_list = edje_edit_program_targets_get(prop.style->obj, prop.program);

   box = elm_object_part_content_get(prop.targets, "elm.swallow.content");
   childs = elm_box_children_get(box);
   entrys_box = eina_list_nth(childs, 0);
   elm_box_clear(entrys_box);

   targets_list = eina_list_sort(targets_list, eina_list_count(targets_list), _sort_cb);
   EINA_LIST_FOREACH(targets_list, l, target_name)
     _target_item_add(prog_edit, target_name);
   eina_list_free(childs);
   edje_edit_string_list_free(targets_list);
}

static void
_prop_item_program_in_update(Program_Editor *prog_edit)
{
   double val = 0;
   Eina_Stringshare *text = NULL;

   val = edje_edit_program_in_from_get(prop.style->obj, prop.program);
   text = eina_stringshare_printf("%2.3f", val);
   ewe_entry_entry_set(prop.in.entry_from, text);
   eina_stringshare_del(text);

   val = edje_edit_program_in_range_get(prop.style->obj, prop.program);
   text = eina_stringshare_printf("%2.3f", val);
   ewe_entry_entry_set(prop.in.entry_range, text);
   eina_stringshare_del(text);
}

static Evas_Object *
_prop_item_program_in_add(Evas_Object *parent,
                          Program_Editor *prog_edit,
                          const char *tooltip __UNUSED__)
{
   Evas_Object *item, *box;

   if (!parent) return NULL;

   ITEM_ADD_(parent, item, _("in"), "editor");

   BOX_ADD(item, box, false, false);

   ITEM_ADD_(box, prop.in.item_from, _("from"), "editor");
   EWE_ENTRY_ADD(prop.in.item_from, prop.in.entry_from, true, DEFAULT_STYLE);
   REGEX_SET(prop.in.entry_from, FLOAT_NUMBER_REGEX);
   elm_object_part_content_set(prop.in.item_from, "elm.swallow.content", prop.in.entry_from);
   evas_object_smart_callback_add(prop.in.entry_from, "changed,user",
                                  _on_in_from_change, prog_edit);
   elm_box_pack_end(box, prop.in.item_from);

   ITEM_ADD_(box, prop.in.item_range, _("range"), "editor");
   EWE_ENTRY_ADD(prop.in.item_range, prop.in.entry_range, true, DEFAULT_STYLE);
   REGEX_SET(prop.in.entry_range, FLOAT_NUMBER_REGEX);
   elm_object_part_content_set(prop.in.item_range, "elm.swallow.content", prop.in.entry_range);
   evas_object_smart_callback_add(prop.in.entry_range, "changed,user",
                                  _on_in_range_change, prog_edit);
   elm_box_pack_end(box, prop.in.item_range);

   elm_object_part_content_set(item, "elm.swallow.content", box);
   return item;
}

static void
_prop_item_program_name_update(Program_Editor *prog_edit)
{
   Evas_Object *entry = elm_object_part_content_get(prop.name,
                                                    "elm.swallow.content");

   ewe_entry_entry_set(entry, prop.program);
}

static Evas_Object *
_prop_progs_add(Evas_Object *parent, Program_Editor *prog_edit)
{
   Evas_Object *box;

   BOX_ADD(parent, box, false, false);
   evas_object_size_hint_align_set(box, 0.5, 0);

   prop.name = _prop_item_program_name_add(box, _("Unique name of program "), _on_program_name_change, prog_edit);
   prop.signal = _prop_item_program_signal_add(box, _("signal"), _on_program_signal_change, prog_edit);
   prop.source = _prop_item_program_source_add(box, _("source"), _on_program_source_change, prog_edit);
   prop.in.item = _prop_item_program_in_add(box, prog_edit, _("in"));
   action.item = _prop_item_program_action_add(box, prog_edit, _("action"));
   transition.item = _prop_item_program_transition_add(box, prog_edit, _("transition"));
   prop.afters = _prop_item_program_after_add(box, prog_edit, _("afters"));

   elm_box_pack_end(box, prop.name);
   elm_box_pack_end(box, prop.signal);
   elm_box_pack_end(box, prop.source);
   elm_box_pack_end(box, prop.in.item);
   elm_box_pack_end(box, action.item);
   elm_box_pack_end(box, transition.item);
   elm_box_pack_end(box, prop.afters);

   return box;
}

static void
_prop_progs_update(Program_Editor *prog_edit)
{
   if (!prop.prop_box)
     {
        prop.prop_box = _prop_progs_add(prop.prop_scroller, prog_edit);
        elm_object_content_set(prop.prop_scroller, prop.prop_box);
     }
   _prop_item_program_name_update(prog_edit);
   _prop_item_program_signal_update(prop.signal, prog_edit);
   _prop_item_program_source_update(prop.source, prog_edit);
   _prop_item_program_action_update(prog_edit);
   _prop_item_program_in_update(prog_edit);
   _prop_item_program_transition_update(prog_edit);
   _prop_item_program_after_update(prog_edit);
   _program_reset(prog_edit);
}

Eina_Bool
prog_editor_program_set(Evas_Object *obj, const char* program_name)
{
   Program_Editor *prog_edit = evas_object_data_get(obj, DATA_KEY);
   if (!prog_edit) return false;

   if (prop.program) eina_stringshare_del(prop.program);
   prop.program = eina_stringshare_add(program_name);
   _prop_progs_update(prog_edit);
   return true;
}

Evas_Object *
program_editor_add(Evas_Object *parent, Style *style, Live_View *live)
{
   Evas_Object *editor_layout;
   Evas_Object *bottom_panes;
   Evas_Object *scroller;
   Evas_Object *sl;
   Program_Editor *prog_edit = NULL;

   if ((!parent) || (!style) || (!style->obj) || (!live))
     {
        ERR("Failed create program editor for current style");
        return NULL;
     }

   prog_edit = (Program_Editor *)mem_calloc(1, sizeof(Program_Editor));

   prog_edit->parent = parent;
   prop.style = style;
   prog_edit->live = live;
   editor_layout = elm_layout_add(parent);
   elm_layout_file_set(editor_layout, EFLETE_EDJ, "eflete/animator/program_editor");
   prog_edit->editor_layout = editor_layout;

   prog_edit->playback.is_cycled = true;

   sl = elm_slider_add(editor_layout);
   elm_slider_indicator_format_set(sl, "%1.2f");
   elm_slider_min_max_set(sl, 0.0, 1.0);
   elm_slider_indicator_show_set(sl, true);
   evas_object_size_hint_align_set(sl, EVAS_HINT_FILL, 0.5);
   evas_object_size_hint_weight_set(sl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_smart_callback_add(sl, "changed", _slider_changed_cb, prog_edit);
   evas_object_smart_callback_add(sl, "slider,drag,start", _slider_seek_start_cb, prog_edit);
   evas_object_smart_callback_add(sl, "slider,drag,stop", _slider_seek_stop_cb, prog_edit);
   elm_layout_content_set(editor_layout, "swallow.slider", sl);
   evas_object_show(sl);
   prog_edit->slider = sl;

   bottom_panes = elm_panes_add(editor_layout);
   evas_object_size_hint_weight_set(bottom_panes, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(bottom_panes, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_left_size_set(bottom_panes, 0.2);
   evas_object_show(bottom_panes);

   SCROLLER_ADD(editor_layout, scroller);
   elm_scroller_policy_set(scroller, ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_AUTO);
   elm_object_part_content_set(editor_layout, "swallow.edit", scroller);
   prop.prop_scroller = scroller;

   evas_object_show(editor_layout);
   evas_object_data_set(editor_layout, DATA_KEY, prog_edit);
   return editor_layout;
}

void program_editor_free(Evas_Object * obj)
{
   Program_Editor *prog_edit = evas_object_data_get(obj, DATA_KEY);

   if (prog_edit)
     {
        program_editor_program_stop(obj);
        free(prog_edit);
     }
   evas_object_del(obj);
}

#undef action
#undef transition
#undef prop
#undef ACTION_STATE_GET
#undef ACTION_VAL_GET
#undef CALLBACK_UPDATE
#undef TRANS_ENTRIES_DEFAULT_SET
#undef TRANS_VAL_UPDATE
#undef REGEX_SET
