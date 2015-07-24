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

#include "test_groupedit.h"

/**
 * @addtogroup groupedit_test
 * @{
 * @addtogroup groupedit_edit_object_set
 * @{
 * Groupedit
 * <TABLE>
 * @}
 */

/**
 * @addtogroup groupedit_edit_object_set
 * @{
 * <tr>
 * <td>groupedit_edit_object_set</td>
 * <td>groupedit_edit_object_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a groupedit
 * @step 4 create a edje edit obejct
 * @step 5 load some group from file to edje edit object
 *
 * @procedure
 * @step 1 set to the groupedit editable object
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) groupedit, (Evas_Object) *edje_edit, (const char *) file</td>
 * <td>EINA_TRUE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(groupedit_edit_object_set_test_p)
{
   Evas_Object *parent, *groupedit, *edje_edit;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   groupedit = groupedit_add(parent);
   edje_edit = edje_edit_object_add(evas_object_evas_get(parent));
   edje_object_file_set(edje_edit, "edj_build/groupedit_edit_object_set.edj", "elm/radio/base/def");

   ck_assert_msg(groupedit_edit_object_set(groupedit, edje_edit, "./edj_build/groupedit_edit_object_set.edj"),
                 "The edje edit object not setted to the groupedit.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup groupedit_edit_object_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
