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

#include "test_groupedit.h"

Suite* test_suite (void) {
   Suite *suite = suite_create("test_groupedit");
   TCase *tcase = tcase_create("TCase");
/*
   tcase_add_test(tcase, groupedit_add_test_p);
   tcase_add_test(tcase, groupedit_add_test_n);

   tcase_add_test(tcase, groupedit_handler_size_set_test_p);
   tcase_add_test(tcase, groupedit_handler_size_set_test_n1);
   tcase_add_test(tcase, groupedit_handler_size_set_test_n2);
   tcase_add_test(tcase, groupedit_handler_size_set_test_n3);
   tcase_add_test(tcase, groupedit_handler_size_set_test_n4);
   tcase_add_test(tcase, groupedit_handler_size_set_test_n5);

   tcase_add_test(tcase, groupedit_handler_size_get_test_p);
   tcase_add_test(tcase, groupedit_handler_size_get_test_n);

   tcase_add_test(tcase, groupedit_min_size_set_test_p);
   tcase_add_test(tcase, groupedit_min_size_set_test_n);

   tcase_add_test(tcase, groupedit_max_size_set_test_p);
   tcase_add_test(tcase, groupedit_max_size_set_test_n);

   tcase_add_test(tcase, groupedit_container_size_set_test_p);
   tcase_add_test(tcase, groupedit_container_size_set_test_n1);
   tcase_add_test(tcase, groupedit_container_size_set_test_n2);
   tcase_add_test(tcase, groupedit_container_size_set_test_n3);

   tcase_add_test(tcase, groupedit_container_size_get_test_p);
   tcase_add_test(tcase, groupedit_container_size_get_test_n1);
   tcase_add_test(tcase, groupedit_container_size_get_test_n2);
   tcase_add_test(tcase, groupedit_container_size_get_test_n3);

   tcase_add_test(tcase, groupedit_style_set_test_p);
   tcase_add_test(tcase, groupedit_style_set_test_n1);
   tcase_add_test(tcase, groupedit_style_set_test_n2);

   tcase_add_test(tcase, groupedit_style_get_test_p);
   tcase_add_test(tcase, groupedit_style_get_test_n1);
   tcase_add_test(tcase, groupedit_style_get_test_n2);

   tcase_add_test(tcase, groupedit_edit_object_set_test_p);
   tcase_add_test(tcase, groupedit_edit_object_set_test_n1);
   tcase_add_test(tcase, groupedit_edit_object_set_test_n2);
   tcase_add_test(tcase, groupedit_edit_object_set_test_n3);

   tcase_add_test(tcase, groupedit_edit_object_unset_test_p);
   tcase_add_test(tcase, groupedit_edit_object_unset_test_n);

   tcase_add_test(tcase, groupedit_edit_object_recalc_all_test_p);
   tcase_add_test(tcase, groupedit_edit_object_recalc_all_test_n);

   tcase_add_test(tcase, groupedit_edit_object_part_draw_get_test_p);
   tcase_add_test(tcase, groupedit_edit_object_part_draw_get_test_n1);
   tcase_add_test(tcase, groupedit_edit_object_part_draw_get_test_n2);

   tcase_add_test(tcase, groupedit_edit_object_part_add_test_p);
   tcase_add_test(tcase, groupedit_edit_object_part_add_test_n1);
   tcase_add_test(tcase, groupedit_edit_object_part_add_test_n2);
   tcase_add_test(tcase, groupedit_edit_object_part_add_test_n3);
   tcase_add_test(tcase, groupedit_edit_object_part_add_test_n4);

   tcase_add_test(tcase, groupedit_edit_object_part_del_test_p);
   tcase_add_test(tcase, groupedit_edit_object_part_del_test_n1);
   tcase_add_test(tcase, groupedit_edit_object_part_del_test_n2);

   tcase_add_test(tcase, groupedit_edit_object_part_state_set_test_p);
   tcase_add_test(tcase, groupedit_edit_object_part_state_set_test_n1);
   tcase_add_test(tcase, groupedit_edit_object_part_state_set_test_n2);
   tcase_add_test(tcase, groupedit_edit_object_part_state_set_test_n3);
   tcase_add_test(tcase, groupedit_edit_object_part_state_set_test_n4);

   tcase_add_test(tcase, groupedit_edit_object_part_state_add_test_p);
   tcase_add_test(tcase, groupedit_edit_object_part_state_add_test_n1);
   tcase_add_test(tcase, groupedit_edit_object_part_state_add_test_n2);
   tcase_add_test(tcase, groupedit_edit_object_part_state_add_test_n3);
   tcase_add_test(tcase, groupedit_edit_object_part_state_add_test_n4);

   tcase_add_test(tcase, groupedit_edit_object_part_state_del_test_p);
   tcase_add_test(tcase, groupedit_edit_object_part_state_del_test_n1);
   tcase_add_test(tcase, groupedit_edit_object_part_state_del_test_n2);
   tcase_add_test(tcase, groupedit_edit_object_part_state_del_test_n3);
   tcase_add_test(tcase, groupedit_edit_object_part_state_del_test_n4);

   tcase_add_test(tcase, groupedit_part_object_area_set_test_p);
   tcase_add_test(tcase, groupedit_part_object_area_set_test_n1);
   tcase_add_test(tcase, groupedit_part_object_area_set_test_n2);

   tcase_add_test(tcase, groupedit_part_object_area_get_test_p);
   tcase_add_test(tcase, groupedit_part_object_area_get_test_n1);
   tcase_add_test(tcase, groupedit_part_object_area_get_test_n2);

   tcase_add_test(tcase, groupedit_part_object_area_visible_set_test_p);
   tcase_add_test(tcase, groupedit_part_object_area_visible_set_test_n);

   tcase_add_test(tcase, groupedit_part_object_area_visible_get_test_p);
   tcase_add_test(tcase, groupedit_part_object_area_visible_get_test_n);

   tcase_add_test(tcase, groupedit_edit_object_parts_separated_test_p);
   tcase_add_test(tcase, groupedit_edit_object_parts_separated_test_n1);
   tcase_add_test(tcase, groupedit_edit_object_parts_separated_test_n2);
   tcase_add_test(tcase, groupedit_edit_object_parts_separated_test_n3);

   tcase_add_test(tcase, groupedit_edit_object_parts_separated_is_test_p);
   tcase_add_test(tcase, groupedit_edit_object_parts_separated_is_test_n);

   tcase_add_test(tcase, groupedit_edit_object_part_select_test_p);
   tcase_add_test(tcase, groupedit_edit_object_part_select_test_n1);
   tcase_add_test(tcase, groupedit_edit_object_part_select_test_n2);

   tcase_add_test(tcase, groupedit_bg_set_test_p);
   tcase_add_test(tcase, groupedit_bg_set_test_n1);
   tcase_add_test(tcase, groupedit_bg_set_test_n2);

   tcase_add_test(tcase, groupedit_bg_unset_test_p);
   tcase_add_test(tcase, groupedit_bg_unset_test_n);

   tcase_add_test(tcase, groupedit_part_visible_set_test_p);
   tcase_add_test(tcase, groupedit_part_visible_set_test_n1);
   tcase_add_test(tcase, groupedit_part_visible_set_test_n2);

   tcase_add_test(tcase, groupedit_edit_object_part_above_test_p);
   tcase_add_test(tcase, groupedit_edit_object_part_above_test_n1);
   tcase_add_test(tcase, groupedit_edit_object_part_above_test_n2);

   tcase_add_test(tcase, groupedit_edit_object_part_below_test_p);
   tcase_add_test(tcase, groupedit_edit_object_part_below_test_n1);
   tcase_add_test(tcase, groupedit_edit_object_part_below_test_n2);
*/
   suite_add_tcase(suite, tcase);
   return suite;
}

int main(void) {
   int number_failed;
   Suite *suite = test_suite();
   SRunner *runner = srunner_create(suite);
   srunner_set_xml (runner, "test_groupedit.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
