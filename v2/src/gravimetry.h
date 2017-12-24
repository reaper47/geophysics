#ifndef GRAVIMETRY_H_
#define GRAVIMETRY_H_

#define _GNU_SOURCE
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/csv.h"
#include "./utils/sll.h"

#define STATION  "station"
#define TIME     "time"
#define TIME_MIN "min"
#define READING  "reading"
#define PURPOSE  "purpose"
#define AREA	 "area"
#define POI	     "poi"
#define ADDRESS  "address"
#define DATE	 "date"
#define TEMP	 "temperature"
#define LAT      "latitude"
#define DIR		 "direction"

#define IDX_STATION  0
#define IDX_TIME     1
#define IDX_TIME_MIN 2
#define IDX_READING  50
#define IDX_PURPOSE  3
#define IDX_AREA	 4
#define IDX_POI	     5
#define IDX_ADDRESS  6
#define IDX_DATE	 7
#define IDX_TEMP	 8
#define IDX_LAT      9
#define IDX_DIR		 10

#define NUM_READINGS 4

struct worden807_t {
	float *attract_derivations;
	float *avg_readings;
	float *bouguer_anomaly;
	float *bouguer_corr;
	float *bouguer_rel_grav_fields;
	float *free_air_corr;
	float *grav_anomaly_notcorr;
	float *lat_corr;
	float  operation_temp;
	float *readings;
	float  ref_station_lat;
	float *rel_grav_fields;
	float *regional_anomaly;
	float *residual_anomaly;
	float *stations;
	float *std;
	char  *survey_address;
	char  *survey_area;
	char  *survey_date;
	float  survey_dir;
	char  *survey_poi;
	char  *survey_purpose;
	float *temporal_vars;
	float *times;
	float *times_min;
};

int   alloc_worden807(struct worden807_t *worden, unsigned int n);
void  assign_idx_node(struct list_t *list);
void  free_worden807(struct worden807_t *worden);
int   load_grav_csv(struct worden807_t *worden, const char *csv_file);
void store_fields_struct(struct list_t *fields, struct list_t *headers, struct worden807_t *worden, int idx, int *ridx);

#endif /* gravimetry.h */
