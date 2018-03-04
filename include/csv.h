/*******************************************************************
 * NAME: csv.h
 *
 * PURPOSE: Defines utility functions for manipulating csv files.
 * 
 * GLOBAL VARIABLES: None
 *
 * Variables              Type      Description
 * ---------              ----      -----------
 * APOSTROPHE             int       ASCII id of an apostrophe
 *
 * COMMA                  int       ASCII id of a comma
 *
 * LF                     char      ASCII id of a line feed
 *
 * NUM_LINES_PROCESS      int       Number of lines to read
 *                                  before determining the csv
 *                                  file's delimiter
 *
 * QUOTES                 int       ASCII id of a quote
 *
 * delims_t               struct    Holds the number of occurences
 *                                  for every delimiter 
 *
 *    colon                  int    Number of colon occurences
 *    comma                  int    Number of comma occurences
 *     pipe                  int    Number of pipe occurences
 *     semi                  int    Number of semicolon occurences
 *      tab                  int    Number of tab occurences
 *
 *******************************************************************/
#ifndef CSV_H_
#define CSV_H_

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "./sll.h"

#define NUM_LINES_PROCESS   10
#define LF                  '\n'
#define COMMA               44
#define APOSTROPHE          39
#define QUOTES              34

struct delims_t {
    int colon;
    int comma;
    int pipe;
    int semi;
    int tab;
};

char              *create_file_name   (size_t n, const char *out_dir, const char *ext);
char               determine_delim    (FILE *fp);
struct list_t     *gather_lines       (FILE *fp);
unsigned int       num_lines_file     (FILE *fp);
struct list_t     *parse_header       (FILE *fp, const char delim);
struct list_t     *parse_line         (const char *str, const char delim);

#endif /* CSV_H_ */

