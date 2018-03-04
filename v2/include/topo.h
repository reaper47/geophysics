/*******************************************************************
 * NAME: topo.h
 *
 * PURPOSE: Defines functions related to corrections applied to raw
 *          topographic data as to eventually transfer elevations and
 *          altitudes to the gravimeter structure.
 *
 * GLOBAL VARIABLES: None
 *
 * Variable         Type         Description
 * --------         ----         -----------
 * AREA             char*        Reference to the word "area"
 *                               when parsing a csv file.
 *
 * ADDRESS          char*        Reference to the word "address"
 *                               when parsing a csv file.
 *
 * DATE             char*        Reference to the word "date"
 *                               when parsing a csv file.
 *
 * DIFF             char*        Reference to the word "difference"
 *                               when parsing a csv file.
 *
 * ELEVATION        char*        Reference to the word "elevation"
 *                               when parsing a csv file.
 *
 * HEADER_LINE_NUM  int          The line number of a csv file's header.
 *
 * IDX_ADDRESS_TOPO int          Index used to store the address
 *                               column in a singly-linked-list.
 * 
 * IDX_AREA_TOPO    int          Index used to store the area
 *                               column in a singly-linked-list.
 *
 * IDX_AVG_SEA_LVL_TOPO   int    Index used to store the average sea
 *                               level column in a singly-linked-list.
 *
 * IDX_DATE_TOPO    int          Index used to store the survey's
 *                               date column in a singly-linked-list.
 *
 * IDX_ELEV_DIFF_TOPO     int    Index used to store the elevation
 *                               difference column in a singly-linked-list.
 *
 * IDX_ELEV_DIFF_QUALITY_TOPO    Index used to store the elevation
 *                               difference quality column in a 
 *                               singly-linked-list.
 *
 * IDX_POI_TOPO     int          Index used to store the point-of-interest
 *                               column in a singly-linked-list.
 *
 * IDX_PURPOSE_TOPO int          Index used to store the survey's
 *                               purpose column in a singly-linked-list.
 *
 * IDX_STATION_TOPO int          Index used to store the stations
 *                               column in a singly-linked-list.
 *
 * IDX_ZERO_TOPO    int          Index used to store the zero
 *                               column in a singly-linked-list.
 *
 * IDX_ZERO_QUALITY_TOPO  int    Index used to store the zero quality
 *                               column in a singly-linked-list.
 *
 * POI              char*        Reference to the word "poi"
 *                               when parsing a csv file.      
 *
 * PURPOSE          char*        Reference to the word "purpose"
 *                               when parsing a csv file. 
 * 
 * SEA              char*        Reference to the word "sea"
 *                               when parsing a csv file. 
 *
 * STATION          char*        Reference to the word "station"
 *                               when parsing a csv file. 
 *
 * QUALITY          char*        Reference to the word "quality"
 *                               when parsing a csv file. 
 *
 * ZERO             char*        Reference to the word "zero"
 *                               when parsing a csv file.   
 *
 * topo_t           struct       Holds all data related to a 
 *                               topographical survey related to
 *                               gravimetry.
 *
 *  altitudes                double*   Stores the calculated altitudes.
 *  avg_sea_lvl              double    Holds the average sea level.
 *	elevation_corr           double*   Stores the calculated elevations.
 *	elevation_cmp_ref        double*   Stores the elevations compared to
 *                                     the reference station.
 *
 *	elevation_diff           double*   Stores the calculated elevation differences.
 *	elevation_diff_corr      double*   Stores the corrected elevation differences.
 *
 *	elevation_diff_quality   double*   Stores the elevation difference quality.
 *	err_dist_btwn_stations_m double*   Stores the distance error between stations.
 *
 *	num_lines                uint      Holds the number of lines in the csv file.
 * 
 *	station_num_before_return_to_ref  int  References the station before 
 *                                         returning to the reference station. 
 *                                         
 *	stations        double*      Stores the stations.
 *	survey_address  char*        Holds the survey's address.
 *	survey_area     char*        Holds the survey's regional area.
 *	survey_date     char*        Holds the date when the survey was done.
 *	survey_poi      char*        Holds a point-of-interest near the survey.
 *	survey_purpose  char*        Holds the purpose of the survey.
 *	zeros           double*      Stores the zeros of the survey.
 *	zeros_quality   double*      Stores the zeros qualities of the survey.
 *
 *******************************************************************/
#ifndef TOPO_H_
#define TOPO_H_

#define _GNU_SOURCE
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./arrays.h"
#include "./calcs.h"
#include "./csv.h"
#include "./sll.h"
#include "./strings.h"

#define STATION    "station"
#define ZERO       "zero"
#define QUALITY    "quality"
#define ELEVATION  "elevation"
#define DIFF       "difference"
#define PURPOSE    "purpose"
#define AREA       "area"
#define POI        "poi"
#define ADDRESS    "address"
#define DATE       "date"
#define SEA        "sea"
#define ORDER      "order"

#define IDX_STATION_TOPO           0
#define IDX_ZERO_TOPO              1
#define IDX_ZERO_QUALITY_TOPO      2
#define IDX_ELEV_DIFF_TOPO         3
#define IDX_ELEV_DIFF_QUALITY_TOPO 4
#define IDX_PURPOSE_TOPO           5
#define IDX_AREA_TOPO              6
#define IDX_POI_TOPO               7
#define IDX_ADDRESS_TOPO           8
#define IDX_DATE_TOPO              9
#define IDX_AVG_SEA_LVL_TOPO       10

#define HEADER_LINE_NUM            1

struct topo_t {
	double *altitudes;
	double  avg_sea_lvl;
	double *elevation_corr;
	double *elevation_cmp_ref;
	double *elevation_diff;
	double *elevation_diff_corr;
	double *elevation_diff_quality;
	double *err_dist_btwn_stations_m;
	unsigned int num_lines;
	int     station_num_before_return_to_ref;
	double *stations;
	char   *survey_address;
	char   *survey_area;
	char   *survey_date;
	char   *survey_poi;
	char   *survey_purpose;
	double *zeros;
	double *zeros_quality;
};

int    alloc_topo                   (struct topo_t *topo, unsigned int n);
void   assign_idx_node_topo         (struct list_t *list);
void   free_topo                    (struct topo_t *topo, _Bool free_chars);
int    load_topo_csv                (struct topo_t *topo, const char *csv_file);
void   store_fields_topo_struct     (struct list_t *fields, struct list_t *headers, struct topo_t *topo, int idx);
void   store_elevation_diff_corr    (struct topo_t *topo);
void   store_elevation_cmp_ref      (struct topo_t *topo);
void   store_err_dist_btwn_stations (struct topo_t *topo);
void   store_elevation_corr         (struct topo_t *topo);
void   store_altitudes              (struct topo_t *topo);
void   populate_calc_fields         (struct topo_t *topo);

#endif /* topo.h */

