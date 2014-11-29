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

#include "wizard_common.h"

static Eina_Bool
_setup_splash(void *data)
{
   Wizard_Import_Edj_Win *wiew;

   wiew = (Wizard_Import_Edj_Win *)data;
   wiew->thread = pm_project_import_edj(elm_entry_entry_get(wiew->name),
                                        elm_entry_entry_get(wiew->path),
                                        elm_entry_entry_get(wiew->edj),
                                        _progress_print,
                                        _progress_end,
                                        wiew);

   return true;
}

static Eina_Bool
_edje_filter(const char *path,
             Eina_Bool dir,
             void *data __UNUSED__)
{
   if (dir) return true;

   if (eina_str_has_extension(path, ".edj"))
     return true;
   return false;
}
FILESELCTOR_WINDOW(_on_edj_bt, _("Select edj file for import"), false, true, _edje_filter, edj)

Evas_Object *
wizard_import_edj_add(App_Data *ap __UNUSED__)
{
   Wizard_Import_Edj_Win *wiew;
   wiew = wizard_import_common_add("elm/layout/wizard/import_edj");
   if (!wiew) return NULL;

   mw_title_set(wiew->win, _("Wizard: import edj"));
   wiew->splash_setup_func = _setup_splash;
   project_path_item_add(wiew, _("Path to edj file:"), _on_edj_bt);

   return wiew->win;
}

