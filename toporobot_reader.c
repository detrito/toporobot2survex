/* file: toporobot_reader.c
 * author: detrito
 * date: january 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "toporobot_reader.h"

int c1 = 0;

void toporobot_process_input_file(const char *filename) {
	int i = 0; // line counter
	char buf[256]; // line bouffer
	
	// read input file
	FILE *file = fopen ( filename, "r" );

	// load a line in the buffer
	//while (fgets (buf, sizeof(buf), file) && i<=102) {
	while (fgets (buf, sizeof(buf), file)) {
		i++;
		printf("%i ",i);
		
		// if not newline (\n contains 2 chars) 
		if(strlen(buf) >= 2)
		{
			toporobot_parse_line(buf);
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

void toporobot_parse_line(char *buf) {
	const char s[] = "\t";	// tabulator as separator
	char *token;	// token between separators
	char *fields[TOPOROBOT_FIELD_ITEMS];	// vector of tokens
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
				toporobot_parse_name(fields);
			break;
		case -5:
			printf("Coordonnée entrée\n");
			break;
		case -2:
			printf("Expé\n");
				toporobot_parse_survey(fields);
			break;
		case -1:
			printf("Code\n");
				toporobot_parse_code(fields);			
			break;
		default:
			if(c1>=1) {
				toporobot_parse_measurement(fields);
			}
			else {
				exit(0);
			}
	}
}

void toporobot_parse_name(char **fields) {
	// copy string and delete last caracter (newline)
	strncpy(cave->name, fields[3], strlen(fields[3])-1);
}


void toporobot_parse_survey(char **fields) {
	// allocate the memory for the survey
	printf("Alocating survey memory... ");
	current_survey = (Survey*) malloc(sizeof (Survey));
	
	current_survey->top = 0; // initialise top
	current_survey->serie = atoi(fields[2]); // serie number
	current_survey->day = atoi(fields[3]); // day
	current_survey->month = atoi(fields[4]); // month
	current_survey->year = atoi(fields[5]); // year	
	strcpy(current_survey->name_person_measuring, fields[6]); // spéléomètre
	strcpy(current_survey->name_person_drawing, fields[7]); // spéléographe
	current_survey->auto_declination = atoi(fields[8]);// declination (0=manual)	
	current_survey->correction_azimuth = atof(fields[9]); // azimuth correction
	current_survey->correction_dip = atof(fields[10]); // azimuth correction	
	
	cave_add_survey(cave, current_survey, current_survey->serie);
	//free(current_survey);
	
	printf("done for serie %d", current_survey->serie);
	printf(", name, %s\n", cave->surveys[current_survey->serie]->name_person_measuring);
}

void toporobot_parse_code(char **fields) {
	int serie;

	serie = atoi(fields[2]);
	printf("serie: %d", serie);
	printf(", pointer: %p\n", cave->surveys[serie]);
	
	// retrive pointer to surve
	if(cave->surveys[atoi(fields[2])] != NULL) {
			current_survey = cave->surveys[serie];
	}
	else {
		printf("alloating survey memory\n");
		current_survey = (Survey*) malloc(sizeof (Survey));
		current_survey->top = 0; // initialise top
		current_survey->serie = atoi(fields[2]); // serie	
		cave_add_survey(cave, current_survey, serie);
	}

	// units
	current_survey->unit_azimuth = atoi(fields[3]);
	current_survey->unit_dip = atoi(fields[4]);
	
	// accuracy
	current_survey->accuracy_length = atof(fields[5]);
	current_survey->accuracy_azimuth = atof(fields[6]);
	current_survey->accuracy_dip = atof(fields[7]);

	// manual declination
	if(current_survey->auto_declination == 0) {
		current_survey->correction_azimuth = atof(fields[8]);
	}
}

void toporobot_parse_measurement(char **fields) {
	int c2;
	
	c2 = atoi(fields[2]);
	
	// -1 at column 2: begin of a new serie
	if( c2 == -1) {
	/*
		//cave->top = c1;
		// pointer to the current surve
		printf("serie: %d",c1);
		printf(", pointer: %p", current_survey);		
		printf(", pointer: %p", cave->surveys[c1]);
		printf(", pointed serie: %d\n", cave->surveys[c1]->serie);	
		cave->current_survey = cave->surveys[c1];
		printf("ok");
	*/

		if(cave->surveys[c1] != NULL) {
			cave->current_survey = cave->surveys[c1];
		}
		else
		{
			// allocate memory for surveys without -1 or -2 lines
			printf("alloating survey memory\n");
			current_survey = (Survey*) malloc(sizeof (Survey));
			current_survey->top = 0; // initialise top
			
			// try to retrive some data
			current_survey->serie = c1;			
			
			cave_add_survey(cave, current_survey, c1);
			cave->current_survey = current_survey;
		}
		// copy string and delete last caracter (newline)
		strncpy(cave->current_survey->name_survey, fields[10],
			strlen(fields[10])-1);
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
