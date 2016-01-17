/* file: survey.h
 * author: detrito
 * date: january 2016
 */
 
#ifndef SURVEY_H
#define SURVEY_H

#define MAX_MEASURES 512
#define MAX_SURVEYS 256

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
	char name[128];
	
	Measure *survey_measures[MAX_MEASURES];
} Survey;

typedef struct {
	char name[128];
	int top;
	Survey *current_survey;
	Survey *surveys[MAX_SURVEYS];
} Cave;

int survey_push_measure(Survey *survey, Measure *measure);
void survey_print(Survey *survey);
void survey_close(Survey *survey);

int cave_push_survey(Cave *cave, Survey *survey);
void cave_print(Cave *cave);
void cave_close(Cave *cave);

#endif // survey_h
