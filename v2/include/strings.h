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

void   CharPtrToStatic  (char *str, char *buff, int cnt); 
void   RmSpaces         (char *src);
void   StrLower         (char *src, _Bool is_csv);

#endif /* STRINGS_H_ */
