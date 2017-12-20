#include "../src/utils/csv.h"
#include "test.h"

#define TST_CSV1 "./test_data/gas-production.csv"
#define TST_CSV2 "./test_data/TB_laboratories_2017-12-03.csv"
#define TST_CSV3 "./test_data/grav.csv"
#define TST_CSV4 "./test_data/test1.csv"

static FILE *csv_f1, *csv_f2, *csv_f3, *csv_f4;
static unsigned int num_lines1_expected = 0;

void test_setup(void)
{
	csv_f1 = fopen(TST_CSV1, "rb");
	csv_f2 = fopen(TST_CSV2, "rb");
	csv_f3 = fopen(TST_CSV3, "rb");
	csv_f4 = fopen(TST_CSV4, "rb");
	
	num_lines1_expected = 36;
}



void test_teardown(void)
{
	fclose(csv_f1);
	fclose(csv_f2);
	fclose(csv_f3);
	fclose(csv_f4);
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
	fseek(csv_f1, 0, SEEK_SET);
	
	const char *headers_expected[] = {
		"year",
		"month",
		"total gas production (million cubic feet)",
		"natural gas (million cubic feet)",
		"associated gas (million cubic feet)",
		"associated gas (,000) u.s. barrel",
		"naphtha (,000) u.s. barrel",
		"butane (,000) u.s. barrel",
		"propane (,000) u.s. barrel"
	};
	
	const char delim_f1 = determine_delim(csv_f1);
	struct list_t *list_headers = parse_header(csv_f1, delim_f1);
	
	struct node_t *curr;
	int i = 0;
	
	for(curr = list_headers->head; curr != NULL; curr = curr->next, i++)
		mu_assert_string_eq(headers_expected[i], curr->data);

	delete_list(list_headers);
}



MU_TEST(test_assert_parse_header_grav)
{
	fseek(csv_f3, 0, SEEK_SET);
	
	const char *headers_expected[] = {
		"station (m)",
		"time",
		"time (min)",
		"reading 1",
		"reading 2",
		"reading 3",
		"reading 4",
		"purpose",
		"area",
		"poi",
		"address",
		"date of survey",
		"operation temperature (f)",
		"reference station latitude",
		"gravimetric survey direction",
		"comments"
	};
	
	const char delim_f3 = determine_delim(csv_f3);
	struct list_t *list_headers = parse_header(csv_f3, delim_f3);

	struct node_t *curr = list_headers->head;
	int i = 0;

	for( ; curr != NULL; curr = curr->next, i++)
		mu_assert_string_eq(headers_expected[i], curr->data);

	delete_list(list_headers);
}



/*
 * tests - parse_line
 *
 */ 
MU_TEST(test_assert_parse_line)
{
	const char *str = "20,0.580555556,0,1802.8,1802.5,1802.3,1802.2,LABORATOIRE 1 - LEVÉ GRAVIMÉTRIQUE,LES PLAINES D’ABRAHAM - QUÉBEC,RÉSERVOIR D'EAU MUNICIPAL DE LA VILLE DE QUÉBEC,\"555 plaines abraham, quebec, canada, g2j 5h6\",1997-12-09,74,46.8,34.25,hello";
	const char *fields_expected[] = {
		"20",
		"0.580555556",
		"0",
		"1802.8",
		"1802.5",
		"1802.3",
		"1802.2",
		"LABORATOIRE 1 - LEVÉ GRAVIMÉTRIQUE",
		"LES PLAINES D’ABRAHAM - QUÉBEC",
		"RÉSERVOIR D'EAU MUNICIPAL DE LA VILLE DE QUÉBEC",
		"\"555 plaines abraham, quebec, canada, g2j 5h6\"",
		"1997-12-09",
		"74",
		"46.8",
		"34.25",
		"hello"
	};
	
	const char delim = ',';
	struct list_t *fields = parse_line(str, delim);
	struct node_t *node = fields->head;

	for(int i = 0 ; node != NULL; node = node->next, i++)
		mu_assert_string_eq(fields_expected[i], node->data);
	
	delete_list(fields);
}



/*
 * tests - gather_lines
 *
 */ 
MU_TEST(test_assert_gather_lines)
{
	const char *lines_expected[] = {
		"1997;Ford;E350;\"ac, abs, moon\";3000.00",
		"1999;Chevy;\"Venture \"\"Extended Edition\"\"\";\"\";4900.00",
		"1999;Chevy;\"Venture \"\"Extended Edition, Very Large\"\"\";;5000.00",
		"1996;Jeep;Grand Cherokee;\"MUST SELL!",
		"air, moon roof, loaded\";4799.00"
	};
	
	struct list_t *lines_actual = gather_lines(csv_f4);
	
	struct node_t *node = lines_actual->head;
	for(int i = 0; node != NULL; node = node->next, i++) 
		mu_assert_string_eq(lines_expected[i], node->data);

	delete_list(lines_actual);
}



MU_TEST_SUITE(test_suite)
{
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
	
	MU_RUN_TEST(test_check_numlines1);
	MU_RUN_TEST(test_assert_num_lines_file_csv_f1);
	MU_RUN_TEST(test_assert_determine_delim);
	MU_RUN_TEST(test_assert_parse_header);
	MU_RUN_TEST(test_assert_parse_line);
	MU_RUN_TEST(test_assert_parse_header_grav);
	MU_RUN_TEST(test_assert_gather_lines);
}



int main(void)
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return 0;
}
