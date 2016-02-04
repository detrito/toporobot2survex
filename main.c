/* file: main.c
 * author: detrito
 * date: january 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "toporobot_reader.h"
#include "survey.h"
#include "survex_writer.h"

#define PROGRAM_NAME "toporobot2survex"
#define VERSION "0.0.1"

// temporary pointers to objects
Cave *cave;
SSurvey *ssurvey;
Code *code;
Serie *serie;
Measure *measure;

#include "vector.h"

int main (int argc, char *argv[]) {
	if(argc == 2) {
		if (!strncmp(argv[1], "-v", 2)) {
			printf(PROGRAM_NAME " v" VERSION "\n");
			return 0;
		}
		else if(file_exists( argv[1] )){
			printf("processing\n");

			// allocate memory for cave			
			cave = (Cave*) malloc(sizeof (Cave));
			
			// process input file
			toporobot_process_input_file(argv[1]);
			
			// write cave to survey files
			survex_write_cave(cave);
			
			cave_close(cave);

			// debug
			int n = 1;
			ssurvey = cave_get_survey(cave, n);	
			printf("cave survey %d, name_person_measuring %s, name_person_drawing \n",
				n, ssurvey->name_person_drawing);
			
			int k = 7;
			serie = cave_get_serie(cave, n);
			measure = serie_get_measure(serie, k);
			
			printf("serie %d, measure %d: %f/%f/%f\n",
				serie->serie, k, measure->azimuth, measure->dip, measure->length);

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