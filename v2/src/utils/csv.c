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



struct list_t *parse_header(FILE *fp)
{
	struct list_t *list = create_list();
	
	char *line = NULL, *tok, *str, *tofree;
	size_t len = 0;
	const char delim = determine_delim(fp);
	
	if(getline(&line, &len, fp) != 0) {
		tofree = str = strdup(line);
		strlower(str, true);
		
		while((tok = strsep(&str, &delim)))
			add_head_list(list, tok);

		free(tofree);
	}

	free(line);
	return list;
}
