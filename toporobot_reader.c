/* file: toporobot_reader.c
 * author: detrito
 * date: january 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "toporobot_reader.h"

int c1 = 0;

void toporobot_parse_measurement(char **fields) {
	int c2;
	
	c2 = atoi(fields[2]);
	
	// -1 at column 2: begin of a new serie
	if( c2 == -1) {
		cave->top = c1;		
		// pointer to the current survey
		current_survey = cave->surveys[cave->top];
	
		// copy string and delete last caracter (newline)
		strncpy(current_survey->name_survey, fields[10], strlen(fields[10])-1);
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

void toporobot_parse_name(char **fields) {
	// copy string and delete last caracter (newline)
	strncpy(cave->name, fields[3], strlen(fields[3])-1);
}

void toporobot_parse_survey(char **fields) {
	// allocate the memory for the survey
	printf("Alocating survey memory\n");
	current_survey = (Survey*) malloc(sizeof (Survey));
	
	current_survey->top = 0; // initialise top
	current_survey->serie = atoi(fields[2]); // serie number
	current_survey->day = atoi(fields[3]); // day
	current_survey->month = atoi(fields[4]); // month
	current_survey->year = atoi(fields[5]); // year	
	strcpy(current_survey->name_person_measuring, fields[6]); // spéléomètre
	strcpy(current_survey->name_person_drawing, fields[7]); // spéléographe
	current_survey->declination = atoi(fields[8]); // declination (0=manual)	
	current_survey->declination_value = atof(fields[9]); // declination (0=manual)
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

void toporobot_process_input_file(const char *filename) {
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
