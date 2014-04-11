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

#ifndef GROUPEDIT_H
#define GROUPEDIT_H
/**
 * @defgroup Groupedit Groupedit
 * @ingroup Workspace
 *
 * The Groupedit it a graphical view of editable object. Groupedit has a
 * container and two handlers. Handlers match of rel1 and rel2 point of Edje
 * object, it means top-left corner (rel1) and bottom-right corner (rel2).
 * Handler's moving will cause groupedit size change'. Moving the handler
 * groupedit size will be changed.
 *
 * The Groupedit has its style. The style comprises of three edje group:
 * @li "eflete/groupedit/base/default";
 * @li "eflete/groupedit/handler_TL/default";
 * @li "eflete/groupedit/handler_BR/default".
 *
 * Primitives of the editable object are drawn in the groupedit container. Each
 * primitive has the border, this border show the Edje part geometry, it means
 * that Edje uses this geometry for the object calculation. Real size and
 * position of primitive will be differ from calculating part geometry.
 * Object area is calculating together with editable object primitives. Object
 * area is a region which set in the parameters of rel1 and rel2 blocks
 * in the edc-file. It's' parameters are 'relative' and 'to' (to_x, to_y).
 *
 */

struct _Geom{
   int x, y, w, h;
};

/**
 * Geometry of groupedit part primitive.
 *
 * @ingroup Groupedit
 */
typedef struct _Geom Groupedit_Geom;

/**
 * Add new groupedit object to parent object.
 * To delete groupedit object use evas_object_del.
 *
 * @param parent The parent object.
 *
 * @return The groupedit object.
 *
 * @ingroup Groupedit
 */
Evas_Object *
groupedit_add(Evas_Object *parent);

/**
 * Set the size of groupedit handlers.
 *
 * @param obj The groupedit object,
 * @param htl_w wigth of top-left handler,
 * @param htl_h height of top-left handler,
 * @param hbr_w wigth of bottom-right handler,
 * @param hbr_h height of bottom-tight handler.
 *
 * @note if trying to set the htl_w, htl_h, hbr_w, hbr_h < 0, will be set 5.
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_handler_size_set(Evas_Object *obj, int htl_w, int htl_h, int hbr_w, int hbr_h);

/**
 * Get the size of groupedit handlers.
 *
 * @param obj The groupedit object,
 * @param htl_w pointer of int width of top-left handler,
 * @param htl_h pointer of int height of top-left handler,
 * @param hbr_w pointer of int width of bottom-right handler,
 * @param hbr_h pointer of int height of bottom-tight handler.
 *
 * @ingroup Groupedit
 */
void
groupedit_handler_size_get(Evas_Object *obj, int *htl_w, int *htl_h, int *hbr_w, int *hbr_h);

/**
 * Set the minimal size of container.
 *
 * @param obj The groupedit object,
 * @param w minimal width of container,
 * @param h minimal height of container.
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @note if tring to set the w or h < 0, will be set 0
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_min_size_set(Evas_Object *obj, int w, int h);

/**
 * Set the maximum size of container.
 *
 * @param obj The groupedit object,
 * @param w maximum width of container,
 * @param h maximum height of container.
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @note if tring to set the w or h < 0, will be set -1
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_max_size_set(Evas_Object *obj, int w, int h);

/**
 * Set the size of container.
 *
 * @param obj The groupedit object,
 * @param w The new width of container,
 * @param h The new height of container.
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @note if w over the maximum weight will be set maximum width of container;
 *       if h over the maximum height will be set maximum height of container;
 *       if w less the minimum weight will be set minimum width of container;
 *       if h less the minimum height will be set minimum height of container.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_container_size_set(Evas_Object *obj, int w, int h);

/**
 * Get the size of container.
 *
 * @param obj The groupedit object,
 * @param w The int pointer where width will be set,
 * @param h The int pointer where height will be set.
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_container_size_get(Evas_Object *obj, int *w, int *h);

/*TODO: aspect for groupedit */
//Eina_Bool
//ws_groupedit_aspect_set(Evas_Object *obj, double x, double y);


/**
 * Set the new style to groupedit object.
 *
 * @param obj The groupedit object,
 * @param style The name of style that will be set.
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @note if the style description missing for some groupedit parts (container,
 *       handlers), will be used default style.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_style_set(Evas_Object *obj, const char *style);

/**
 * Get the current style name of groupedit.
 *
 * @param obj The groupedit object.
 *
 * @return the style name.
 *
 * @ingroup Groupedit
 */
const char *
groupedit_style_get(Evas_Object *obj);



/******************************************************************************/
/******************************************************************************/

/**
 * Set the editable object, parts, which will be drawn in groupedit.
 *
 * @param obj The groupedit object,
 * @param edit_obj The object being edited,
 * @param file The editable object file, means the file was loaded from an
 *        object.
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_edit_object_set(Evas_Object *obj,
                          Evas_Object *edit_obj,
                          const char *file);

/**
 * Unset the editable object. All pictured parts from groupedit will be deleted.
 *
 * @param obj The groupedit object.
 *
 * @return The editable object will be returned, or NULL if object not set
 * setted.
 *
 * @ingroup Groupedit
 */
Evas_Object *
groupedit_edit_object_unset(Evas_Object *obj);

/**
 * Recalculate all parts geometry for groupedit.
 *
 * @param obj The groupedit object.
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_edit_object_recalc_all(Evas_Object *obj);

/**
 * Get the part object, which geometry corresponds to Edje part geometry from
 * editable object.
 *
 * @param obj The groupedit object,
 * @param part The part name, whose object need to get.
 *
 * @return The groupedit part object, with geometry like Edje part.
 *
 * @ingroup Groupedit
 */
Evas_Object *
groupedit_edit_object_part_draw_get(Evas_Object *obj, const char *part);

/**
 * Add to groupedit a new part. The function add part to edje edit object and
 * add groupedit part.
 *
 * @param obj The groupedit object,
 * @param part The name of the new part,
 * @param type The part type. See <a href="http://docs.enlightenment.org/auto/edje/group__Edje__Object__Part.html">Edje_Part_Type</a>,
 * @param data The string data, used for setting image name, with part type EDJE_PART_TYPE_IMAGE.
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_edit_object_part_add(Evas_Object *obj, const char *part,
                               Edje_Part_Type type, const char *data);

/**
 * Delete the part from groupedit and from editable object.
 *
 * @param obj The groupedit object,
 * @param part The name of the part, which will be deleted.
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_edit_object_part_del(Evas_Object *obj, const char *part);

/**
 * Set the current state value to the part.
 *
 * @param obj The groupedit object,
 * @param part The name of the part to set the state,
 * @param state Name of the state to set,
 * @param value Value of the state.
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_edit_object_part_state_set(Evas_Object *obj, const char *part,
                                     const char *state, double value);

/**
 * Create new state for the give part.
 *
 * @param obj The groupedit object,
 * @param part The name of the part to add the new state,
 * @param state Name for the new state,
 * @param value The state value.
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_edit_object_part_state_add(Evas_Object *obj, const char *part,
                                     const char *state, double value);

/**
 * Delete the given part state from the edje edit object.
 *
 * @param obj The groupedit object,
 * @param part Part that contain state,
 * @param state The current name of the state,
 * @param value The state value.
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_edit_object_part_state_del(Evas_Object *obj, const char *part,
                                     const char *state, double value);

/**
 * Get the object with the object area geometry.
 *
 * @param obj The groupedit object.
 *
 * @return The groupedit part object, with object area geometry.
 *
 * @ingroup Groupedit
 */
Evas_Object *
groupedit_part_object_area_get(Evas_Object *obj);

/**
 * Set the object area visibility.
 *
 * @param obj The groupedit object,
 * @param visible The object area visibility.
 *
 * @ingroup Groupedit
 */
void
groupedit_part_object_area_visible_set(Evas_Object *obj, Eina_Bool visible);

/**
 * Get the object area visibility, if object area is not set, EINA_FALSE will be
 * returned.
 *
 * @param obj The groupedit object.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_part_object_area_visible_get(Evas_Object *obj);

/**
 * Set the groupedit's separate mode.
 *
 * @param obj The groupedit object,
 * @param separated ON/OFF (true/false) the separate mode,
 *
 * @ingroup Groupedit
 */
void
groupedit_edit_object_parts_separated(Evas_Object *obj,
                                      Eina_Bool separated);

/**
 * Check if separate mode is activated.
 *
 * @param obj The groupedit object.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_edit_object_parts_separated_is(Evas_Object *obj);

/**
 * Select the part in groupedit.
 *
 * @param obj The groupedit object,
 * @param part The name of the part which has to be selected.
 *
 * @ingroup Groupedit
 */
void
groupedit_edit_object_part_select(Evas_Object *obj, const char *part);

/**
 * Set the bg image.
 *
 * @param obj The groupedit object,
 * @param bg The Evas image.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_bg_set(Evas_Object *obj, Evas_Object *bg);

/**
 * Unset the bg image.
 *
 * @param obj The groupedit object.
 *
 * @return The image object.
 *
 * @ingroup Groupedit
 */
Evas_Object *
groupedit_bg_unset(Evas_Object *obj);

/**
 * Set a visibility for groupedit part.
 *
 * @param obj The groupedit object
 * @param part The name of groupedit part to be showed of hided
 * @param visible Use EINA_TRUE to show, EINA_FALSE to hide part.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_part_visible_set(Evas_Object *obj, const char *part, Eina_Bool visible);

/**
 * Stack part above above in groupedit module.
 *
 * @param obj The groupedit object.
 * @param part The name of part, which need above above.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_edit_object_part_above(Evas_Object *obj, const char *part);

/**
 * Stack part below below in groupedit module.
 *
 * @param obj The groupedit object.
 * @param part The name of part, which need below below.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_edit_object_part_below(Evas_Object *obj, const char *part);

#endif /* GROUPEDIT_H */
