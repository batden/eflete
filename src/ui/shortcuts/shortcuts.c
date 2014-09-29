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

#include "shortcuts.h"

/*========================================================*/
/*               SHORTCUTS CB FUNCTION                    */
/*========================================================*/
Eina_Bool
_new_theme_cb(App_Data *app __UNUSED__)
{
   printf("New Theme\n");
   return true;
}

Eina_Bool
_open_edc_cb(App_Data *app __UNUSED__)
{
   printf("Open Edc\n");
   return true;
}

Eina_Bool
_open_edj_cb(App_Data *app __UNUSED__)
{
   printf("Open Edj\n");
   return true;
}

Eina_Bool
_save_cb(App_Data *app __UNUSED__)
{
   printf("Save\n");
   return true;
}

Eina_Bool
_save_as_cb(App_Data *app __UNUSED__)
{
   printf("Save As...\n");
   return true;
}

Eina_Bool
_quit_cb(App_Data *app __UNUSED__)
{
   printf("QUIT!\n");
   return true;
}

/*========================================================*/
/*                 HELPFULL STRUCTURES                    */
/*========================================================*/

/*
 * Pointer to a function that will be called after clicking on some shortcut.
 */
typedef Eina_Bool (*Shortcut_Function_Cb)(App_Data *app);
struct _Shortcut_Function
{
   const char           *keyname;
   unsigned int          keycode;
   unsigned int          modifiers;
   const char           *description;
   Shortcut_Function_Cb  function;
};
typedef struct _Shortcut_Function Shortcut_Function;

/*
 * Depending on keycode AND modifiers there could be different behaviour.
 * for example Ctrl + N (modifier = 2 and keycode = 57) is different from
 * Ctrl + Shift + N (modifier = 3 and keycode = 57).
 * So different function should be returned.
 * So this is actually Key to eina_hash_find.
 */
struct _Key_Pair
{
   unsigned int modifiers;
   unsigned int keycode;
};
typedef struct _Key_Pair Key_Pair;

/*
 * Private structure, exists for creating hash of
 * "shortcut description" <=> "shortcut callback function"
 */
struct _Function_Set
{
   const char           *descr;
   Shortcut_Function_Cb  func;
};
typedef struct _Function_Set Function_Set;

/*
 * Set of functions attached to it's definition.
 * When shortcut_init() is being called it generated
 */
static Function_Set _sc_func_set_init[] =
{
     {"new_theme", _new_theme_cb},
     {"open_edc", _open_edc_cb},
     {"open_edj", _open_edj_cb},
     {"save", _save_cb},
     {"save_as", _save_as_cb},
     {"quit", _quit_cb},
     {NULL, NULL}
};
static Eina_Hash *_sc_functions = NULL;

static Eina_Bool
_key_press_event_cb(void *data, int type __UNUSED__, void *event)
{
   Ecore_Event_Key *ev = (Ecore_Event_Key *)event;
   App_Data *ap = (App_Data *)data;
   Shortcut_Function *sc_func;
   Key_Pair *key = malloc(sizeof(Key_Pair));

   /*
    *  (ev->modifiers && 255) because modifiers contain both locks and modifs,
    *  so if Caps Lock is clicked, then with SHIFT it will return not 1, but 257.
    *  So we need to set a mask for real modifiers (Ctrl, Shift, Alt etc)
    */
   key->modifiers = ev->modifiers & 255;
   key->keycode = ev->keycode;

   sc_func = eina_hash_find(ap->shortcut_functions, key);
   if (sc_func)
     sc_func->function(ap);

   return ECORE_CALLBACK_PASS_ON;
}

/* key_length for custom hash */
static unsigned int
_eina_int_key_length(const void *key __UNUSED__)
{
   return 0;
}
/* key_cmp for custom hash */
static int
_eina_int_key_cmp(const void *key1, int key1_length __UNUSED__,
                  const void *key2, int key2_length __UNUSED__)
{
   Key_Pair *keys1 = (Key_Pair *)key1;
   Key_Pair *keys2 = (Key_Pair *)key2;

   unsigned int key_cmp = keys1->keycode - keys2->keycode;

   if (key_cmp == 0)
      return keys1->modifiers - keys2->modifiers;

   return key_cmp;
}
/* key_free for custom hash */
static void
_eina_hash_free(void *data)
{
   Shortcut_Function *sc_func = data;
   if (!sc_func) return;
   free(sc_func);
}
/*=============================================*/
/*               PUBLIC API                    */
/*=============================================*/

Eina_Bool
shortcuts_main_add(App_Data *ap)
{
   if ((!ap) || (ap->shortcuts_handler))
     return false;

   ap->shortcuts_handler = ecore_event_handler_add(ECORE_EVENT_KEY_DOWN,
                                                   _key_press_event_cb,
                                                   ap);
   return true;
}

Eina_Bool
shortcuts_main_del(App_Data *ap)
{
   if ((!ap) || (!ap->shortcuts_handler))
     return false;

   ecore_event_handler_del(ap->shortcuts_handler);
   ap->shortcuts_handler = NULL;
   eina_hash_free(ap->shortcut_functions);
   ap->shortcut_functions = NULL;

   return true;
}

Eina_Bool
shortcuts_profile_load(App_Data *ap, Profile *profile)
{
   Shortcuts *sc;
   Shortcut_Function *sc_func;
   Eina_List *l;
   Key_Pair *key;
   if ((!ap) || (!profile)) return false;

   if (ap->shortcut_functions)
     eina_hash_free(ap->shortcut_functions);
   ap->shortcut_functions = eina_hash_new(EINA_KEY_LENGTH(_eina_int_key_length),
                                          EINA_KEY_CMP(_eina_int_key_cmp),
                                          EINA_KEY_HASH(eina_hash_int32),
                                          _eina_hash_free,
                                          8);

   EINA_LIST_FOREACH(profile->shortcuts, l, sc)
     {
        key = malloc(sizeof(Key_Pair));
        key->keycode = sc->keycode;
        key->modifiers = sc->modifiers;

        sc_func = malloc(sizeof(Shortcut_Function));
        sc_func->keyname = sc->keyname;
        sc_func->keycode = sc->keycode;
        sc_func->modifiers = sc->modifiers;
        sc_func->description = sc->description;
        sc_func->function = eina_hash_find(_sc_functions, sc->description);
        if (eina_hash_find(ap->shortcut_functions, key) ||
            (!eina_hash_direct_add(ap->shortcut_functions, key, sc_func)))
          {
             free(sc_func);
             return false;
          }
     }

   return true;
}

Eina_Bool
shortcuts_init()
{
   if (_sc_functions) return false;

   Function_Set *_sc_func_set = _sc_func_set_init;
   _sc_functions = eina_hash_string_superfast_new(NULL);
   while (_sc_func_set->descr)
     {
        eina_hash_direct_add(_sc_functions, _sc_func_set->descr, _sc_func_set->func);
        _sc_func_set++;
     }

   return true;
}

Eina_Bool
shortcuts_shutdown()
{
   if (!_sc_functions) return false;

   eina_hash_free(_sc_functions);

   return true;
}
