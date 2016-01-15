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

void survex_write_survey(Survey *survey)
{
	char buffer[1024*10];
	char filename[128];
	
	// assemble filename
	strcpy(filename,"\0");
	appendToStr(filename, sizeof(filename), "data/fb_%d.txt", survey->serie);
	//strcpy(filename,"data/asd_");
	//strcat(filename,survey->serie);
	//strcat(filename,".txt");
	
	
	// ovverride existing files
	if(file_exists(filename)) {
		remove(filename);
	}
	
	// puts
	strcpy(buffer,"\0");
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
	
	// open file in appening mode
	FILE *file = fopen(filename, "a");
	if (file != NULL)
	{
		fputs(buffer, file);
		fclose(file);
	}
}
