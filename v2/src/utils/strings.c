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

void strlower(char *src, bool is_csv)
{
	int n = (int)strlen(src);
	for(int i = 0; i < n; i++)
		src[i] = (char)tolower(src[i]);
	
	if(is_csv)
		src[n-1] = '\0';
}
