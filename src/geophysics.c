#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/gravimetry.h"


int main(int argc, char **argv)
{
    if(argc < 3) {
        printf("usage: geophysics path_to_geophysics grav_file topo_file\n");
        printf("example: ./bin/geophysics ./data/uploads/grav.csv ./data/uploads/topo.csv\n");
        return 1;                     
    }
    
    const char *grav_path = argv[1];
    const char *topo_path = argv[2];
    
    struct worden807_t worden;
    if(load_grav_csv(&worden, grav_path, topo_path) < 0) {
        printf("error loading worden807\n");
        return 2;
    }
    populate_calc_fields_worden807(&worden);
    
    char *csv_path = generate_grav_csv(&worden, "./data/processed/");
    printf("csv file location: %s\n", csv_path);
    
    free_worden807(&worden);
	return 0;
}

