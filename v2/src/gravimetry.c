#include "../include/gravimetry.h"

int AllocWorden807(struct worden807_t *worden, unsigned int n)
{
	worden->altitudes               = malloc(sizeof(worden->altitudes) * n);
	worden->attraction_deviation    = malloc(sizeof(worden->attraction_deviation) * n);
	worden->avg_readings            = malloc(sizeof(worden->avg_readings) * n);
	worden->bouguer_anomaly         = malloc(sizeof(worden->bouguer_anomaly) * n);
	worden->bouguer_corr            = malloc(sizeof(worden->bouguer_corr) * n);
	worden->bouguer_rel_grav_fields = malloc(sizeof(worden->bouguer_rel_grav_fields) * n);
	worden->elevations		= malloc(sizeof(worden->elevations) * n);
	worden->free_air_corr           = malloc(sizeof(worden->free_air_corr) * n);
	worden->grav_anomaly_uncorr     = malloc(sizeof(worden->grav_anomaly_uncorr) * n);
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
	worden->num_readings = 0;
	worden->operation_temp_unit = DEFAULT_TEMP_UNIT;
	
	if(worden->attraction_deviation     == NULL || worden->avg_readings     == NULL ||
		worden->bouguer_anomaly         == NULL || worden->bouguer_corr     == NULL ||
		worden->bouguer_rel_grav_fields == NULL || worden->free_air_corr    == NULL ||
		worden->grav_anomaly_uncorr     == NULL || worden->lat_corr         == NULL ||
		worden->readings                == NULL || worden->rel_grav_fields  == NULL ||
		worden->regional_anomaly        == NULL || worden->residual_anomaly == NULL ||
		worden->stations                == NULL || worden->std              == NULL ||
		worden->temporal_vars           == NULL || worden->times            == NULL ||
		worden->times_min               == NULL || worden->elevations       == NULL ||
		worden->altitudes               == NULL)
		return -1;

	return 0;
}



void FreeWorden807(struct worden807_t *worden)
{
	free(worden->altitudes);
	free(worden->attraction_deviation);
	free(worden->avg_readings);
	free(worden->bouguer_anomaly);
	free(worden->bouguer_corr);
	free(worden->bouguer_rel_grav_fields);
	free(worden->elevations);
	free(worden->free_air_corr);
	free(worden->grav_anomaly_uncorr);
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



void AssignIdxNode(struct list_t *list, struct worden807_t *worden)
{
	struct node_t *curr;
	uint8_t read_idx = IDX_READING;
	
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
		else if(strstr(curr->data, TEMP) != NULL) {
			curr->idx = IDX_TEMP;

			int n = (int)strlen(curr->data);
			char s[n+1];
			CharPtrToStatic(curr->data, s, n);

			char *unit = NULL;
			for(char *p = strtok(s, TEMP_BTWN); p != NULL; p = strtok(NULL, TEMP_BTWN))
				unit = strdup(p);
			worden->operation_temp_unit = (char)tolower(unit[0]);
			free(unit);
		}
		else if(strstr(curr->data, DIR) != NULL)
			curr->idx = IDX_DIR;
	}

	worden->num_readings = (uint8_t)(read_idx - IDX_READING);
}



double DialConstWorden807(struct worden807_t *worden)
{
	const double uppery = WORDEN807_UPPERY;
	const double lowery = WORDEN807_LOWERY;

	double leftx, rightx, dial_const;
	leftx = rightx = dial_const = 0.0;

	if(worden->operation_temp_unit == CELSIUS) {
		leftx  = WORDEN807_LEFTX_C;
		rightx = WORDEN807_RIGHTX_C;
	} else if(worden->operation_temp_unit == FAHR) {
		leftx  = WORDEN807_LEFTX_F;
		rightx = WORDEN807_RIGHTX_F;
	} else {
		return INVALID_TEMP;
	}

	return ((uppery-lowery)/(leftx-rightx)) * worden->operation_temp + lowery;
}



int LoadGravCsv(struct worden807_t *worden, const char *csv_file, const char *topo_file)
{
	FILE *fp = fopen(csv_file, "rb");
	if(fp == NULL) {
		printf("unable to open %s\n", csv_file);
		fclose(fp);
		exit(EXIT_FAILURE);
	}
		
	unsigned int num_lines = NumLinesFile(fp);
	if(AllocWorden807(worden, num_lines) != 0) {
		printf("malloc worden807 failed\n");
		return -1;
	}

	unsigned int header_line_number = 1;
	worden->num_lines = num_lines - header_line_number;
        worden->topo_file = (char*)topo_file;

	const char delim = DetermineDelim(fp);
	
	struct list_t *lines = GatherLines(fp);
	struct list_t *headers = ParseHeader(fp, delim);
	AssignIdxNode(headers, worden);
	
	int arr_idx = 0;
	int readings_idx = 0;
	
	for(struct node_t *node = lines->head; node != NULL; node = node->next) {
		struct list_t *fields = ParseLine(node->data, delim);
		StoreFieldsStruct(fields, headers, worden, arr_idx, &readings_idx);
		arr_idx++;
		DeleteList(fields);
	}
	
	DeleteList(lines);
	DeleteList(headers);
	fclose(fp);
	
	return 0;
}



void StoreAvgReadingsStd(struct worden807_t *worden, int is_std)
{
	uint8_t num_readings = worden->num_readings;
	unsigned int n = worden->num_lines * num_readings;

	int pos = 0;
	int c = 0;
	double readings[num_readings];

	for(unsigned int i = 0; i < n; i++) {
		readings[c] = worden->readings[i];
		c++;

		if(c == num_readings) {
			if(is_std == 0)
				worden->avg_readings[pos] = AvgArr(readings, num_readings);
			else {
				double avg = worden->avg_readings[pos];
				worden->std[pos] = StdArr(readings, avg, num_readings);
			}

			c = 0;
			pos++;
		}
	}

}



void StoreFieldsStruct(struct list_t *fields, struct list_t *headers, struct worden807_t *worden, int idx, int *ridx)
{	
	struct node_t *field  = fields->head;
	struct node_t *header = headers->head;
	
	while(field != NULL) {
		uint8_t hidx = header->idx;
		
		if(hidx >= IDX_READING && hidx < 128) {
			worden->readings[*ridx] = atof(field->data);
			*ridx += 1;
		}
		else if(hidx == IDX_STATION)
			worden->stations[idx] = atof(field->data);
		else if(hidx == IDX_TIME_MIN)
			worden->times_min[idx] = atof(field->data);
		else if(hidx == IDX_TIME)
			worden->times[idx] = atof(field->data);
		else if(idx == 0 && hidx == IDX_TEMP) 
			worden->operation_temp = atof(field->data);
		else if(idx == 0 && hidx == IDX_DIR)
			worden->survey_dir = atof(field->data);
		else if(idx == 0 && hidx == IDX_PURPOSE)
			worden->survey_purpose = strdup(field->data);
		else if(idx == 0 && hidx == IDX_LAT)
			worden->ref_station_lat = atof(field->data);
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



void StoreGravAnomalyUncorr(struct worden807_t *worden)
{
	double ref_station_grav = worden->rel_grav_fields[0];
	unsigned int num_lines = worden->num_lines;
	double grav;

	for(unsigned int i = 0; i < num_lines; i++) {
		grav = worden->rel_grav_fields[i];
		worden->grav_anomaly_uncorr[i] = ref_station_grav - grav;
	}	
}



void StoreRelGravFields(struct worden807_t *worden)
{
	unsigned int num_lines = worden->num_lines;
	double dial_const = DialConstWorden807(worden);
	
	for(unsigned int i = 0; i < num_lines; i++)
		worden->rel_grav_fields[i] = worden->avg_readings[i] * dial_const;

}



void StoreTemporalVars(struct worden807_t *worden)
{
	unsigned int num_lines = worden->num_lines;
	double epsilon = 1e-1;
	double station, anomaly;
	
	for(unsigned int i = 0; i < num_lines; i++) {
		station = worden->stations[i];

		if(ApproxEq(station, REF_STATION, epsilon)) {
			anomaly = worden->grav_anomaly_uncorr[i];
			worden->temporal_vars[i] = -anomaly;
		} else {
			worden->temporal_vars[i] = 0.0;
		}
	}
}



void StoreAttractionDeviation(struct worden807_t *worden)
{
	unsigned int num_lines = worden->num_lines;

	double epsilon = 1e-1;
	int ref_station_count = 1;
	int cpy_idx = 0;

	double start_value = 0.0;
	double end_value = 0.0;
	int num_steps = 0;

	double station;
	_Bool is_approx_eq;

	for(unsigned int i = 0; i < num_lines; i++) {
		station = worden->stations[i];
		is_approx_eq = ApproxEq(station, REF_STATION, epsilon);
					
		if(ref_station_count == 2 && is_approx_eq) {
			end_value = worden->temporal_vars[i];
			num_steps++;
			
			double *results = InterpolatePts(start_value, end_value, num_steps);
			
			for(int k = 0; k < num_steps; k++) {
				worden->attraction_deviation[cpy_idx] = results[k];
				cpy_idx++;	
			}
			free(results);

			num_steps = 0;
			ref_station_count--;
		} else if(is_approx_eq) {
			start_value = worden->temporal_vars[i];
			num_steps++;
			ref_station_count++;
		} else {
			num_steps++;
		}
	}
}



void StoreLatCorr(struct worden807_t *worden)
{
	unsigned int num_lines = worden->num_lines;

	double lat = worden->ref_station_lat;
	double lng = worden->survey_dir;

	for(unsigned int i = 0; i < num_lines; i++)
		worden->lat_corr[i] = CorrectLatitude(lat, lng, worden->stations[i]);
}



void SetStationNumBeforeReturnToRef(struct worden807_t *worden, struct topo_t *topo)
{
	unsigned int num_lines = worden->num_lines;
	double next_station;

	for(unsigned int i = 0; i < num_lines-1; i++) {
		next_station = worden->stations[i+1];
		
		if(ApproxEq(next_station, 0.0, 1e-1))
			topo->station_num_before_return_to_ref = (int)i;
	}
}



void TransferTopoDataToGrav(struct topo_t *topo, struct worden807_t *worden)
{
	unsigned int num_lines = worden->num_lines;
	double altitude, elevation;

	for(unsigned int i = 0; i < num_lines; i++) {

		if(ApproxEq(worden->stations[i], REF_STATION, 1e-1)) {
			altitude = topo->altitudes[0];
			elevation = topo->elevation_corr[0];			
		} else {
			altitude = topo->altitudes[i];
			elevation = topo->elevation_corr[i];
		}

		worden->altitudes[i] = altitude;
		worden->elevations[i] = elevation;
	}
}



void StoreFreeAirCorr(struct worden807_t *worden)
{
	unsigned int num_lines = worden->num_lines;
	double h;

	for(unsigned int i = 0; i < num_lines; i++) {
		h = worden->altitudes[i];
		
		worden->free_air_corr[i] = FREE_AIR_VARIATION * h;
	}
}



void StoreBouguerCorr(struct worden807_t *worden)
{
    unsigned int num_lines = worden->num_lines;

    double h;
    for(unsigned int i = 0; i < num_lines; i++) {
        h = worden->altitudes[i];

        worden->bouguer_corr[i] = -TWO_PI_G * DENSITY_G_CM3 * h;
    }
}



void StoreBouguerRelGravFields(struct worden807_t *worden)
{
    unsigned int num_lines = worden->num_lines;

    double rel_grav_field;
    double temporal_var;
    double lat_corr;
    double free_air_corr;
    double bouguer_corr;
    double cumulative_corr;

    for(unsigned int i = 0; i < num_lines; i++) {
        rel_grav_field = worden->rel_grav_fields[i];
        temporal_var = worden->temporal_vars[i];
        lat_corr = worden->lat_corr[i];
        free_air_corr = worden->free_air_corr[i];
        bouguer_corr = worden->bouguer_corr[i];

        cumulative_corr = lat_corr + free_air_corr + bouguer_corr;

        worden->bouguer_rel_grav_fields[i] = rel_grav_field - temporal_var + cumulative_corr; 	
    }	
}



void StoreBouguerAnomaly(struct worden807_t *worden)
{
    unsigned int num_lines = worden->num_lines;
	
    double bouguer_rel_grav_ref = worden->bouguer_rel_grav_fields[0];
    double curr_bouguer_rel_grav;

    for(unsigned int i = 0; i < num_lines; i++) {
        curr_bouguer_rel_grav = worden->bouguer_rel_grav_fields[i];

	worden->bouguer_anomaly[i] = curr_bouguer_rel_grav - bouguer_rel_grav_ref;
    }	
}



void StoreRegionalAnomaly(struct worden807_t *worden)
{
    unsigned int num_lines = worden->num_lines;

    double max_station = MaxArr(worden->stations, (int)num_lines);
    double endpoint = 0.30;

    for(unsigned int i = 0; i < num_lines; i++) {
        double progression = worden->stations[i] / max_station;

        worden->regional_anomaly[i] = endpoint * progression;
    }
}



void StoreResidualAnomaly(struct worden807_t *worden)
{
    unsigned int num_lines = worden->num_lines;

    double bouguer, regional;
    for(unsigned int i = 0; i < num_lines; i++) {
        bouguer = worden->bouguer_anomaly[i];
        regional = worden->regional_anomaly[i];

        worden->residual_anomaly[i] = bouguer - regional;
    }
}



void PopulateCalcFieldsWorden807(struct worden807_t *worden)
{
    StoreAvgReadingsStd(worden, 0);
    StoreAvgReadingsStd(worden, 1);
    StoreRelGravFields(worden);
    StoreGravAnomalyUncorr(worden);
    StoreTemporalVars(worden);
    StoreAttractionDeviation(worden);
    StoreLatCorr(worden);

    struct topo_t topo;
    if(LoadTopoCsv(&topo, worden->topo_file) != 0)
        printf("failed opening %s\n", worden->topo_file);

    PopulateCalcFields(&topo);
    TransferTopoDataToGrav(&topo, worden);
    FreeTopo(&topo);

    StoreFreeAirCorr(worden);
    StoreBouguerCorr(worden);
    StoreBouguerRelGravFields(worden);
    StoreBouguerAnomaly(worden);
    StoreRegionalAnomaly(worden);
    StoreResidualAnomaly(worden);
}

