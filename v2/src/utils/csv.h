#ifndef CSV_H_
#define CSV_H_

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if !defined(ssize_t)
	#define ssize_t long
#endif

unsigned int num_lines_file(FILE *fp);
char *strtok_imprv(char *str, const char *delim);

#endif /* csv.h */