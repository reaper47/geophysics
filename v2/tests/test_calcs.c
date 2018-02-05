#include "../src/utils/calcs.h"
#include "test.h"

#define NUM_ELS 10

double vals[10];
double vals_avg_expected;
double vals_std_expected;

void test_setup(void)
{
	vals[0] = 78.100151533596;
	vals[1] = -14.161500690805;
	vals[2] = -38.867300464445;
	vals[3] = -53.610708785364;
	vals[4] = -20.892893578359;
	vals[5] = 99.988707450519;
	vals[6] = 74.090175607621;
	vals[7] = 4.7065723438196;
	vals[8] = 41.76149784796;
	vals[9] = 37.86441720121;
	
	vals_avg_expected = 20.89791184657;
	vals_std_expected = 50.4392972539932;
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
	mu_check(vals[0] == 78.100151533596);
	mu_check(vals[1] == -14.161500690805);
	mu_check(vals[2] == -38.867300464445);
	mu_check(vals[3] == -53.610708785364);
	mu_check(vals[4] == -20.892893578359);
	mu_check(vals[5] == 99.988707450519);
	mu_check(vals[6] == 74.090175607621);
	mu_check(vals[7] == 4.7065723438196);
	mu_check(vals[8] == 41.76149784796);
	mu_check(vals[9] == 37.86441720121);
	
	mu_check(vals_avg_expected == 20.89791184657);
	mu_check(vals_std_expected == 50.4392972539932);
}



/*
 * tests - avg_arrf
 *
 */
MU_TEST(test_assert_avg_arrf)
{
	double avg_actual = avg_arrf(vals, NUM_ELS);

	const char *msg = "avg_actual not as expected";
	mu_assert(approx_eq(avg_actual, vals_avg_expected, EPSILON), msg);
}



/*
 * tests - std_arrf
 * 
 */
MU_TEST(test_assert_std_arrf)
{
	double avg = avg_arrf(vals, NUM_ELS);
	double std_actual = std_arrf(vals, avg, NUM_ELS);

	const char *msg = "std_actual not as expected";
	mu_assert(approx_eq(std_actual, vals_std_expected, EPSILON), msg);
}



/*
 * tests - interpolate_pts
 *
 */
MU_TEST(test_assert_interpolate_pts)
{
	double start_value = 0.0;
	double end_value = 0.010133433333294;
	int num_steps = 13;

	double arr_expected[] = {
		-0.000000000000000, -0.000844452777775, -0.001688905555549,
		-0.002533358333324, -0.003377811111098, -0.004222263888873,
		-0.005066716666647, -0.005911169444422, -0.006755622222196, 
		-0.007600074999971, -0.008444527777745, -0.009288980555520,
		-0.010133433333294	
	};

	double *arr_actual = interpolate_pts(start_value, end_value, num_steps);

	const char *msg = "interpolation between two points error";
	for(int i = 0; i < num_steps; i++) {
		double expected = arr_expected[i];
		double actual = arr_actual[i];
			
		mu_assert(approx_eq(expected, actual, EPSILON), msg);
	}

	free(arr_actual);
}



/*
 * tests - correct_latitude
 *
 */
MU_TEST(test_assert_correct_latitude)
{
	double lat_corr_expected = -0.335754051077702;
	
	double lat = 46.8;
	double lng = 34.25;
	double pos = 500.0;
	double lat_corr_actual = correct_latitude(lat, lng, pos);

	const char *msg = "error when correcting the latitude";
	bool is_approx_eq = approx_eq(lat_corr_expected, lat_corr_actual, EPSILON);
	mu_assert(is_approx_eq, msg);
}



MU_TEST(test_assert_max_arrf)
{
	double max_val_expected = 99.988707450519;

	double max_val_actual = max_arrf(vals, NUM_ELS);

	mu_assert_double_eq(max_val_expected, max_val_actual);
}



MU_TEST_SUITE(test_suite)
{
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
	
	MU_RUN_TEST(test_check_setup);
	MU_RUN_TEST(test_assert_avg_arrf);
	MU_RUN_TEST(test_assert_interpolate_pts);
	MU_RUN_TEST(test_assert_std_arrf);
	MU_RUN_TEST(test_assert_correct_latitude);
	MU_RUN_TEST(test_assert_max_arrf);
}



int main(void)
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return 0;
}
