#include "arrays.h"
#include "test.h"

#define NUM_ELS 10

double test_arr[NUM_ELS];

void test_setup(void)
{
	test_arr[0] = -2.0;
	test_arr[1] = 78.0;
	test_arr[2] = -78.0;
	test_arr[3] = 43.0;
	test_arr[4] = 44.0;
	test_arr[5] = -2.0;
	test_arr[6] = -100.0;
	test_arr[7] = 101.0;
	test_arr[8] = 3.0;
	test_arr[9] = -1.0;
	
}



void test_teardown(void)
{

}

  

MU_TEST(test_check_test_arr)
{
	mu_check(test_arr[0] == -2.0);
	mu_check(test_arr[1] == 78.0);
	mu_check(test_arr[2] == -78.0);
	mu_check(test_arr[3] == 43.0);
	mu_check(test_arr[4] == 44.0);
	mu_check(test_arr[5] == -2.0);
	mu_check(test_arr[6] == -100.0);
	mu_check(test_arr[7] == 101.0);
	mu_check(test_arr[8] == 3.0);
	mu_check(test_arr[9] == -1.0);
}



/*
 * tests - avg_arr
 *
 */
MU_TEST(test_assert_avg_arr)
{
    double avg_expected = 8.6;

	double avg_actual = avg_arr(test_arr, NUM_ELS);

	mu_assert_double_eq(avg_expected, avg_actual);
}



/*
 * tests - max_arr
 *
 */
MU_TEST(test_assert_max_arr)
{
	double max_val_expected = 101.0;

	double max_val_actual = max_arr(test_arr, NUM_ELS);

	mu_assert_double_eq(max_val_expected, max_val_actual);
}




/*
 * tests - std_arr
 * 
 */
MU_TEST(test_assert_std_arr)
{
	double avg = avg_arr(test_arr, NUM_ELS);
	double std_expected = 59.5251207474626;
	
	double std_actual = std_arr(test_arr, avg, NUM_ELS);

	mu_assert_double_eq(std_expected, std_actual);
}




MU_TEST_SUITE(test_suite)
{
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
	
	MU_RUN_TEST(test_check_test_arr);
	MU_RUN_TEST(test_assert_avg_arr);
	MU_RUN_TEST(test_assert_max_arr);
	MU_RUN_TEST(test_assert_std_arr);
}



int main(void)
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return 0;
}

