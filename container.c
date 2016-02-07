/* file: container.c
 * author: detrito
 * date: january 2016
 */
 
#include <stdio.h>
#include <stdlib.h>

#include "container.h"

/*
 * cave functions
 */ 

// serie

void cave_push_serie(Cave *cave, Serie *serie) {	
	// push pointer to serie to vector v_series
	vector_push_back(cave->v_series, serie);
}

Serie* cave_get_serie(Cave *cave, int i) {
	return cave->v_series[i-1];
}


int cave_get_series_length(Cave *cave) {
	return vector_size(cave->v_series);
}

// survey

void cave_push_survey(Cave *cave, SSurvey *ssurvey) {
	// push pointer to survey to vector v_ssurveys
	vector_push_back(cave->v_ssurveys, ssurvey);
}

SSurvey* cave_get_survey(Cave *cave, int i) {
	return cave->v_ssurveys[i-1];
}

// code

void cave_set_code(Cave *cave, Code *code, int i) {
	size_t __cap = vector_capacity(cave->v_codes);
	if(__cap <= i) {
		vector_grow(cave->v_codes, i);
		// FIXME: size does not actually make sense enymore: don't use it!
		vector_set_size(cave->v_codes, i);
	}
	cave->v_codes[i-1] = code;
}

// links

void cave_create_links(Cave *cave) {
	Serie *serie;
	
	// cycle series
	for(int i=1; i<=cave_get_series_length(cave); i++) {
		serie = cave_get_serie(cave, i);
		if(serie) {
			printf("serie %d: from %d/%d to %d/%d\n",
				i,
				serie->link_begin_serie,
				serie->link_begin_measure,
				serie->link_end_serie,
				serie->link_end_measure);
		}
	}
}

// destroy

Code* cave_get_code(Cave *cave, int i) {
	return cave->v_codes[i-1];
}

void cave_close(Cave *cave) {
	Serie *serie;

	// cycle series
	for(int i=1; i<=cave_get_series_length(cave); i++) {
		serie = cave_get_serie(cave, i);
		vector_free(serie->v_measures);
	}
	vector_free(cave->v_series);
	vector_free(cave->v_codes);
	vector_free(cave->v_ssurveys);
	
	free(cave);
}

/*
 * serie functions
 */

int serie_get_measures_length(Serie *serie);


void serie_push_measure(Serie *serie, Measure *measure) {
	// push pointer to measure to vector v_measures
	vector_push_back(serie->v_measures, measure);
}

Measure* serie_get_measure(Serie *serie, int i) {
	return serie->v_measures[i];
}

int serie_get_measures_length(Serie *serie) {
	return vector_size(serie->v_measures);
}

void serie_set_survey(Serie *serie, SSurvey *ssurvey) {
	serie->ssurvey = ssurvey;
}

SSurvey* serie_get_survey(Serie *serie) {
	return serie->ssurvey;
}

void serie_set_code(Serie *serie, Code *code) {
	serie->code = code;
}

Code* serie_get_code(Serie *serie) {
	return serie->code;
}
