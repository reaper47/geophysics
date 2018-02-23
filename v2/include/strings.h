/*******************************************************************
 * NAME: strings.h
 *
 * PURPOSE: Defines general-purpose functions on strings.
 *
 * GLOBAL VARIABLES: None
 *
 * Variable       Type         Description
 * --------       ----         -----------
 * 
 *
 *******************************************************************/
#ifndef STRINGS_H_
#define STRINGS_H_

#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

void   charptr_to_static   (char *str, char *buff, int cnt);
char  *concat              (const char *s1, const char *s2);
char  *rand_str_seq        (size_t size);
void   rm_spaces           (char *src);
void   strlower            (char *src, _Bool is_csv);

#endif /* STRINGS_H_ */

