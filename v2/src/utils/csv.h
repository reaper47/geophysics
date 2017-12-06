#ifndef CSV_H_
#define CSV_H_

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./sll.h"

#define NUM_LINES_PROCESS 10
#define LF '\n'
#define FREE_DATA true

struct delims_t {
	int comma;
	int semi;
	int colon;
	int pipe;
	int tab;
};

char determine_delim(FILE *fp);
int num_lines_file(FILE *fp);
struct list_t *parse_header(FILE *fp);
char *strtok_imprv(char *str, const char *delim);

#endif /* CSV_H_ */