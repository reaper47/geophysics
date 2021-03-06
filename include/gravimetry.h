#ifndef GRAVIMETRY_H_
#define GRAVIMETRY_H_

#define _GNU_SOURCE
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <jansson.h>
#include "topo.h"
#include "calcs.h"
#include "csv.h"
#include "sll.h"

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
#define CELSIUS           'c'
#define FAHR              'f'
#define TEMP_BTWN         "("

#define NUM_READINGS       4

/* Instrument Dial Constants */
#define WORDEN807_LEFTX_C   48.8889
#define WORDEN807_LEFTX_F   120.0
#define WORDEN807_LOWERY    0.40514
#define WORDEN807_RIGHTX_C -17.7778
#define WORDEN807_RIGHTX_F  0.0
#define WORDEN807_UPPERY    0.40546
#define INVALID_TEMP       -999

#define REF_STATION         0.0
#define FREE_AIR_VARIATION  0.3086
#define DENSITY_G_CM3       2.4
#define TWO_PI_G            0.04193

#define GRAV_PATH_LEN       26
#define GRAV_TEST_DIR       "./test_data/"
#define CSV                 ".csv"

struct worden807_t {
    double *altitudes;
    double *attraction_deviation;
    double *avg_readings;
    double *bouguer_anomaly;
    double *bouguer_corr;
    double *bouguer_rel_grav_fields;
    double *elevations;
    double *free_air_corr;
    double *grav_anomaly_uncorr;
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
    char   *survey_address;
    char   *survey_area;
    char   *survey_date;
    double  survey_dir;
    char   *survey_poi;
    char   *survey_purpose;
    double *temporal_vars;
    double *times;
    double *times_min;
    char   *topo_file;
};

void   assign_idx_node                      (struct list_t *list, struct worden807_t *worden);
double dial_const_worden807                 (struct worden807_t *worden);
void   free_worden807                       (struct worden807_t *worden);
int    populate_calc_fields_worden807       (struct worden807_t *worden);
void   set_station_num_before_return_to_ref (struct worden807_t *worden, struct topo_t *topo);
void   store_avg_readings_std               (struct worden807_t *worden, int is_std);
void   store_fields_struct                  (struct list_t *fields, struct list_t *headers, struct worden807_t *worden, int idx, int *ridx);
void   store_grav_anomaly_uncorr            (struct worden807_t *worden);
void   store_rel_grav_fields                (struct worden807_t *worden);
void   store_temporal_vars                  (struct worden807_t *worden);
void   store_attraction_deviation           (struct worden807_t *worden);
void   store_lat_corr                       (struct worden807_t *worden);
void   store_free_air_corr                  (struct worden807_t *worden);
void   store_bouguer_corr                   (struct worden807_t *worden);
void   store_bouguer_rel_grav_fields        (struct worden807_t *worden);
void   store_bouguer_anomaly                (struct worden807_t *worden);
void   store_regional_anomaly               (struct worden807_t *worden);
void   store_residual_anomaly               (struct worden807_t *worden);
void   transfer_topo_data_to_grav           (struct topo_t *topo, struct worden807_t *worden807);
char  *generate_grav_csv                    (struct worden807_t *worden, const char *out_dir);
char  *generate_grav_json                   (struct worden807_t *worden);
struct worden807_t init_worden807           (size_t n);
struct worden807_t load_grav_csv            (const char *csv_file, const char *topo_file);

#endif /* gravimetry.h */

