#include "../include/strings.h"
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
MU_TEST(test_assert_strlower_not_csv)
{
	char str_tst[]      = "I, MARKUS, WAS AT HOME YESTERDAY!";
	char str_expected[] = "i, markus, was at home yesterday!";
	char *str_actual, *tofree;
	
	tofree = str_actual = strdup(str_tst);
	strlower(str_actual, 0);
	
	mu_assert_string_eq(str_expected, str_actual);
	free(tofree);
}



/*
 * tests - strlower_csv
 *
 */
MU_TEST(test_assert_strlower_csv)
{
	char str_tst[]      = "I, MARKUS, WAS AT HOME YESTERDAY!\n";
	char str_expected[] = "i, markus, was at home yesterday!";
	char *str_actual, *tofree;
	
	tofree = str_actual = strdup(str_tst);
	strlower(str_actual, 1);
	
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
	charptr_to_static((char*)s_expected, s_actual, n);

	mu_assert_string_eq(s_expected, s_actual);
}



/*
 * tests - rand_str_seq
 *
 */
MU_TEST(test_assert_rand_str_seq)
{
    int strlen_expected = 53;

    char *str_actual = rand_str_seq(53);
    int strlen_actual = (int)strlen(str_actual);

    mu_assert_int_eq(strlen_expected, strlen_actual);
    free(str_actual);
}



/*
 * tests - concat
 *
 */
MU_TEST(test_assert_concat_nonempty_strings)
{
    const char *str1 = "Hello, ";
    const char *str2 = "world";
    const char *str_expected = "Hello, world";

    char *str_actual = concat(str1, str2);

    mu_assert_string_eq(str_expected, str_actual);
    free(str_actual);
}



MU_TEST(test_assert_concat_empty_string)
{
    const char *str1 = "Hello, ";
    const char *str_expected = "Hello, ";

    char *str_actual = concat(str1, "");

    mu_assert_string_eq(str_expected, str_actual);
    free(str_actual);
}


MU_TEST_SUITE(test_suite)
{
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
	
    MU_RUN_TEST(test_assert_rm_spaces);
    MU_RUN_TEST(test_assert_strlower_not_csv);
    MU_RUN_TEST(test_assert_strlower_csv);
    MU_RUN_TEST(test_assert_charptr_to_static);
    MU_RUN_TEST(test_assert_rand_str_seq);
    MU_RUN_TEST(test_assert_concat_nonempty_strings);
    MU_RUN_TEST(test_assert_concat_empty_string);
}



int main(void)
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return 0;
}

