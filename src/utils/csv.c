#include "csv.h"

char *create_file_name(size_t n, const char *out_dir, const char *ext)
{
    size_t nchars = n + strlen(out_dir) + strlen(ext) + 1;
    char path_csv[nchars];

    do {
        memset(path_csv, 0, sizeof path_csv);
        char *file_name = rand_str_seq(n);
        strcat(path_csv, out_dir);
        strcat(path_csv, file_name);
        strcat(path_csv, ext);
        free(file_name);
    } while(access(path_csv, F_OK) != -1);

    char *ret = malloc(nchars);
    charptr_to_static(path_csv, ret, (int)nchars);
    
    return ret;
}



char determine_delim(FILE *fp)
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



struct list_t *gather_lines(FILE *fp)
{
	rewind(fp);
	struct list_t *list = create_list();
	
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
		
		add_head_list(list, buffer);
		free(buffer);
	}
	
	if(line)
		free(line);
	
	rewind(fp);
	return list;
}



unsigned int num_lines_file(FILE *fp)
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



struct list_t *parse_header(FILE *fp, const char delim)
{
	struct list_t *list = create_list();
	
	char *line = NULL, *tok, *str, *tofree;
	size_t len = 0;

	if(getline(&line, &len, fp) != 0) {
		tofree = str = strdup(line);
		strlower(str, 1);
		
		while((tok = strsep(&str, (char*)&delim)))
			add_head_list(list, tok);

		free(tofree);
	}
	
	if(line)
		free(line);
	
	return list;
}



struct list_t *parse_line(const char *str, const char delim)
{
	struct list_t *list = create_list();
	
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
			add_head_list(list, tok);
		} else if(i == n-1) {
			tok[count] = c;
			tok[count+1] = '\0';
			add_head_list(list, tok);
		} else {
			tok[count] = c;
			++count;
		}
	}
	
	free(tok);
	return list;
}
