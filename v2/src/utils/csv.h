#ifndef CSV_H_
#define CSV_H_

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./sll.h"

#define NUM_LINES_PROCESS 10
#define LF '\n'
#define COMMA  44
#define APOSTROPHE 39
#define QUOTES 34

struct delims_t {
	int comma;
	int semi;
	int colon;
	int pipe;
	int tab;
};

char determine_delim(FILE *fp);
struct list_t *gather_lines(FILE *fp);
unsigned int num_lines_file(FILE *fp);
struct list_t *parse_header(FILE *fp, const char delim);
struct list_t *parse_line(const char *str, const char delim);

#endif /* CSV_H_ */
