#include "../include/gravimetry.h"

int alloc_worden807(struct worden807_t *worden, unsigned int n)
{
    worden->altitudes               = malloc(sizeof(worden->altitudes) * n);
    worden->attraction_deviation    = malloc(sizeof(worden->attraction_deviation) * n);
    worden->avg_readings            = malloc(sizeof(worden->avg_readings) * n);
    worden->bouguer_anomaly         = malloc(sizeof(worden->bouguer_anomaly) * n);
    worden->bouguer_corr            = malloc(sizeof(worden->bouguer_corr) * n);
    worden->bouguer_rel_grav_fields = malloc(sizeof(worden->bouguer_rel_grav_fields) * n);
    worden->elevations              = malloc(sizeof(worden->elevations) * n);
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



void free_worden807(struct worden807_t *worden)
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



void assign_idx_node(struct list_t *list, struct worden807_t *worden)
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
            charptr_to_static(curr->data, s, n);

            char *p = strtok(s, TEMP_BTWN);
            p = strtok(NULL, TEMP_BTWN);
            
            worden->operation_temp_unit = (char)tolower(p[0]);
        }
        else if(strstr(curr->data, DIR) != NULL)
            curr->idx = IDX_DIR;
    }

    worden->num_readings = (uint8_t)(read_idx - IDX_READING);
}



double dial_const_worden807(struct worden807_t *worden)
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



int load_grav_csv(struct worden807_t *worden, const char *csv_file, const char *topo_file)
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

    unsigned int header_line_number = 1;
    worden->num_lines = num_lines - header_line_number;
    worden->topo_file = (char*)topo_file;

    const char delim = determine_delim(fp);
    
    struct list_t *lines = gather_lines(fp);
    struct list_t *headers = parse_header(fp, delim);
    assign_idx_node(headers, worden);
    
    int arr_idx = 0;
    int readings_idx = 0;
    
    for(struct node_t *node = lines->head; node != NULL; node = node->next) {
        struct list_t *fields = parse_line(node->data, delim);
        store_fields_struct(fields, headers, worden, arr_idx, &readings_idx);
        arr_idx++;
        del_list(fields);
    }
    
    del_list(lines);
    del_list(headers);
    fclose(fp);
    
    return 0;
}



char *generate_grav_csv(struct worden807_t *worden, const char *out_dir)
{
    unsigned int num_lines = worden->num_lines;
    char *path_csv = create_file_name(GRAV_PATH_LEN, out_dir, CSV);

    FILE *csv_fp = fopen(path_csv, "w+");
    if( !csv_fp ) {
        printf("failed to open %s\n", path_csv);
        const char *err = "-1";
        return (char*)err;
    }

    const char *csv_header_expected = 
        "Station (m),Time (hh:mm),Time (mm),Reading 1,Reading 2,Reading 3,"
        "Reading 4,Average reading (div),Standard deviation,"
        "Relative gravitational field (mGal),Uncorrected gravimetric "
        "anomaly (mGal),Temporal variations (mGal),Attraction/instrumental"
        " deviation (mGal),Latitude correction (mGal),Elevation (m),Altitude"
        " (m),Free air correction (mGal),Bouguer correction (mGal),Relative "
        "Bouguer gravitational field (mGal),Bouguer anomaly (mGal),Regional "
        "anomaly (mGal),Residual anomaly (mGal)";
        
    fprintf(csv_fp, "%s\n", csv_header_expected);
    
    int reading_idx = 0;
    for(unsigned int i = 0; i < num_lines; i++) {
        fprintf(csv_fp, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", 
                worden->stations[i], 
                worden->times[i], 
                worden->times_min[i],
                worden->readings[reading_idx],
                worden->readings[reading_idx+1],
                worden->readings[reading_idx+2],
                worden->readings[reading_idx+3],
                worden->avg_readings[i], 
                worden->std[i],
                worden->rel_grav_fields[i],
                worden->grav_anomaly_uncorr[i], 
                worden->temporal_vars[i],
                worden->attraction_deviation[i], 
                worden->lat_corr[i],
                worden->elevations[i], 
                worden->altitudes[i], 
                worden->free_air_corr[i], 
                worden->bouguer_corr[i],
                worden->bouguer_rel_grav_fields[i], 
                worden->bouguer_anomaly[i],
                worden->regional_anomaly[i], 
                worden->residual_anomaly[i]);
        reading_idx += 4;
    }

    fclose(csv_fp);
    return path_csv;
}



char *generate_grav_json(struct worden807_t *worden)
{
    unsigned int n = worden->num_lines;
    unsigned int i;
    
    char *s = NULL;
    json_t *root = json_object();

    json_t *json_arr_stations = json_array();
    json_t *json_arr_times = json_array();
    json_t *json_arr_times_min = json_array();
    json_t *json_arr_readings_avg = json_array();
    json_t *json_arr_std = json_array();
    json_t *json_arr_rel_grav = json_array();
    json_t *json_arr_grav_anom_uncorr = json_array();
    json_t *json_arr_temp_vars = json_array();
    json_t *json_arr_attract_dev = json_array();
    json_t *json_arr_lat_corr = json_array();
    json_t *json_arr_elev = json_array();
    json_t *json_arr_alts = json_array();
    json_t *json_arr_free_air = json_array();
    json_t *json_arr_bouguer_corr = json_array();
    json_t *json_arr_bouguer_rel = json_array();
    json_t *json_arr_bouguer_anom = json_array();
    json_t *json_arr_regional_anom = json_array();
    json_t *json_arr_residual_anom = json_array();
    
    for(i = 0; i < n; i++) {
        double station = worden->stations[i];
        double t = worden->times[i];
        double t_min = worden->times_min[i];
        double avg = worden->avg_readings[i];
        double std = worden->std[i];
        double rel = worden->rel_grav_fields[i];
        double anom = worden->grav_anomaly_uncorr[i];
        double temp = worden->temporal_vars[i];
        double dev = worden->attraction_deviation[i];
        double lat = worden->lat_corr[i];
        double elev = worden->elevations[i];
        double alt = worden->altitudes[i];
        double air = worden->free_air_corr[i];
        double bcorr = worden->bouguer_corr[i];
        double brel = worden->bouguer_rel_grav_fields[i];
        double banom = worden->bouguer_anomaly[i];
        double reg = worden->regional_anomaly[i];
        double res = worden->residual_anomaly[i];
        
        json_array_append_new(json_arr_stations, json_real(station));
        json_array_append_new(json_arr_times, json_real(t));
        json_array_append_new(json_arr_times_min, json_real(t_min));
        json_array_append_new(json_arr_readings_avg, json_real(avg));
        json_array_append_new(json_arr_std, json_real(std));
        json_array_append_new(json_arr_rel_grav, json_real(rel));
        json_array_append_new(json_arr_grav_anom_uncorr, json_real(anom));
        json_array_append_new(json_arr_temp_vars, json_real(temp));
        json_array_append_new(json_arr_attract_dev, json_real(dev));
        json_array_append_new(json_arr_lat_corr, json_real(lat));
        json_array_append_new(json_arr_elev, json_real(elev));
        json_array_append_new(json_arr_alts, json_real(alt));
        json_array_append_new(json_arr_free_air, json_real(air));
        json_array_append_new(json_arr_bouguer_corr, json_real(bcorr));
        json_array_append_new(json_arr_bouguer_rel, json_real(brel));
        json_array_append_new(json_arr_bouguer_anom, json_real(banom));
        json_array_append_new(json_arr_regional_anom, json_real(reg));
        json_array_append_new(json_arr_residual_anom, json_real(res));
    }
    
    json_object_set_new(root, "stations", json_arr_stations);
    json_object_set_new(root, "times", json_arr_times);
    json_object_set_new(root, "times_min", json_arr_times_min);
    json_object_set_new(root, "readings_avg", json_arr_readings_avg);
    json_object_set_new(root, "std", json_arr_std);
    json_object_set_new(root, "rel_grav_fields", json_arr_rel_grav);
    json_object_set_new(root, "grav_anom_uncorr", json_arr_grav_anom_uncorr);
    json_object_set_new(root, "temp_vars", json_arr_temp_vars);
    json_object_set_new(root, "attract_dev", json_arr_attract_dev);
    json_object_set_new(root, "lat_corr", json_arr_lat_corr);
    json_object_set_new(root, "elev", json_arr_elev);
    json_object_set_new(root, "alts", json_arr_alts);
    json_object_set_new(root, "free_air_corr", json_arr_free_air);
    json_object_set_new(root, "bouguer_corr", json_arr_bouguer_corr);
    json_object_set_new(root, "bouguer_rel", json_arr_bouguer_rel);
    json_object_set_new(root, "bouguer_anom", json_arr_bouguer_anom);
    json_object_set_new(root, "regional_anom", json_arr_regional_anom);
    json_object_set_new(root, "residual_anom", json_arr_residual_anom);
    json_object_set_new(root, "survey_purpose", json_string("LABORATOIRE 1 - LEVE GRAVIMETRIQUE"));
    json_object_set_new(root, "survey_area", json_string("LES PLAINES D’ABRAHAM - QUEBEC"));
    json_object_set_new(root, "survey_poi", json_string("RESERVOIR D'EAU MUNICIPAL DE LA VILLE DE QUEBEC"));
    json_object_set_new(root, "survey_addr", json_string("\"555 plaines abraham, quebec, canada, g2j 5h6\""));
    json_object_set_new(root, "survey_date", json_string("1997-12-09"));
    json_object_set_new(root, "op_temp", json_real(74.0));
    json_object_set_new(root, "ref_lat", json_real(46.8));
    json_object_set_new(root, "survey_dir", json_real(34.25));
    json_object_set_new(root, "op_temp_unit", json_string("F"));

    s = json_dumps(root, 0);
    
    json_decref(root);
    return s;
}



void store_avg_readings_std(struct worden807_t *worden, int is_std)
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
                worden->avg_readings[pos] = avg_arr(readings, num_readings);
            else {
                double avg = worden->avg_readings[pos];
                worden->std[pos] = std_arr(readings, avg, num_readings);
            }

	        c = 0;
	        pos++;
	    }
    }
}



void store_fields_struct(struct list_t *fields, struct list_t *headers, struct worden807_t *worden, int idx, int *ridx)
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



void store_grav_anomaly_uncorr(struct worden807_t *worden)
{
    double ref_station_grav = worden->rel_grav_fields[0];
    unsigned int num_lines = worden->num_lines;
    double grav;

    for(unsigned int i = 0; i < num_lines; i++) {
	    grav = worden->rel_grav_fields[i];
	    worden->grav_anomaly_uncorr[i] = ref_station_grav - grav;
    }	
}



void store_rel_grav_fields(struct worden807_t *worden)
{
    unsigned int num_lines = worden->num_lines;
    double dial_const = dial_const_worden807(worden);
	
    for(unsigned int i = 0; i < num_lines; i++)
	    worden->rel_grav_fields[i] = worden->avg_readings[i] * dial_const;
}



void store_temporal_vars(struct worden807_t *worden)
{
    unsigned int num_lines = worden->num_lines;
    double epsilon = 1e-1;
    double station, anomaly;
	
    for(unsigned int i = 0; i < num_lines; i++) {
	    station = worden->stations[i];

	if(approx_eq(station, REF_STATION, epsilon)) {
	    anomaly = worden->grav_anomaly_uncorr[i];
	    worden->temporal_vars[i] = -anomaly;
	} else {
	    worden->temporal_vars[i] = 0.0;
	}
    }
}



void store_attraction_deviation(struct worden807_t *worden)
{
    unsigned int num_lines = worden->num_lines;

    double epsilon = 1e-1;
    int ref_station_count = 1;
    int cpy_idx = 0;

    unsigned int start_idx = 0;
    double start_value = 0.0;
    double end_value = 0.0;
    
    int num_steps = 0;

    double station;
    _Bool is_approx_eq;

    for(unsigned int i = 0; i < num_lines; i++) {
        station = worden->stations[i];
        is_approx_eq = approx_eq(station, REF_STATION, epsilon);
                              
        if(ref_station_count == 2 && is_approx_eq) {
            end_value = worden->temporal_vars[i];
            num_steps++;
                    
            double *results = interpolate_pts(start_value, end_value, num_steps);
                
            for(int k = 0; k < num_steps; k++) {
                worden->attraction_deviation[cpy_idx] = results[k];
                cpy_idx++;	
            }
            free(results);

            num_steps = 0;
            start_idx = i;
            ref_station_count++;
        } else if(ref_station_count > 2 && is_approx_eq) {
            struct point_t pt1 = { 
                worden->times_min[start_idx], 
                worden->temporal_vars[start_idx]
            };
            
            struct point_t pt2 = {
                worden->times_min[i], 
                worden->temporal_vars[i]
            };
            
            double slope = -(pt2.y - pt1.y) / (pt2.x - pt1.x);
            double intercept = line_intercept(pt1, pt2, slope);
            
            for(unsigned int k = start_idx+1; k <= i; k++) {
                double y = slope * worden->times_min[k] + intercept;
                worden->attraction_deviation[cpy_idx] = y;
                cpy_idx++;	
            }
            
            start_idx = i;
            ref_station_count++;
        } else if(is_approx_eq) {
            start_value = worden->temporal_vars[i];
            num_steps++;
            ref_station_count++;
        } else {
            num_steps++;
        }
    }
}



void store_lat_corr(struct worden807_t *worden)
{
    unsigned int num_lines = worden->num_lines;

    double lat = worden->ref_station_lat;
    double lng = worden->survey_dir;

    for(unsigned int i = 0; i < num_lines; i++)
	    worden->lat_corr[i] = correct_latitude(lat, lng, worden->stations[i]);
}



void set_station_num_before_return_to_ref(struct worden807_t *worden, struct topo_t *topo)
{
    unsigned int num_lines = worden->num_lines;
    double next_station;

    for(unsigned int i = 0; i < num_lines-1; i++) {
	    next_station = worden->stations[i+1];
		
        if(approx_eq(next_station, 0.0, 1e-1)) {
	        topo->station_num_before_return_to_ref = (int)i;
	        return;
	    }
    }
}



void transfer_topo_data_to_grav(struct topo_t *topo, struct worden807_t *worden)
{
    unsigned int num_lines = worden->num_lines;
    double altitude, elevation;
    int idx = 1;

    for(unsigned int i = 0; i < num_lines; i++) {

    	if(approx_eq(worden->stations[i], REF_STATION, 1e-1)) {
            altitude = topo->altitudes[0];
	        elevation = topo->elevation_corr[0];		
    	} else {
	        altitude = topo->altitudes[idx];
	        elevation = topo->elevation_corr[idx];
	        idx++;
	    }
        
        worden->elevations[i] = elevation;
    	worden->altitudes[i] = altitude; 
    }
}



void store_free_air_corr(struct worden807_t *worden)
{
    unsigned int num_lines = worden->num_lines;
    double h;

    for(unsigned int i = 0; i < num_lines; i++) {
	    h = worden->altitudes[i];
	    
	    worden->free_air_corr[i] = FREE_AIR_VARIATION * h;
    }
}



void store_bouguer_corr(struct worden807_t *worden)
{
    unsigned int num_lines = worden->num_lines;

    double h;
    for(unsigned int i = 0; i < num_lines; i++) {
        h = worden->altitudes[i];

        worden->bouguer_corr[i] = -TWO_PI_G * DENSITY_G_CM3 * h;
    }
}



void store_bouguer_rel_grav_fields(struct worden807_t *worden)
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



void store_bouguer_anomaly(struct worden807_t *worden)
{
    unsigned int num_lines = worden->num_lines;
	
    double bouguer_rel_grav_ref = worden->bouguer_rel_grav_fields[0];
    double curr_bouguer_rel_grav;

    for(unsigned int i = 0; i < num_lines; i++) {
        curr_bouguer_rel_grav = worden->bouguer_rel_grav_fields[i];

	    worden->bouguer_anomaly[i] = curr_bouguer_rel_grav - bouguer_rel_grav_ref;
    }	
}



void store_regional_anomaly(struct worden807_t *worden)
{
    unsigned int num_lines = worden->num_lines;

    double max_station = max_arr(worden->stations, (int)num_lines);
    double endpoint = 0.30;

    for(unsigned int i = 0; i < num_lines; i++) {
        double progression = worden->stations[i] / max_station;

        worden->regional_anomaly[i] = endpoint * progression;
    }
}



void store_residual_anomaly(struct worden807_t *worden)
{
    unsigned int num_lines = worden->num_lines;

    double bouguer, regional;
    for(unsigned int i = 0; i < num_lines; i++) {
        bouguer = worden->bouguer_anomaly[i];
        regional = worden->regional_anomaly[i];

        worden->residual_anomaly[i] = bouguer - regional;
    }
}



int populate_calc_fields_worden807(struct worden807_t *worden)
{
    store_avg_readings_std(worden, 0);
    store_avg_readings_std(worden, 1);
    store_rel_grav_fields(worden);
    store_grav_anomaly_uncorr(worden);
    store_temporal_vars(worden);
    store_attraction_deviation(worden);
    store_lat_corr(worden);
    
    struct topo_t topo;
    if(load_topo_csv(&topo, worden->topo_file) != 0) {
        printf("failed opening %s\n", worden->topo_file);
        return -1;
    }

    set_station_num_before_return_to_ref(worden, &topo);
    populate_calc_fields(&topo);
    transfer_topo_data_to_grav(&topo, worden);
    free_topo(&topo, 1);

    store_free_air_corr(worden);
    store_bouguer_corr(worden);
    store_bouguer_rel_grav_fields(worden);
    store_bouguer_anomaly(worden);
    store_regional_anomaly(worden);
    store_residual_anomaly(worden);

    return 0;
}

