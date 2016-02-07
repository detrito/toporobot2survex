/* file: main.c
 * author: detrito
 * date: january 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "toporobot_reader.h"
#include "container.h"
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
			cave = (Cave*) malloc(sizeof (Cave)
				+ MAX_SURVEY_POINTERS * sizeof(SSurvey*)
				+ MAX_SERIE_POINTERS * sizeof(Serie*)
				+ MAX_CODE_POINTERS * sizeof(Code*));

			// process input file
			toporobot_process_input_file(argv[1]);
			
			// write cave to survey files
			survex_write_cave(cave);
			
			// debug
			int n = 2;
			ssurvey = cave_get_survey(cave, n);	
			printf("CAVE survey %d, name_person_measuring %s, name_person_drawing %s \n",
				n, ssurvey->name_person_measuring, ssurvey->name_person_drawing);
			
			int k = 5;
			serie = cave_get_serie(cave, n);
			measure = serie_get_measure(serie, k);
			
			printf("SERIE %d measure %d: %f/%f/%f\n",
				serie->serie, k, measure->azimuth, measure->dip, measure->length);
			
			n = 40;
			code = cave_get_code(cave, n);
			printf("CODE %d: unit az %d, unit dip %d\n",
				n, code->unit_azimuth, code->unit_dip);
			
			// free all dynamical allocated memory
			printf("Closing cave... ");
			cave_close(cave);
			printf("done.\n");

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