#include "../src/utils/csv.h"
#include "test.h"

#define TST_CSV1 "./test_data/gas-production.csv"
#define TST_CSV2 "./test_data/TB_laboratories_2017-12-03.csv"

static FILE *csv_f1, *csv_f2;
static int num_lines1_expected = 0;



void test_setup(void)
{
	csv_f1 = fopen(TST_CSV1, "rb");
	csv_f2 = fopen(TST_CSV2, "rb");
	num_lines1_expected = 36;
}



void test_teardown(void)
{
	fclose(csv_f1);
	fclose(csv_f2);
}



/*
 * tests - num_lines_file
 *
 */
MU_TEST(test_check_numlines1)
{
	int num_lines1_actual = 36;
	
	mu_check(num_lines1_expected == num_lines1_actual);
}



MU_TEST(test_assert_num_lines_file_csv_f1)
{
	int num_lines1_actual = num_lines_file(csv_f1);
	
	mu_assert_int_eq(num_lines1_expected, num_lines1_actual);
}



/*
 * tests - strtok_imprv
 *
 */
MU_TEST(test_assert_strtok_imprv)
{
	char str[41] = ",hello, world, this is the,, speaking,";
	char delim = ',';
	char *tok_actual = strtok_imprv(str, &delim);
	
	const char *tok_expected[7] = {
		"",
		"hello",
		" world",
		" this is the",
		"",
		" speaking",
		""
	};
	
	int i = 0;
	while(tok_actual != NULL) {
		mu_assert_string_eq(tok_expected[i], tok_actual);
		++i;
		tok_actual = strtok_imprv(NULL, &delim);
	}
}
 

 
/*
 * tests - determine_delim
 *
 */ 
MU_TEST(test_assert_determine_delim)
{
	char csvf1_delim_expected = ';';
	char csvf2_delim_expected = ',';
	
	char csvf1_delim_actual = determine_delim(csv_f1);
	char csvf2_delim_actual = determine_delim(csv_f2);
	
	mu_assert_int_eq(csvf1_delim_expected, csvf1_delim_actual);
	mu_assert_int_eq(csvf2_delim_expected, csvf2_delim_actual);
}



/*
 * tests - parse_header
 *
 */
MU_TEST(test_assert_parse_header)
{
	char *headers = "Year;Month;Total Gas Production (Million Cubic Feet);Natural Gas (Million Cubic Feet);Associated Gas (Million Cubic Feet);Associated Gas (,000) U.S. Barrel;Naphtha (,000) U.S. Barrel;Butane (,000) U.S. Barrel;Propane (,000) U.S. Barrel";

}



MU_TEST_SUITE(test_suite)
{
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
	
	MU_RUN_TEST(test_check_numlines1);
	MU_RUN_TEST(test_assert_num_lines_file_csv_f1);
	MU_RUN_TEST(test_assert_strtok_imprv);
	MU_RUN_TEST(test_assert_determine_delim);
}



int main(void)
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return 0;
}
