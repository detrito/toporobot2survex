/* file: survey.h
 * author: detrito
 * date: january 2016
 */
 
#ifndef SURVEY_H
#define SURVEY_H

#define MAX_MEASURES 1024

typedef struct {
	int start;
	int end;

	// centerline
	float azimuth;
	float dip;
	float length;
	
	// walls
	float up;
	float down;
	float right;
	float left;
	
	char* comment;	
} Measure;

typedef struct {
	int serie;
	int top;
	char* name;
	
	Measure *survey_measures[MAX_MEASURES];
} Survey;

void survey_push_measure(Survey *survey, Measure *measure);
void survey_print(Survey *survey);
void survey_close(Survey *survey);

#endif // survey_h