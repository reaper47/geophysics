#include "../src/gravimetry.h"
#include "../src/utils/calcs.h"
#include "test.h"

#define GRAV_FILE "./test_data/grav.csv"
#define TOPO_FILE "./test_data/topo1.csv"

#define ROWS           13
#define ROWS_TOPO      14
#define TOTAL_READINGS 52 

struct worden807_t worden807_expected;
struct worden807_t worden807_actual;

struct worden807_t worden807_expected_populate;
struct worden807_t worden807_actual_populate;

struct topo_t topo;

void test_setup(void)
{
	if(alloc_worden807(&worden807_expected, ROWS) == -1)
	    return;

        if(alloc_worden807(&worden807_expected_populate, ROWS) == -1)
            return;
	
	double stations[] = { 
		0.0, 20.0, 40.0, 
		60.0, 80.0, 100.0,
		120.0, 140.0, 160.0, 
		180.0, 200.0, 220.0, 
		0.0
	};
	
	double times[] = { 
		0.580555556, 0.588194444, 0.597222222,
		0.604166667, 0.611111111, 0.622916667,
		0.630555556, 0.636111111, 0.642361111,
		0.647222222, 0.650694444, 0.654861111,
		0.663888889
	};
	
	double times_min[] = {
		0.0, 11.0, 24.0,
		34.0, 44.0, 61.0,
		72.0, 80.0, 89.0,
		96.0, 101.0, 107.0,
		120.0
	};
	
	double readings[] = {
		1802.8, 1802.5, 1802.3, 1802.2,
		1802.3, 1802.5, 1802.6, 1802.4,
		1802.4, 1802.3, 1802.3, 1802.3,
		1801.0, 1801.4, 1801.7, 1801.1,
		1800.7, 1801.1, 1800.7, 1800.8,
		1801.1, 1801.0, 1801.2, 1801.1,
		1801.0,	1801.1,	1801.0,	1801.0,
		1799.5,	1799.6,	1799.5,	1799.5,
		1798.8,	1799.0,	1799.1,	1799.1,
		1798.8,	1799.1,	1799.0,	1799.1,
		1798.9,	1799.0,	1799.1,	1799.0,
		1799.3,	1799.1,	1799.1,	1799.3,
		1802.6,	1802.4,	1802.6,	1802.3

	};
	
	const char *purpose = "LABORATOIRE 1 - LEVE GRAVIMETRIQUE";
	const char *area = "LES PLAINES D’ABRAHAM - QUEBEC";
	const char *poi = "RESERVOIR D'EAU MUNICIPAL DE LA VILLE DE QUEBEC";
	const char *addr = "\"555 plaines abraham, quebec, canada, g2j 5h6\"";
	const char *date = "1997-12-09";
	
        /* worden807_expected */
	memcpy(worden807_expected.stations, stations, sizeof(stations));
	memcpy(worden807_expected.times, times, sizeof(times));
	memcpy(worden807_expected.times_min, times_min, sizeof(times_min));
	memcpy(worden807_expected.readings, readings, sizeof(readings));

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
        worden807_expected.topo_file = (char*)TOPO_FILE;

        /* worden807_expected_populate */
	memcpy(worden807_expected_populate.stations, stations, sizeof(stations));
	memcpy(worden807_expected_populate.times, times, sizeof(times));
	memcpy(worden807_expected_populate.times_min, times_min, sizeof(times_min));
	memcpy(worden807_expected_populate.readings, readings, sizeof(readings));

	worden807_expected_populate.num_readings = 4;	
	worden807_expected_populate.survey_purpose = (char*)purpose;
	worden807_expected_populate.survey_area = (char*)area;
	worden807_expected_populate.survey_poi = (char*)poi;
	worden807_expected_populate.survey_address = (char*)addr;
	worden807_expected_populate.survey_date = (char*)date;
	worden807_expected_populate.operation_temp = 74.0;
	worden807_expected_populate.ref_station_lat = 46.8;
	worden807_expected_populate.survey_dir = 34.25;
	worden807_expected_populate.operation_temp_unit = 'F';
        worden807_expected_populate.topo_file = (char*)TOPO_FILE;
}




void test_teardown(void)
{	
	//free_worden807(&worden807_expected);
        free_worden807(&worden807_expected_populate);
	//free_topo(&topo);
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
        assign_idx_node(headers_actual, &worden807_expected_populate);
	
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




/*
 * tests - load_grav_csv
 *
 */ 
MU_TEST(test_assert_load_grav_csv)
{
	int status_expected = 0;

	int status_actual = load_grav_csv(&worden807_actual, GRAV_FILE, TOPO_FILE);
        int status_actual_populate = load_grav_csv(&worden807_actual_populate, GRAV_FILE, TOPO_FILE);
	mu_assert_int_eq(status_expected, status_actual);
        mu_assert_int_eq(status_expected, status_actual_populate);
	
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

	unsigned int num_lines_expected = ROWS;
	unsigned int num_lines_actual = worden807_actual.num_lines;
	mu_assert_int_eq((int)num_lines_expected, (int)num_lines_actual);

        const char *topo_file_expected = TOPO_FILE;
        const char *topo_file_actual = worden807_actual.topo_file;
        mu_assert_string_eq(topo_file_expected, topo_file_actual);
}



/*
 * tests - store_avg_readings
 *
 */
MU_TEST(test_assert_store_avg_readings)
{
	double expected[] = {
		1802.45, 1802.45, 1802.325, 
		1801.3, 1800.825, 1801.1,
		1801.025, 1799.525, 1799.0,
		1799.0, 1799.0, 1799.2,
		1802.475
	};
	
	memcpy(worden807_expected.avg_readings, expected, sizeof(expected));

	store_avg_readings_std(&worden807_actual, 0);

	const char *msg = "unexpected average in average readings";
	for(int i = 0; i < ROWS; i++) {
		double e = worden807_expected.avg_readings[i];
	       	double a = worden807_actual.avg_readings[i];

		mu_assert(approx_eq(e, a, EPSILON), msg);
	}

}



/*
 * tests - store_std
 *
 */
MU_TEST(test_assert_store_std)
{
	double expected[] = {
		0.229128784747770, 0.111803398874964, 0.043301270189281,
		0.273861278752625, 0.163935963107496, 0.070710678118671,
		0.043301270189183, 0.043301270189183, 0.122474487139140,
		0.122474487139140, 0.070710678118591, 0.100000000000023,
		0.129903810567624
	};

	memcpy(worden807_expected.std, expected, sizeof(expected));
	
	store_avg_readings_std(&worden807_actual, 1);

	const char *msg = "unexpected std value";
	for(int i = 0; i < ROWS; i++) {
		double e = worden807_expected.std[i];
		double a = worden807_actual.std[i];
		
		mu_assert(approx_eq(e, a, EPSILON), msg);
	}
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



/*
 * tests - store_relative_grav_field
 *
 */
MU_TEST(test_assert_store_rel_grav_fields)
{
	double grav_expected[] = {
	        730.6002163850, 730.6002163850, 730.5495492225,
		730.1340784900, 729.9415432725, 730.0530110300, 
		730.0226107325, 729.4146047825, 729.2018027000,
		729.2018027000, 729.2018027000, 729.2828701600, 
		730.6103498175
	};

	memcpy(worden807_expected.rel_grav_fields, grav_expected, sizeof(grav_expected));

	store_rel_grav_fields(&worden807_actual);

	const char *msg = "error when calculating relative gravitational fields";
	for(int i = 0; i < ROWS; i++) {
		double e = worden807_expected.rel_grav_fields[i];
		double a = worden807_actual.rel_grav_fields[i];
		
		mu_assert(approx_eq(e, a, EPSILON), msg);
	}
}



/*
 * tests - store_grav_anomaly_notcorr
 *
 */
MU_TEST(test_assert_store_grav_anomaly_notcorr)
{
	double grav_expected[] = {
		0.000000000000000, 0.000000000000000, 0.050667162499963,
		0.466137895000088, 0.658673112499969, 0.547205355000074,
		0.577605652499983, 1.185611602499990, 1.398413685000040,
		1.398413685000040, 1.3984136850000400, 1.31734622500005000,
		-0.01013343249997	
	};

	memcpy(worden807_expected.grav_anomaly_notcorr, grav_expected, sizeof(grav_expected));

	store_grav_anomaly_notcorr(&worden807_actual);

	const char *msg = "error when calculating uncorrected gravimetric anomalies"; 
	for(int i = 0; i < ROWS; i++) {
		double e = worden807_expected.grav_anomaly_notcorr[i];
		double a = worden807_actual.grav_anomaly_notcorr[i];
		mu_assert(approx_eq(e, a, EPSILON), msg);
	}
}



/*
 * tests - store_temporal_vars
 *
 */
MU_TEST(test_assert_store_temporal_vars)
{
	double expected[] = {
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.010133433333294
	};

	memcpy(worden807_expected.temporal_vars, expected, sizeof(expected));

	store_temporal_vars(&worden807_actual);

	const char *msg = "error when calculating temporal variations of ref stations";
	for(int i = 0; i < ROWS; i++) {
		double e = worden807_expected.temporal_vars[i];
		double a = worden807_actual.temporal_vars[i];
		mu_assert(approx_eq(e, a, EPSILON), msg);
	}
}



/*
 * tests - store attraction_deviation
 *
 */
MU_TEST(test_assert_store_attraction_deviation)
{
	double expected[] = { 
		-0.000000000000000, -0.000844452777775, -0.001688905555549,
		-0.002533358333324, -0.003377811111098, -0.004222263888873,
		-0.005066716666647, -0.005911169444422, -0.006755622222196, 
		-0.007600074999971, -0.008444527777745, -0.009288980555520,
		-0.010133433333294	
	};

	memcpy(worden807_expected.attraction_deviation, expected, sizeof(expected));
	
	store_attraction_deviation(&worden807_actual);

	const char *msg = "attraction deviations do not match";
	for(int i = 0; i < ROWS; i++) {
		double e = worden807_expected.attraction_deviation[i];
		double a = worden807_actual.attraction_deviation[i];
		
		mu_assert(approx_eq(e, a, EPSILON), msg);
	}
}



/*
 * tests - store_lat_corr
 *
 */
MU_TEST(test_assert_store_lat_corr)
{
	double lat_corr_expected[] = {
        	 0.0000000000000000, -0.013430162043108, -0.026860324086216,
		-0.0402904861293240, -0.053720648172432, -0.067150810215541,
		-0.0805809722586490, -0.094011134301757, -0.107441296344865,
		-0.1208714583879730, -0.134301620431081, -0.147731782474189,
		 0.0000000000000000
	};

	memcpy(worden807_expected.lat_corr, lat_corr_expected, sizeof(lat_corr_expected));

	store_lat_corr(&worden807_actual);

	const char *msg = "corrected latitudes do not match";
	for(int i = 0; i < ROWS; i++) {
		double e = worden807_expected.lat_corr[i];
		double a = worden807_actual.lat_corr[i];
		
		mu_assert(approx_eq(e, a, EPSILON), msg);
	}
}



/*
 * tests - determine_station_num_before_return_to_ref
 *
 */
MU_TEST(test_assert_set_station_num_before_return_to_ref)
{
	load_topo_csv(&topo, TOPO_FILE);
	
	int station_num_expected = 11;
	
	set_station_num_before_return_to_ref(&worden807_actual, &topo);
	int station_num_actual = topo.station_num_before_return_to_ref;

	mu_assert_int_eq(station_num_expected, station_num_actual);
}



/*
 * tests - transfer_topo_data_to_grav
 *
 */
MU_TEST(test_assert_transfer_topo_data_to_grav)
{	
	store_elevation_diff_corr(&topo);
	store_elevation_cmp_ref(&topo);

	topo.elevation_cmp_ref[ROWS_TOPO-1] = 0.200;
	topo.stations[ROWS_TOPO-1] = 500;
	store_err_dist_btwn_stations(&topo);
	topo.elevation_cmp_ref[ROWS_TOPO-1] = 6.041;
	topo.stations[ROWS_TOPO-1] = 260.0;
	topo.err_dist_btwn_stations_m[ROWS_TOPO-1] = -0.052;

	store_elevation_corr(&topo);
	store_altitudes(&topo);	
	
	double elev_expected[] = {
		-0.00, 0.243, 0.022, 
		2.247, 3.471, 3.817, 
		3.955, 5.749, 5.965, 
		5.882, 5.998, 5.916, 
		0.000
	};
 
	double alts_expected[] = {
		93.084, 93.327, 93.106, 
		95.331, 96.555, 96.901, 
		97.039, 98.833, 99.049, 
		98.966, 99.082, 99.000, 
		93.084

	};

	memcpy(worden807_expected.elevations, elev_expected, sizeof(elev_expected));
	memcpy(worden807_expected.altitudes, alts_expected, sizeof(alts_expected));

	transfer_topo_data_to_grav(&topo, &worden807_actual);

	for(int i = 0; i < ROWS; i++) {
		double ee = worden807_expected.elevations[i];
		double ae = worden807_actual.elevations[i];

		double ea = worden807_expected.altitudes[i];
		double aa = worden807_actual.altitudes[i];

		mu_assert_double_eq(ee, ae);
		mu_assert_double_eq(ea, aa);
	}
}



/*
 * tests - store_free_air_corr
 *
 */
MU_TEST(test_assert_store_free_air_corr)
{
	double expected[] = {
		28.7257224, 28.8007122, 28.7325116,
		29.4191466, 29.7968730, 29.9036486,
		29.9462354, 30.4998638, 30.5665214,
		30.5409076, 30.5767052, 30.5514000,
		28.7257224
	};

	memcpy(worden807_expected.free_air_corr, expected, sizeof(expected));

	store_free_air_corr(&worden807_actual);

	const char *msg = "error when calculating free air correction";
	for(int i = 0; i < ROWS; i++) {
		double e = worden807_expected.free_air_corr[i];
		double a = worden807_actual.free_air_corr[i];

		mu_assert(approx_eq(e, a, EPSILON), msg);
	}
}



/*
 * tests - store_bouguer_corr
 *
 */
MU_TEST(test_assert_store_bouguer_corr)
{
	double expected[] = {
		-9.367229088, -9.391682664, -9.369442992, 
		-9.593349192, -9.716522760, -9.751341432, 
		-9.765228648, -9.945762456, -9.967498968, 
		-9.959146512, -9.970819824, -9.962568000, 
		-9.367229088
	};

	memcpy(worden807_expected.bouguer_corr, expected, sizeof(expected));

	store_bouguer_corr(&worden807_actual);

	const char *msg = "error when calculating Bouguer correction";
	for(int i = 0; i < ROWS; i++) {
		double e = worden807_expected.bouguer_corr[i];
		double a = worden807_actual.bouguer_corr[i];

		mu_assert(approx_eq(e, a, EPSILON), msg);
	}
}



/*
 * tests - store_bouguer_rel_grav_fields
 *
 */
MU_TEST(test_assert_store_bouguer_rel_grav_fields)
{
	double expected[] = {
        	749.958709697000, 749.995815758957, 749.885757506414, 
		749.919585411871, 749.968172864328, 750.138167387784, 
		750.123036512241, 749.874694992198, 749.693383835655, 
		749.662692329612, 749.673386455569, 749.723970377526, 
		749.958709697000
	};

	memcpy(worden807_expected.bouguer_rel_grav_fields, expected, sizeof(expected));

	store_bouguer_rel_grav_fields(&worden807_actual);

	const char *msg = "error in calculating relative Bouguer gravitational fields";
	for(int i = 0; i < ROWS; i++) {
		double e = worden807_expected.bouguer_rel_grav_fields[i];
		double a = worden807_actual.bouguer_rel_grav_fields[i];

		mu_assert(approx_eq(e, a, EPSILON), msg);
	}
}



/*
 * tests - store_bouguer_anomaly
 *
 */
MU_TEST(test_assert_store_bouguer_anomaly)
{
	double expected[] = {
		 0.000000000000000, 0.037106061956934, -0.072952190586193, 
		-0.039124323129333, 0.009463113327681, 0.179457640784488, 
		 0.164326767241452, -0.084014802801803, -0.26532597644763, 
		-0.29601748738789, -0.285323351431001, -0.234739427474215, 
	         0.00000000000000
	};

	memcpy(worden807_expected.bouguer_anomaly, expected, sizeof(expected));

	store_bouguer_anomaly(&worden807_actual);

	const char *msg = "error in calculating Bouguer anomaly";
	for(int i = 0; i < ROWS; i++) {
		double e = worden807_expected.bouguer_anomaly[i];
		double a = worden807_actual.bouguer_anomaly[i];

		mu_assert(approx_eq(e, a, EPSILON), msg);
	}
}



/*
 * tests - store_regional_anomaly
 *
 */
MU_TEST(test_assert_store_regional_anomaly)
{
	double expected[] = {
		0.000000000000000, 0.027272727272727, 0.054545454545455, 
		0.081818181818182, 0.109090909090909, 0.136363636363636, 
		0.163636363636364, 0.190909090909091, 0.218181818181818, 
		0.245454545454545, 0.272727272727273, 0.300000000000000, 
		0.000000000000000
	};

	memcpy(worden807_expected.regional_anomaly, expected, sizeof(expected));

	store_regional_anomaly(&worden807_actual);

	const char *msg = "error in determining the regional anomaly";
	for(int i = 0; i < ROWS; i++) {
		double e = worden807_expected.regional_anomaly[i];
		double a = worden807_actual.regional_anomaly[i];

		mu_assert(approx_eq(e, a, EPSILON), msg);
	}
}



/*
 * tests - store_residual_anomaly
 *
 */
MU_TEST(test_assert_store_residual_anomaly)
{
	double expected[] = {
	  	 0.000000000000000,  0.009833334684207, -0.127497645131648, 
		-0.120942504947515, -0.099627795763228,  0.04309400920852, 
	 	 0.000690404105088, -0.274923893710894, -0.483507794629448, 
		-0.541472032842435, -0.558050624158274, -0.534739427474215, 
		 0.000000000000000
	};

	memcpy(worden807_expected.residual_anomaly, expected, sizeof(expected));

	store_residual_anomaly(&worden807_actual);

	const char *msg = "error in calculating the residual anomaly";
	for(int i = 0; i < ROWS; i++) {
		double e = worden807_expected.residual_anomaly[i];
		double a = worden807_actual.residual_anomaly[i];

		mu_assert(approx_eq(e, a, EPSILON), msg);
	}
}



/*
 * tests - populate_calc_fields_grav
 *
 */
MU_TEST(test_assert_populate_calc_fields_worden807)
{
    double expected_avg_readings[] = {
        1802.45, 1802.45, 1802.325, 
	1801.3, 1800.825, 1801.1,
	1801.025, 1799.525, 1799.0,
	1799.0, 1799.0, 1799.2,
	1802.475
    };

    double expected_std[] = {
        0.229128784747770, 0.111803398874964, 0.043301270189281,
	0.273861278752625, 0.163935963107496, 0.070710678118671,
	0.043301270189183, 0.043301270189183, 0.122474487139140,
	0.122474487139140, 0.070710678118591, 0.100000000000023,
	0.129903810567624
    };

    double expected_grav[] = {
	730.6002163850, 730.6002163850, 730.5495492225,
	730.1340784900, 729.9415432725, 730.0530110300, 
	730.0226107325, 729.4146047825, 729.2018027000,
	729.2018027000, 729.2018027000, 729.2828701600, 
	730.6103498175
    };

    double expected_grav_uncorr[] = {
	0.000000000000000, 0.000000000000000, 0.050667162499963,
	0.466137895000088, 0.658673112499969, 0.547205355000074,
	0.577605652499983, 1.185611602499990, 1.398413685000040,
	1.398413685000040, 1.3984136850000400, 1.31734622500005000,
	-0.01013343249997	
    };

    double expected_tmp[] = {
	0.0, 0.0, 0.0,
	0.0, 0.0, 0.0,
	0.0, 0.0, 0.0,
	0.0, 0.0, 0.0,
	0.010133433333294
    };

    double expected_attract[] = { 
	-0.000000000000000, -0.000844452777775, -0.001688905555549,
	-0.002533358333324, -0.003377811111098, -0.004222263888873,
	-0.005066716666647, -0.005911169444422, -0.006755622222196, 
	-0.007600074999971, -0.008444527777745, -0.009288980555520,
	-0.010133433333294	
    };

    double expected_lat[] = {
       	 0.0000000000000000, -0.013430162043108, -0.026860324086216,
	-0.0402904861293240, -0.053720648172432, -0.067150810215541,
	-0.0805809722586490, -0.094011134301757, -0.107441296344865,
	-0.1208714583879730, -0.134301620431081, -0.147731782474189,
	 0.0000000000000000
    };

    double expected_elev[] = {
	-0.00, 0.243, 0.022, 
	2.247, 3.471, 3.817, 
	3.955, 5.749, 5.965, 
	5.882, 5.998, 5.916, 
	0.000
    };
 
    double expected_alts[] = {
	93.084, 93.327, 93.106, 
	95.331, 96.555, 96.901, 
	97.039, 98.833, 99.049, 
	98.966, 99.082, 99.000, 
	93.084
    };

    double expected_free[] = {
	28.7257224, 28.8007122, 28.7325116,
	29.4191466, 29.7968730, 29.9036486,
	29.9462354, 30.4998638, 30.5665214,
	30.5409076, 30.5767052, 30.5514000,
	28.7257224
    };

    double expected_bcorr[] = {
        -9.962568, -9.964043, -9.918824, 
        -10.119751, -10.219946, -10.231786, 
        -10.222694, -10.380249, -10.379006, 
        -10.347675, -10.336369, -10.305139, 
        -9.962568
    };

    double expected_brel[] = {
        751.189108, 751.178726, 751.021179, 
        751.007518, 751.008617, 751.131123, 
        751.068503, 750.772673, 750.543873, 
        750.465693, 750.428898, 750.431993, 
        751.189108
    };
    
    double expected_banom[] = {
        0.000000, -0.010383, -0.167930, 
        -0.181590, -0.180492, -0.057986, 
        -0.120605, -0.416436, -0.645236, 
        -0.723416, -0.760210, -0.757115, 
        0.000000
    };

    double expected_ranom[] = {
        0.000000, 0.027273, 0.054545, 
        0.081818, 0.109091, 0.136364, 
        0.163636, 0.190909, 0.218182, 
        0.245455, 0.272727, 0.300000, 
        0.000000
    };

    double expected_res[] = {
        0.000000, -0.037655, -0.222475, 
        -0.263409, -0.289583, -0.194349, 
        -0.284242, -0.607345, -0.863417, 
        -0.968870, -1.032938, -1.057115, 
        0.000000
    };

    memcpy(worden807_expected_populate.avg_readings, expected_avg_readings, sizeof(expected_avg_readings));
    memcpy(worden807_expected_populate.std, expected_std, sizeof(expected_std));
    memcpy(worden807_expected_populate.rel_grav_fields, expected_grav, sizeof(expected_grav));
    memcpy(worden807_expected_populate.grav_anomaly_notcorr, expected_grav_uncorr, sizeof(expected_grav_uncorr));
    memcpy(worden807_expected_populate.temporal_vars, expected_tmp, sizeof(expected_tmp));
    memcpy(worden807_expected_populate.attraction_deviation, expected_attract, sizeof(expected_attract));
    memcpy(worden807_expected_populate.lat_corr, expected_lat, sizeof(expected_lat));
    memcpy(worden807_actual_populate.elevations, expected_elev, sizeof(expected_elev));
    memcpy(worden807_actual_populate.altitudes, expected_alts, sizeof(expected_alts));
    memcpy(worden807_expected_populate.free_air_corr, expected_free, sizeof(expected_free));
    memcpy(worden807_expected_populate.bouguer_corr, expected_bcorr, sizeof(expected_bcorr));
    memcpy(worden807_expected_populate.bouguer_rel_grav_fields, expected_brel, sizeof(expected_brel));
    memcpy(worden807_expected_populate.bouguer_anomaly, expected_banom, sizeof(expected_banom));
    memcpy(worden807_expected_populate.regional_anomaly, expected_ranom, sizeof(expected_ranom));
    memcpy(worden807_expected_populate.residual_anomaly, expected_res, sizeof(expected_res));

    populate_calc_fields_worden807(&worden807_actual_populate);

    double e_avg, a_avg;
    double e_std, a_std;
    double e_rel, a_rel;
    double e_grav_uncorr, a_grav_uncorr;
    double e_tmp, a_tmp;
    double e_attract, a_attract;
    double e_lat, a_lat;
    double e_free, a_free;
    double e_bcorr, a_bcorr;
    double e_brel, a_brel;
    double e_banom, a_banom;
    double e_ranom, a_ranom;
    double e_res, a_res;

    const char *msg_avg = "error populating avg_readings of the worden807 struct";
    const char *msg_std = "error populating std of the worden807_struct";
    const char *msg_rel = "error populating rel_grav_fields of the worden807 struct";
    const char *msg_grav_uncorr = "error populating grav_uncorr of the worden807 struct";
    const char *msg_tmp = "error populating temporal_vars of the worden807 struct";
    const char *msg_attract = "error populating attraction_deviation of the worden807 struct";
    const char *msg_lat = "error populating lat_corr of the worden807 struct";
    const char *msg_free = "error populating free_air_corr of the worden807 struct";
    const char *msg_bcorr = "error populating bouguer_corr of the worden807 struct";
    const char *msg_brel = "error populating bouguer_rel_grav_fields of the worden807 struct";
    const char *msg_banom = "error populating bouguer_anomaly of the worden807 struct";
    const char *msg_ranom = "error populating regional_anomaly of the worden807 struct";
    const char *msg_res = "error populating residual_anomaly of the worden807 struct";
    
    for(int i = 0; i < ROWS; i++) {
        e_avg = worden807_expected_populate.avg_readings[i];
        a_avg = worden807_actual_populate.avg_readings[i];
        mu_assert(approx_eq(e_avg, a_avg, EPSILON), msg_avg);

        e_std = worden807_expected_populate.std[i];
        a_std = worden807_actual_populate.std[i];
        mu_assert(approx_eq(e_std, a_std, EPSILON), msg_std);

        e_rel = worden807_expected_populate.rel_grav_fields[i];
        a_rel = worden807_actual_populate.rel_grav_fields[i];
        mu_assert(approx_eq(e_rel, a_rel, EPSILON), msg_rel);

        e_grav_uncorr = worden807_expected_populate.grav_anomaly_notcorr[i];
        a_grav_uncorr = worden807_actual_populate.grav_anomaly_notcorr[i];
        mu_assert(approx_eq(e_grav_uncorr, a_grav_uncorr, EPSILON), msg_grav_uncorr);

        e_tmp = worden807_expected_populate.temporal_vars[i];
        a_tmp = worden807_expected_populate.temporal_vars[i];
        mu_assert(approx_eq(e_tmp, a_tmp, EPSILON), msg_tmp);

        e_attract = worden807_expected_populate.attraction_deviation[i];
        a_attract = worden807_actual_populate.attraction_deviation[i];
        mu_assert(approx_eq(e_attract, a_attract, EPSILON), msg_attract);

        e_lat = worden807_expected_populate.lat_corr[i];
        a_lat = worden807_actual_populate.lat_corr[i];
        mu_assert(approx_eq(e_lat, a_lat, EPSILON), msg_lat);

        e_free = worden807_expected_populate.free_air_corr[i];
        a_free = worden807_expected_populate.free_air_corr[i];
        mu_assert(approx_eq(e_free, a_free, EPSILON), msg_free);

        e_bcorr = worden807_expected_populate.bouguer_corr[i];
        a_bcorr = worden807_actual_populate.bouguer_corr[i];
        mu_assert(approx_eq(e_bcorr, a_bcorr, EPSILON), msg_bcorr);

        e_brel = worden807_expected_populate.bouguer_rel_grav_fields[i];
        a_brel = worden807_actual_populate.bouguer_rel_grav_fields[i];
        mu_assert(approx_eq(e_brel, a_brel, EPSILON), msg_brel);

        e_banom = worden807_expected_populate.bouguer_anomaly[i];
        a_banom = worden807_actual_populate.bouguer_anomaly[i];
        mu_assert(approx_eq(e_banom, a_banom, 1e-4), msg_banom);

        e_ranom = worden807_expected_populate.regional_anomaly[i];
        a_ranom = worden807_actual_populate.regional_anomaly[i];
        mu_assert(approx_eq(e_ranom, a_ranom, 1e-4), msg_ranom);

        e_res = worden807_expected_populate.residual_anomaly[i];
        a_res = worden807_actual_populate.residual_anomaly[i];
        mu_assert(approx_eq(e_res, a_res, 1e-4), msg_res);
    }
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
	MU_RUN_TEST(test_assert_store_rel_grav_fields);
	MU_RUN_TEST(test_assert_store_grav_anomaly_notcorr);
	MU_RUN_TEST(test_assert_store_temporal_vars);
	MU_RUN_TEST(test_assert_store_attraction_deviation);
	MU_RUN_TEST(test_assert_store_lat_corr);
	MU_RUN_TEST(test_assert_set_station_num_before_return_to_ref);
	MU_RUN_TEST(test_assert_transfer_topo_data_to_grav);
	MU_RUN_TEST(test_assert_store_free_air_corr);
	MU_RUN_TEST(test_assert_store_bouguer_corr);
	MU_RUN_TEST(test_assert_store_bouguer_rel_grav_fields);
	MU_RUN_TEST(test_assert_store_bouguer_anomaly);
	MU_RUN_TEST(test_assert_store_regional_anomaly);
	MU_RUN_TEST(test_assert_store_residual_anomaly);
        MU_RUN_TEST(test_assert_populate_calc_fields_worden807);
}

 
 
int main(void)
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return 0;
}

