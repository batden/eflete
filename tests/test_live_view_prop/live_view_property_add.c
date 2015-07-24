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

#include "test_live_view_prop.h"

/**
 * @addtogroup live_view_prop_test
 * @{
 * @addtogroup live_view_property_add
 * @{
 * live_view_prop
 * <TABLE>
 * @}
 */

/**
 * @addtogroup live_view_property_add
 * @{
 * <tr>
 * <td>live_view_property_add</td>
 * <td>live_view_property_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Create parent window.
 *
 * @procedure
 * @step 1 Call live_view_property_add.
 * @step 2 Check returned pointer.
 * </td>
 * <td>(Evas_Object *)parent</td>
 * <td>Not NULL pointer returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_view_property_add_test_p)
{
   Evas_Object *parent = NULL;
   Evas_Object *live = NULL;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   live = live_view_property_add(parent, false);
   ck_assert_msg(live != NULL, "Failed to create live view property object.");

   evas_object_del(parent);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup live_view_property_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
