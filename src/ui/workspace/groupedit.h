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
 * Renaming the part object with new name.
 *
 * @param obj The groupedit object,
 * @param part_old_name Old name of the part that is going to be renamed with new name.
 * @param part_new_name New name of the part that is going to be setted.
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_edit_object_part_rename(Evas_Object *obj,
                                  const char *part_old_name,
                                  const char *part_new_name);

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
 * Copy new state for the give part, from another state of this part.
 *
 * @param obj The groupedit object,
 * @param part Part that contain state.
 * @param state_from to copy from (not including state value).
 * @param value_from The value of the state to copy from.
 * @param state_to copy into (not including state value).
 * @param value_to The value of the state to copy into.
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_edit_object_part_state_copy(Evas_Object *obj, const char *part,
                                     const char *state_from, double value_from,
                                     const char *state_to, double value_to);
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
 * @return EINA_TRUE when success, EINA_FALSE otherwise.
 *
 * @ingroup Groupedit
 */
Eina_Bool
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
 * Restack part above another part in groupedit module.
 *
 * @param obj The groupedit object.
 * @param part The name of part, which need above above.
 * @param above Name of part for which will be moved 'part'.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_edit_object_part_move_above(Evas_Object *obj,
                                      const char *part,
                                      const char *above);

/**
 * Restack part below another part in groupedit module.
 *
 * @param obj The groupedit object.
 * @param part The name of part, which need above above.
 * @param below Name of part for which will be moved 'part'.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_edit_object_part_move_below(Evas_Object *obj,
                                      const char *part,
                                      const char *below);

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

/**
 * Set zoom factor for view zoommed style in groupedit object.
 *
 * @param obj The groupedit object.
 * @param factor A factor for scale. Where value 1.0 = 100% scale (not zoom
 * object). Minimum is 0.01, maximum is 20. (from 1% to 2000% zoom).
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_zoom_factor_set(Evas_Object *obj, double factor);

#endif /* GROUPEDIT_H */
