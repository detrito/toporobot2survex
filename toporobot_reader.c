/* file: toporobot_reader.c
 * author: detrito
 * date: january 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"
#include "toporobot_reader.h"

int c1 = 0;
int field_counter = 0;
char fields[FIELD_ITEMS][FIELD_SIZE]={0x0}; // array of fields
const char separator[] = "\t";	// tabulator as separator
const char newline[] = "\n"; // tabulator as separator
	

void toporobot_process_input_file(const char *filename) {
	int i = 0; // line counter
	char buf[256]={0x0}; // line bouffer

	// read input file
	FILE *file = fopen ( filename, "r" );


	// load a line in the buffer
	if(file!=NULL) {
		// read a line
		while ( fgets(buf,sizeof(buf),file) != 0 ) { 
			i++;
			if(verbose) {
				printf("\n%d ",i);
			}
			
			// parse this line, if the line is not empty
			if( strncmp(buf, newline, strlen(newline)) !=0
				&& strncmp(buf, separator, strlen(separator)) !=0) {
				toporobot_parse_line(buf);
			}
			else {
				if(verbose) {
					printf("Blank line");
				}
			}
		}
	}
	else {
		fprintf(stderr,"Error openinf file %s\n", filename);
		abort();	
	}
	
	if (ferror(stdin)) {
		fprintf(stderr,"Error reading stdin\n");
		abort();
	}
}

int toporobot_parse_line(char *buf) {
	field_counter = 0; // but start putting token at position 1

	// get the first token
	char *token = strtok(buf, separator);
	
    while(token!=0)
    {
		field_counter++;
        strcpy(fields[field_counter], clean_string(token));
		token=strtok('\0',separator);
		//printf("f%d:%s|",i,clean_string(fields[i]));
	}

	c1 = atoi(fields[1]); // value of first column
	
	switch (c1) {
		case -6:
			if(verbose) {
				printf("Name - ");
			}
			toporobot_parse_name();
			break;
		case -5:
			if(verbose) {
				printf("Coordinates - ");
			}
			toporobot_parse_coordinates();
			break;
		case -2:
			if(verbose) {
				printf("Survey - ");
			}
			toporobot_parse_survey();
			break;
		case -1:
			if(verbose) {
				printf("Code - ");
			}
			toporobot_parse_code();			
			break;
		default:
			if(c1>=1) {
				if(verbose) {
					printf("Measure - ");
				}
				toporobot_parse_measure();
			}
	}
}

void toporobot_parse_name() {
	// copy string and delete last caracter (newline)
	strcpy(cave->name, fields[3]);
}

void toporobot_parse_coordinates() {
	// coordinates of the first point of the first serie
	cave->entrance.x = atoi(fields[3]);
	cave->entrance.y = atoi(fields[4]);
	cave->entrance.z = atoi(fields[5]);
}

void toporobot_parse_survey() {
	// allocate the memory for the Serie
	if(verbose) {
		printf("Alloating Survey memory... ");
	}
	
	survey = (Survey*) malloc(sizeof (Survey));
	
	survey->day = atoi(fields[3]); // day
	survey->month = atoi(fields[4]); // month
	survey->year = atoi(fields[5]); // year	
	
	// if year has less then 2 digits ( 98 or 8)
	if( strlen(fields[5]) <= 2) {
		if (survey->year <= 99 && survey->year >= 40)
			survey->year += 1900;
		else
			survey->year += 2000;
	}
	
	strcpy(survey->name_person_measuring, fields[6]); // spéléomètre
	strcpy(survey->name_person_drawing, fields[7]); // spéléographe
	survey->auto_declination = atoi(fields[8]);// declination (0=manual)	
	survey->correction_azimuth = atof(fields[9]); // azimuth correction
	survey->correction_dip = atof(fields[10]); // azimuth correction
	
	cave_push_survey(cave, survey);
}

void toporobot_parse_code() {
	int id_code;

	id_code = atoi(fields[2]);
	if(verbose) {
		printf("code: %d\n", id_code);
	}
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
	//printf("code: %i %i %f %f %f", code->unit_azimuth,code->unit_dip, code->accuracy_length, code->accuracy_azimuth, code->accuracy_dip);
	cave_set_code(cave, code, id_code);
}

void toporobot_parse_measure() {
	int id_measure;
	id_measure = atoi(fields[2]);
	
	// -1 at column 2: begin of a new serie
	if( id_measure == -1) {
		if(verbose) {
			printf("Alloating Serie memory... ");
		}

		serie = (Serie*) malloc(sizeof (Serie) +
			MAX_MEASURE_POINTERS * sizeof(Measure*));
		
		serie->id_serie = c1;
		
		// copy string and delete last caracter (newline)
		strcpy(serie->name, fields[10]);
		
		// links the serie's begin and end points
		serie->link_begin_serie = atoi(fields[3]);
		serie->link_begin_measure = atoi(fields[4]);
		if(serie->link_begin_measure > 0)
			serie->link_begin_measure++; // add 1 to convert measure to point		
		serie->link_end_serie = atoi(fields[5]);
		serie->link_end_measure = atoi(fields[6]);
		if(serie->link_end_measure > 0)
			serie->link_end_measure++; // add 1 to convert measure to point
		
		cave_push_serie(cave,serie);
	}

	else {
		// set serie's survey and code from the first measure-line
		if(id_measure == 0) {
			// associate code
			code = cave_get_code(cave, atoi(fields[3]));

			if(code) {
				serie_set_code(serie, code);
			}
			else {
				fprintf(stderr,"Error: code %d not found\n", code);
				abort();
			}

			// associate serie		
			survey = cave_get_survey(cave, atoi(fields[4]));
			if(survey) {
				serie_set_survey(serie, survey);
			}
			else {
				fprintf(stderr,"Error: survey %d not found\n", survey);
				abort();
			}
		}
		
		// allocate the memory for a measure
		measure = (Measure*) malloc(sizeof (Measure));
		measure->length = atof(fields[5]);
		measure->azimuth = atof(fields[6]);
		measure->dip = atof(fields[7]);
		measure->left = atof(fields[8]);
		measure->right = atof(fields[9]);
		measure->up = atof(fields[10]);
		measure->down = atof(fields[11]);
		
		if(field_counter == 12 && strcmp(fields[12], "")!=0) {
			//printf("field:  |%s|", fields[12]);
			strcpy(measure->comment, fields[12]);
		}
		
		// seto pointer to serie
		measure->serie = serie;

		// push the measure to the end of the vector of pointers to Series
		serie_push_measure(serie, measure);
	}
}
