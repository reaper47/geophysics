#include "gravimetry.h"
#include "calcs.h"
#include "test.h"

#define GRAV_FILE "./test_data/grav.csv"
#define TOPO_FILE "./test_data/topo1.csv"

#define ROWS           13
#define ROWS_TOPO      14
#define TOTAL_READINGS 52 

#define ARBITRARY_NUM  -99999.0
#define SPACE          " "

void test_json_field(json_t *root, const char *which, const char *msg, double *expected, double epsilon);
void test_json_field_str_real(json_t *root, const char *which, const char *expected, double real, int mode);

struct worden807_t worden807_expected;
struct worden807_t worden807_actual;

struct worden807_t worden807_expected_populate;
struct worden807_t worden807_actual_populate;

struct topo_t topo;

void test_setup(void)
{
    if(alloc_worden807(&worden807_expected, ROWS) == -1)
        return;
	    
    if(alloc_worden807(&worden807_expected_populate, ROWS) == -1) {
        return;
    }
   	
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
        1801.0, 1801.1, 1801.0, 1801.0,
        1799.5, 1799.6, 1799.5, 1799.5,
        1798.8, 1799.0, 1799.1, 1799.1,
        1798.8, 1799.1, 1799.0, 1799.1,
        1798.9, 1799.0, 1799.1, 1799.0,
        1799.3, 1799.1, 1799.1, 1799.3,
        1802.6, 1802.4, 1802.6, 1802.3
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
    free_worden807(&worden807_expected);
    free_worden807(&worden807_expected_populate);
}



/*
 * tests - assign_idx_node
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
	
	del_list(headers_expected);
	del_list(headers_actual);
	
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
 * tests - store_avg_readings_std
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
 * tests - store_avg_readings_std
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
 * tests - store_rel_grav_fields
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
 * tests - store_grav_anomaly_uncorr
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

	memcpy(worden807_expected.grav_anomaly_uncorr, grav_expected, sizeof(grav_expected));

	store_grav_anomaly_uncorr(&worden807_actual);

	const char *msg = "error when calculating uncorrected gravimetric anomalies"; 
	for(int i = 0; i < ROWS; i++) {
		double e = worden807_expected.grav_anomaly_uncorr[i];
		double a = worden807_actual.grav_anomaly_uncorr[i];
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
 * tests - store_attraction_deviation
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
 * tests - set_station_num_before_return_to_ref
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
 * tests - populate_calc_fields_worden807
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

    double expected_free[] = {
        28.7257224, 28.8007122, 28.7325116,
        29.4191466, 29.7968730, 29.9036486,
        29.9462354, 30.4998638, 30.5665214,
        30.5409076, 30.5767052, 30.5514000,
        28.7257224
    };

    double expected_bcorr[] = {
        -9.619997, -9.621472, -9.576253, 
        -9.777181, -9.877375, -9.889215, 
        -9.880123, -10.037678, -10.036436, 
        -10.005104, -9.993799, -9.962568, 
        -9.619997
    };

    double expected_brel[] = {
        750.481145, 750.470763, 750.313216, 
        750.299555, 750.300654, 750.423160, 
        750.360540, 750.064710, 749.835910, 
        749.757730, 749.720935, 749.724030, 
        750.481145
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
    memcpy(worden807_expected_populate.grav_anomaly_uncorr, expected_grav_uncorr, sizeof(expected_grav_uncorr));
    memcpy(worden807_expected_populate.temporal_vars, expected_tmp, sizeof(expected_tmp));
    memcpy(worden807_expected_populate.attraction_deviation, expected_attract, sizeof(expected_attract));
    memcpy(worden807_expected_populate.lat_corr, expected_lat, sizeof(expected_lat));    
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

        e_grav_uncorr = worden807_expected_populate.grav_anomaly_uncorr[i];
        a_grav_uncorr = worden807_actual_populate.grav_anomaly_uncorr[i];
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



/*
 * tests - generate_grav_csv
 *
 */
MU_TEST(test_assert_generate_grav_csv)
{
    const char *csv_header_expected = 
        "Station (m),Time (hh:mm),Time (mm),Reading 1,Reading 2,Reading 3,"
        "Reading 4,Average reading (div),Standard deviation,"
        "Relative gravitational field (mGal),Uncorrected gravimetric "
        "anomaly (mGal),Temporal variations (mGal),Attraction/instrumental"
        " deviation (mGal),Latitude correction (mGal),Elevation (m),Altitude"
        " (m),Free air correction (mGal),Bouguer correction (mGal),Relative "
        "Bouguer gravitational field (mGal),Bouguer anomaly (mGal),Regional "
        "anomaly (mGal),Residual anomaly (mGal)";

    const char *csv_line1_expected =
        "0.0,0.580555556,0.0,1802.8,1802.5,1802.3,1802.2,1802.45,"
        "0.22912878474777,730.600216385,0.0,0.0,-0.0,0.0,-0.0,93.084,"
        "28.7257224,-9.367229088,749.958709697,0.0,0.0,0.0";

    const char *csv_line2_expected =
        "20.0,0.588194444,11.0,1802.3,1802.5,1802.6,1802.4,1802.45,"
        "0.111803398874964,730.600216385,0.0,0.0,-0.000844452777775,"
        "-0.013430162043108,0.243,93.327,28.8007122,-9.391682664,"
        "749.995815758957,0.037106061956934,0.027272727272727,0.009833334684207";   

    const char *csv_line3_expected =
        "40.0,0.597222222,24.0,1802.4,1802.3,1802.3,1802.3,1802.325,"
        "0.043301270189281,730.5495492225,0.050667162499963,0.0,"
        "-0.001688905555549,-0.026860324086216,0.022,93.106,28.7325116,"
        "-9.369442992,749.885757506414,-0.072952190586193,"
        "0.054545454545455,-0.127497645131648";   
    
    const char *csv_line4_expected =
        "60.0,0.604166667,34.0,1801.0,1801.4,1801.7,1801.1,1801.3,"
        "0.273861278752625,730.13407849,0.466137895000088,0.0,"
        "-0.002533358333324,-0.040290486129324,2.247,95.331,"
        "29.4191466,-9.593349192,749.919585411871,-0.039124323129333,"
        "0.081818181818182,-0.120942504947515";   
    
    const char *csv_line5_expected =
        "80.0,0.611111111,44.0,1800.7,1801.1,1800.7,1800.8,1800.825,"
        "0.163935963107496,729.9415432725,0.658673112499969,0.0,"
        "-0.003377811111098,-0.053720648172432,3.471,96.555,29.796873,"
        "-9.71652276,749.968172864328,0.009463113327681,"
        "0.109090909090909,-0.099627795763228";   
    
    const char *csv_line6_expected =
        "100.0,0.622916667,61.0,1801.1,1801.0,1801.2,1801.1,1801.1,"
        "0.070710678118671,730.05301103,0.547205355000074,0.0,"
        "-0.004222263888873,-0.067150810215541,3.817,96.901,29.9036486,"
        "-9.751341432,750.138167387784,0.179457640784488,"
        "0.136363636363636,0.04309400920852";   
    
    const char *csv_line7_expected =
        "120.0,0.630555556,72.0,1801.0,1801.1,1801.0,1801.0,1801.025,"
        "0.043301270189183,730.0226107325,0.577605652499983,0.0,"
        "-0.005066716666647,-0.080580972258649,3.955,97.039,29.9462354,"
        "-9.765228648,750.123036512241,0.164326767241452,"
        "0.163636363636364,0.000690404105088";   
    
    const char *csv_line8_expected =
        "140.0,0.636111111,80.0,1799.5,1799.6,1799.5,1799.5,1799.525,"
        "0.043301270189183,729.4146047825,1.18561160249999,0.0,"
        "-0.005911169444422,-0.094011134301757,5.749,98.833,30.4998638,"
        "-9.945762456,749.874694992198,-0.084014802801803,"
        "0.190909090909091,-0.274923893710894";   
    
    const char *csv_line9_expected =
        "160.0,0.642361111,89.0,1798.8,1799.0,1799.1,1799.1,1799.0,"
        "0.12247448713914,729.2018027,1.39841368500004,0.0,"
        "-0.006755622222196,-0.107441296344865,5.965,99.049,30.5665214,"
        "-9.967498968,749.693383835655,-0.26532597644763,"
        "0.218181818181818,-0.483507794629448";   
    
    const char *csv_line10_expected =
        "180.0,0.647222222,96.0,1798.8,1799.1,1799.0, 1799.1,1799.0,"
        "0.12247448713914,729.2018027,1.39841368500004,0.0,"
        "-0.007600074999971,-0.120871458387973,5.882,98.966,30.5409076,"
        "-9.959146512,749.662692329612,-0.29601748738789,"
        "0.245454545454545,-0.541472032842435";   
    
    const char *csv_line11_expected =
        "200.0,0.650694444,101.0,1798.9,1799.0,1799.1,1799.0,1799.0,"
        "0.070710678118591,729.2018027,1.39841368500004,0.0,"
        "-0.008444527777745,-0.134301620431081,5.998,99.082,30.5767052,"
        "-9.970819824,749.673386455569,-0.285323351431001,"
        "0.272727272727273,-0.558050624158274";   
    
    const char *csv_line12_expected =
        "220.0,0.654861111,107.0,1799.3,1799.1,1799.1,1799.3,1799.2,"
        "0.100000000000023,729.28287016,1.31734622500005,0.0,"
        "-0.00928898055552,-0.147731782474189,5.916,99.0,30.5514,"
        "-9.962568,749.723970377526,-0.234739427474215,0.3,-0.534739427474215";
    
    const char *csv_line13_expected =
        "0.0,0.663888889,120.0,1802.6, 1802.4,1802.6,1802.3,1802.475,"
        "0.129903810567624,730.6103498175,-0.01013343249997,"
        "0.010133433333294,-0.010133433333294,0.0,0.0,93.084,28.7257224,"
        "-9.367229088,749.958709697,0.0,0.0,0.0";

    char *csv_path = generate_grav_csv(&worden807_actual, GRAV_TEST_DIR);
    FILE *csv_fp = fopen(csv_path, "r");
    if(csv_fp == NULL) {
        printf("could not open %s\n", csv_path);
        exit(EXIT_FAILURE);
    }

    char *line = NULL;
    size_t len = 0;
    long read;
    int counter = 0;
    const char *str_to_cmp;

    while((read = getline(&line, &len, csv_fp)) != -1) {
        _Bool is_double = 1;
        size_t n = strlen(line);
        char *buffer = malloc(sizeof(char) * n+1);

        for(int i = 0; i < (int)n; i++)
            buffer[i] = line[i];
        buffer[(int)n-1] = '\0';

        switch(counter) {
            case 0:
                str_to_cmp = csv_header_expected;
                is_double = 0;
                break;
            case 1:
                str_to_cmp = csv_line1_expected;
                break;
            case 2:
                str_to_cmp = csv_line2_expected;
                break;
            case 3:
                str_to_cmp = csv_line3_expected;
                break;
            case 4:
                str_to_cmp = csv_line4_expected;
                break;
            case 5:
                str_to_cmp = csv_line5_expected;
                break;
            case 6:
                str_to_cmp = csv_line6_expected;
                break;
            case 7:
                str_to_cmp = csv_line7_expected;
                break;
            case 8:
                str_to_cmp = csv_line8_expected;
                break;
            case 9:
                str_to_cmp = csv_line9_expected;
                break;
            case 10:
                str_to_cmp = csv_line10_expected;
                break;
            case 11:
                str_to_cmp = csv_line11_expected;
                break;
            case 12:
                str_to_cmp = csv_line12_expected;
                break;
            case 13:
                str_to_cmp = csv_line13_expected;
                break;
            default:
                str_to_cmp = csv_header_expected;
                break;
        }
        
        struct list_t *parsed_buffer = parse_line(buffer, ',');
        struct list_t *parsed_str = parse_line(str_to_cmp, ',');

        struct node_t *node1 = parsed_buffer->head;
        struct node_t *node2 = parsed_str->head;

        double e_double, a_double;
        const char *msg = "data difference in .csv file";

        while(node1 != NULL) {
            if(is_double) {
                e_double = atof(node1->data);
                a_double = atof(node2->data);

                mu_assert(approx_eq(e_double, a_double, 1e-3), msg);
            } else 
                mu_assert_string_eq(node1->data, node2->data);

            node1 = node1->next;
            node2 = node2->next;            
        }

        del_list(parsed_str);
        del_list(parsed_buffer);
        
        free(buffer);
        counter++;
    }

    fclose(csv_fp);
    free(csv_path);
}




/*
 * tests - genereate_grav_json
 *
 */
void test_json_field(json_t *root, const char *which, const char *msg, double *expected, double epsilon)
{
    size_t num_data = 0;
    
    json_t *field = NULL;
    json_unpack(root, "{s:o}", which, &field);

    if(field && json_is_array(field))
        num_data = json_array_size(field);
        
    for(size_t i = 0; i < num_data; i++) {
        double e = expected[i];
        double a = json_real_value(json_array_get(field, i));

        mu_assert(approx_eq(e, a, epsilon), msg);
    }
}

void test_json_field_str_real(json_t *root, const char *which, const char *expected, double real, int mode)
{
    json_t *field = NULL;
    json_unpack(root, "{s:o}", which, &field);

    if(mode)
        mu_assert_string_eq(expected, json_string_value(field));
    else
        mu_assert_double_eq(real, json_real_value(field));
}
    
MU_TEST(test_assert_generate_grav_json_all)
{ 
    const double ep3 = 1e-3;
    const double ep4 = 1e-4;

    double stations_expected[] = { 
        0.0, 20.0, 40.0, 60.0, 80.0, 100.0, 120.0,
        140.0, 160.0, 180.0, 200.0, 220.0, 0.0
    };
   
    double times_expected[] = { 
        0.580556, 0.588194, 0.597222, 0.604167, 0.611111,
        0.622917, 0.630556, 0.636111, 0.642361, 0.647222,
        0.650694, 0.654861, 0.663889
    };
    
    double times_min_expected[] = {
        0.000000, 11.000000, 24.000000, 34.000000,
        44.000000, 61.000000, 72.000000, 80.000000, 89.000000,
        96.000000, 101.000000, 107.000000, 120.000000
    };
    
    double readings_avg_expected[] = {
        1802.450000, 1802.450000, 1802.325000,
        1801.300000, 1800.825000, 1801.100000, 1801.025000, 1799.525000,
        1799.000000, 1799.000000, 1799.000000, 1799.200000, 1802.475000
    };
    
    double std_expected[] = {
        0.229129, 0.111803, 0.043301, 0.273861, 0.163936, 0.070711,
        0.043301, 0.043301, 0.122474, 0.122474, 0.070711, 0.100000,
        0.129904
    };
    
    double rel_grav_fields_expected[] = {
        730.600276, 730.600276, 730.549609, 730.134139,
        729.941603, 730.053071, 730.022671, 729.414665, 729.201863,
        729.201863, 729.201863, 729.282930, 730.610410
    };

    double grav_anom_uncorr_expected[] = {
        0.000000, 0.000000, 0.050667, 0.466138,
        0.658673, 0.547205, 0.577606, 1.185612, 1.398414, 1.398414,
        1.398414, 1.317346, -0.010133
    };
    
    double temp_vars_expected[] = {
        -0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
         0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
         0.000000, 0.010133
    };

    double attraction_dev_expected[] = { 
        -0.000000, -0.000844, -0.001689, -0.002533,
        -0.003378, -0.004222, -0.005067, -0.005911, -0.006756,
        -0.007600, -0.008445, -0.009289, -0.010133
    };
    
    double lat_corr_expected[] = {
        -0.000000, -0.013430, -0.026860, -0.040290,
        -0.053721, -0.067151, -0.080581, -0.094011, -0.107441,
        -0.120871, -0.134302, -0.147732, -0.000000
    };
    
    double elev_expected[] = {
        0.000000, 0.243000, 0.022000, 2.247000, 3.471000,
        3.817000, 3.955000, 5.749000, 5.965000, 5.882000, 5.998000,
        5.916000, 0.000000
    };
    
    double alts_expected[] = {
        93.084000, 93.327000, 93.106000, 95.331000, 96.555000,
        96.901000, 97.039000, 98.833000, 99.049000, 98.966000, 99.082000,
        99.000000, 93.084000
    };
    
    double free_air_corr_expected[] = {
        28.725722, 28.800712, 28.732512, 29.419147,
        29.796873, 29.903649, 29.946235, 30.499864, 30.566521,
        30.540908, 30.576705, 30.551400, 28.725722
    };
   
    double bouguer_rel_expected[] = {
        749.958770, 749.995876, 749.885818, 749.919645,
        749.968233, 750.138227, 750.123097, 749.874755, 749.693444,
        749.662752, 749.673446, 749.724030, 749.958770
    };
    
    double bouguer_corr_expected[] = {
        -9.367229, -9.391683, -9.369443, -9.593349,
        -9.716523, -9.751341, -9.765229, -9.945762, -9.967499,
        -9.959147, -9.970820, -9.962568, -9.367229
    };
    
    double bouguer_anom_expected[] = {
        0.000000, 0.037106, -0.072952, -0.039124,
        0.009463, 0.179458, 0.164327, -0.084015, -0.265326, -0.296017,
        -0.285323, -0.234739, 0.000000
    };
    
    double regional_anom_expected[] = {
        0.000000, 0.027273, 0.054545, 0.081818, 0.109091,
        0.136364, 0.163636, 0.190909, 0.218182, 0.245455, 0.272727,
        0.300000, 0.000000
    };
    
    double residual_anom_expected[] = {
        0.000000, 0.009833, -0.127498, -0.120943,
        -0.099628, 0.043094, 0.000690, -0.274924, -0.483508, -0.541472,
        -0.558051, -0.534739, 0.000000
    };

    const char *purpose_expected = "LABORATOIRE 1 - LEVE GRAVIMETRIQUE";
    const char *area_expected = "LES PLAINES D’ABRAHAM - QUEBEC";
    const char *poi_expected = "RESERVOIR D'EAU MUNICIPAL DE LA VILLE DE QUEBEC";
    const char *addr_expected = "\"555 plaines abraham, quebec, canada, g2j 5h6\"";
    const char *date_expected = "1997-12-09";
    const double op_temp_expected = 74.0;
    const double ref_lat_expected = 46.8;
    const double dir_expected = 34.25;
    const char *unit_expected = "F";

    char *json_actual = generate_grav_json(&worden807_actual);
    json_error_t err;
    json_t *root = json_loads(json_actual, 0, &err);

    test_json_field(root, "stations", "error in json stations", stations_expected, EPSILON);
    test_json_field(root, "times", "error in json times", times_expected, 1e-6);
    test_json_field(root, "times_min", "error in json times_min", times_min_expected, EPSILON);
    test_json_field(root, "readings_avg", "error in json average readings", readings_avg_expected, EPSILON);
    test_json_field(root, "std", "error in json std", std_expected, ep4);
    test_json_field(root, "rel_grav_fields", "error in json rel_grav_fields", rel_grav_fields_expected, EPSILON);
    test_json_field(root, "grav_anom_uncorr", "error in json grav_anom_uncorr", grav_anom_uncorr_expected, ep4);
    test_json_field(root, "temp_vars", "error in json temp_vars", temp_vars_expected, ep4);
    test_json_field(root, "attract_dev", "error in json attract_dev", attraction_dev_expected, ep3);
    test_json_field(root, "lat_corr", "error in json lat_corr", lat_corr_expected, ep4);
    test_json_field(root, "elev", "error in json elev", elev_expected, EPSILON);
    test_json_field(root, "alts", "error in json alts", alts_expected, EPSILON);
    test_json_field(root, "free_air_corr", "error in json free_air_corr", free_air_corr_expected, EPSILON);
    test_json_field(root, "bouguer_corr", "error in json bouguer_corr", bouguer_corr_expected, EPSILON);
    test_json_field(root, "bouguer_rel", "error in json bouguer_rel", bouguer_rel_expected, ep4);
    test_json_field(root, "bouguer_anom", "error in json bouguer_anom", bouguer_anom_expected, ep4);
    test_json_field(root, "regional_anom", "error in json regional_anom", regional_anom_expected, ep4);
    test_json_field(root, "residual_anom", "error in json residual_anom", residual_anom_expected, ep3);
    
    test_json_field_str_real(root, "survey_purpose", purpose_expected, ARBITRARY_NUM, 1);
    test_json_field_str_real(root, "survey_area", area_expected, ARBITRARY_NUM, 1);
    test_json_field_str_real(root, "survey_poi", poi_expected, ARBITRARY_NUM, 1);
    test_json_field_str_real(root, "survey_addr", addr_expected, ARBITRARY_NUM, 1);
    test_json_field_str_real(root, "survey_date", date_expected, ARBITRARY_NUM, 1);
    test_json_field_str_real(root, "op_temp_unit", unit_expected, ARBITRARY_NUM, 1);

    test_json_field_str_real(root, "op_temp", SPACE, op_temp_expected, 0);
    test_json_field_str_real(root, "ref_lat", SPACE, ref_lat_expected, 0);
    test_json_field_str_real(root, "survey_dir", SPACE, dir_expected, 0);

    json_decref(root);
    free(json_actual);
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
    MU_RUN_TEST(test_assert_generate_grav_csv);
    MU_RUN_TEST(test_assert_generate_grav_json_all);
}

 
 
int main(void)
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return 0;
}

