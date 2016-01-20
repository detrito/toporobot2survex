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

int appendToStr (char *target, size_t targetSize,
	const char * restrict format,...) {
	
	va_list args;
	char temp[targetSize];
	int result;

	va_start(args, format);
	result = vsnprintf(temp, targetSize, format, args);
	
	if (result != EOF) {
		if (strlen(temp) + strlen(target) > targetSize) {
			fprintf(stderr, "appendToStr: target buffer not large enough to "
				"hold additional string");
			return 0;
		}
		strcat(target, temp);
	}
	va_end(args);
	return result;
}

int write_buffer(char *filename) {
	// clear file or create path
	init_output_file(filename);
	
	// open file in appening mode
	FILE *file = fopen(filename, "a");
	
	if (file != NULL) {
		fputs(buffer, file);
		return fclose(file);
	}
	else {
		printf("Error: impossible to open the file \"%s\" in appening mode.\n",
			filename);
		exit(1);
	}
}

void survex_write_survey(Survey *survey)
{	
	// assemble filename
	strcpy(filename,"\0");
	appendToStr(filename, sizeof(filename), "data/surveys/fb_%d.txt", survey->serie);
	//strcpy(filename,"data/asd_");
	//strcat(filename,survey->serie);
	//strcat(filename,".txt");
	
	strcpy(buffer,"\0"); 	// empty buffer
	appendToStr(buffer, sizeof(buffer), "*begin %d\n", survey->serie);
	appendToStr(buffer, sizeof(buffer), "*title \"%s\"\n", survey->name);
	
	// append measures
	appendToStr(buffer, sizeof(buffer),
		"\n*data normal from to tape compass clino\n");
	
	for(int i=1; i<=survey->top; i++) {
		appendToStr(buffer, sizeof(buffer), "%f,%f,%f\n",
			survey->survey_measures[i]->azimuth,
			survey->survey_measures[i]->dip,
			survey->survey_measures[i]->length
		);
	}
	
	// append passage
	appendToStr(buffer, sizeof(buffer),
		"\n*data passage station left right up down ignoreall\n");
	
	for(int i=1; i<=survey->top; i++) {
		appendToStr(buffer, sizeof(buffer), "%f,%f,%f,%f\n",
			survey->survey_measures[i]->left,
			survey->survey_measures[i]->right,
			survey->survey_measures[i]->up,
			survey->survey_measures[i]->down
		);
	}
	
	appendToStr(buffer, sizeof(buffer), "*end %d\n",survey->serie);
	
	write_buffer(filename);
}

void survex_write_main(Cave *cave) {

	// assemble filename
	strcpy(filename,"data/main.svx");
		
	// puts
	strcpy(buffer,"\0");
	appendToStr(buffer, sizeof(buffer), "*begin\n");
	appendToStr(buffer, sizeof(buffer), "*title \"%s\"\n", cave->name);
	
	for(int i=1; i<=cave->top; i++) {
		appendToStr(buffer, sizeof(buffer), "*include surveys/%d\n",
			cave->surveys[i]->serie);
	}
	
	appendToStr(buffer, sizeof(buffer), "*end\n");
	
	write_buffer(filename);
}


void survex_write_cave(Cave *cave) {
	survex_write_main(cave);

	for(int i=1; i<=cave->top; i++) {
		printf("Writing survey %d... ", i);
		survex_write_survey(cave->surveys[i]);
		printf("done\n");
	}
}
