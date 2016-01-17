/* file: survex_writer.h
 * author: detrito
 * date: january 2016
 */
 
#ifndef SURVEX_WRITER_H
#define SURVEX_WRITER_H

#include <stdlib.h>
#include <stdarg.h>
#include "survey.h"

char buffer[1024*10];
char filename[128];

typedef struct {
	float data[5];
} SurvexData;

typedef struct {
	float data[5];
} SurvexPassage;

typedef struct {
	SurvexData *container;
} SurveyDataContainer;

typedef struct {
	SurvexPassage *container;
} SurveyPassageContainer;

int appendToStr(char *target, size_t targetSize, const char * restrict format, ...);
void survex_write_survey(Survey *survey);
void survex_write_main(Cave *cave);
void survex_write_cave(Cave *cave);

#endif // survex_writer_h