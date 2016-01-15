/* file: survex_writer.h
 * author: detrito
 * date: january 2016
 */
 
#ifndef SURVEX_WRITER_H
#define SURVEX_WRITER_H

#include <stdlib.h>
#include <stdarg.h>
#include "survey.h"

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

#endif // survex_writer_h