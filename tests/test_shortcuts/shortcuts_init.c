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

#include "test_shortcuts.h"

/**
 * @addtogroup shortcuts_test
 * @{
 * @addtogroup shortcuts_init
 * @{
 * Shortcuts
 * <TABLE>
 * @}
 */

/**
 * @addtogroup shortcuts_init
 * @{
 * <tr>
 * <td>shortcuts_init</td>
 * <td>shortcuts_init_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create App_Data
 *
 * @procedure
 * @step 1 call shortcuts_init
 * </td>
 * <td>App_Data *ap</td>
 * <td>EINA_TRUE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(shortcuts_init_test_p1)
{
   App_Data *ap;

   elm_init(0,0);
   ap = app_data_get();

   ck_assert_msg(shortcuts_init(ap), "Shortcuts not initialized.");

   shortcuts_shutdown(ap);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup shortcuts_init
 * @{
 * <tr>
 * <td>shortcuts_init</td>
 * <td>shortcuts_init_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create App_Data
 *
 * @procedure
 * @step 1 call shortcuts_init
 * @step 2 call shortcuts_shutdown
 * @step 3 call shortcuts_init
 * </td>
 * <td>App_Data</td>
 * <td>EINA_TRUE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(shortcuts_init_test_p2)
{
   App_Data *ap;

   elm_init(0,0);
   ap = app_data_get();

   ck_assert_msg(shortcuts_init(ap), "Shortcuts not initialized at first time.");
   shortcuts_shutdown(ap);
   ck_assert_msg(shortcuts_init(ap), "Shortcuts not initialized at second time.");

   shortcuts_shutdown(ap);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup shortcuts_init
 * @{
 * <tr>
 * <td>shortcuts_init</td>
 * <td>shortcuts_init_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create App_Data
 *
 * @procedure
 * @step 1 call shortcuts_init
 * @step 2 call shortcuts_init
 * </td>
 * <td>App_Data *ap</td>
 * <td>EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(shortcuts_init_test_n1)
{
   App_Data *ap;

   elm_init(0,0);
   ap = app_data_get();

   ck_assert_msg(shortcuts_init(ap), "Shortcuts not initialized at first time.");
   ck_assert_msg(!shortcuts_init(ap), "Shortcuts initialized after it was already initialized.");

   shortcuts_shutdown(ap);
   elm_shutdown();
}
END_TEST


/**
 * @addtogroup shortcuts_init
 * @{
 * <tr>
 * <td>shortcuts_init</td>
 * <td>shortcuts_init_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 call shortcuts_init
 * </td>
 * <td>NULL</td>
 * <td>EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(shortcuts_init_test_n2)
{
   elm_init(0,0);

   ck_assert_msg(!shortcuts_init(NULL), "Shortcuts initialized without application data.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup shortcuts_init
 * @{
 * </TABLE>
 * @}
 * @}
 */

