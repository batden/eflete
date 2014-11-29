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

#include "eflete.h"
#include "ui_workspace.h"
#include "utc_common.h"

/**
 * @defgroup test_ui_workspace
 */

void workspace_add_test_p(int);
void workspace_add_test_n(int);

void workspace_zoom_factor_set_p(int);
void workspace_zoom_factor_set_n1(int);
void workspace_zoom_factor_set_n2(int);
void workspace_zoom_factor_set_n3(int);

void workspace_zoom_factor_get_test_p(int);
void workspace_zoom_factor_get_test_n1(int);

void workspace_edit_object_set_test_p(int);
void workspace_edit_object_set_test_n(int);
void workspace_edit_object_set_test_n1(int);
void workspace_edit_object_set_test_n2(int);
void workspace_edit_object_set_test_n3(int);

void workspace_edit_object_unset_test_p(int);
void workspace_edit_object_unset_test_n(int);
void workspace_edit_object_unset_test_n1(int);

void workspace_edit_object_get_test_p(int);
void workspace_edit_object_get_test_p1(int);
void workspace_edit_object_get_test_n(int);

void workspace_edit_object_recalc_test_p(int);
void workspace_edit_object_recalc_test_n(int);
void workspace_edit_object_recalc_test_n1(int);

void workspace_edit_object_part_add_test_p(int);
void workspace_edit_object_part_add_test_p1(int);
void workspace_edit_object_part_add_test_n(int);
void workspace_edit_object_part_add_test_n1(int);
void workspace_edit_object_part_add_test_n2(int);
void workspace_edit_object_part_add_test_n3(int);

void workspace_edit_object_part_del_test_p(int);
void workspace_edit_object_part_del_test_n(int);
void workspace_edit_object_part_del_test_n1(int);
void workspace_edit_object_part_del_test_n2(int);

void workspace_edit_object_part_above_test_p(int);
void workspace_edit_object_part_above_test_n(int);
void workspace_edit_object_part_above_test_n1(int);
void workspace_edit_object_part_above_test_n2(int);

void workspace_edit_object_part_below_test_p(int);
void workspace_edit_object_part_below_test_n(int);
void workspace_edit_object_part_below_test_n1(int);
void workspace_edit_object_part_below_test_n2(int);

void workspace_edit_object_part_state_set_test_p(int);
void workspace_edit_object_part_state_set_test_n(int);
void workspace_edit_object_part_state_set_test_n1(int);
void workspace_edit_object_part_state_set_test_n2(int);

void workspace_edit_object_part_state_add_test_p(int);
void workspace_edit_object_part_state_add_test_p1(int);
void workspace_edit_object_part_state_add_test_n(int);
void workspace_edit_object_part_state_add_test_n1(int);
void workspace_edit_object_part_state_add_test_n2(int);
void workspace_edit_object_part_state_add_test_n3(int);
void workspace_edit_object_part_state_add_test_n4(int);

void workspace_edit_object_part_state_del_test_p(int);
void workspace_edit_object_part_state_del_test_p1(int);
void workspace_edit_object_part_state_del_test_n(int);
void workspace_edit_object_part_state_del_test_n1(int);
void workspace_edit_object_part_state_del_test_n2(int);
void workspace_edit_object_part_state_del_test_n3(int);
void workspace_edit_object_part_state_del_test_n4(int);

void workspace_edit_object_part_rename_test_p1(int);
void workspace_edit_object_part_rename_test_p2(int);
void workspace_edit_object_part_rename_test_n1(int);
void workspace_edit_object_part_rename_test_n2(int);
void workspace_edit_object_part_rename_test_n3(int);
void workspace_edit_object_part_rename_test_n4(int);

void workspace_edit_object_visible_set_test_p(int);
void workspace_edit_object_visible_set_test_n(int);
void workspace_edit_object_visible_set_test_n1(int);

void workspace_highlight_set_test_p(int);
void workspace_highlight_set_test_n(int);
void workspace_highlight_set_test_n1(int);
void workspace_highlight_set_test_n2(int);
void workspace_highlight_set_test_n3(int);

void workspace_highlight_unset_test_p(int);
void workspace_highlight_unset_test_n(int);
void workspace_highlight_unset_test_n1(int);

void workspace_separate_mode_set_test_p1(int);
void workspace_separate_mode_set_test_p2(int);
void workspace_separate_mode_set_test_n1(int);
void workspace_separate_mode_set_test_n2(int);

void workspace_separate_mode_get_test_p1(int);
void workspace_separate_mode_get_test_p2(int);
void workspace_separate_mode_get_test_n1(int);
void workspace_separate_mode_get_test_n2(int);

void workspace_edit_object_part_restack_test_p1(int);
void workspace_edit_object_part_restack_test_p2(int);
void workspace_edit_object_part_restack_test_n1(int);
void workspace_edit_object_part_restack_test_n2(int);
void workspace_edit_object_part_restack_test_n3(int);
void workspace_edit_object_part_restack_test_n4(int);
void workspace_edit_object_part_restack_test_n5(int);

void workspace_highlight_align_visible_get_test_p1(int);
void workspace_highlight_align_visible_get_test_p2(int);
void workspace_highlight_align_visible_get_test_n1(int);
void workspace_highlight_align_visible_get_test_n2(int);

void workspace_highlight_align_visible_set_test_p1(int);
void workspace_highlight_align_visible_set_test_p2(int);
void workspace_highlight_align_visible_set_test_n(int);

void workspace_object_area_visible_get_test_p1(int);
void workspace_object_area_visible_get_test_p2(int);
void workspace_object_area_visible_get_test_n1(int);
void workspace_object_area_visible_get_test_n2(int);

void workspace_object_area_visible_set_test_p1(int);
void workspace_object_area_visible_set_test_p2(int);
void workspace_object_area_visible_set_test_n(int);
