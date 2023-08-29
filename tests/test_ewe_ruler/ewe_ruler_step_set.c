#include "test_ewe_ruler.h"

/**
 * @addtogroup ewe_ruler_test
 * @{
 * @addtogroup ewe_ruler_step_set
 * @{
 * ewe_ruler
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_ruler_step_set
 * @{
 * <tr>
 * <td>ewe_ruler_step_set</td>
 * <td>ewe_ruler_step_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 parent window create
 * @step 3 ewe_ruler added
 *
 * @procedure
 * @step 1 call ewe_ruler_step_set with step "100"
 * @step 2 check returned value
 * @step 3 compare setted and getted values
 * </td>
 * <td>Evas_Object *ruler, unsigned int step</td>
 * <td>EINA_TRUE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */

EFL_START_TEST(ewe_ruler_step_set_test_p)
{
   
   unsigned int step;
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *ruler = ewe_ruler_add(win);

   ck_assert_msg(ewe_ruler_step_set(ruler, NULL, 100) == EINA_TRUE,
                 "Cannot set step for the ruler");
   step = ewe_ruler_step_get(ruler, NULL);
   ck_assert_msg(step == 100,
                 "Getted and setted value are not equal");

   evas_object_del(win);
}
EFL_END_TEST

/**
 * @addtogroup ewe_ruler_step_set
 * @{
 * <tr>
 * <td>ewe_ruler_step_set</td>
 * <td>ewe_ruler_step_set_test_n</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 don't create ruler object
 *
 * @procedure
 * @step 1 call ewe_ruler_step_set
 * @step 2 check returned value
 * </td>
 * <td>NULL, unsigned int step</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFL_START_TEST(ewe_ruler_step_set_test_n)
{
   

   ck_assert_msg(ewe_ruler_step_set(NULL, NULL, 100) == EINA_FALSE, "Setted step to the NULL ruler object");

}
EFL_END_TEST

/**
 * @addtogroup ewe_ruler_step_set
 * @{
 * </TABLE>
 * @}
 * @}
 */

void ewe_ruler_step_set_test(TCase *tc)
{
   tcase_add_test(tc, ewe_ruler_step_set_test_p);
   tcase_add_test(tc, ewe_ruler_step_set_test_n);
}