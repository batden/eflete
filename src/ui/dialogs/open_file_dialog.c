#include "choose_file_dialog.h"

struct _fs_entries
{
   char *project_name;
   Evas_Object *edc;
   Evas_Object *id;
   Evas_Object *fd;
   Evas_Object *sd;
};

typedef struct _fs_entries fs_entries;

static fs_entries *fs_ent = NULL;

static void
_on_edj_done(void *data, Evas_Object *obj, void *event_info)
{
   Evas_Object *wd_list;
   App_Data *ap;
   const char *selected = event_info;

   ap = (App_Data *)data;

   if (selected)
     {
        if (eina_str_has_suffix(selected, ".edj"))
          {
             INFO("Select file: %s", selected);
             ap->project = pm_open_project_edj(selected, selected);
             wd_list = ui_widget_list_add(ap, ap->project->widgets);
             ui_block_widget_list_set(ap, wd_list);
             evas_object_show(wd_list);
             ui_panes_show(ap);
          }
        else
          /*TODO: add notify about a wrong file extension */
          ERR("The file must have a extension '.edj'");
     }
   else
     ui_panes_hide(ap);

   evas_object_hide(elm_object_parent_widget_get(obj));
   evas_object_del(obj);
}

Eina_Bool
open_edj_file(App_Data *ap)
{
   Evas_Object *fs;

   if ((!ap) || (!ap->win)) return EINA_FALSE;

   if (!ap->inwin)
     ap->inwin = elm_win_inwin_add(ap->win);

   fs = elm_fileselector_add(ap->inwin);

   evas_object_size_hint_weight_set(fs, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(fs, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_fileselector_path_set(fs, getenv("HOME"));
   elm_fileselector_buttons_ok_cancel_set(fs, EINA_TRUE);
   elm_fileselector_expandable_set(fs, EINA_FALSE);
   elm_fileselector_mode_set(fs, ELM_FILESELECTOR_LIST);
   evas_object_smart_callback_add(fs, "done", _on_edj_done, ap);

   elm_win_inwin_content_set(ap->inwin, fs);

   evas_object_show(fs);
   elm_win_inwin_activate(ap->inwin);

   return EINA_TRUE;
}

static void
_on_ok_cb(void *data,
          Evas_Object *obj __UNUSED__,
          void *event_info __UNUSED__)
{
   Evas_Object *wd_list;
   App_Data *ap;
   const char *path_edc = elm_fileselector_entry_selected_get(fs_ent->edc);
   const char *path_id = elm_fileselector_entry_selected_get(fs_ent->id);
   const char *path_sd = elm_fileselector_entry_selected_get(fs_ent->sd);
   const char *path_fd = elm_fileselector_entry_selected_get(fs_ent->fd);

   ap = (App_Data *)data;

   if (eina_str_has_suffix(path_edc, ".edc"))
     {
        INFO("Select file: %s", path_edc);
        ap->project = pm_open_project_edc(fs_ent->project_name,
                                          path_edc,
                                          path_id,
                                          path_sd,
                                          path_fd);
        wd_list = ui_widget_list_add(ap, ap->project->widgets);
        ui_block_widget_list_set(ap, wd_list);
        evas_object_show(wd_list);
        ui_panes_show(ap);
     }
   else
     /*TODO: add notify about a wrong file extension */
     ERR("The file must have a extension '.edc'");

   evas_object_del(elm_object_content_get(ap->inwin));
   evas_object_hide(ap->inwin);

   free(fs_ent->project_name);
   free(fs_ent);
}

static void
_on_cancel_cb(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   Evas_Object *inwin = (Evas_Object *)data;

   evas_object_del(elm_object_content_get(inwin));
   evas_object_hide(inwin);
}

static void
_on_fs_edc_done(void *data __UNUSED__,
                  Evas_Object *obj __UNUSED__,
                  void *event_info)
{
   char *path = (char *)event_info;
   char *images, *sounds, *fonts;
   char tmp_path[PATH_MAX];
   Eina_Array *array;
   int i, j;

   array = eina_file_split(strdup(path));
   i = eina_array_count(array);
   eina_strlcpy(tmp_path, "/", sizeof("/"));
   for (j = 0; j < i - 1; j++)
     {
        eina_strlcat(tmp_path, eina_array_data_get(array, j), PATH_MAX);
        eina_strlcat(tmp_path, "/", PATH_MAX);
     }
   fs_ent->project_name = strdup(eina_array_data_get(array, i - 1));
   eina_array_free(array);

#define _create_path(target, prefix, prefix_size, suffix, suffix_size) \
   size_t size_##target = prefix_size + prefix_size + 1; \
   target = calloc(size_##target, sizeof(char)); \
   eina_strlcat(target, prefix, size_##target); \
   eina_strlcat(target, suffix, size_##target);

   _create_path(images, tmp_path, strlen(tmp_path), "images/", strlen("images/"));
   elm_fileselector_entry_path_set(fs_ent->id, images);

   _create_path(sounds, tmp_path, strlen(tmp_path), "sounds/", strlen("sounds/"));
   elm_fileselector_entry_path_set(fs_ent->sd, sounds);

   _create_path(fonts, tmp_path, strlen(tmp_path), "fonts/", strlen("fonts/"));
   elm_fileselector_entry_path_set(fs_ent->fd, fonts);
#undef _create_path
}

Eina_Bool
open_edc_file(App_Data *ap)
{
   Evas_Object *box, *box_item;
   Evas_Object *label, *fs_entry;
   Evas_Object *bt;

   if ((!ap) || (!ap->win)) return EINA_FALSE;

   if (!ap->inwin)
     ap->inwin = elm_win_inwin_add(ap->win);

   if (!fs_ent)
     fs_ent = calloc(1, sizeof(fs_entries));

   box = elm_box_add(ap->inwin);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(box);

   #define _item_add(box, label_text, button_text, isFolder, _fs) \
      box_item = elm_box_add(box); \
      evas_object_size_hint_weight_set(box_item, EVAS_HINT_EXPAND, 0.0); \
      evas_object_size_hint_align_set(box_item, EVAS_HINT_FILL, EVAS_HINT_FILL); \
      elm_box_horizontal_set(box_item, EINA_TRUE); \
      evas_object_show(box_item); \
      label = elm_label_add(box_item); \
      elm_object_text_set(label, label_text); \
      evas_object_show(label); \
      elm_box_pack_end(box_item, label); \
      fs_entry = elm_fileselector_entry_add(box_item); \
      elm_fileselector_entry_path_set(fs_entry, getenv("HOME")); \
      elm_fileselector_entry_inwin_mode_set(fs_entry, EINA_TRUE); \
      elm_fileselector_entry_folder_only_set(fs_entry, isFolder); \
      elm_object_text_set(fs_entry, button_text); \
      evas_object_size_hint_weight_set(fs_entry, EVAS_HINT_EXPAND, 0.0); \
      evas_object_size_hint_align_set(fs_entry, EVAS_HINT_FILL, EVAS_HINT_FILL); \
      elm_fileselector_entry_expandable_set(fs_entry, EINA_FALSE); \
      _fs = fs_entry; \
      evas_object_show(fs_entry); \
      elm_box_pack_end(box_item, fs_entry); \
      elm_box_pack_end(box, box_item);

   _item_add(box, "EDC:", "Select edc", EINA_FALSE, fs_ent->edc);
   evas_object_smart_callback_add(fs_entry, "file,chosen", _on_fs_edc_done, fs_ent);
   _item_add(box, "Image directory:", "Select", EINA_TRUE, fs_ent->id);
   _item_add(box, "Sound directory:", "Select", EINA_TRUE, fs_ent->sd);
   _item_add(box, "Font directory:", "Select", EINA_TRUE, fs_ent->fd);
   #undef _item_add

   box_item = elm_box_add(box);
   evas_object_size_hint_weight_set(box_item, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(box_item, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_horizontal_set(box_item, EINA_TRUE);
   evas_object_show(box_item);

   #define _button_add(box, text, func, data) \
      bt = elm_button_add(box); \
      elm_object_text_set(bt, text); \
      evas_object_smart_callback_add(bt, "clicked", func, data); \
      evas_object_show(bt); \
      elm_box_pack_end(box, bt);

   _button_add(box_item, "Ok", _on_ok_cb, ap);
   _button_add(box_item, "Cansel", _on_cancel_cb, ap->inwin);
   #undef _button_add
   elm_box_pack_end(box, box_item);

   elm_win_inwin_content_set(ap->inwin, box);
   elm_win_inwin_activate(ap->inwin);

   return EINA_TRUE;
}
