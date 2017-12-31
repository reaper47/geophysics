#include <stdio.h>
#include <stdlib.h>
#include "gravimetry.h"

int main(void)
{
	const char *grav_csv = "../data/uploads/grav2.csv";
	const char *delim = ";";
	
	struct worden807_t worden;	
	load_grav_csv(&worden, grav_csv, delim);
	
	free_worden807(&worden);
	return 0;
}

