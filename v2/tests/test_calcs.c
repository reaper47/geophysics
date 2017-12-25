#include "../src/utils/calcs.h"
#include "test.h"
#include <float.h>
#define NUM_ELS 10
#define EPSILON 1e-5

float vals[10];
float vals_avg_expected;

void test_setup(void)
{
	vals[0] = 78.100151533596f;
	vals[1] = -14.161500690805f;
	vals[2] = -38.867300464445f;
	vals[3] = -53.610708785364f;
	vals[4] = -20.892893578359f;
	vals[5] = 99.988707450519f;
	vals[6] = 74.090175607621f;
	vals[7] = 4.7065723438196f;
	vals[8] = 41.76149784796f;
	vals[9] = 37.86441720121f;
	
	vals_avg_expected = 20.89791184657f;
}



void test_teardown(void)
{
	
}



/*
 * tests - setup
 *
 */
MU_TEST(test_check_setup)
{
	mu_check(vals[0] == 78.1001515335f);
	mu_check(vals[1] == -14.161500690f);
	mu_check(vals[2] == -38.86730046444f);
	mu_check(vals[3] == -53.610708785f);
	mu_check(vals[4] == -20.8928935783f);
	mu_check(vals[5] == 99.9887074505f);
	mu_check(vals[6] == 74.0901756076f);
	mu_check(vals[7] == 4.70657234381f);
	mu_check(vals[8] == 41.76149784796f);
	mu_check(vals[9] == 37.86441720121f);
	
	mu_check(vals_avg_expected == 20.89791184657f);
}



/*
 * tests - create_node
 *
 */
MU_TEST(test_assert_avg_arr)
{
	float avg_actual = avg_arr_f(vals, NUM_ELS);

	mu_assert(fabs(avg_actual - vals_avg_expected) < FLT_EPSILON, "avg_actual not as expected");
	//mu_assert_float_eq(vals_avg_expected, avg_actual);
}



MU_TEST_SUITE(test_suite)
{
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
	
	MU_RUN_TEST(test_check_setup);
	MU_RUN_TEST(test_assert_avg_arr);
}



int main(void)
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return 0;
}
