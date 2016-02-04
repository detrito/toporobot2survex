/* file: survex_writer.c
 * author: detrito
 * date: january 2016
 */

 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "survex_writer.h"
#include "functions.h"
#include "survey.h"

int write_buffer(char *filename) {
	init_output_file(filename); // clear file or create path

	FILE *file = fopen(filename, "a"); 	// open file in appening mode
	
	if (file != NULL) {
		fputs(buffer, file); // write buffer to file
		return fclose(file);
	}
	else {
		printf("Error: impossible to open the file \"%s\" in appening mode.\n",
			filename);
		exit(1);
	}
}

void survex_write_cave(Cave *cave) {
	Serie *serie;

	printf("Writing cave... \n");
	
	survex_write_main(cave);

	for(int i=1; i<=cave_get_series_length(cave); i++) {
		serie = cave_get_serie(cave, i);

		if(serie != NULL) {
			printf("Serie serie %d... ", i);
			survex_write_serie(cave_get_serie(cave, i));
			printf("done.\n");
		}
		else {
			printf("Skipping survey %d... ", i);		
		}
	}
	printf("Done writing cave.\n");
}

// write the main file who includes all the series
void survex_write_main(Cave *cave) {

	// assemble filename
	strcpy(filename,"data/main.svx");
		
	// puts
	strcpy(buffer,"\0");
	appendToStr(buffer, sizeof(buffer), "*begin\n");
	appendToStr(buffer, sizeof(buffer), "*title \"%s\"\n", cave->name);
	
	// include all serie files
	for(int i=1; i<=cave_get_series_length(cave); i++) {
		serie = cave_get_serie(cave, i);
		
		if(serie != NULL) {
			appendToStr(buffer, sizeof(buffer), "*include surveys/%d\n",
				serie->serie);
		}
	}
	
	appendToStr(buffer, sizeof(buffer), "*end\n");
	write_buffer(filename);
}


void survex_write_serie(Serie *serie)
{
	float azimuth;
	float dip;
	float length;

	// assemble filename
	strcpy(filename,"\0");
	appendToStr(filename, sizeof(filename),
		"data/surveys/fb_%d.txt", serie->serie);

	// begin serie
	strcpy(buffer,"\0"); 	// empty buffer
	appendToStr(buffer, sizeof(buffer), "*begin %d\n", serie->serie);
	appendToStr(buffer, sizeof(buffer), "*title \"%s\"\n", serie->name);
	
	/* FIXME: populate surveys
	ssurvey = serie_get_survey(serie);
	
	// azimuth correction
	if(ssurvey->correction_azimuth != 0) {
		appendToStr(buffer, sizeof(buffer), "*CALIBRATE COMPASS %f\n",
			ssurvey->correction_azimuth);
	}
	
		// azimuth correction
	if(ssurvey->correction_dip != 0) {
		appendToStr(buffer, sizeof(buffer), "*CALIBRATE CLINO %f\n",
			ssurvey->correction_dip);
	}
	*/
	
	// append measures
	appendToStr(buffer, sizeof(buffer),
		"\n*data normal from to tape compass clino\n");
	
	for(int i=0; i<serie_get_measures_length(serie); i++) {
		measure = serie_get_measure(serie, i);
		appendToStr(buffer, sizeof(buffer), "%f,%f,%f\n",
			measure->azimuth,
			measure->dip,
			measure->length
		);
	}
	
	// append passage
	appendToStr(buffer, sizeof(buffer),
		"\n*data passage station left right up down ignoreall\n");
	
	for(int i=0; i<serie_get_measures_length(serie); i++) {
		measure = serie_get_measure(serie, i);
		appendToStr(buffer, sizeof(buffer), "%f,%f,%f,%f\n",
			measure->left,
			measure->right,
			measure->up,
			measure->down
		);
	}
	
	// end serie
	appendToStr(buffer, sizeof(buffer), "*end %d\n",serie->serie);
	
	// write to file
	write_buffer(filename);
}
