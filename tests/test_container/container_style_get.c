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

#include "test_container.h"

/**
 * @addtogroup container_test
 * @{
 * @addtogroup container_style_get
 * @{
 * Container
 * <TABLE>
 * @}
 */

/**
 * @addtogroup container_style_get
 * @{
 * <tr>
 * <td>container_style_get</td>
 * <td>container_style_get_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a container
 *
 * @procedure
 * @step 1 set the new style to container
 * @step 2 get the style name from container
 * @step 3 check returned value
 * </td>
 * <td>(Evas_Object *) container, "default"</td>
 * <td>EINA_TRUE, "default"</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_style_get_test_p)
{
   Evas_Object *parent, *container;

   elm_init(0, 0);

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   container = container_add(parent);
   container_style_set(container, "default");
   ck_assert_msg(strcmp(container_style_get(container), "default") == 0,
                 "Failed to get the container a style.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_style_get
 * @{
 * </TABLE>
 * @}
 * @}
 */
