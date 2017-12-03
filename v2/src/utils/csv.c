#include "csv.h"

char determine_delim(FILE *fp)
{
	fseek(fp, 0, SEEK_SET);
	
	int c, nl;
	struct delims_t delims = {
		.comma = 0,
		.semi  = 0,
		.colon = 0,
		.pipe  = 0,
		.tab   = 0
	};
	
	nl = 0;
	while(nl < 10 && (c = fgetc(fp)) != EOF) {
		switch(c) {
			case ',':
				delims.comma++;
				break;
			case ';':
				delims.semi++;
				break;
			case ':':
				delims.colon++;
				break;
			case '|':
				delims.pipe++;
				break;
			case '	':
				delims.tab++;
				break;
			case '\n':
				++nl;
			default:
				break;
		}
	}
	
	char delim = 0;
	int max = 0;
	
	if(delims.comma > 0) {
		max = delims.comma;
		delim = ',';
	}
	
	if(delims.semi > max) {
		max = delims.semi;
		delim = ';';
	} 
	
	if(delims.colon > max) {
		max = delims.colon;
		delim = ':';
	}

	if(delims.pipe > max) {
		max = delims.pipe;
		delim = '|';
	}

	if(delims.tab > max)
		delim = '	';
	
	fseek(fp, 0, SEEK_SET);
	return delim;
}



int num_lines_file(FILE *fp)
{
	fseek(fp, 0, SEEK_SET);
	int c, nl;
	
	nl = 0;
	while((c = fgetc(fp)) != EOF)
		if(c == '\n')
			++nl;
	
	fseek(fp, 0, SEEK_SET);
	return nl;
}

char *strtok_imprv(char *str, const char *delim)
{
	// https://stackoverflow.com/questions/26522583/
	
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
