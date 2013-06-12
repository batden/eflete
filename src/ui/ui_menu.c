#include "ui_main_window.h"
#include "choose_file_dialog.h"
#include "ui_style_viewer_window.h"

Eina_List *ui_list_menu;

static void
_on_edc_open_menu(void *data, Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   open_edc_file(ap);
}

static void
_on_edj_open_menu (void *data, Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   open_edj_file(ap);
}

static void
_on_exit_menu (void *data, Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   ui_main_window_del(ap);
}

static void
_on_style_window_menu (void *data, Evas_Object *obj __UNUSED__,
                       void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   style_viewer_init(ap);
}

Eina_Bool
ui_menu_add (App_Data *ap)
{
   Evas_Object *tb, *menu;
   Elm_Object_Item *tb_it;

   tb = elm_toolbar_add(ap->win_layout);
   if (tb == NULL) return EINA_FALSE;
   else
     ui_list_menu = eina_list_append(ui_list_menu, tb);

   evas_object_size_hint_weight_set(tb, 0.0, 0.0);
   elm_toolbar_shrink_mode_set (tb, ELM_TOOLBAR_SHRINK_NONE);
   elm_toolbar_align_set (tb, 0.0);
   elm_toolbar_homogeneous_set (tb, EINA_TRUE);
   elm_object_part_content_set (ap->win_layout, "swallow/toolbar", tb);
   evas_object_show(tb);
   elm_toolbar_icon_size_set (tb,16);

   elm_toolbar_icon_order_lookup_set (tb, ELM_ICON_LOOKUP_FDO_THEME);
   tb_it = elm_toolbar_item_append(tb, "menu/arrow_down", "File", NULL, NULL);
   if (tb_it == NULL) return EINA_FALSE;

   elm_toolbar_item_menu_set(tb_it, EINA_TRUE);
   menu = elm_toolbar_item_menu_get(tb_it);
   if (menu == NULL) return EINA_FALSE;
   else
      ui_list_menu = eina_list_append(ui_list_menu, menu);

   elm_menu_item_add(menu, NULL, "menu/folder", "Open edc-file",
                     _on_edc_open_menu, ap);
   elm_menu_item_add(menu, NULL, "menu/folder", "Open edj-file",
                     _on_edj_open_menu, ap);
   elm_menu_item_add(menu, NULL, "menu/file", "Save", NULL, NULL);
   elm_menu_item_add(menu, NULL, "menu/close", "Exit", _on_exit_menu, ap);

   tb_it=elm_toolbar_item_append(tb,"menu/arrow_down", "Options", NULL, NULL);
   elm_toolbar_item_menu_set(tb_it, EINA_TRUE);
   menu = elm_toolbar_item_menu_get(tb_it);
   elm_menu_item_add(menu, NULL, "menu/apps", "Settings", NULL, NULL);
   elm_toolbar_menu_parent_set(tb, ap->win_layout);

   tb_it = elm_toolbar_item_append (tb, NULL, "Separator", NULL, NULL);
   elm_toolbar_item_separator_set (tb_it, EINA_TRUE);

   elm_toolbar_icon_order_lookup_set (tb, ELM_ICON_LOOKUP_THEME);

   elm_toolbar_item_append(tb, "file", "File", NULL, NULL);
   elm_toolbar_item_append(tb, "folder", "Folder", NULL, NULL);
   elm_toolbar_item_append(tb, "edit", "Edit", NULL, NULL);
   elm_toolbar_item_append(tb, "refresh", "Refresh", NULL, NULL);

   tb_it = elm_toolbar_item_append (tb, NULL, "Separator", NULL, NULL);
   elm_toolbar_item_separator_set (tb_it, EINA_TRUE);

   elm_toolbar_item_append(tb, "item", "Style Viewer", _on_style_window_menu, ap);

   ap->main_menu = tb;
   return EINA_TRUE;
}
