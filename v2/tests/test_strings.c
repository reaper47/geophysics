#include "../src/utils/strings.h"
#include "test.h"


void test_setup(void)
{

}



void test_teardown(void)
{

}



/*
 * tests - rm_spaces
 *
 */
MU_TEST(test_assert_rm_spaces)
{
	char str_expected[] = "abababab";
	char str_tst[]      = " ab ab ab ab ";
	
	rm_spaces(str_tst);
	
	mu_assert_string_eq(str_expected, str_tst);
}



/*
 * tests - strlower
 *
 */
MU_TEST(test_assert_strlower)
{
	char str_tst[]      = "I, MARKUS, WAS AT HOME YESTERDAY!";
	char str_expected[] = "i, markus, was at home yesterday!";
	
	char *str_actual = strlower(str_tst);
	
	mu_assert_string_eq(str_expected, str_actual);
}



MU_TEST_SUITE(test_suite)
{
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
	
	MU_RUN_TEST(test_assert_rm_spaces);
	MU_RUN_TEST(test_assert_strlower);

}



int main(void)
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return 0;
}
