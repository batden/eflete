/**
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

#ifndef LIVE_WIDGETS_H
#define LIVE_WIDGETS_H

#include "eflete.h"
/*****************************************************
 ************** COMMON MACRO AND FUNCTIONS************
 *****************************************************/

#define ITEM "item"
#define EDJE_EDIT_OBJ "edje_edit_obj"
#define PART_NAME "part_name"
#define SIGNAL_NAME "signal_data_name"
#define SIGNAL_SOURCE "signal_data_source"

#define TEXT_FUNC "text_func"
#define SWALLOW_FUNC "swallow_func"
#define SWALLOW_CLEAN_FUNC "swallow_clean_func"
#define SIGNAL_FUNC "signal_func"
#define SWALLOW_LIST "swallow_list"
#define TEXT_LIST "part_list"

#define RECT_COLOR 136, 24, 242, 255

#define COLOR_BLUE_LIGHT 57, 102, 147, 255
#define COLOR_BLUE_DARK 58, 92, 126, 255
#define ELEMENTS_COUNT 16

typedef void (* Swallow_Clean_Func)(const char *, Evas_Object *);

/**
 * Special "widget" for program editor. Privat API
 */
Evas_Object *
layout_prog_edit_create(Evas_Object *parent);
#endif
