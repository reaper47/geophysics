#include "strings.h"

void charptr_to_static(char *str, char *buff, int cnt)
{	
	for(int i = 0; i < cnt; ++i)
		buff[i] = str[i];
	buff[cnt] = '\0';	
}



char *rand_str_seq(size_t size)
{
    char *s = malloc(size + 1);
    
    if(s != NULL) {
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

