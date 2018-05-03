/*******************************************************************
 * NAME: gravimetry.h
 *
 * PURPOSE: Defines functions related to corrections applied to raw
 *          gravimetric data as to acquire the residual anomaly.
 *
 * GLOBAL VARIABLES: None
 *
 * Variables              Type      Description
 * ---------              ----      -----------
 * ADDRESS                char*     Reference to the word "address"
 *                                  when parsing a csv file.
 *
 * AREA	                  char*     Reference to the word "area"
 *                                  when parsing a csv file.
 *
 * CELSIUS                char      Character representing a 
 *                                  temperature measured in Celsius
 *
 * DATE                   char*     Reference to the word "date"
 *                                  when parsing a csv file.
 *
 * DEFAULT_TEMP_UNIT      char      Random temperature unit used while
 *                                  parsing to determine the
 *                                  true unit.
 *
 * DENSITY_G_CM3          double    Rock density ρ (g/cm^3).
 *
 * DIR                    char*     Reference to the word "direction"
 *                                  when parsing a csv file.
 *
 * FAHR                   char      Character representing a
                                    temperature measured in Fahrenheit.
 *
 * FREE_AIR_VARIATION     double    Gravitational acceleration gradient 
 *                                  for an increasing distance between 
 *                                  the gravimeter and the centre of 
 *                                  the Earth (mGal/m).
 *
 * GRAV_BASE_DIR          char*     Base directory where generated
 *                                  csv files are dumped.
 *
 * GRAV_PATH_LEN          int       Length of the path for the generated 
 *                                  csv from the results.
 *
 * IDX_ADDRESS            int       Index used to store the address
 *                                  column in a singly-linked-list.
 *
 * IDX_AREA               int       Index used to store the survey's
 *                                  area column in a singly-linked-list.
 *
 * IDX_DATE               int       Index used to store the survey's
 *                                  date column in a singly-linked-list.
 *
 * IDX_DIR                int       Index used to store the survey 
 *                                  direction column in a singly-linked-list.
 *
 * IDX_LAT                int       Index used to store the survey's
 *                                  latitude column in a singly-linked-list.
 *
 * IDX_POI                int       Index used to store the point of 
 *                                  interest column in a singly-linked-list.
 *
 * IDX_PURPOSE            int       Index used to store the survey's
 *                                  purpose column in a singly-linked-list.
 *
 * IDX_READING            int       Index used to store the survey's
 *                                  readings columns in a singly-linked-list.
 *
 * IDX_STATION            int       Index used to store the stations
 *                                  column  in a singly-linked-list.
 *
 * IDX_TEMP               int       Index used to store the survey's 
 *                                  temperature column in a 
 *                                  singly-linked-list.
 *
 * IDX_TIME               int       Index used to store the times column 
 *                                  in a singly-linked-list.
 *
 * IDX_TIME_MIN           int       Index used to store the minimum 
 *                                  times column in a singly-linked-list.

 * INVALID_TEMP           int       Random invalid temperature used
 *                                  when parsing. Serves as an 
 *                                  error-detection mecanism.
 *
 * LAT                    char*     Reference to the word "latitude"
 *                                  when parsing a csv file.
 *
 * NUM_READINGS           int       Number of readings per station.
 *                                  Defaults at 4 while an autodetect
 *                                  mecanism will be implemented.
 *
 * POI                    char*     Reference to the acronym "poi"
 *                                  (Point of Interest) when 
 *                                  parsing a csv file.
 *
 * PURPOSE                char*     Reference to the word "purpose"
 *                                  when parsing a csv file.
 *
 * READING                char*     Reference to the word "reading"
 *                                  when parsing a csv file.
 *
 * REF_STATION            double    Reference station usually set at
 *                                  x = 0.0m
 *
 * STATION                char*     Reference to the word "station"
 *                                  when parsing a csv file.
 *
 * TEMP                   char*     Reference to the word "temperature"
 *                                  when parsing a csv file.
 *
 * TEMP_BTWN              char      Opening parenthesis used when 
 *                                  parsing a temperature range.
 *
 * TIME                   char*     Reference to the word "time"
 *                                  when parsing a csv file.
 *
 * TIME_MIN               char*     Reference to the word "min" 
 *                                  for a minimum time.
 *
 * TWO_PI_G               double    Calculated value of 2*π*g, where
 *                                  g is an approximate gravitational 
 *                                  constant.
 *
 * WORDEN807_LEFTX_C      double    X-axis lower bound for a Worden 807 
 *                                  dial constant graph when the operation 
 *                                  temperature is in Celsius.
 *
 * WORDEN807_LEFTX_F      double    X-axis lower bound for a Worden 807 
 *                                  dial constant graph when the operation 
 *                                  temperature is in Fahrenheit.
 *
 * WORDEN807_LOWERY       double    Y-axis lower bound for a Worden 807 
 *                                  dial constant graph.
 *
 * WORDEN807_RIGHTX_C     double    X-axis upper bound for a Worden 807 
 *                                  dial constant graph when the operation 
 *                                  temperature is in Celsius.
 *
 * WORDEN807_RIGHTX_F     double    X-axis upper bound for a Worden 807 
 *                                  dial constant graph when the operation 
 *                                  temperature is in Fahrenheit.
 *  
 * WORDEN807_UPPERY       double    Y-axis upper bound for a Worden 807 
 *                                  dial constant graph.
 *
 * worden807_t            struct    Stores all information related to 
 *                                  a Worden 807 gravimeter.
 *
 *   altitudes              double* Stores the calculated altitudes.
 *   attraction_deviation   double* Stores the calculated attraction deviations
 *	 avg_readings           double* Stores the average readings for each station.
 *	 bouguer_anomaly        double* Stores the calculated Bouguer anomaly.
 *	 bouguer_corr           double* Stores the calculated Bouguer corrections.
 *	bouguer_rel_grav_fields double* Stores Bouguer relative gravitational fields.
 *	 elevations             double* Stores calculated elevations.
 *	 free_air_corr          double* Stores calculated free air corrections.
 *	 grav_anomaly_uncorr    double* Stores uncorrected gravimetric anomalies.
 *	 lat_corr               double* Stores corrected latitudes.
 *	 num_readings           uint8_t Stores the number of readings per station.
 *	 num_lines              uint    Stores the number of lines a csv file.
 *	 operation_temp         double  Stores the operation temperature.
 *	 operation_temp_unit    char    Stores the operation temperature unit.
 *	 readings               double* Stores all raw gravimetric readings.
 *	 ref_station_lat        double  Stores the reference station's latitude.
 *	 rel_grav_fields        double* Stores relative gravitational fields.
 *	 regional_anomaly       double* Stores the calculated regional anomaly.
 *	 residual_anomaly       double* Stores the calculated residual anomaly.
 *	 stations               double* Stores the stations, usually in meters.
 *	 std                    double* Stores the standard deviation from the readings.
 *	 survey_address         char*   Stores the survey's address.
 *	 survey_area            char*   Stores the survey's area.
 *	 survey_date            char*   Stores the date when the survey was done.
 *	 survey_dir             double  Stores the direction of the survey.
 *	 survey_poi             char*   Stores a point of interest close to the survey.
 *	 survey_purpose         char*   Stores the purpose of the survey.
 *	 temporal_vars          double* Stores calculated temporal variations.
 *	 times                  double* Stores the time for each station.
 *	 times_min              double* Stores the minimum time for each station.
 *   topo_file              char*   Stores the name of the associated 
 *                                  topographic file used to calculate
 *                                  the altitudes and elevations.
 *
 *******************************************************************/
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

