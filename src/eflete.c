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
* along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
*/

#include "eflete.h"
#include "ui_main_window.h"

App_Data *ap = NULL;

Evas_Object *
win_layout_get(void)
{
   if ((ap) && (ap->win_layout))
     return ap->win_layout;
   else
     return NULL;
}

App_Data *
app_create (void)
{
   if (!ap)
     ap = mem_calloc(1, sizeof (App_Data));
   return ap;
}

Evas_Object *
main_window_get (void)
{
   if (!ap)
     {
        ERR("Application data structure does'nt exist");
        return NULL;
     }

   return ap->win;
}

void
app_free(App_Data *ap)
{
   if (ap) free(ap);
}

Eina_Bool
app_init ()
{
   if (!eina_init())
     {
        CRIT("Cannot initialize the Eina library");
        return EINA_FALSE;
     }

   if (!efreet_init())
     {
        CRIT("Cannot initialize the Efreet system");
        return EINA_FALSE;
     }

   if (!ecore_init())
     {
        CRIT("Cannot initialize the Ecore library");
        return EINA_FALSE;
     }

   if (!edje_init())
     {
        CRIT("Cannot initialize the Edje Library");
        return EINA_FALSE;
     }

   if (!logger_init())
     {
        CRIT("Cannot initialize the logger library");
        return EINA_FALSE;
     }

   if (!ecore_evas_init())
     {
        CRIT("Cannot initialize the Ecore_Evas system");
        return EINA_FALSE;
     }

   elm_theme_extension_add(NULL, TET_THEME);
   return EINA_TRUE;
}

void
app_shutdown ()
{
   app_free(ap);
   elm_theme_extension_del(NULL, TET_THEME);
   eina_shutdown();
   efreet_shutdown();
   ecore_shutdown();
   edje_shutdown();
   logger_shutdown();
   ecore_evas_shutdown();
}
