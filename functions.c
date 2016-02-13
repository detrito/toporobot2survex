/* file: functions.c
 * author: detrito
 * date: january 2016
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <libgen.h>

#include "functions.h"

// check if file exists and is writable
int file_exists(const char *filename) {
	if (access(filename, F_OK) != -1)
		return 1;
	else
		return 0;
}

// check path and create directories if needed
int create_path(char *filename) {
	char *path;
	char *cfilename;
	struct stat st;
	
	// Both dirname() and basename() may modify the contents of path, so it may
	// be desirable to pass a copy when calling one of these functions.
	cfilename = strdup(filename);
	path = dirname(cfilename);
	
	if (stat(path, &st) == -1) {
    	return mkdir(path, 0700);
	}
	return 0;
}

int init_output_file(char *filename) {
	// ovverride existing files
	if( file_exists(filename)) {
		return remove(filename);
	}
	// create path directories if needed
	else {
		return create_path(filename);
	}
}	

int append_to_str (char *target, size_t targetSize,
	const char * restrict format,...) {
	
	va_list args;
	char temp[targetSize];
	int result;

	va_start(args, format);
	result = vsnprintf(temp, targetSize, format, args);
	
	if (result != EOF) {
		if (strlen(temp) + strlen(target) > targetSize) {
			fprintf(stderr, "appendToStr: target buffer not large enough to "
				"hold additional string");
			return 0;
		}
		strcat(target, temp);
	}
	va_end(args);
	return result;
}

char *clean_string(char *p) {
	return remove_quote(remove_newline(p));
}

char *remove_quote(char *p) {
	if (p[0] == '"') {
		if (p[strlen(p)-1] == '"')
			p[strlen(p)-1] = '\0';
		p++;
	}
	return p;
}

char *remove_newline(char *p) {
	p[strcspn(p, "\n")] = 0;
	return p;
}