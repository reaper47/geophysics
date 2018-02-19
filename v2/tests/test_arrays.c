#include "../include/arrays.h"
#include "test.h"

#define TST_ARR_ELS 10

static int tst_arr[TST_ARR_ELS];

void test_setup(void)
{
	tst_arr[0] = -2;
	tst_arr[1] = 78;
	tst_arr[2] = -78;
	tst_arr[3] = 43;
	tst_arr[4] = 44;
	tst_arr[5] = -2;
	tst_arr[6] = -100;
	tst_arr[7] = 101;
	tst_arr[8] = 3;
	tst_arr[9] = -1;
	
}



void test_teardown(void)
{
}

  

/*
 * tests - FindMaxArr
 *
 */ 
MU_TEST(test_check_tst_arr)
{
	mu_check(tst_arr[0] == -2);
	mu_check(tst_arr[1] == 78);
	mu_check(tst_arr[2] == -78);
	mu_check(tst_arr[3] == 43);
	mu_check(tst_arr[4] == 44);
	mu_check(tst_arr[5] == -2);
	mu_check(tst_arr[6] == -100);
	mu_check(tst_arr[7] == 101);
	mu_check(tst_arr[8] == 3);
	mu_check(tst_arr[9] == -1);
}



MU_TEST(test_assert_find_max_arr_tst_arr)
{
	int val_expected = 101;
	
	int val_actual = FindMaxArr(tst_arr, TST_ARR_ELS);
	
	mu_assert_int_eq(val_expected, val_actual);
}



MU_TEST_SUITE(test_suite)
{
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
	
	MU_RUN_TEST(test_check_tst_arr);
	MU_RUN_TEST(test_assert_find_max_arr_tst_arr);
	
	
}



int main(void)
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return 0;
}
