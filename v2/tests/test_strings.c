#include "../include/strings.h"
#include "test.h"


void test_setup(void)
{
}



void test_teardown(void)
{
}



/*
 * tests - RmSpaces
 *
 */
MU_TEST(test_assert_rm_spaces)
{
	char str_expected[] = "abababab";
	char str_tst[]      = " ab ab ab ab ";
	
	RmSpaces(str_tst);
	
	mu_assert_string_eq(str_expected, str_tst);
}



/*
 * tests - StrLower
 *
 */
MU_TEST(test_assert_strlower_not_csv)
{
	char str_tst[]      = "I, MARKUS, WAS AT HOME YESTERDAY!";
	char str_expected[] = "i, markus, was at home yesterday!";
	char *str_actual, *tofree;
	
	tofree = str_actual = strdup(str_tst);
	StrLower(str_actual, 0);
	
	mu_assert_string_eq(str_expected, str_actual);
	free(tofree);
}



/*
 * tests - StrLower_csv
 *
 */
MU_TEST(test_assert_strlower_csv)
{
	char str_tst[]      = "I, MARKUS, WAS AT HOME YESTERDAY!\n";
	char str_expected[] = "i, markus, was at home yesterday!";
	char *str_actual, *tofree;
	
	tofree = str_actual = strdup(str_tst);
	StrLower(str_actual, 1);
	
	mu_assert_string_eq(str_expected, str_actual);
	free(tofree);
}



/*
 * tests - char* to char[]
 *
 */
MU_TEST(test_assert_charptr_to_static)
{
	const char *s_expected = "Hello, World (F)!";

	int n = (int)strlen(s_expected);
	char s_actual[n+1];
	CharPtrToStatic((char*)s_expected, s_actual, n);

	mu_assert_string_eq(s_expected, s_actual);
}



MU_TEST_SUITE(test_suite)
{
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
	
	MU_RUN_TEST(test_assert_rm_spaces);
	MU_RUN_TEST(test_assert_strlower_not_csv);
	MU_RUN_TEST(test_assert_strlower_csv);
	MU_RUN_TEST(test_assert_charptr_to_static);
}



int main(void)
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return 0;
}
