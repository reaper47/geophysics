#ifndef GRAVIMETRY_H_
#define GRAVIMETRY_H_

#define _GNU_SOURCE
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./utils/calcs.h"
#include "./utils/csv.h"
#include "./utils/sll.h"

#define STATION  "station"
#define TIME     "time"
#define TIME_MIN "min"
#define READING  "reading"
#define PURPOSE  "purpose"
#define AREA	 "area"
#define POI      "poi"
#define ADDRESS  "address"
#define DATE	 "date"
#define TEMP	 "temperature"
#define LAT      "latitude"
#define DIR      "direction"

#define IDX_STATION  0
#define IDX_TIME     1
#define IDX_TIME_MIN 2
#define IDX_READING  50
#define IDX_PURPOSE  3
#define IDX_AREA     4
#define IDX_POI	     5
#define IDX_ADDRESS  6
#define IDX_DATE     7
#define IDX_TEMP     8
#define IDX_LAT      9
#define IDX_DIR	     10

#define DEFAULT_TEMP_UNIT 'z'
#define CELSIUS 'c'
#define FAHR 'f'
#define TEMP_BTWN "("

#define NUM_READINGS 4

/* Instrument Dial Constants */
#define WORDEN807_LEFTX_C   48.8889
#define WORDEN807_LEFTX_F  120.0
#define WORDEN807_LOWERY     0.40514
#define WORDEN807_RIGHTX_C -17.7778
#define WORDEN807_RIGHTX_F   0.0
#define WORDEN807_UPPERY     0.40546
#define INVALID_TEMP -999

struct worden807_t {
	double *attraction_deviation;
	double *avg_readings;
	double *bouguer_anomaly;
	double *bouguer_corr;
	double *bouguer_rel_grav_fields;
	double *free_air_corr;
	double *grav_anomaly_notcorr;
	double *lat_corr;
	uint8_t num_readings;
	unsigned int num_lines;
	double  operation_temp;
	char    operation_temp_unit;
	double *readings;
	double  ref_station_lat;
	double *rel_grav_fields;
	double *regional_anomaly;
	double *residual_anomaly;
	double *stations;
	double *std;
	char  *survey_address;
	char  *survey_area;
	char  *survey_date;
	double  survey_dir;
	char  *survey_poi;
	char  *survey_purpose;
	double *temporal_vars;
	double *times;
	double *times_min;
};

int  alloc_worden807(struct worden807_t *worden, unsigned int n);
void assign_idx_node(struct list_t *list, struct worden807_t *worden);
double dial_const_worden807(struct worden807_t *worden);
void free_worden807(struct worden807_t *worden);
int  load_grav_csv(struct worden807_t *worden, const char *csv_file);
void store_avg_readings_std(struct worden807_t *worden, int is_std);
void store_fields_struct(struct list_t *fields, struct list_t *headers, struct worden807_t *worden, int idx, int *ridx);
void store_grav_anomaly_notcorr(struct worden807_t *worden);
void store_rel_grav_fields(struct worden807_t *worden);
void store_temporal_vars(struct worden807_t *worden);
void store_attraction_deviation(struct worden807_t *worden);

#endif /* gravimetry.h */

