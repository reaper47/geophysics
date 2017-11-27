#ifndef GRAVIMETRY_H_
#define GRAVIMETRY_H_

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void  free_worden807(struct worden807_t *worden);
void  load_grav_csv(struct worden807_t *worden, const char *csv_file, const char *delim);

#endif /* gravimetry.h */
