#include "../src/gravimetry.h"
#include "../src/utils/calcs.h"
#include "test.h"

#define GRAV_FILE "./test_data/grav.csv"
#define ROWS 6
#define TOTAL_READINGS 24

struct worden807_t worden807_expected;
struct worden807_t worden807_actual;

void test_setup(void)
{
	if(alloc_worden807(&worden807_expected, ROWS) == -1)
		return;
	
	double stations[] = { 
		0.0, 20.0, 40.0, 
		60.0, 80.0, 100.0 
	};
	
	double times[] = { 
		0.580555556, 0.588194444, 0.597222222,
		0.604166667, 0.611111111, 0.622916667
	};
	
	double times_min[] = {
		0.0, 11.0, 24.0,
		34.0, 44.0, 61.0
	};
	
	double readings[] = {
		1802.8, 1802.5, 1802.3, 1802.2,
		1802.3, 1802.5, 1802.6, 1802.4,
		1802.4, 1802.3, 1802.3, 1802.3,
		1801.0, 1801.4, 1801.7, 1801.1,
		1800.7, 1801.1, 1800.7, 1800.8,
		1801.1, 1801.0, 1801.2, 1801.1
	};
	
	for(int i = 0; i < ROWS; i++) {
		worden807_expected.stations[i] = stations[i];
		worden807_expected.times[i] = times[i];
		worden807_expected.times_min[i] = times_min[i];
	}
	
	for(int i = 0; i < TOTAL_READINGS; i++)
		worden807_expected.readings[i] = readings[i];
	
	const char *purpose = "LABORATOIRE 1 - LEVE GRAVIMETRIQUE";
	const char *area = "LES PLAINES D’ABRAHAM - QUEBEC";
	const char *poi = "RESERVOIR D'EAU MUNICIPAL DE LA VILLE DE QUEBEC";
	const char *addr = "\"555 plaines abraham, quebec, canada, g2j 5h6\"";
	const char *date = "1997-12-09";

	worden807_expected.num_readings = 4;	
	worden807_expected.survey_purpose = (char*)purpose;
	worden807_expected.survey_area = (char*)area;
	worden807_expected.survey_poi = (char*)poi;
	worden807_expected.survey_address = (char*)addr;
	worden807_expected.survey_date = (char*)date;
	worden807_expected.operation_temp = 74.0;
	worden807_expected.ref_station_lat = 46.8;
	worden807_expected.survey_dir = 34.25;
	worden807_expected.operation_temp_unit = 'F';
}



void test_teardown(void)
{
	
	free_worden807(&worden807_expected);
}



/*
 * tests - assign_idx_nod
 *
 */ 
MU_TEST(test_assert_assign_idx_node)
{
	FILE *fp = fopen(GRAV_FILE, "rb");
	
	struct list_t *headers_expected = create_list();

	const char *station     = "station (m)";
	const char *time        = "time";
	const char *time_min    = "time (min)";
	const char *reading1    = "reading 1";
	const char *reading2    = "reading 2";
	const char *reading3    = "reading 3";
	const char *reading4    = "reading 4";
	const char *purpose     = "purpose";
	const char *area        = "area";
	const char *poi         = "poi";
	const char *address     = "address";
	const char *date        = "date of survey";
	const char *temperature = "operation temperature (F)";
	const char *latitude    = "reference station latitude";
	const char *direction   = "gravimetric survey direction";
	
	add_head_list(headers_expected, station);
	add_head_list(headers_expected, time);
	add_head_list(headers_expected, time_min);
	add_head_list(headers_expected, reading1);
	add_head_list(headers_expected, reading2);
	add_head_list(headers_expected, reading3);
	add_head_list(headers_expected, reading4);
	add_head_list(headers_expected, purpose);
	add_head_list(headers_expected, area);
	add_head_list(headers_expected, poi);
	add_head_list(headers_expected, address);
	add_head_list(headers_expected, date);
	add_head_list(headers_expected, temperature);
	add_head_list(headers_expected, latitude);
	add_head_list(headers_expected, direction);
	
	uint8_t read_idx = 50;
	struct node_t *curr_expect = headers_expected->head;
	
	for( ; curr_expect != NULL; curr_expect = curr_expect->next) {
		if(strcmp(curr_expect->data, station) == 0)
			curr_expect->idx = IDX_STATION;
		else if(strcmp(curr_expect->data, time) == 0) 
			curr_expect->idx = IDX_TIME;
		else if(strcmp(curr_expect->data, time_min) == 0)
			curr_expect->idx = IDX_TIME_MIN;
		else if(strcmp(curr_expect->data, reading1) == 0)
			curr_expect->idx = ++read_idx;
		else if(strcmp(curr_expect->data, reading2) == 0)
			curr_expect->idx = ++read_idx;
		else if(strcmp(curr_expect->data, reading3) == 0)
			curr_expect->idx = ++read_idx;
		else if(strcmp(curr_expect->data, reading4) == 0)
			curr_expect->idx = ++read_idx;
		else if(strcmp(curr_expect->data, purpose) == 0)
			curr_expect->idx = IDX_PURPOSE;
		else if(strcmp(curr_expect->data, area) == 0)
			curr_expect->idx = IDX_AREA;
		else if(strcmp(curr_expect->data, poi) == 0)
			curr_expect->idx = IDX_POI; 
		else if(strcmp(curr_expect->data, address) == 0)
			curr_expect->idx = IDX_ADDRESS;
		else if(strcmp(curr_expect->data, date) == 0)
			curr_expect->idx = IDX_DATE;
		else if(strcmp(curr_expect->data, temperature) == 0)
			curr_expect->idx = IDX_TEMP;
		else if(strcmp(curr_expect->data, latitude) == 0)
			curr_expect->idx = IDX_LAT;
		else if(strcmp(curr_expect->data, direction) == 0)
			curr_expect->idx = IDX_DIR;
	}
	
	const char delim = determine_delim(fp);
	struct list_t *headers_actual = parse_header(fp, delim);
	assign_idx_node(headers_actual, &worden807_expected);
	
	curr_expect = headers_expected->head;
	struct node_t *curr_actual = headers_actual->head;

	for( ; curr_expect != NULL; curr_expect = curr_expect->next) {
		for(curr_actual = headers_actual->head; curr_actual != NULL; curr_actual = curr_actual->next) {
			if(strcmp(curr_expect->data, curr_actual->data) == 0) {
				break;
			}
		}
	}
	
	delete_list(headers_expected);
	delete_list(headers_actual);
	
	fclose(fp);
}




/*
 * tests - load_grav_csv
 *
 */ 
MU_TEST(test_assert_load_grav_csv)
{
	int status_expected = 0;

	int status_actual = load_grav_csv(&worden807_actual, GRAV_FILE);
	mu_assert_int_eq(status_expected, status_actual);
	
	double expected, actual;
	for(int i = 0; i < ROWS; i++) {
		expected = worden807_expected.stations[i];
		actual = worden807_actual.stations[i];
		mu_assert_double_eq(expected, actual);
		
		expected = worden807_expected.times[i];
		actual = worden807_actual.times[i];
		mu_assert_double_eq(expected, actual);
	}
	
	for(int i = 0; i < TOTAL_READINGS; i++) {
		expected = worden807_expected.readings[i];
		actual = worden807_actual.readings[i];
		mu_assert_double_eq(expected, actual);
	}
	
	const char *purpose_expected = "laboratoire 1 - leve gravimetrique";
	const char *purpose_actual = worden807_actual.survey_purpose;
	mu_assert_string_eq(purpose_expected, purpose_actual);
	
	const char *area_expected = "les plaines d’abraham - quebec";
	const char *area_actual = worden807_actual.survey_area;
	mu_assert_string_eq(area_expected, area_actual);
	
	const char *poi_expected = "reservoir d'eau municipal de la ville de quebec";
	const char *poi_actual = worden807_actual.survey_poi;
	mu_assert_string_eq(poi_expected, poi_actual);
	
	const char *address_expected = "\"555 plaines abraham, quebec, canada, g2j 5h6\"";
	const char *address_actual = worden807_actual.survey_address;
	mu_assert_string_eq(address_expected, address_actual);
	
	const char *date_expected = "1997-12-09";
	const char *date_actual = worden807_actual.survey_date;
	mu_assert_string_eq(date_expected, date_actual);
	
	double temp_expected = 74.0;
	double temp_actual = worden807_actual.operation_temp;
	mu_assert_double_eq(temp_expected, temp_actual);
	
	double lat_expected = 46.8;
	double lat_actual = worden807_actual.ref_station_lat;
	mu_assert_double_eq(lat_expected, lat_actual);
	
	double dir_expected = 34.25;
	double dir_actual = worden807_actual.survey_dir;
	mu_assert_double_eq(dir_expected, dir_actual);

	char temp_unit_expected = 'f';
	char temp_unit_actual = worden807_actual.operation_temp_unit;
	mu_assert_int_eq(temp_unit_expected, temp_unit_actual);
}



/*
 * tests - store_avg_readings
 *
 */
MU_TEST(test_assert_store_avg_readings)
{
	double expected[] = {
		1802.45, 1802.45, 1802.325, 
		1801.3, 1800.825, 1801.1
	};

	for(int i = 0; i < ROWS; i++)
		worden807_expected.avg_readings[i] = expected[i];

	store_avg_readings_std(&worden807_actual, 0);

	for(int i = 0; i < ROWS; i++)		
		mu_assert(approx_eq(worden807_expected.avg_readings[i], worden807_actual.avg_readings[i], EPSILON), "unexpected average in average readings");	
}



/*
 * tests - store_std
 *
 */
MU_TEST(test_assert_store_std)
{
	double expected[] = {
		0.22912878474777, 0.111803398874964, 0.043301270189281,
		0.273861289, 0.163935963107496, 0.070710678118671
	};

	for(int i = 0; i < ROWS; i++)
		worden807_expected.std[i] = expected[i];
	
	store_avg_readings_std(&worden807_actual, 1);

	for(int i = 0; i < ROWS; i++)
		mu_assert(approx_eq(worden807_expected.std[i], worden807_actual.std[i], EPSILON), "unexpected std value");
}



/*
 * tests - dial_const_worden807
 *
 */
MU_TEST(test_assert_dial_const_worden807)
{
	double dial_expected = 0.4053373;

	double dial_actual = dial_const_worden807(&worden807_actual);

	mu_assert(approx_eq(dial_expected, dial_actual, EPSILON), "dial const incorrect");
}



MU_TEST(test_assert_dial_const_worden807_invalid)
{
	double dial_expected = INVALID_TEMP;

	char tmp = worden807_actual.operation_temp_unit;
	worden807_actual.operation_temp_unit = 'v';
	double dial_actual = dial_const_worden807(&worden807_actual);
	worden807_actual.operation_temp_unit = tmp;

	mu_assert_double_eq(dial_expected, dial_actual);
}



MU_TEST_SUITE(test_suite)
{
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
	
	MU_RUN_TEST(test_assert_assign_idx_node);
	MU_RUN_TEST(test_assert_load_grav_csv);
	MU_RUN_TEST(test_assert_store_avg_readings);
	MU_RUN_TEST(test_assert_store_std);
	MU_RUN_TEST(test_assert_dial_const_worden807);
	MU_RUN_TEST(test_assert_dial_const_worden807_invalid);
}

 
 
int main(void)
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return 0;
}

