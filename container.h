/* file: container.h
 * author: detrito
 * date: january 2016
 */
 
#ifndef CONTAINER_H
#define CONTAINER_H

#define LOGARITHMIC_GROWTH // for vector
#define MAX_MEASURE_POINTERS 1024
#define MAX_SURVEY_POINTERS 512
#define MAX_SERIE_POINTERS 512
#define MAX_CODE_POINTERS 512
#define MAX_LINK_POINTERS 512

#include "vector.h"

// early definition of Serie (needed by the Measure struct)
typedef struct Serie Serie;

typedef struct {
	// centerline
	float azimuth;
	float dip;
	float length;
	
	// walls
	float up;
	float down;
	float right;
	float left;
	
	char comment [256];
	
	Serie *serie; // pointer to serie
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
	
	char name_person_measuring[32];
	char name_person_drawing[32];
	
	// corrections
	int auto_declination; // 0=manual; 1=auto
	float correction_azimuth; // more often called declination
	float correction_dip;
} Survey;

struct Serie{
	int id_serie;	// survey id
	char name[256];
	
	// temporary first and last point's links
	int link_begin_serie;
	int link_begin_measure;
	int link_end_serie;
	int link_end_measure;

	Measure **v_measures; // vector of pointers to Measures
	Code *code;	// pointer to code
	Survey *survey; // pointer to survey
};

typedef struct {
	// long term structure: pointers to measures without duplicates
	Measure *measure_1;
	Measure *measure_2;
} Link;

typedef struct {
	int x;
	int y;
	int z;
} Coordinate;

typedef struct {
	char name[256];
	
	Coordinate entrance; // coordinates of the first point of the first serie
	
	Survey **v_surveys; // vector of pointers to surveys
	Serie **v_series; // vector of pointers to series
	Code **v_codes; // vector of pointers to codes
	Link **v_links; // vector of pointers to links
} Cave;


// cave functions
void cave_push_serie(Cave *cave, Serie *serie);
Serie* cave_get_serie(Cave *cave, int i);
int cave_get_series_length(Cave *cave);

void cave_push_survey(Cave *cave, Survey *survey);
Survey* cave_get_survey(Cave *cave, int i);

void cave_set_code(Cave *cave, Code *code, int i);
Code* cave_get_code(Cave *cave, int i);

void cave_create_links(Cave *cave);

void cave_close(Cave *cave);

// serie functions
void serie_push_measure(Serie *serie, Measure *measure);
Measure* serie_get_measure(Serie *serie, int i);
int serie_get_measures_length(Serie *serie);

void serie_set_survey(Serie *serie, Survey *survey);
Survey* serie_get_survey(Serie *serie);

void serie_set_code(Serie *serie, Code *code);
Code* serie_get_code(Serie *serie);


#endif // container_h
