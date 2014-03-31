/* Edje Theme Editor
* Copyright (C) 2013 Samsung Electronics.
*
* This file is part of Edje Theme Editor.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; If not, see http://www.gnu.org/licenses/gpl-2.0.html.
*/

#include "config.h"
#include "ui_block.h"

struct _Panes_Attributes
{
   int left_size;
   int right_size;
};
typedef struct _Panes_Attributes Panes_Attributes;

struct _Window_Attributes
{
   int width;
   int height;
};
typedef struct _Window_Attributes Window_Attributes;

struct _UI_Current_State_Panes
{
   Panes_Attributes panes_left;
   Panes_Attributes panes_left_hor;
   Panes_Attributes panes_right;
   Panes_Attributes panes_right_hor;
   Panes_Attributes panes_center;
   Panes_Attributes panes_center_down;
   Window_Attributes window;
};
typedef struct _UI_Current_State_Panes UI_Current_State_Panes;

UI_Current_State_Panes *ui_csp;
Config *config;

/*
   Store main panes in global pointer. This need for have access to all panes.
 */
Evas_Object *panes;

UI_Current_State_Panes *
_ui_panes_current_state_struct_init(void)
{
   UI_Current_State_Panes *csp = mem_malloc (sizeof(UI_Current_State_Panes));
   csp->panes_left.left_size = 0;
   csp->panes_left.right_size = 0;

   csp->panes_left_hor.left_size = 0;
   csp->panes_left_hor.right_size = 0;

   csp->panes_right.left_size = 0;
   csp->panes_right.right_size = 0;

   csp->panes_right_hor.left_size = 0;
   csp->panes_right_hor.right_size = 0;

   csp->panes_center.left_size = 0;
   csp->panes_center.right_size = 0;

   csp->panes_center_down.left_size = 0;
   csp->panes_center_down.right_size = 0;

   csp->window.width = 1000;
   csp->window.height = 800;

   return csp;
}

void
_ui_panes_current_state_get(void)
{
   double size_get = 0.0;

   Evas_Object *_panes;
   Evas_Object *_panes_temp;

   _panes = panes;
   _panes_temp = NULL;

   size_get = elm_panes_content_left_size_get(_panes);
   ui_csp->panes_left.left_size = (int)(ui_csp->window.width * size_get);
   size_get = elm_panes_content_right_size_get(_panes);
   ui_csp->panes_left.right_size = (int)(ui_csp->window.width * size_get);

   _panes = elm_object_part_content_get(panes, "left");
   size_get = elm_panes_content_left_size_get(_panes);
   ui_csp->panes_left_hor.left_size =
      (int)((ui_csp->window.height - 20) * size_get);
   size_get = elm_panes_content_right_size_get(_panes);
   ui_csp->panes_left_hor.right_size =
      (int)((ui_csp->window.height - 20) * size_get);

   _panes = elm_object_part_content_get(panes, "right");
   size_get = elm_panes_content_left_size_get(_panes);
   ui_csp->panes_right.left_size =
      (int)((ui_csp->window.width - ui_csp->panes_left.left_size)* size_get);
   size_get = 1 - elm_panes_content_left_size_get(_panes);
   ui_csp->panes_right.right_size =
      (int)((ui_csp->window.width-ui_csp->panes_left.left_size)* size_get);

   _panes_temp = elm_object_part_content_get(_panes, "right");
   size_get = elm_panes_content_left_size_get(_panes_temp);
   ui_csp->panes_right_hor.left_size =
      (int)((ui_csp->window.height - 20)*size_get);
   size_get = elm_panes_content_right_size_get(_panes_temp);
   ui_csp->panes_right_hor.right_size =
      (int)((ui_csp->window.height - 20) * size_get);

   _panes_temp = elm_object_part_content_get(_panes, "left");
   size_get = elm_panes_content_left_size_get(_panes_temp);
   ui_csp->panes_center.left_size =
      (int)((ui_csp->window.height - 20)* size_get);
   size_get = elm_panes_content_right_size_get(_panes_temp);
   ui_csp->panes_center.right_size =
      (int)((ui_csp->window.height - 20)* size_get);

   _panes = elm_object_part_content_get(_panes_temp, "right");
   size_get = elm_panes_content_left_size_get(_panes);
   ui_csp->panes_center_down.left_size=
      (int)(ui_csp->panes_right.left_size*size_get);
   size_get =elm_panes_content_right_size_get(_panes);
   ui_csp->panes_center_down.right_size=
      (int)(ui_csp->panes_right.left_size*size_get);
}

static void
_unpress_cb(void *data __UNUSED__,
            Evas_Object *obj __UNUSED__,
            void *event_info __UNUSED__)
{
   _ui_panes_current_state_get();
}

static void
_double_click_up_cb(void * data __UNUSED__,
                    Evas_Object *obj,
                    void *event_info __UNUSED__)
{
   static volatile double _size = 0.0;
   if (elm_panes_content_left_size_get(obj) > 0.0)
     {
        _size = elm_panes_content_left_size_get(obj);
        elm_panes_content_left_size_set(obj, 0.0);
     }
   else
     {
        if (!_size) _size = 0.3;
        elm_panes_content_left_size_set(obj, _size);
     }

}

static void
_double_click_left_panes_down_cb(void * data __UNUSED__,
                                 Evas_Object *obj,
                                 void *event_info __UNUSED__)
{
   static volatile double _size = 0.0;
   if (elm_panes_content_right_size_get(obj) > 0.0)
     {
        _size = elm_panes_content_right_size_get(obj);
        elm_panes_content_right_size_set(obj, 0.0);
     }
   else
     {
        if (!_size) _size = 0.4;
        elm_panes_content_right_size_set(obj, _size);
     }
}

static void
_double_click_center_panes_down_cb(void * data __UNUSED__,
                                   Evas_Object *obj,
                                   void *event_info __UNUSED__)
{
   static volatile double _size = 0.0;
   if (elm_panes_content_right_size_get(obj) > 0.0)
     {
        _size=elm_panes_content_right_size_get(obj);
        elm_panes_content_right_size_set(obj, 0.0);
     }
   else
     {
        if (!_size) _size = 0.2;
        elm_panes_content_right_size_set(obj, _size);
     }
}

Eina_Bool
ui_panes_settings_save()
{
   if (!panes)
     {
        ERR("Can't load panes settings: panes is NULL");
        return false;
     }
   config_save();
   return true;
}

Eina_Bool
ui_panes_settings_load()
{
   if (!panes)
     {
        ERR("Can't load panes settings: panes is NULL");
        return false;
     }
   config_load();
   _ui_panes_current_state_get();
   return true;
}
Eina_Bool
ui_panes_add(App_Data *ap)
{
   Evas_Object *block;
   Evas_Object *panes_left, *panes_left_hor, *panes_right;
   Evas_Object *panes_center, *panes_center_down, *panes_right_hor;
   if ((!ap) || (!ap->win_layout))
     {
        ERR("Can't create the panes. Application Data is NULL");
        return false;
     }

   config = config_init();
   ui_csp =_ui_panes_current_state_struct_init();

   panes_left = elm_panes_add(ap->win_layout);
   elm_object_style_set(panes_left, DEFAULT_STYLE);
   evas_object_size_hint_weight_set(panes_left, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes_left, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_left_size_set(panes_left, 0.2);
   elm_object_part_content_set(ap->win_layout, "eflete.swallow.panes", panes_left);
   panes = panes_left;

   panes_left_hor = elm_panes_add(ap->win_layout);
   elm_panes_horizontal_set(panes_left_hor, true);
   elm_object_style_set(panes_left_hor, DEFAULT_STYLE);
   evas_object_size_hint_weight_set(panes_left_hor, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes_left_hor, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_left_size_set(panes_left_hor, 0.45);
   elm_object_part_content_set(panes_left, "left", panes_left_hor);

   panes_right = elm_panes_add(ap->win_layout);
   elm_object_style_set(panes_right, DEFAULT_STYLE);
   evas_object_size_hint_weight_set(panes_right, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes_right, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_left_size_set(panes_right, 0.75);
   elm_object_part_content_set(panes_left, "right", panes_right);

   panes_right_hor = elm_panes_add(ap->win_layout);
   elm_panes_horizontal_set(panes_right_hor, true);
   elm_object_style_set(panes_right_hor, DEFAULT_STYLE);
   evas_object_size_hint_weight_set(panes_right_hor, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes_right_hor, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_left_size_set(panes_right_hor, 0.45);
   elm_object_part_content_set(panes_right, "right", panes_right_hor);

   panes_center = elm_panes_add(ap->win_layout);
   elm_panes_horizontal_set(panes_center, true);
   elm_object_style_set(panes_center, DEFAULT_STYLE);
   evas_object_size_hint_weight_set(panes_center, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes_center, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_left_size_set(panes_center, 0.65);
   elm_object_part_content_set(panes_right, "left", panes_center);

   panes_center_down = elm_panes_add(ap->win_layout);
   elm_object_style_set(panes_center_down, DEFAULT_STYLE);
   evas_object_size_hint_weight_set(panes_center_down, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes_center_down, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_part_content_set(panes_center, "right", panes_center_down);

   config->panes_left = panes_left;
   config->panes_right = panes_right;
   config->panes_left_hor = panes_left_hor;
   config->panes_right_hor = panes_right_hor;
   config->panes_center = panes_center;
   config->panes_center_down = panes_center_down;
   config->window = ap->win;

   evas_object_smart_callback_add(panes_left,"unpress",_unpress_cb, NULL);
   evas_object_smart_callback_add(panes_left_hor,"unpress",_unpress_cb, NULL);
   evas_object_smart_callback_add(panes_right,"unpress",_unpress_cb, NULL);
   evas_object_smart_callback_add(panes_right_hor,"unpress",_unpress_cb, NULL);
   evas_object_smart_callback_add(panes_center,"unpress",_unpress_cb, NULL);
   evas_object_smart_callback_add(panes_center_down,"unpress",
                                  _unpress_cb, NULL);

   evas_object_smart_callback_add(config->panes_right_hor, "clicked,double",
                                  _double_click_up_cb, NULL);
   evas_object_smart_callback_add(panes_left_hor, "clicked,double",
                                 _double_click_left_panes_down_cb, NULL);
   evas_object_smart_callback_add(panes_center, "clicked,double",
                                _double_click_center_panes_down_cb, NULL);


   block = ui_block_add(ap->win_layout);
   ui_block_title_text_set(block, _("Part States"));
   elm_object_part_content_set(panes_center_down, "left", block);

   evas_object_show(block);
   ap->block.bottom_left = block;

   block = ui_block_add(ap->win_layout);
   ui_block_title_text_set(block, _("Live View"));
   elm_object_part_content_set(panes_center_down, "right", block);
   evas_object_show(block);
   ap->block.bottom_right = block;

   block = ui_block_add(ap->win_layout);
   elm_object_part_content_set(panes_center, "left", block);
   evas_object_show(block);
   ap->block.canvas = block;

   block = ui_block_add(ap->win_layout);
   elm_object_part_content_set(panes_left_hor, "left", block);
   evas_object_show(block);
   ap->block.left_top = block;

   block = ui_block_add(ap->win_layout);
   ui_block_title_text_set(block, _("Using Signals"));
   elm_object_part_content_set(panes_left_hor, "right", block);
   evas_object_show(block);
   ap->block.left_bottom = block;

   block = ui_block_add(ap->win_layout);
   elm_object_part_content_set(panes_right_hor, "left", block);
   evas_object_show(block);
   ap->block.right_top = block;

   block = ui_block_add(ap->win_layout);
   ui_block_title_text_set(block, _("Property"));
   elm_object_part_content_set(panes_right_hor, "right", block);
   evas_object_show(block);
   ap->block.right_bottom = block;

   return true;
}

Eina_Bool
ui_panes_show(App_Data *ap)
{
   if ((!ap) || (!ap->win_layout))
     {
        ERR("Application Data is missing! Can't emit the signal to win_layout.");
        return false;
     }
   elm_object_signal_emit(ap->win_layout, "window,panes,show", "eflete");
   return true;
}

Eina_Bool
ui_panes_hide(App_Data *ap)
{
   if ((!ap) || (!ap->win_layout))
     {
        ERR("Application Data is missing! Can't emit the signal to win_layout.");
        return false;
     }
   elm_object_signal_emit(ap->win_layout, "window,panes,hide", "eflete");
   return true;
}
