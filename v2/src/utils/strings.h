#ifndef STRINGS_H_
#define STRINGS_H_

#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void rm_spaces(char *src);
char *strlower(char *src);

#endif /* STRINGS_H_ */