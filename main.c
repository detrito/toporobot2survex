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
Survey *survey;
Code *code;
Serie *serie;
Measure *measure;

int verbose = 1;

#include "vector.h"

int print_help() {
	printf("USE: " PROGRAM_NAME " [input toporobot file] [destination survex folder]\n");
	return 1;
}

int main (int argc, char *argv[]) {
	if(argc == 2) {
		if (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version")) {
			printf(PROGRAM_NAME " v" VERSION "\n");
			return 0;
		}
		else {
			return print_help();
		}
	}
	else if(argc == 3) {
		if(file_exists( argv[1] ) && file_exists(argv[2])) {
			if(verbose) {
				printf("Processing input file...");
			}
			
			// allocate memory for cave			
			cave = (Cave*) malloc(sizeof (Cave)
				+ MAX_SURVEY_POINTERS * sizeof(Survey*)
				+ MAX_SERIE_POINTERS * sizeof(Serie*)
				+ MAX_CODE_POINTERS * sizeof(Code*)
				+ MAX_LINK_POINTERS * sizeof(Link*));

			// process input file
			toporobot_process_input_file(argv[1]);
	
			// TODO: create graph of links
			//cave_create_links(cave);
			
			// write cave to survey files
			if(verbose) {
				printf("\nWriting to output files...\n");
			}
			survex_write_cave(argv[2]);
			
			/*
			// debug
			int n = 2;
			survey = cave_get_survey(cave, n);	
			printf("CAVE survey %d, name_person_measuring %s, name_person_drawing %s \n",
				n, survey->name_person_measuring, survey->name_person_drawing);
	
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
			*/

			if(verbose) {
				printf("Done. Exiting.\n");
			}

			return 0;
		}
		else {
			fprintf(stderr,"Error: input file or output folder not found.\n");
			return 1;
		}
	}
	else {
		return print_help();
	}

	fprintf(stderr,"Error: unknown error.\n");
	return 1;
}