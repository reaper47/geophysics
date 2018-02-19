#include "../../include/strings.h"

void CharPtrToStatic(char *str, char *buff, int cnt)
{	
	for(int i = 0; i < cnt; ++i)
		buff[i] = str[i];
	buff[cnt] = '\0';	
}



void RmSpaces(char *src)
{
	// https://stackoverflow.com/questions/1726302/
	
	char *i = src;

	do {
		*i = *src;
		if(!isspace(*i))
			i++;
	} while(*src++ != 0);
}



void StrLower(char *src, _Bool is_csv)
{
	int n = (int)strlen(src);
	for(int i = 0; i < n; i++)
		src[i] = (char)tolower(src[i]);
	
	if(is_csv)
		src[n-1] = '\0';
}

