/* file: main.c
 * author: detrito
 * date: january 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "survey.h"

#define PROGRAM_NAME "toporobot2survex"
#define VERSION "0.0.1"
#define FIELD_ITEMS 11

/* STATIC */

static const char filename[] = "data/input_unix.tab"; // input file path
int c1 = 0; // current or last first-column value

// survey pointer for dynamic memory allocation
Survey *current_survey;

void ParseMeasurement(char **fields) {

	int c2;
	Measure *current_measure;
	
	c2 = atoi(fields[2]);

	// -1 at column 2: begin of a new serie
	if( c2 == -1)
	{
		// allocate the memory for the survey
		current_survey = (Survey*) malloc(sizeof (Survey));
		
		current_survey->serie = c1; // serie number
		current_survey->name = malloc(sizeof(char)*256); // survey name
		// copy string and delete last two caracters (newline)
		strncpy(current_survey->name, fields[10], strlen(fields[10])-1); 
	}
	else
	{
		// allocate the memory for a measure
		current_measure = (Measure*) malloc(sizeof (Measure));
		current_measure->length = atoi(fields[5]);
		current_measure->azimuth = atoi(fields[6]);
		current_measure->dip = atoi(fields[7]);

		// push the measurement to the end of the survey's pointer array
		push_measure(current_survey,current_measure);
	}
	/*
	printf("**%p**",current_measure);			
	printf(" **%p**",current_survey);		
	printf(" **%s**", &(current_survey->name));
	*/
	/*for(int i=1; i<=FIELD_ITEMS; i++) {
		printf("[%d:%s] ", i, fields[i]);
	}
	*/
	//Measure current_measure;
}


void ParseLine(char *buf) {
	//printf("line: %s", buf);
	const char s[] = "\t";	// tabulator as separator
	char *token;	// token between separators
	char *fields[FIELD_ITEMS];	// vector of tokens
	int i = 0; // but start putting token at position 1
	int k;
	
	// get the first token
	token = strtok(buf, s);
	
	// get tokens until the end of the buffer
   	while( token != NULL )
   	{
   		i++;
   		fields[i] = token;
   		//printf( "<%s> ", token );
   		token = strtok(NULL, s);
   	}
	
	c1 = atoi(fields[1]);
	
	switch (c1) {
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
			if(c1>=1) {
				//printf("Visée: ");
				ParseMeasurement(fields);
			}
			else {
				exit(0);
			}
	}
}

void ProcessInputFile(const char *filename) {

	//current_survey = &c_survey;
	
	int i = 0; // line counter
	char buf[256]; // line bouffer
	
	// read input file
	FILE *file = fopen ( filename, "r" );

	// load a line in the buffer
	while (fgets (buf, sizeof(buf), file) && i<=102) {
		i++;
		
		// print line number
		printf("%i ",i);
		
		//ParseLine(buf);
		
		// if line contain more than 2 chars
		if(strlen(buf) >= 2)
		{
			ParseLine(buf);
		}
		// else if newline
		else if(!strncmp(buf, "\n", 2))
		{
			// if there is a survey to write
			if(c1 >= 1)
			{
				write_survey(current_survey);
				close_survey(current_survey);
			}
			else
				printf("New line\n");
		}
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
			printf("EOF\n");
			return 0;
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