#include "utils/csv.h"
#include "gravimetry.h"

int init_worden807(struct worden807_t *worden, unsigned int n)
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
	free( worden->residual_anomaly);
	free(worden->stations);
	free(worden->std);
	free(worden->temporal_vars);
	free(worden->times);
	free(worden->times_min);
}

void load_grav_csv(struct worden807_t *worden, const char *csv_file, const char *delim)
{
	FILE *fp = fopen(csv_file, "rb");
	if(fp == NULL) {
		printf("unable to open %s\n", csv_file);
		exit(EXIT_FAILURE);
	}
	
	char *tok, *line = NULL;
	ssize_t read;
	size_t len = 0;
	int i;
	
	unsigned int num_lines = num_lines_file(fp);
	init_worden807(worden, num_lines);
	num_lines = 0;
	
	while((read = getline(&line, &len, fp)) != -1) {
		if(num_lines == 0) {
			num_lines++;
			continue;
		}
		
		tok = strtok_imprv(line, delim);
		i = 0;
		
		while(tok != NULL) {
			if(i == 0)
				worden->stations[num_lines-1] = (float)atof(tok);
			else if(i == 2)
				worden->times[num_lines-1] = (float)atof(tok);
			else if(i == 3)
				worden->times_min[num_lines-1] = (float)atof(tok);
			else if(i > 3)
				worden->readings[(int)(num_lines-1)*NUM_READINGS + (i - NUM_READINGS)] = (float)atof(tok);
			
			i++;
			tok = strtok_imprv(NULL, delim);
		}
		
		num_lines++;
	}
	
	free(line);
	fclose(fp);
	return;
}
