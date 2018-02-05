#ifndef TOPO_H_
#define TOPO_H_

#define _GNU_SOURCE
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./utils/calcs.h"
#include "./utils/csv.h"
#include "./utils/sll.h"
#include "./utils/strings.h"

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

#define IDX_STATION           0
#define IDX_ZERO              1
#define IDX_ZERO_QUALITY      2
#define IDX_ELEV_DIFF         3
#define IDX_ELEV_DIFF_QUALITY 4
#define IDX_PURPOSE           5
#define IDX_AREA              6
#define IDX_POI               7
#define IDX_ADDRESS           8
#define IDX_DATE              9

#define HEADER_LINE_NUM       1

struct topo_t {
	double *altitudes;
	double *elevation_corr;
	double *elevation_cmp_ref;
	double *elevation_diff;
	double *elevation_diff_corr;
	double *elevation_diff_quality;
	double *err_dist_btwn_stations_m;
	unsigned int num_lines;
	int     station_num_before_return_to_ref;
	double *stations;
	int    *stations_numeric_order;
	char   *survey_address;
	char   *survey_area;
	char   *survey_date;
	char   *survey_poi;
	char   *survey_purpose;
	double *zeros;
	double *zeros_quality;
};

int alloc_topo(struct topo_t *topo, unsigned int n);
void assign_idx_node_topo(struct list_t *list);
void free_topo(struct topo_t *topo);
int load_topo_csv(struct topo_t *topo, const char *csv_file);
void store_fields_topo_struct(struct list_t *fields, struct list_t *headers, struct topo_t *topo, int idx);
void store_elevation_diff_corr(struct topo_t *topo);
void store_elevation_cmp_ref(struct topo_t *topo);
void store_err_dist_btwn_stations(struct topo_t *topo);
void store_elevation_corr(struct topo_t *topo);
void store_altitudes(struct topo_t *topo);
void populate_calc_fields(struct topo_t *topo);


#endif /* topo.h */
