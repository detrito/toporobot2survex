/* file: main.c
 * author: detrito
 * date: january 2016
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define FIELD_SIZE 256

#define PROGRAM_NAME "toporobot2survex"
#define VERSION "0.0.1"
#define FIELD_ITEMS 11

/* STATIC */

static const char filename[] = "data/input_unix.tab"; // input file path


void ParseMeasurement(char **fields) {
	for(int i=0; i<=FIELD_ITEMS; i++) {
		printf("[%s] ", fields[i]);
	}
}

void ParseLine(char *buf) {
	//printf("line: %s", buf);
	const char s[] = "\t";	// tabulator as separator
	char *token;	// token between separators
	char *fields[FIELD_ITEMS];	// vector of tokens
	int i = 0;
	int k;
	
	// get the first token
	token = strtok(buf, s);
	
	fields[i] = token;
	
	// get tokens until the end of the buffer
   	while( token != NULL )
   	{
   		i++;
   		fields[i] = token;
   		//printf( "<%s> ", token );
   		token = strtok(NULL, s);
   	}
	
	k = atoi(fields[1]);
	
	switch (k) {
		case -6:
			printf("Nome entrée\n");
			break;
		case -5:
			printf("Coordonnée entrée\n");
			break;
		case -2:
			printf("Expé\n");
			break;
		case -1:
			printf("Code\n");
			break;
		default:
			if(k>=1) {
				printf("Visée: ");
				ParseMeasurement(fields);
			}
			else {
				exit(0);
			}
	}
}

void ProcessInputFile(const char *filename) {
	int i = 0; // line counter
	char buf[256]; // line bouffer
	
	// read input file
	FILE *file = fopen ( filename, "r" );

	// load a line in the buffer
	while (fgets (buf, sizeof(buf), file) && i<=100) {
		i++;
		// print line number
		printf("%i ",i);
		
		// if line contain more than 2 chars
		if(strlen(buf) >= 2)
			ParseLine(buf);
		else
			printf("New line\n");

	}
	
	if (ferror(stdin)) {
		fprintf(stderr,"Error reading stdin\n");
		abort();
	}
}

int file_exists(const char *fname)
{
    FILE *file;
    if (file = fopen(fname, "r"))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

int main (int argc, char *argv[])
{
	if(argc == 2) {
		if (!strncmp(argv[1], "-v", 2)) {
			printf(PROGRAM_NAME " v" VERSION "\n");
			return 0;
		}
		else if(file_exists( argv[1] )){
			printf("processing\n");
			ProcessInputFile(argv[1]);
		}
		else
		{
			printf("Error: file not found.\n");
			return 1;
		}
	}
	else
	{
		printf("USE: " PROGRAM_NAME " [file name]\n");
		return 1;
	}

	printf("Unknown error\n");
	return 1;
}