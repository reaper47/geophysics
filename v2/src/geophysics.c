#include <stdio.h>
#include <stdlib.h>
#include "../include/gravimetry.h"

int main(void)
{
	const char *grav_csv = "../data/uploads/grav2.csv";
	const char *topo_csv = "../data/uploads/topo.csv";

	struct worden807_t worden;	
	LoadGravCsv(&worden, grav_csv, topo_csv);
	
	FreeWorden807(&worden);
	return 0;
}

