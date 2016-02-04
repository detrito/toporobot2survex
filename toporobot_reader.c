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
	while (fgets (buf, sizeof(buf), file)) {
		i++;
		printf("%i ",i);
		
		// if not newline (\n + \0) 
		if(strlen(buf) >= 2) {
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
	char *token; // token between separators
	char *fields[TOPOROBOT_FIELD_ITEMS];	// vector of tokens
	int i = 0; // but start putting token at position 1
		
	// get the first token
	token = strtok(buf, s);
	
	// get tokens until the end of the buffer
	while( token != NULL ) {
		i++;
		fields[i] = token;
		token = strtok(NULL, s); // get next token
	}
	
	c1 = atoi(fields[1]); // value of first column
	
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
			//toporobot_parse_code(fields);			
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
	// allocate the memory for the Serie
	printf("alloating Survey memory... ");
	ssurvey = (SSurvey*) malloc(sizeof (SSurvey));
	printf(" done.\n");
	
	ssurvey->day = atoi(fields[3]); // day
	ssurvey->month = atoi(fields[4]); // month
	ssurvey->year = atoi(fields[5]); // year	
	strcpy(ssurvey->name_person_measuring, fields[6]); // spéléomètre
	strcpy(ssurvey->name_person_drawing, fields[7]); // spéléographe
	ssurvey->auto_declination = atoi(fields[8]);// declination (0=manual)	
	ssurvey->correction_azimuth = atof(fields[9]); // azimuth correction
	ssurvey->correction_dip = atof(fields[10]); // azimuth correction	
	
	cave_push_survey(cave, ssurvey);
}

void toporobot_parse_code(char **fields) {
	int id_code;

	id_code = atoi(fields[2]);
	printf("code: %d", id_code);
	
	code = (Code*) malloc(sizeof (Code));

	// units
	code->unit_azimuth = atoi(fields[3]);
	code->unit_dip = atoi(fields[4]);
	
	// accuracy
	code->accuracy_length = atof(fields[5]);
	code->accuracy_azimuth = atof(fields[6]);
	code->accuracy_dip = atof(fields[7]);

	// manual declination
	/* FIXME
	if(serie->auto_declination == 0) {
		serie->correction_azimuth = atof(fields[8]);
	}
	*/
}

void toporobot_parse_measurement(char **fields) {
	int id_serie;
	id_serie = atoi(fields[2]);
	
	// -1 at column 2: begin of a new serie
	if( id_serie == -1) {
		printf("alloating Serie memory... ");
		serie = (Serie*) malloc(sizeof (Serie));
		//serie = (Serie*) malloc(sizeof (Serie) +
		//	MAX_MEASURE_POINTERS * sizeof(Measure*));
		printf("done.\n");
		
		serie->serie = c1;
		//cave_add_serie(cave, serie, c1);
		
		// copy string and delete last caracter (newline)
		strncpy(serie->name, fields[10],
			strlen(fields[10])-1);
		
			
		cave_push_serie(cave,serie);
	}

	else {
		// set serie's survey and code from the first line
		if(id_serie == 0) {
			code = cave_get_code(cave, atoi(fields[3]));
			serie_set_code(serie, code);
		
			ssurvey = cave_get_survey(cave, atoi(fields[4]));
			serie_set_survey(serie, ssurvey);
		}
	
		// allocate the memory for a measure		
		measure = (Measure*) malloc(sizeof (Measure));
		measure->length = atof(fields[5]);
		printf("length: %f\n",measure->length);
		measure->azimuth = atof(fields[6]);
		measure->dip = atof(fields[7]);
		measure->left = atof(fields[8]);
		measure->right = atof(fields[9]);
		measure->up = atof(fields[10]);
		measure->down = atof(fields[11]);	
		
		// push the measure to the end of the vector of pointers to Series 
		serie_push_measure(serie, measure);
		
		printf (" measure parsed\n");
	}
}
