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

#include "history.h"
#include "history_private.h"

/**
 * @struct _Attribute_Diff
 *
 * @brief Structure that store the diff data for attributes. (like part params).
 * This structure "inherit" from @c _Diff. It's implement with use field Diff.
 * All fields of this structure provide save diff, that can be undo and
 * redo.
 *
 * @ingroup History
 */
struct _Attribute_Diff
{
   Diff diff; /**<  Instance of main diff structure.*/
   Eina_Stringshare *style; /**< Full group name */
   Eina_Stringshare *part; /**< Part name, can be NULL */
   Eina_Stringshare *state; /**< State name, can present only if present part.
                                 Can be NULL */
   double state_value; /**< State value, uses only with field state*/
   void (*func)(Evas_Object *obj, ...); /**< Function that provide change
                                          attribute in edit object*/
   void (*func_revert)(Evas_Object *obj, ...); /**< Function that provide
                                                 revert change attribute in
                                                 edit object. Needed for ADD/DEL*/
   Value_Type param_type; /**< Helper, that make faster search needed
                               struct in union */
   union {
      struct {
         Eina_Stringshare *old;
         Eina_Stringshare *new;
      } string; /**< Save diffs for string values. */
      struct {
         int old;
         int new;
      } integer; /**< Save diffs for integer values. */
      struct {
         double old;
         double new;
      } doubl; /**< Save diffs for double values. */
      struct {
         int old_1;
         int old_2;
         int old_3;
         int old_4;
         int new_1;
         int new_2;
         int new_3;
         int new_4;
      } four; /**< Save diffs for four int values. Usually being used for store colors.*/
      struct {
         double old_1;
         double old_2;
         double new_1;
         double new_2;
      } twice_double; /**< Save diffs for two double values. Used for highlight changes */
      struct {
         int old_1;
         int old_2;
         int new_1;
         int new_2;
      } twice_int; /**< Save diffs for two int values. Used for highlight changes */
   };
};

/*
 * This function updates EFLETE ui accordingly to the given change.
 * It's placed here, because needed access to fields of the Attribute_Diff
 * structure.
 */
static void
_history_ui_attribute_update(Evas_Object *source, Attribute_Diff *change)
{
   Style *style = NULL;
   Part *part = NULL;

   assert(change != NULL);
   assert(source != NULL);

   assert(ap.project != NULL);
   assert(ap.project->current_style != NULL);
   assert(ap.project->current_style->obj == source);

   style = ap.project->current_style;

   Evas_Object *prop = ui_block_property_get();

   if (change->part)
     {
        /* this case mean, that change have action with part attributes */
        part = wm_part_by_name_find(style, change->part);

        assert(part != NULL);

        ui_property_part_set(prop, part);
        if (change->state)
          {
             part->curr_state = change->state;
             part->curr_state_value = change->state_value;
             ui_states_list_data_set(style, part);
          }
        else if (change->param_type == VAL_RENAME)
          evas_object_smart_callback_call(ap.workspace, "part,name,changed", part);
        evas_object_smart_callback_call(ap.workspace, "part,changed", part);
        evas_object_smart_callback_call(ap.workspace, "ws,part,selected",
                                        (void *)change->part);
        //workspace_edit_object_part_state_set(ap.workspace, part);
     }
   else
     {
        /* next lines of code update UI of EFLETE for group params change */
        part = ui_widget_list_selected_part_get(
                                 ui_block_widget_list_get());
        if (part)
          {
             evas_object_smart_callback_call(ap.workspace, "ws,part,unselected",
                                             (void *)part->name);
             //workspace_highlight_unset(ap.workspace);
          }
        ui_property_part_unset(prop);
        ui_property_style_set(prop, style, ap.workspace);
        workspace_edit_object_recalc(ap.workspace);
     }
}

static Eina_Bool
_attribute_modify_redo(Evas_Object *source, Attribute_Diff *change)
{
   Part *part = NULL;

   assert(change != NULL);
   assert(source != NULL);

   assert(ap.project != NULL);
   assert(ap.project->current_style != NULL);
   assert(ap.project->current_style->obj == source);

   switch(change->param_type)
    {
     case VAL_INT:
        if (change->state)
          change->func(source, change->part, change->state,
                       change->state_value, change->integer.new);
        else
          change->func(source, change->part, change->integer.new);
     break;
     case VAL_DOUBLE:
        assert(change->state != NULL);

        change->func(source, change->part, change->state,
                     change->state_value, change->doubl.new);
     break;
     case VAL_STRING:
        if (change->state)
          change->func(source, change->part, change->state,
                       change->state_value, change->string.new);
        else
          change->func(source, change->part, change->string.new);
     break;
     case VAL_FOUR:
        assert(change->state != NULL);

        change->func(source, change->part, change->state,
                     change->state_value, change->four.new_1,
                     change->four.new_2, change->four.new_3,
                     change->four.new_4);
     break;
     case VAL_RENAME:
        assert(change->state == NULL);

        part = wm_part_by_name_find(ap.project->current_style, change->part);

        assert(part != NULL);

        change->func(source, change->part, change->string.new);
        change->part = change->string.new;
        part->name = change->string.new;
     break;
     default:
       ERR("Unsupported param type");
       abort();
     break;
    }

   return true;
}

static Eina_Bool
_attribute_curd_redo(Evas_Object *source, Attribute_Diff *change)
{
   assert(change != NULL);
   assert(source != NULL);

   switch(change->param_type)
    {
     case VAL_STRING:
        assert(change->state != NULL);

        change->func(source, change->part, change->state,
                     change->state_value, change->string.old);
     break;
     default:
       ERR("Unsupported module type, that store diff");
       abort();
    }

   return true;
}

static Eina_Bool
_attribute_highlight_redo(Evas_Object *source, Attribute_Diff *change)
{
   assert(change != NULL);
   assert(source != NULL);

   switch(change->param_type)
    {
     case VAL_GROUP:
       change->func_revert(source, change->twice_int.new_1);
       change->func(source, change->twice_int.new_2);
     break;
     case VAL_INT:
        assert(change->state != NULL);

        change->func_revert(source, change->part, change->state,
                            change->state_value, change->twice_int.new_1);
        change->func(source, change->part, change->state,
                     change->state_value, change->twice_int.new_2);
     break;
     case VAL_DOUBLE:
        assert(change->state != NULL);

        change->func_revert(source, change->part, change->state,
                            change->state_value, change->twice_double.new_1);
        change->func(source, change->part, change->state,
                     change->state_value, change->twice_double.new_2);
     break;
     default:
       ERR("Unsupported value type, in the given diff");
       abort();
    }
   return true;
}

Eina_Bool
_attribute_redo(Evas_Object *source, Attribute_Diff *change)
{
   Eina_Bool redo = false;

   assert(change != NULL);
   assert(source != NULL);

   switch(change->diff.action_type)
     {
      case MODIFY:
         redo = _attribute_modify_redo(source, change);
      break;
      case DEL:
      case ADD:
         redo = _attribute_curd_redo(source, change);
      break;
      case CONTAINER:
         redo = _attribute_highlight_redo(source, change);
      break;
      default:
          ERR("Unsupported action type[%d]", change->diff.action_type);
          abort();
     }

   if (redo)
     _history_ui_attribute_update(source, change);
   return redo;
}

static Eina_Bool
_attribute_modify_undo(Evas_Object *source, Attribute_Diff *change)
{
   Style *style = NULL;
   Part *part = NULL;

   assert(change != NULL);
   assert(source != NULL);


   assert(ap.project != NULL);
   assert(ap.project->current_style != NULL);
   assert(ap.project->current_style->obj == source);

   style = ap.project->current_style;

   switch(change->param_type)
    {
     case VAL_INT:
        if (change->state)
          change->func(source, change->part, change->state,
                       change->state_value, change->integer.old);
        else
          change->func(source, change->part, change->integer.old);
     break;
     case VAL_DOUBLE:
        assert(change->state != NULL);

        change->func(source, change->part, change->state,
                     change->state_value, change->doubl.old);
     break;
     case VAL_STRING:
        if (change->state)
           change->func(source, change->part, change->state,
                        change->state_value, change->string.old);
        else
           change->func(source, change->part, change->string.old);
     break;
     case VAL_FOUR:
        assert(change->state != NULL);

        change->func(source, change->part, change->state,
                     change->state_value, change->four.old_1,
                     change->four.old_2, change->four.old_3,
                     change->four.old_4);
     break;
     case VAL_RENAME:
        assert(change->state == NULL);

        part = wm_part_by_name_find(style, change->part);

        assert(part != NULL);

        change->func(source, change->part, change->string.old);
        change->part = change->string.old;
        part->name = change->string.old;
     break;
     default:
       ERR("Unsupported module type, that store diff");
       abort();
     break;
    }

   return true;
}

static Eina_Bool
_attribute_curd_undo(Evas_Object *source, Attribute_Diff *change)
{
   assert(change != NULL);
   assert(source != NULL);

   switch(change->param_type)
    {
     case VAL_STRING:
        assert(change->state != NULL);

        change->func_revert(source, change->part, change->state,
                            change->state_value, change->string.old);
     break;
     default:
       ERR("Unsupported module type, that store diff");
       abort();
    }

   return true;
}

static Eina_Bool
_attribute_highlight_undo(Evas_Object *source, Attribute_Diff *change)
{
   assert(change != NULL);
   assert(source != NULL);

   switch(change->param_type)
    {
     case VAL_GROUP:
       change->func_revert(source, change->twice_int.old_1);
       change->func(source, change->twice_int.old_2);
     break;
     case VAL_INT:
        assert(change->state != NULL);

        change->func_revert(source, change->part, change->state,
                            change->state_value, change->twice_int.old_1);
        change->func(source, change->part, change->state,
                     change->state_value, change->twice_int.old_2);
     break;
     case VAL_DOUBLE:
        assert(change->state != NULL);

        change->func_revert(source, change->part, change->state,
                            change->state_value, change->twice_double.old_1);
        change->func(source, change->part, change->state,
                     change->state_value, change->twice_double.old_2);
     break;
     default:
       ERR("Unsupported value type, in the given diff");
       abort();
    }
   return true;
}

Eina_Bool
_attribute_undo(Evas_Object *source, Attribute_Diff *change)
{
   Eina_Bool undo = false;

   assert(change != NULL);
   assert(source != NULL);

   /* first update need for set the current part to property, because we
    * don't know what a part been selected early. */
   _history_ui_attribute_update(source, change);
   switch(change->diff.action_type)
     {
      case MODIFY:
         undo = _attribute_modify_undo(source, change);
      break;
      case DEL:
      case ADD:
         undo = _attribute_curd_undo(source, change);
      break;
      case CONTAINER:
         undo = _attribute_highlight_undo(source, change);
      break;
      default:
          ERR("Unsupported action type[%d]", change->diff.action_type);
     }
   /* second update need for update the part attributes in the property. */
   if (undo)
     _history_ui_attribute_update(source, change);

   return undo;
}

void
_attribute_change_free(Attribute_Diff *change)
{
   assert(change != NULL);

   eina_stringshare_del(change->style);
   eina_stringshare_del(change->part);
   eina_stringshare_del(change->state);

   eina_stringshare_del(change->diff.description);
   eina_stringshare_del(change->diff.source);
   eina_stringshare_del(change->diff.state);
   eina_stringshare_del(change->diff.new);
   eina_stringshare_del(change->diff.old);

   if (change->diff.ui_item)
     elm_object_item_del(change->diff.ui_item);

   if (change->param_type == VAL_STRING)
     {
        eina_stringshare_del(change->string.old);
        eina_stringshare_del(change->string.new);
     }
   free(change);
   change = NULL;
}

static Attribute_Diff *
_attribute_modify_merge(Attribute_Diff *previous, Attribute_Diff *change)
{
   assert(change != NULL);
   assert(previous != NULL);

   if ((previous->func == change->func) &&
       ((previous->part == change->part) ||  /* or if this and previous change for the same part*/
        (change->param_type == VAL_RENAME)))
     {
        switch(previous->param_type)
         {
          case VAL_INT:
             previous->integer.new = change->integer.new;
          break;
          case VAL_DOUBLE:
             previous->doubl.new = change->doubl.new;
          break;
          case VAL_RENAME:
             eina_stringshare_replace(&previous->part, change->string.new);
          case VAL_STRING:
             eina_stringshare_replace(&previous->string.new, change->string.new);
          break;
          case VAL_FOUR:
             previous->four.new_1 = change->four.new_1;
             previous->four.new_2 = change->four.new_2;
             previous->four.new_3 = change->four.new_3;
             previous->four.new_4 = change->four.new_4;
          break;
          default:
             return change;
         }
        eina_stringshare_replace(&previous->diff.new, change->diff.new);
        elm_genlist_item_update(previous->diff.ui_item);
        _attribute_change_free(change);
        return NULL;
     }
   return change;
}

static Attribute_Diff *
_attribute_highlight_merge(Attribute_Diff *previous, Attribute_Diff *change)
{
   assert(change != NULL);
   assert(previous != NULL);

   if ((previous->func == change->func) &&
       (previous->func_revert == change->func_revert) &&
       (previous->part == change->part))
     {
        switch(previous->param_type)
         {
          case VAL_GROUP:
          case VAL_INT:
             previous->twice_int.new_1 = change->twice_int.new_1;
             previous->twice_int.new_2 = change->twice_int.new_2;
          break;
          case VAL_DOUBLE:
             previous->twice_double.new_1 = change->twice_double.new_1;
             previous->twice_double.new_2 = change->twice_double.new_2;
          break;
          default:
             return change;
         }
        eina_stringshare_replace(&previous->diff.new, change->diff.new);
        elm_genlist_item_update(previous->diff.ui_item);
        _attribute_change_free(change);
        return NULL;
     }
   return change;
}

Diff *
_attribute_change_merge(Attribute_Diff *change, History_Module *module)
{
   Diff *prev_general = NULL;
   Attribute_Diff *previous = NULL;

   assert(change != NULL);
   assert(module != NULL);

   if (module->current_change)
      prev_general = module->current_change;
   else
     return (Diff *)change;

   if (prev_general->action_type == change->diff.action_type)
     previous = (Attribute_Diff *)prev_general;
   else
     return (Diff *)change;

   switch (previous->diff.action_type)
     {
      case MODIFY:
         return (Diff *)_attribute_modify_merge(previous, change);
      break;
      case CONTAINER:
         return (Diff *)_attribute_highlight_merge(previous, change);
      break;
      default:
        return (Diff *)change;
     }

   return (Diff *)change;
}

#define _attribute_modify_parse(ret, list, change) \
{ \
   Eina_Stringshare *string = NULL; \
 \
   assert(change != NULL); \
 \
   switch(change->param_type) \
     { \
      case VAL_INT: \
         change->integer.old = (int)va_arg(list, int); \
         change->integer.new = (int)va_arg(list, int); \
         change->diff.new = eina_stringshare_printf("%d", change->integer.new); \
         change->diff.old = eina_stringshare_printf("%d", change->integer.old); \
         ret = true; \
      break; \
      case VAL_DOUBLE: \
         change->doubl.old = (double)va_arg(list, double); \
         change->doubl.new = (double)va_arg(list, double); \
         change->diff.new = eina_stringshare_printf("%.3f", change->doubl.new); \
         change->diff.old = eina_stringshare_printf("%.3f", change->doubl.old); \
         ret = true; \
      break; \
      case VAL_RENAME: \
      case VAL_STRING: \
         string = (char *)va_arg(list, char *); \
         change->string.old = eina_stringshare_add(string); \
         string = (char *)va_arg(list, char *); \
         change->string.new = eina_stringshare_add(string); \
         if (change->string.new) \
           change->diff.new = eina_stringshare_add(change->string.new); \
         else \
           change->diff.new = eina_stringshare_add("None"); \
         if (change->string.old) \
           change->diff.old = eina_stringshare_add(change->string.old); \
         else \
           change->diff.old = eina_stringshare_add("None"); \
         ret = true; \
      break; \
      case VAL_FOUR: \
         change->four.old_1 = (int)va_arg(list, int); \
         change->four.old_2 = (int)va_arg(list, int); \
         change->four.old_3 = (int)va_arg(list, int); \
         change->four.old_4 = (int)va_arg(list, int); \
         change->four.new_1 = (int)va_arg(list, int); \
         change->four.new_2 = (int)va_arg(list, int); \
         change->four.new_3 = (int)va_arg(list, int); \
         change->four.new_4 = (int)va_arg(list, int); \
 \
         change->diff.new = eina_stringshare_printf("%d %d %d %d", \
                              change->four.new_1, change->four.new_2, \
                              change->four.new_3, change->four.new_4); \
 \
         change->diff.old = eina_stringshare_printf("%d %d %d %d", \
                              change->four.old_1, change->four.old_2, \
                              change->four.old_3, change->four.old_4); \
 \
         ret = true; \
      break; \
      default: \
         ERR("Unsupported value type."); \
         abort(); \
     } \
} \

#define _attribute_curd_parse(ret, list, change) \
{ \
   assert(change != NULL); \
 \
   switch(change->param_type) \
     { \
      case VAL_STRING: \
         change->string.old = eina_stringshare_add((char *)va_arg(list, char *)); \
         if (change->string.old) \
           change->diff.new = eina_stringshare_add(change->string.old); \
         else \
           change->diff.new = eina_stringshare_add("None"); \
         change->diff.old = eina_stringshare_add("-"); \
         change->func_revert = (void *)va_arg(list, void *); \
         ret = true; \
      break; \
      default: \
         ERR("Unsupported value type."); \
         abort(); \
     } \
   assert(change->func_revert != NULL); \
}

#define _attribute_highlight_parse(ret, list, change) \
{ \
   assert(change != NULL); \
 \
   switch(change->param_type) \
     { \
      case VAL_GROUP: \
      case VAL_INT: \
         change->twice_int.old_1 = (int)va_arg(list, int); \
         change->twice_int.new_1 = (int)va_arg(list, int); \
         change->twice_int.old_2 = (int)va_arg(list, int); \
         change->twice_int.new_2 = (int)va_arg(list, int); \
         change->diff.new = eina_stringshare_printf("%d %d", \
                              change->twice_int.new_1, change->twice_int.new_2); \
         change->diff.old = eina_stringshare_printf("%d %d", \
                              change->twice_int.old_1, change->twice_int.old_2); \
         change->func_revert = (void *)va_arg(list, void *); \
         ret = true; \
      break; \
      case VAL_DOUBLE: \
         change->twice_double.old_1 = (double)va_arg(list, double); \
         change->twice_double.new_1 = (double)va_arg(list, double); \
         change->twice_double.old_2 = (double)va_arg(list, double); \
         change->twice_double.new_2 = (double)va_arg(list, double); \
         change->diff.new = eina_stringshare_printf("%.3f %.3f", \
                              change->twice_double.new_1, change->twice_double.new_2); \
         change->diff.old = eina_stringshare_printf("%.3f %.3f", \
                              change->twice_double.old_1, change->twice_double.old_2); \
         change->func_revert = (void *)va_arg(list, void *); \
         ret = true; \
      break; \
      default: \
         ERR("Unsupported value type."); \
         abort(); \
     } \
   assert(change->func_revert != NULL); \
}

Diff *
_attribute_change_new(va_list list)
{
   Attribute_Diff *change = NULL;
   Eina_Bool parse = false;

   change = (Attribute_Diff *)mem_calloc(1, sizeof(Attribute_Diff));
   change->diff.module_type = PROPERTY;
   change->diff.action_type = va_arg(list, Action);
   change->param_type = (Value_Type)va_arg(list, Value_Type);

   switch(change->diff.action_type)
     {
      case MODIFY:
         _attribute_modify_parse(parse, list, change);
      break;
      case DEL:
      case ADD:
         _attribute_curd_parse(parse, list, change); /**this parse add and del actions*/
      break;
      case CONTAINER:
         _attribute_highlight_parse(parse, list, change);/* here parse cases like change
                                                            align x and y in one time */
      break;
      default:
          ERR("Unsupported action type.");
          abort();
     }
   assert(parse == true);

   change->style = eina_stringshare_add((char *)va_arg(list, char *));

   assert(change->style != NULL);

   change->func = (void *)va_arg(list, void *);

   assert(change->func != NULL);

   change->diff.description = eina_stringshare_add((char *)va_arg(list, char *));

   change->part = eina_stringshare_add((char *)va_arg(list, char *));
   if (change->part)
     {
        change->diff.source = eina_stringshare_add(change->part);
        change->state = eina_stringshare_add((char *)va_arg(list, char *));
        if (change->state)
          {
             change->state_value = (double)va_arg(list, double);
             change->diff.state = eina_stringshare_printf("%s %.2f",
                                                          change->state,
                                                          change->state_value);
          }
     }
   else
     change->diff.source = eina_stringshare_add("Group");

   return (Diff *)change;
}
