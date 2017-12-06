#include "strings.h"

void rm_spaces(char *src)
{
	// https://stackoverflow.com/questions/1726302/
	
	char *i = src;

	do {
		*i = *src;
		if(!isspace(*i))
			i++;
	} while(*src++ != 0);
}

char *strlower(char *src)
{
	char *str = malloc(sizeof(char) * strlen(src)+1);
	strcpy(str, src);
	
	int n = (int)strlen(str);
	for(int i = 0; i < n; i++)
		str[i] = (char)tolower(str[i]);
		
	return str;
}
