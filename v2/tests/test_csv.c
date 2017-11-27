
#include "../src/utils/csv.h"
#include "test.h"

#define TST_CSV1 "./test_data/gas-production.csv"

static FILE *csv_f1;
static unsigned int num_lines1_expected = 0;

void test_setup(void)
{
	csv_f1 = fopen(TST_CSV1, "rb");
	num_lines1_expected = 36;
}



void test_teardown(void)
{
	fclose(csv_f1);
}



/*
 * tests - num_lines_file
 *
 */
MU_TEST(test_check_numlines1)
{
	unsigned int num_lines1_actual = 36;
	
	mu_check(num_lines1_expected == num_lines1_actual);
}



MU_TEST(test_assert_num_lines_file_csv_f1)
{
	unsigned int num_lines1_actual = num_lines_file(csv_f1);
	
	mu_assert_uint_eq(num_lines1_expected, num_lines1_actual);
}



/*
 * tests - strtok_imprv
 *
 */

 

/*
 * tests - determine_delim
 *
 */ 

 
 
MU_TEST_SUITE(test_suite)
{
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
	
	MU_RUN_TEST(test_check_numlines1);
	MU_RUN_TEST(test_assert_num_lines_file_csv_f1);
	
	
}

 
int main(void)
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return 0;
}
