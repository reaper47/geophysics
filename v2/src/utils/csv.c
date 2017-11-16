#include "csv.h"

unsigned int num_lines_file(FILE *fp)
{
	ssize_t read;
	char *line = NULL;
	size_t len = 0;
	
	unsigned int n = 0;
	while((read = getline(&line, &len, fp)) != -1)
		n++;
	
	fseek(fp, 0, SEEK_SET);
	free(line);
	return n;
}

char *strtok_imprv(char *str, const char *delim)
{
	// see https://stackoverflow.com/questions/26522583/
	
	static char *tpos;
	static char *tok;
	static char *pos = NULL;
	static char savech;
	
	if(str != NULL) {
		pos = str;
		savech = 'x';
	} else {
		if(pos == NULL)
			return NULL;
		
		while(*pos != '\0')
			pos++;
		*pos++ = savech;
	}
	
	if(savech == '\0')
		return NULL;
	
	tpos = pos;
	while(*tpos != '\0') {
		tok = strchr(delim, *tpos);
		if(tok != NULL)
			break;
		tpos++;
	}
	
	savech = *tpos;
	*tpos = '\0';
	
	return pos;
}
