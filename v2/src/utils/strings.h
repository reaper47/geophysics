#ifndef STRINGS_H_
#define STRINGS_H_

#define _GNU_SOURCE
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void charptr_to_static(char *str, char *buff, int cnt); 
void rm_spaces(char *src);
void strlower(char *src, bool is_csv);

#endif /* STRINGS_H_ */
