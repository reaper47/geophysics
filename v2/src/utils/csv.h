#ifndef CSV_H_
#define CSV_H_

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct delims_t {
	int comma;
	int semi;
	int colon;
	int pipe;
	int tab;
};

int num_lines_file(FILE *fp);
char *strtok_imprv(char *str, const char *delim);
char determine_delim(FILE *fp);

#endif /* csv.h */