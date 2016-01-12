/* file: survey.h
 * author: detrito
 * date: january 2016
 */
 
#ifndef SURVEY_H
#define SURVEY_H

#include <stdlib.h>
#define MAX_MEASURES 1024

typedef struct {
	// 
	int start;
	int end;

	// centerline
	float azimuth;
	float dip;
	float length;
	
	// walls
	float top;
	float bottom;
	float right;
	float left;
	
	char* comment;	
} Measure;

typedef struct {
	int serie;
	char* name;
	
	Measure *survey_measures[MAX_MEASURES];
} Survey;

void push_measure(Survey *survey, Measure *measure);
void write_survey(Survey *survey);
void close_survey(Survey *survey);


#endif // survey_h