#include "test_ewe_ruler.h"

/**
 * @addtogroup ewe_ruler_test
 * @{
 * @addtogroup ewe_ruler_style_set
 * @{
 * ewe_ruler
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_ruler_style_set
 * @{
 * <tr>
 * <td>ewe_ruler_style_set</td>
 * <td>ewe_ruler_style_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create window
 * @step 3 add ewe_ruler
 * @step 4 add scale to ruler
 *
 * @procedure
 * @step 1 call ewe_ruler_style_set(ruler, scale, "green")
 * @step 2 check that returned value is EINA_TRUE
 * @step 3 check that value is setted
 * @step 4 call ewe_ruler_style_set(ruler, scale, "default")
 * @step 5 check that returned value is EINA_TRUE
 * @step 6 check that value is setted
 * </td>
 * <td>Evas_Object *ruler, Ewe_Ruler_Scale* scale</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */

EFL_START_TEST(ewe_ruler_style_set_test_p)
{
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *ruler = ewe_ruler_add(win);
   Ewe_Ruler_Scale *scale = ewe_ruler_scale_add(ruler, NULL);

   ck_assert_msg(ewe_ruler_ewe_style_set(ruler, scale, "green") == EINA_TRUE, "EINA_FALSE returned");
   ck_assert_str_eq(ewe_ruler_ewe_style_get(ruler, scale), "green");
   ck_assert_msg(ewe_ruler_ewe_style_set(ruler, scale, "default") == EINA_TRUE, "EINA_FALSE returned");
   ck_assert_str_eq(ewe_ruler_ewe_style_get(ruler, scale), "default");

   evas_object_del(win);
}
EFL_END_TEST

/**
 * @addtogroup ewe_ruler_style_set
 * @{
 * <tr>
 * <td>ewe_ruler_style_set</td>
 * <td>ewe_ruler_style_set_test_n</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 don't create ruler object
 *
 * @procedure
 * @step 1 call ewe_ruler_style_set
 * @step 2 check returned value
 * </td>
 * <td>NULL, NULL</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFL_START_TEST(ewe_ruler_style_set_test_n)
{
   ck_assert_msg(ewe_ruler_ewe_style_set(NULL, NULL, "green") == EINA_FALSE, "Setted style for NULL ruler object");
}
EFL_END_TEST

/**
 * @addtogroup ewe_ruler_style_set
 * @{
 * </TABLE>
 * @}
 * @}
 */

void ewe_ruler_style_set_test(TCase *tc)
{
   tcase_add_test(tc, ewe_ruler_style_set_test_p);
   tcase_add_test(tc, ewe_ruler_style_set_test_n);
}