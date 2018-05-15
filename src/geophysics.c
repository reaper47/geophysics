#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "gravimetry.h"


int main(int argc, char **argv)
{
    if(argc < 3) {
        printf("usage: geophysics path_to_geophysics grav_file topo_file\n");
        printf("example: ./bin/geophysics ./data/uploads/grav.csv ./data/uploads/topo.csv\n");
        return 1;                     
    }
    
    const char *grav_path = argv[1];
    const char *topo_path = argv[2];
    
    struct worden807_t worden = load_grav_csv(grav_path, topo_path);
    populate_calc_fields_worden807(&worden);
    
    char *csv_path = generate_grav_csv(&worden, "./data/processed/");
    char *json = generate_grav_json(&worden);
    printf("%s|%s", json, csv_path);
    
    free_worden807(&worden);
	return 0;
}

