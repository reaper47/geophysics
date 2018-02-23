#include "../include/topo.h"

int alloc_topo(struct topo_t *topo, unsigned int n)
{
	topo->altitudes	             = malloc(sizeof(topo->altitudes) * n);
	topo->elevation_corr         = malloc(sizeof(topo->elevation_corr) * n);
	topo->elevation_cmp_ref      = malloc(sizeof(topo->elevation_cmp_ref) * n);
	topo->elevation_diff         = malloc(sizeof(topo->elevation_diff) * n);
	topo->elevation_diff_corr    = malloc(sizeof(topo->elevation_diff_corr) * n);
	topo->elevation_diff_quality = malloc(sizeof(topo->elevation_diff_quality) * n);
	topo->err_dist_btwn_stations_m = malloc(sizeof(topo->err_dist_btwn_stations_m) * n);
	topo->stations               = malloc(sizeof(topo->stations) * n);
	topo->zeros                  = malloc(sizeof(topo->zeros) * n);
	topo->zeros_quality          = malloc(sizeof(topo->zeros_quality) * n);

	if(topo->elevation_diff == NULL || topo->elevation_diff_quality   == NULL ||
	   topo->stations       == NULL || topo->zeros                    == NULL || 
	   topo->zeros_quality  == NULL || topo->elevation_diff_corr      == NULL || 
	   topo->elevation_cmp_ref == NULL || topo->altitudes             == NULL ||
	   topo->err_dist_btwn_stations_m == NULL || topo->elevation_corr == NULL)
		return -1;

	return 0;
}



void free_topo(struct topo_t *topo)
{
	free(topo->altitudes);
	free(topo->elevation_corr);
	free(topo->elevation_cmp_ref);
	free(topo->elevation_diff);
	free(topo->elevation_diff_corr);
	free(topo->elevation_diff_quality);
	free(topo->err_dist_btwn_stations_m);
	free(topo->stations);
	free(topo->zeros);
	free(topo->zeros_quality);
}



void assign_idx_node_topo(struct list_t *list)
{
	struct node_t *curr;

	for(curr = list->head; curr != NULL; curr = curr->next) {
		if(strstr(curr->data, STATION) != NULL)
			curr->idx = IDX_STATION_TOPO;
		else if(strstr(curr->data, QUALITY) != NULL && strstr(curr->data, ZERO) != NULL)
			curr->idx = IDX_ZERO_QUALITY_TOPO;
		else if(strstr(curr->data, ZERO) != NULL)
			curr->idx = IDX_ZERO_TOPO;
		else if(strstr(curr->data, QUALITY) != NULL && strstr(curr->data, ELEVATION) != NULL)
			curr->idx = IDX_ELEV_DIFF_QUALITY_TOPO;
		else if(strstr(curr->data, ELEVATION) != NULL)
			curr->idx = IDX_ELEV_DIFF_TOPO;
		else if(strstr(curr->data, PURPOSE) != NULL)
			curr->idx = IDX_PURPOSE_TOPO;
		else if(strstr(curr->data, AREA) != NULL)
			curr->idx = IDX_AREA_TOPO;
		else if(strstr(curr->data, POI) != NULL)
			curr->idx = IDX_POI_TOPO;
		else if(strstr(curr->data, ADDRESS) != NULL)
			curr->idx = IDX_ADDRESS_TOPO;
		else if(strstr(curr->data, DATE) != NULL)
			curr->idx = IDX_DATE_TOPO;
		else if(strstr(curr->data, SEA) != NULL)
			curr->idx = IDX_AVG_SEA_LVL_TOPO;
	}
}



int load_topo_csv(struct topo_t *topo, const char *csv_file)
{
	FILE *fp = fopen(csv_file, "r");
	if(fp == NULL) {
		printf("unable to open %s\n", csv_file);
		fclose(fp);
		return 1;
	}

	unsigned int num_lines = num_lines_file(fp);
	if(alloc_topo(topo, num_lines) != 0) {
		printf("malloc topo failed\n");
		return 2;
	}

	topo->num_lines = num_lines - HEADER_LINE_NUM;

	const char delim = determine_delim(fp);

	struct list_t *lines = gather_lines(fp);
	struct list_t *headers = parse_header(fp, delim);
	assign_idx_node_topo(headers);

	int arr_idx = 0;
	for(struct node_t *node = lines->head; node != NULL; node = node->next) {
		struct list_t *fields = parse_line(node->data, delim);
		store_fields_topo_struct(fields, headers, topo, arr_idx);
		arr_idx++;
		del_list(fields);
	}

	del_list(lines);
	del_list(headers);
	fclose(fp);
    
	return 0;
}



void store_fields_topo_struct(struct list_t *fields,  struct list_t *headers, struct topo_t *topo, int idx)
{
	struct node_t *field = fields->head;
	struct node_t *header = headers->head;

	while(field != NULL) {
		uint8_t hidx = header->idx;
		
		if(hidx == IDX_STATION_TOPO)
			topo->stations[idx] = atof(field->data);
		else if(hidx == IDX_ZERO_TOPO)
			topo->zeros[idx] = atof(field->data);
		else if(hidx == IDX_ZERO_QUALITY_TOPO)
			topo->zeros_quality[idx] = atof(field->data);
		else if(hidx == IDX_ELEV_DIFF_TOPO)
			topo->elevation_diff[idx] = atof(field->data);
		else if(hidx == IDX_ELEV_DIFF_QUALITY_TOPO)
			topo->elevation_diff_quality[idx] = atof(field->data);
		else if(idx == 0 && hidx == IDX_PURPOSE_TOPO) {
			strlower(field->data, 0);
			topo->survey_purpose = strdup(field->data);
		}
		else if(idx == 0 && hidx == IDX_AREA_TOPO) {
			strlower(field->data, 0);
			topo->survey_area = strdup(field->data);
		}
		else if(idx == 0 && hidx == IDX_POI_TOPO) {
			strlower(field->data, 0);
			topo->survey_poi = strdup(field->data);
		}
		else if(idx == 0 && hidx == IDX_ADDRESS_TOPO) {
			strlower(field->data, 0);
			topo->survey_address = strdup(field->data);
		}
		else if(idx == 0 && hidx == IDX_DATE_TOPO)
			topo->survey_date = strdup(field->data);
		else if(idx == 0 && hidx == IDX_AVG_SEA_LVL_TOPO)
			topo->avg_sea_lvl = atof(field->data);

		header = header->next;
		field = field->next;
	}
}



void store_elevation_diff_corr(struct topo_t *topo)
{
	unsigned int num_lines = topo->num_lines;

	topo->elevation_diff_corr[0] = 0.0;
	for(unsigned int i = 1; i < num_lines; i++) {
		double elev_diff = topo->elevation_diff[i];
		double zero = topo->zeros[i-1];
		
		topo->elevation_diff_corr[i] = elev_diff - zero;
	}
}



void store_elevation_cmp_ref(struct topo_t *topo)
{
	unsigned int num_lines = topo->num_lines;

	topo->elevation_cmp_ref[0] = 0.0;
	for(unsigned int i = 1; i < num_lines; i++) {
		double elev_cmp = topo->elevation_cmp_ref[i-1];
		double elev_diff = topo->elevation_diff_corr[i];
		
		topo->elevation_cmp_ref[i] = elev_cmp - elev_diff;	
	}
}



void store_err_dist_btwn_stations(struct topo_t *topo)
{
	unsigned int num_lines = topo->num_lines;

	double max_station = max_arr(topo->stations, (int)num_lines);
	double last_elev_cmp_ref = topo->elevation_cmp_ref[num_lines-1];

	for(unsigned int i = 0; i < num_lines; i++) {
		double err = -last_elev_cmp_ref / 2*topo->stations[i] / max_station;
		topo->err_dist_btwn_stations_m[i] = err;
	}	
}



void store_elevation_corr(struct topo_t *topo)
{
	unsigned int num_lines = topo->num_lines;

	for(unsigned int i = 0; i < num_lines; i++) {
		double elev_cmp_ref = topo->elevation_cmp_ref[i];
		double err_dist = topo->err_dist_btwn_stations_m[i];
		
		topo->elevation_corr[i] = elev_cmp_ref + err_dist;
	}
}



void store_altitudes(struct topo_t *topo)
{
	unsigned int num_lines = topo->num_lines;

	int station_num = topo->station_num_before_return_to_ref;
	double elev_corr_before_return = topo->elevation_corr[station_num];

	for(unsigned int i = 0; i < num_lines; i++) {
		double elev_corr = topo->elevation_corr[i];
		double avg_sea_lvl = topo->avg_sea_lvl;

		topo->altitudes[i] = avg_sea_lvl - elev_corr_before_return + elev_corr;
	}
}



void populate_calc_fields(struct topo_t *topo)
{
	store_elevation_diff_corr(topo);
	store_elevation_cmp_ref(topo);
	store_err_dist_btwn_stations(topo);
	store_elevation_corr(topo);
	store_altitudes(topo);	
}

