#include "../src/topo.h"
#include "../src/utils/calcs.h"
#include "test.h"

#define TOPO_FILE "./test_data/topo1.csv"
#define ROWS 14
#define STATION_NUM_BEFORE_RETURN_TO_REF 11

struct topo_t topo_expected;
struct topo_t topo_actual;

void test_setup(void)
{
	if(alloc_topo(&topo_expected, ROWS) == -1)
		return;

	double stations[] = {
		  0.0,  20.0,  40.0,  60.0, 
		 80.0, 100.0, 120.0, 140.0, 
		160.0, 180.0, 200.0, 220.0,
		240.0, 260.0
	};

	int stations_numeric_order[] = {
		0, 1, 2, 3, 4, 5, 6, 7,
		8, 9, 10, 11, 12, 13
	};

	double zeros[] = {
		0.000, 0.048, 0.045, 0.039, 
		0.044, 0.039, 0.013, 0.032,
	       	0.044, 0.019, 0.043, 0.046, 
		0.044, 0.040	
	};

	double zeros_quality[] = {
		0.001, 0.005, 0.002, 0.002, 
		0.003, 0.003, 0.006, 0.008,
		0.003, 0.002, 0.001, 0.002,
		0.002, 0.002
	};

	double elevation_diff[] = {
	         0.000, -0.247,  0.265, -2.184, 
		-1.189, -0.306, -0.103, -1.785,
	       	-0.188,  0.123, -0.101,  0.121, 
		 0.009,  0.000
	};

	double elevation_diff_quality[] = {
		0.000, 0.003, 0.002, 0.001, 
		0.002, 0.002, 0.003, 0.003, 
		0.005, 0.002, 0.002, 0.002, 
		0.002, 0.003	
	};

	const char *survey_purpose = "laboratoire 1 - leve topographique au niveau electronique";
	const char *survey_area = "\"les plaines d'abraham, quebec\"";
	const char *survey_poi = "reservoir d'eau municipal de la ville de quebec";
	const char *survey_address = " ";
	const char *survey_date = "1997-12-09";

	memcpy(topo_expected.stations, stations, sizeof(stations));
	memcpy(topo_expected.stations_numeric_order, stations_numeric_order, sizeof(stations_numeric_order));
	memcpy(topo_expected.zeros, zeros, sizeof(zeros));
	memcpy(topo_expected.zeros_quality, zeros_quality, sizeof(zeros_quality));
	memcpy(topo_expected.elevation_diff, elevation_diff, sizeof(elevation_diff));
	memcpy(topo_expected.elevation_diff_quality, elevation_diff_quality, sizeof(elevation_diff_quality));

	topo_expected.survey_purpose = (char*)survey_purpose;
	topo_expected.survey_area = (char*)survey_area;
	topo_expected.survey_poi = (char*)survey_poi;
	topo_expected.survey_address = (char*)survey_address;
	topo_expected.survey_date = (char*)survey_date;

	topo_expected.num_lines = ROWS;
	topo_expected.station_num_before_return_to_ref = STATION_NUM_BEFORE_RETURN_TO_REF;
}



/*
 * tests - assign_idx_node_topo
 *
 */
MU_TEST(test_assert_assign_idx_node_topo)
{
	FILE *fp = fopen(TOPO_FILE, "rb");

	struct list_t *headers_expected = create_list();

	const char *station           = "station (m)";
	const char *zero              = "zero (m)";
	const char *zero_quality      = "zero quality";
	const char *elev_diff         = "elevation difference (m)";
	const char *elev_diff_quality = "elevation difference quality";
	const char *purpose           = "purpose";
	const char *area              = "area";
	const char *poi               = "poi";
	const char *address           = "address";
	const char *date              = "date of survey";

	add_head_list(headers_expected, station);
	add_head_list(headers_expected, zero);
	add_head_list(headers_expected, zero_quality);
	add_head_list(headers_expected, elev_diff);
	add_head_list(headers_expected, elev_diff_quality);
	add_head_list(headers_expected, purpose);
	add_head_list(headers_expected, area);
	add_head_list(headers_expected, poi);
	add_head_list(headers_expected, address);
	add_head_list(headers_expected, date);

	struct node_t *curr_expect = headers_expected->head;
	for( ; curr_expect != NULL; curr_expect = curr_expect->next) {
		if(strcmp(curr_expect->data, station) == 0)
			curr_expect->idx = IDX_STATION;
		else if(strcmp(curr_expect->data, zero) == 0)
			curr_expect->idx = IDX_ZERO;
		else if(strcmp(curr_expect->data, zero_quality) == 0)
			curr_expect->idx = IDX_ZERO_QUALITY;
		else if(strcmp(curr_expect->data, elev_diff) == 0)
			curr_expect->idx = IDX_ELEV_DIFF;
		else if(strcmp(curr_expect->data, elev_diff_quality) == 0)
			curr_expect->idx = IDX_ELEV_DIFF_QUALITY;
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
	}

	const char delim = determine_delim(fp);
	struct list_t *headers_actual = parse_header(fp, delim);
	assign_idx_node_topo(headers_actual);

	curr_expect = headers_expected->head;
	struct node_t *curr_actual = headers_actual->head;

	for( ; curr_expect != NULL; curr_expect = curr_expect->next) {
		curr_actual = headers_actual->head;

		for( ; curr_actual != NULL; curr_actual = curr_actual->next) {
			if(strcmp(curr_expect->data, curr_actual->data) == 0)
				break;
		}
	}

	delete_list(headers_expected);
	delete_list(headers_actual);

	fclose(fp);
}



void test_teardown(void)
{
	free_topo(&topo_expected);
}



/*
 * tests - load_topo_csv
 *
 */
MU_TEST(test_assert_load_topo_csv)
{
	int status_expected = 0;

	int status_actual = load_topo_csv(&topo_actual, TOPO_FILE);
	mu_assert_int_eq(status_expected, status_actual);

	double e, a;
	for(int i = 0; i < ROWS; i++) {
		e = topo_expected.stations[i];
		a = topo_actual.stations[i];
		mu_assert_double_eq(e, a);

		e = topo_expected.zeros[i];
		a = topo_actual.zeros[i];
		mu_assert_double_eq(e, a);

		e = topo_expected.zeros_quality[i];
		a = topo_actual.zeros_quality[i];
		mu_assert_double_eq(e, a);

		e = topo_expected.elevation_diff[i];
		a = topo_actual.elevation_diff[i];
		mu_assert_double_eq(e, a);

		e = topo_expected.elevation_diff_quality[i];
		a = topo_expected.elevation_diff_quality[i];
		mu_assert_double_eq(e, a);	
	}

	int num_lines_expected = (int)topo_expected.num_lines;
	int num_lines_actual = (int)topo_actual.num_lines;
	mu_assert_int_eq(num_lines_expected, num_lines_actual);

	const char *survey_purpose_expected = "laboratoire 1 - leve topographique au niveau electronique";
	const char *survey_purpose_actual = topo_actual.survey_purpose;
	mu_assert_string_eq(survey_purpose_expected, survey_purpose_actual);

	const char *survey_area_expected = "\"les plaines d'abraham, quebec\"";
	const char *survey_area_actual = topo_actual.survey_area;
	mu_assert_string_eq(survey_area_expected, survey_area_actual);

	const char *survey_poi_expected = "reservoir d'eau municipal de la ville de quebec";
	const char *survey_poi_actual = topo_actual.survey_poi;
	mu_assert_string_eq(survey_poi_expected, survey_poi_actual);

	const char *survey_address_expected = "";
	const char *survey_address_actual = topo_actual.survey_address;
	mu_assert_string_eq(survey_address_expected, survey_address_actual);

	const char *survey_date_expected = "1997-12-09";
	const char *survey_date_actual = topo_actual.survey_date;
	mu_assert_string_eq(survey_date_expected, survey_date_actual);
}



/*
 * tests - store_elevation_diff_corr
 *
 */
MU_TEST(test_assert_store_elevation_diff_corr)
{
	double expected[] = {
		 0.000, -0.247,  0.217, -2.229,
		-1.228, -0.350, -0.142, -1.798,
		-0.220,  0.079, -0.120,  0.078,
		-0.037, -0.044
	};

	memcpy(topo_expected.elevation_diff_corr, expected, sizeof(expected));

	store_elevation_diff_corr(&topo_actual);

	const char *msg = "error in elevation difference correction";
	for(int i = 0; i < ROWS; i++) {
		double e = topo_expected.elevation_diff_corr[i];
		double a = topo_actual.elevation_diff_corr[i];
		
		mu_assert(approx_eq(e, a, EPSILON), msg);
	}
}



/*
 * tests - store_elevation_cmp_ref
 *
 */
MU_TEST(test_assert_store_elevation_cmp_ref)
{
	double expected[] = {
		0.000, 0.247, 0.030, 2.259,
		3.487, 3.837, 3.979, 5.777,
		5.997, 5.918, 6.038, 5.960,
		5.997, 6.041		
	};

	memcpy(topo_expected.elevation_cmp_ref, expected, sizeof(expected));

	store_elevation_cmp_ref(&topo_actual);

	const char *msg = "error when calculating elevations compared to the reference station";
	for(int i = 0; i < ROWS; i++) {
		double e = topo_expected.elevation_cmp_ref[i];
		double a = topo_actual.elevation_cmp_ref[i];

		mu_assert(approx_eq(e, a, EPSILON), msg);
	}
}



/*
 * tests - store_err_dist_btwn_stations_m
 *
 */
MU_TEST(test_assert_store_err_dist_btwn_stations_m)
{
	double expected[] = {
		 0.000000000000000, -0.232346153846154, -0.464692307692308,
		-0.697038461538461, -0.929384615384615, -1.161730769230770,
	       	-1.394076923076920, -1.626423076923080, -1.858769230769230, 
		-2.091115384615380, -2.323461538461540, -2.55580769230769,
	       	-2.78815384615385, -3.020500000000000
	};

	memcpy(topo_expected.err_dist_btwn_stations_m, expected, sizeof(expected));

	store_err_dist_btwn_stations(&topo_actual);

	const char *msg = "error when calculating the error distribution between stations";
	for(int i = 0; i < ROWS; i++) {
		double e = topo_expected.err_dist_btwn_stations_m[i];
		double a = topo_actual.err_dist_btwn_stations_m[i];
		
		mu_assert(approx_eq(e, a, EPSILON), msg);
	}
}



/*
 * tests - store_elevation_corr
 *
 */
MU_TEST(test_assert_store_elevation_corr)
{
	double expected[] = {
		0.0000000000000000, 0.0146538461538460, -0.434692307692308,
		1.5619615384615400, 2.5576153846153800,  2.6752692307692300,
		2.5849230769230800, 4.1505769230769200,  4.1382307692307700,
		3.8268846153846200, 3.7145384615384600,  3.4041923076923100,
		3.2088461538461500, 3.0205000000000000
	};

	memcpy(topo_expected.elevation_corr, expected, sizeof(expected));

	store_elevation_corr(&topo_actual);

	const char *msg = "error when correcting the elevation";
	for(int i = 0; i < ROWS; i++) {
		double e = topo_expected.elevation_corr[i];
		double a = topo_actual.elevation_corr[i];

		mu_assert(approx_eq(e, a, EPSILON), msg);
	}
}



/*
 * tests - store_altitude
 *
 */
MU_TEST(test_assert_store_altitudes)
{
	topo_actual.station_num_before_return_to_ref = topo_expected.station_num_before_return_to_ref;

	double expected[] = {
		95.29, 95.30, 94.85,
		96.85, 97.84, 97.96,
		97.87, 99.44, 99.42,
		99.11, 99.00, 98.69,
		98.49, 98.31
	};

	memcpy(topo_expected.altitudes, expected, sizeof(expected));

	store_altitudes(&topo_actual);

	const char *msg = "error when determining altitude";
	for(int i = 0; i < ROWS; i++) {
		double e = topo_expected.altitudes[i];
		double a = topo_expected.altitudes[i];

		mu_assert(approx_eq(e, a, EPSILON), msg);
	}
}



MU_TEST_SUITE(test_suite)
{
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

	MU_RUN_TEST(test_assert_assign_idx_node_topo);
	MU_RUN_TEST(test_assert_load_topo_csv);
	MU_RUN_TEST(test_assert_store_elevation_diff_corr);
	MU_RUN_TEST(test_assert_store_elevation_cmp_ref);
	MU_RUN_TEST(test_assert_store_err_dist_btwn_stations_m);
	MU_RUN_TEST(test_assert_store_elevation_corr);
	MU_RUN_TEST(test_assert_store_altitudes);
}



int main(void)
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return 0;
}
