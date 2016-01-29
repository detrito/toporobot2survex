/* file: survey.h
 * author: detrito
 * date: january 2016
 */
 
#ifndef SURVEY_H
#define SURVEY_H

#define LOGARITHMIC_GROWTH // for vector
#define MAX_MEASURE_POINTERS 1024
#define MAX_SURVEY_POINTERS 512
#define MAX_SERIE_POINTERS 512

#include "vector.h"

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
	// units
	int unit_azimuth;
	int unit_dip;
	
	// accuracy
	float accuracy_length;
	float accuracy_azimuth;
	float accuracy_dip;
} Code;

typedef struct {
	// date
	int day;
	int month;
	int year;
	
	char name_person_measuring[128];
	char name_person_drawing[128];
	
	// corrections
	int auto_declination; // 0=manual; 1=auto
	float correction_azimuth; // more often called declination
	float correction_dip;
} SSurvey;

typedef struct {
	int top;

	int serie;	// survey id
	char name[128];
	
	Measure *v_measures; // vector of pointers to Measures
	Code code;	// pointer to code
	SSurvey ssurvey; // pointer to survey
} Serie;

typedef struct {
	char name[128];
	//Survey *surveys[MAX_SURVEYS];
	SSurvey *v_ssurveys; // vector of pointers to surveys
	Serie *v_series;
} Cave;

void cave_push_survey(Cave *cave, SSurvey *ssurvey);
void cave_push_serie(Cave *cave, Serie *serie);
void serie_push_measure(Serie *serie, Measure *measure);

void cave_print(Cave *cave);
void cave_close(Cave *cave);

#endif // survey_h
