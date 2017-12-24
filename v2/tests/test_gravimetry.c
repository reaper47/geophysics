#include "../src/gravimetry.h"
#include "test.h"

#define GRAV_FILE "./test_data/grav.csv"
#define ROWS 6
#define TOTAL_READINGS 24

struct worden807_t worden807_expected;

void test_setup(void)
{
	if(alloc_worden807(&worden807_expected, ROWS) == -1)
		return;
	
	float stations[] = { 
		0.0f, 20.0f, 40.0f, 
		60.0f, 80.0f, 100.0f 
	};
	
	float times[] = { 
		0.580555556f, 0.588194444f, 0.597222222f,
		0.604166667f, 0.611111111f, 0.622916667f
	};
	
	float times_min[] = {
		0.0f, 11.0f, 24.0f,
		34.0f, 44.0f, 61.0f
	};
	
	float readings[] = {
		1802.8f, 1802.5f, 1802.3f, 1802.2f,
		1802.3f, 1802.5f, 1802.6f, 1802.4f,
		1802.4f, 1802.3f, 1802.3f, 1802.3f,
		1801.0f, 1801.4f, 1801.7f, 1801.1f,
		1800.7f, 1801.1f, 1800.7f, 1800.8f,
		1801.1f, 1801.0f, 1801.2f, 1801.1f
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
	
	worden807_expected.survey_purpose = (char*)purpose;
	worden807_expected.survey_area = (char*)area;
	worden807_expected.survey_poi = (char*)poi;
	worden807_expected.survey_address = (char*)addr;
	worden807_expected.survey_date = (char*)date;
	worden807_expected.operation_temp = 74.0f;
	worden807_expected.ref_station_lat = 46.8f;
	worden807_expected.survey_dir = 34.25f;
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
	assign_idx_node(headers_actual);
	
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
	struct worden807_t worden807_actual;

	int status_actual = load_grav_csv(&worden807_actual, GRAV_FILE);
	mu_assert_int_eq(status_expected, status_actual);
	
	float expected, actual;
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
	
	float temp_expected = 74.0;
	float temp_actual = worden807_actual.operation_temp;
	mu_assert_double_eq(temp_expected, temp_actual);
	
	float lat_expected = 46.8f;
	float lat_actual = worden807_actual.ref_station_lat;
	mu_assert_double_eq(lat_expected, lat_actual);
	
	float dir_expected = 34.25;
	float dir_actual = worden807_actual.survey_dir;
	mu_assert_double_eq(dir_expected, dir_actual);
}


 
 
MU_TEST_SUITE(test_suite)
{
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
	
	MU_RUN_TEST(test_assert_assign_idx_node);
	MU_RUN_TEST(test_assert_load_grav_csv);
}

 
 
int main(void)
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return 0;
}
