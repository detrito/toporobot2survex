/* file: main.c
 * author: detrito
 * date: january 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"
#include "survey.h"
#include "survex_writer.h"

#define PROGRAM_NAME "toporobot2survex"
#define VERSION "0.0.1"
#define FIELD_ITEMS 11

int c1 = 0; // current or last first-column value

Cave *cave;
Survey *current_survey;
Measure *current_measure;

void ParseMeasurement(char **fields) {
	int c2;
		
	c2 = atoi(fields[2]);
	
	// -1 at column 2: begin of a new serie
	if( c2 == -1) {
		// allocate the memory for the survey
		printf("Alocating survey memory\n");
		current_survey = (Survey*) malloc(sizeof (Survey));
		current_survey->top = 0; // initialise top
		current_survey->serie = c1; // serie number
		// copy string and delete last caracter (newline)
		strncpy(current_survey->name, fields[10], strlen(fields[10])-1);
		cave_push_survey(cave,current_survey);
	}

	else {
		// allocate the memory for a measure		
		current_measure = (Measure*) malloc(sizeof (Measure));
		current_measure->length = atof(fields[5]);
		current_measure->azimuth = atof(fields[6]);
		current_measure->dip = atof(fields[7]);
		current_measure->left = atof(fields[8]);
		current_measure->right = atof(fields[9]);
		current_measure->up = atof(fields[10]);
		current_measure->down = atof(fields[11]);	
		
		// push the measurement to the end of the survey's pointer array
		survey_push_measure(cave->current_survey,current_measure);
		
		printf (" measure parsed\n");
	}
}

void ParseName(char **fields) {
	// copy string and delete last caracter (newline)
	strncpy(cave->name, fields[3], strlen(fields[3])-1);
}

void ParseLine(char *buf) {
	const char s[] = "\t";	// tabulator as separator
	char *token;	// token between separators
	char *fields[FIELD_ITEMS];	// vector of tokens
	int i = 0; // but start putting token at position 1
	int k;
	
	// get the first token
	token = strtok(buf, s);
	
	// get tokens until the end of the buffer
	while( token != NULL ) {
		i++;
		fields[i] = token;
		//printf( "<%s> ", token );
		token = strtok(NULL, s);
	}
	
	c1 = atoi(fields[1]);
	
	switch (c1) {
		case -6:
			printf("Nome entrée\n");
				ParseName(fields);
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
	while (fgets (buf, sizeof(buf), file) && i<=102) {
		i++;
		printf("%i ",i);
		
		// if not newline (\n contains 2 chars) 
		if(strlen(buf) >= 2)
		{
			ParseLine(buf);
		}
	}
	
	if (ferror(stdin)) {
		fprintf(stderr,"Error reading stdin\n");
		abort();
	}
	else {
		printf("EOF\n");
	}
}

int main (int argc, char *argv[]) {
	if(argc == 2) {
		if (!strncmp(argv[1], "-v", 2)) {
			printf(PROGRAM_NAME " v" VERSION "\n");
			return 0;
		}
		else if(file_exists( argv[1] )){
			printf("processing\n");
			
			// initialise cave
			cave = (Cave*) malloc(sizeof (Cave));
			cave->top = 0;
			
			ProcessInputFile(argv[1]);
			
			survex_write_cave(cave);
			cave_close(cave);
			
			return 0;
		}
		else {
			printf("Error: file not found.\n");
			return 1;
		}
	}
	else {
		printf("USE: " PROGRAM_NAME " [file name]\n");
		return 1;
	}

	printf("Unknown error\n");
	return 1;
}