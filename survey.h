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
	int top;
 
	int serie;	// survey id
	char name_survey[128];
	char name_person_measuring[128];
	char name_person_drawing[128];

	int day;
	int month;
	int year;

	int declination; // 0=manual; 1=auto
	float declination_value; // correction value for the declination
	
	// array of pointers to Measures
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
