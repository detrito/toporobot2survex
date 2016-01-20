/* file: main.c
 * author: detrito
 * date: january 2016
 */

#include <stdio.h>
#include <stdlib.h>

#include "toporobot_reader.h"
#include "survey.h"
#include "survex_writer.h"

#define PROGRAM_NAME "toporobot2survex"
#define VERSION "0.0.1"

Cave *cave;

int main (int argc, char *argv[]) {
	if(argc == 2) {
		if (!strncmp(argv[1], "-v", 2)) {
			printf(PROGRAM_NAME " v" VERSION "\n");
			return 0;
		}
		else if(file_exists( argv[1] )){
			printf("processing\n");
			
			cave = (Cave*) malloc(sizeof (Cave));
			cave->top = 0; // initialise top
			
			// read cave from toporobot file
			toporobot_process_input_file(argv[1]);
			
			// write cave to survey files
			survex_write_cave(cave);
			
			cave_close(cave);
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