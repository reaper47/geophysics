#include "../../include/strings.h"

void charptr_to_static(char *str, char *buff, int cnt)
{	
	for(int i = 0; i < cnt; ++i)
		buff[i] = str[i];
	buff[cnt] = '\0';	
}



char *concat(const char *s1, const char *s2)
{
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1);

    if(!result) {
        const char *err = "error when concatenating strings";
        return (char*)err;
    }

    memcpy(result, s1, len1);
    memcpy(result+len1, s2, len2 + 1);

    return result;    
}


#include <stdio.h>
char *rand_str_seq(size_t size)
{
    char *s = malloc(size + 1);
    
    if(s) {
        time_t t;
        srand((unsigned) time(&t));

        const char charset[] = 
            "0123456789"
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    
        int l = (int)(sizeof(charset) - 1);
        int key;
        
        for(size_t n = 0; n < size; n++) {
            key = rand() % l;
            s[n] = charset[key];
        }
        
        s[size] = '\0';
    }

    return s;
}



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



void strlower(char *src, _Bool is_csv)
{
	int n = (int)strlen(src);
	for(int i = 0; i < n; i++)
		src[i] = (char)tolower(src[i]);
	
	if(is_csv)
		src[n-1] = '\0';
}

