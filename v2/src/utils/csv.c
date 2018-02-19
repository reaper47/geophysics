#include "../../include/csv.h"

char DetermineDelim(FILE *fp)
{
	rewind(fp);
	
	int c, nl;
	struct delims_t delims = {
		.comma = 0,
		.semi  = 0,
		.colon = 0,
		.pipe  = 0,
		.tab   = 0
	};
	
	nl = 0;
	while(nl < NUM_LINES_PROCESS && (c = fgetc(fp)) != EOF) {
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



struct list_t *GatherLines(FILE *fp)
{
	rewind(fp);
	struct list_t *list = CreateList();
	
	char  *line   = NULL;
	size_t len    = 0;
	int    header = 1;
	long   read;
	
	while((read = getline(&line, &len, fp)) != -1) {
		if(header == 1) {
			header = 0;
			continue;
		}
		
		size_t n = strlen(line);
		char *buffer = malloc(sizeof(char) * n+1);
		
		for(int i = 0; i < (int)n; i++)
			buffer[i] = line[i];
		buffer[(int)n-1] = '\0';
		
		AddHeadList(list, buffer);
		free(buffer);
	}
	
	if(line)
		free(line);
	
	rewind(fp);
	return list;
}



unsigned int NumLinesFile(FILE *fp)
{
	rewind(fp);
	
	int c;
	unsigned int nl;
	
	nl = 0;
	while((c = fgetc(fp)) != EOF)
		if(c == '\n')
			++nl;

	rewind(fp);
	return nl;
}



struct list_t *ParseHeader(FILE *fp, const char delim)
{
	struct list_t *list = CreateList();
	
	char *line = NULL, *tok, *str, *tofree;
	size_t len = 0;
	const char delim_cpy = delim;
	
	if(getline(&line, &len, fp) != 0) {
		tofree = str = strdup(line);
		StrLower(str, 1);
		
		while((tok = strsep(&str, &delim_cpy)))
			AddHeadList(list, tok);

		free(tofree);
	}
	
	if(line)
		free(line);
	
	return list;
}



struct list_t *ParseLine(const char *str, const char delim)
{
	struct list_t *list = CreateList();
	
	int have_quotes = -1;
	int n = (int)strlen(str);
	char *tok = malloc(128*sizeof(char));
	int count = 0;
	
	for(int i = 0; i < n; i++) {
		char c = str[i];

		if(c == QUOTES && have_quotes == -1)
			have_quotes = 1;
		else if(c == QUOTES && have_quotes != -1)
			have_quotes = -1;

		if(c == delim && have_quotes == -1) {
			tok[count] = '\0';
			count = 0;
			AddHeadList(list, tok);
		} else if(i == n-1) {
			tok[count] = c;
			tok[count+1] = '\0';
			AddHeadList(list, tok);
		} else {
			tok[count] = c;
			++count;
		}
	}
	
	free(tok);
	return list;
}
