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
			
			//cave = (Cave*) malloc(sizeof (Cave) * sizeof(Survey));
			cave = (Cave*) malloc(sizeof (Cave)
				+ MAX_SURVEY_POINTERS * sizeof(SSurvey*)
				+ MAX_SERIE_POINTERS * sizeof(Serie*));
			// read cave from toporobot file
			
			toporobot_process_input_file(argv[1]);
			
			// write cave to survey files
			//survex_write_cave(cave);
			
			cave_close(cave);
			/*
			printf("\nbegin\n");
			ssurvey = (SSurvey*) malloc(sizeof (SSurvey) + sizeof(Survey)*sizeof(Measure)*MAX_MEASURES);				
			for(int i=0; i<=10; i++) {
				tmp_survey = (Survey*) malloc(sizeof (Survey));
				tmp_survey->serie = i;
				vector_push_back(ssurvey->v_series, *tmp_survey);
			}
			*/
			int n = 1;
			SSurvey tmp_survey;
			tmp_survey = cave->v_ssurveys[n];
			printf("cave survey %d, name_person_measuring %s, name_person_drawing \n",
				n, tmp_survey.name_person_drawing);
			
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