/* file: main.c
 * author: detrito
 * date: january 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "toporobot_reader.h"
#include "functions.h"
#include "container.h"
#include "survex_writer.h"
#include "vector.h"

#define PROGRAM_NAME "toporobot2survex"
#define VERSION "0.1"

Cave *cave;
Survey *survey;
Code *code;
Serie *serie;
Measure *measure;

int verbose = 1;

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

			// read the cave in toporobot data format
			toporobot_process_input_file(argv[1]);
	
			// TODO: create graph of links
			//cave_create_links(cave);
			
			// write cave to survey files
			if(verbose) {
				printf("\nWriting to output files...\n");
			}
			
			// write the cave in survex data format
			survex_write_cave(argv[2]);			
	
			// free all dynamical allocated memory
			printf("Closing cave...\n");
			cave_close(cave);

			if(verbose) {
				printf("Exiting.\n");
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