#include "gravimetry.h"

int alloc_worden807(struct worden807_t *worden, unsigned int n)
{
	worden->attract_derivations     = malloc(sizeof(worden->attract_derivations) * n);
	worden->avg_readings            = malloc(sizeof(worden->avg_readings) * n);
	worden->bouguer_anomaly         = malloc(sizeof(worden->bouguer_anomaly) * n);
	worden->bouguer_corr            = malloc(sizeof(worden->bouguer_corr) * n);
	worden->bouguer_rel_grav_fields = malloc(sizeof(worden->bouguer_rel_grav_fields) * n);
	worden->free_air_corr           = malloc(sizeof(worden->free_air_corr) * n);
	worden->grav_anomaly_notcorr    = malloc(sizeof(worden->grav_anomaly_notcorr) * n);
	worden->lat_corr                = malloc(sizeof(worden->lat_corr) * n);
	worden->readings                = malloc(sizeof(worden->readings) * n * NUM_READINGS);
	worden->rel_grav_fields         = malloc(sizeof(worden->rel_grav_fields) * n);
	worden->regional_anomaly        = malloc(sizeof(worden->regional_anomaly) * n);
	worden->residual_anomaly        = malloc(sizeof(worden->residual_anomaly) * n);
	worden->stations                = malloc(sizeof(worden->stations) * n);
	worden->std                     = malloc(sizeof(worden->std) * n);
	worden->temporal_vars           = malloc(sizeof(worden->temporal_vars) * n);
	worden->times                   = malloc(sizeof(worden->times) * n);
	worden->times_min               = malloc(sizeof(worden->times_min) * n);
	
	if(worden->attract_derivations      == NULL || worden->avg_readings     == NULL ||
		worden->bouguer_anomaly         == NULL || worden->bouguer_corr     == NULL ||
		worden->bouguer_rel_grav_fields == NULL || worden->free_air_corr    == NULL ||
		worden->grav_anomaly_notcorr    == NULL || worden->lat_corr         == NULL ||
		worden->readings                == NULL || worden->rel_grav_fields  == NULL ||
		worden->regional_anomaly        == NULL || worden->residual_anomaly == NULL ||
		worden->stations                == NULL || worden->std              == NULL ||
		worden->temporal_vars           == NULL || worden->times            == NULL ||
		worden->times_min               == NULL)
		return -1;

	return 0;
}



void free_worden807(struct worden807_t *worden)
{
	free(worden->attract_derivations);
	free(worden->avg_readings);
	free(worden->bouguer_anomaly);
	free(worden->bouguer_corr);
	free(worden->bouguer_rel_grav_fields);
	free(worden->free_air_corr);
    free(worden->grav_anomaly_notcorr);
	free(worden->lat_corr);
	free(worden->readings);
	free(worden->rel_grav_fields);
	free(worden->regional_anomaly);
	free(worden->residual_anomaly);
	free(worden->stations);
	free(worden->std);
	free(worden->temporal_vars);
	free(worden->times);
	free(worden->times_min);
}



void assign_idx_node(struct list_t *list)
{
	struct node_t *curr;
	uint8_t read_idx = 50;
	
	for(curr = list->head; curr != NULL; curr = curr->next) {
		if(strstr(curr->data, LAT) != NULL)
			curr->idx = IDX_LAT;
		else if(strstr(curr->data, STATION) != NULL)
			curr->idx = IDX_STATION;
		else if(strstr(curr->data, TIME) != NULL && strstr(curr->data, TIME_MIN) != NULL) 
			curr->idx = IDX_TIME_MIN;
		else if(strstr(curr->data, TIME) != NULL) 
			curr->idx = IDX_TIME;
		else if(strstr(curr->data, READING) != NULL)
			curr->idx = ++read_idx;
		else if(strstr(curr->data, PURPOSE) != NULL)
			curr->idx = IDX_PURPOSE;
		else if(strstr(curr->data, AREA) != NULL)
			curr->idx = IDX_AREA;
		else if(strstr(curr->data, POI) != NULL)
			curr->idx = IDX_POI;		
		else if(strstr(curr->data, ADDRESS) != NULL)
			curr->idx = IDX_ADDRESS;
		else if(strstr(curr->data, DATE) != NULL)
			curr->idx = IDX_DATE;
		else if(strstr(curr->data, TEMP) != NULL)
			curr->idx = IDX_TEMP;
		else if(strstr(curr->data, DIR) != NULL)
			curr->idx = IDX_DIR;
	}
}



int load_grav_csv(struct worden807_t *worden, const char *csv_file)
{
	FILE *fp = fopen(csv_file, "rb");
	if(fp == NULL) {
		printf("unable to open %s\n", csv_file);
		fclose(fp);
		exit(EXIT_FAILURE);
	}
		
	unsigned int num_lines = num_lines_file(fp);
	if(alloc_worden807(worden, num_lines) != 0) {
		printf("malloc worden807 failed\n");
		return -1;
	}

	const char delim = determine_delim(fp);
	
	struct list_t *lines = gather_lines(fp);
	struct list_t *headers = parse_header(fp, delim);
	assign_idx_node(headers);
	
	int arr_idx = 0;
	int readings_idx = 0;
	
	for(struct node_t *node = lines->head ; node != NULL; node = node->next) {
		struct list_t *fields = parse_line(node->data, delim);
		store_fields_struct(fields, headers, worden, arr_idx, &readings_idx);
		arr_idx++;
		delete_list(fields);
	}
	
	delete_list(lines);
	delete_list(headers);
	fclose(fp);
	
	return 0;
}



void store_fields_struct(struct list_t *fields, struct list_t *headers, struct worden807_t *worden, int idx, int *ridx)
{	
	struct node_t *field  = fields->head;
	struct node_t *header = headers->head;
	
	while(field != NULL) {
		uint8_t hidx = header->idx;
		
		if(hidx >= IDX_READING && hidx < 128) {
			worden->readings[*ridx] = (float)atof(field->data);
			*ridx += 1;
		}
		else if(hidx == IDX_STATION)
			worden->stations[idx] = (float)atof(field->data);
		else if(hidx == IDX_TIME_MIN)
			worden->times_min[idx] = (float)atof(field->data);
		else if(hidx == IDX_TIME)
			worden->times[idx] = (float)atof(field->data);
		else if(idx == 0 && hidx == IDX_TEMP)
			worden->operation_temp = (float)atof(field->data);
		else if(idx == 0 && hidx == IDX_DIR)
			worden->survey_dir = (float)atof(field->data);
		else if(idx == 0 && hidx == IDX_PURPOSE)
			worden->survey_purpose = strdup(field->data);
		else if(idx == 0 && hidx == IDX_LAT)
			worden->ref_station_lat = (float)atof(field->data);
		else if(idx == 0 && hidx == IDX_AREA)
			worden->survey_area = strdup(field->data);
		else if(idx == 0 && hidx == IDX_POI)
			worden->survey_poi = strdup(field->data);
		else if(idx == 0 && hidx == IDX_ADDRESS)
			worden->survey_address = strdup(field->data);
		else if(idx == 0 && hidx == IDX_DATE)
			worden->survey_date = strdup(field->data);
		
		header = header->next;
		field  = field->next;
	}	
}
