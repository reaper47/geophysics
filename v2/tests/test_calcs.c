#include "../src/utils/calcs.h"
#include "test.h"

#define NUM_ELS 10

double vals[10];
double vals_avg_expected;
double vals_std_expected;

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
	vals_std_expected = 50.4392972539932f;
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
	mu_check(vals_std_expected == 50.4392972539932f);
}



/*
 * tests - avg_arrf
 *
 */
MU_TEST(test_assert_avg_arrf)
{
	double avg_actual = avg_arrf(vals, NUM_ELS);

	mu_assert(approx_eq(avg_actual, vals_avg_expected, EPSILON), "avg_actual not as expected");
}



MU_TEST(test_assert_std_arrf) {
	double avg = avg_arrf(vals, NUM_ELS);
	double std_actual = std_arrf(vals, avg, NUM_ELS);

	mu_assert(approx_eq(std_actual, vals_std_expected, EPSILON), "std_actual not as expected");
}



MU_TEST_SUITE(test_suite)
{
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
	
	MU_RUN_TEST(test_check_setup);
	MU_RUN_TEST(test_assert_avg_arrf);
	MU_RUN_TEST(test_assert_std_arrf);
}



int main(void)
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return 0;
}
