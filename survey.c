/* file: survey.c
 * author: detrito
 * date: january 2016
 */
 
#include <stdio.h>
#include <stdlib.h>

#include "survey.h"

// for array push and pop
int status;

// push a dynamic allocated measurement to the end of
// the pointer array

/*
int survey_push_measure(Survey *survey, Measure *measure) {
	if (survey->top == (MAX_MEASURES-1))
		status = 0;
	else
	{
		status = 1;
		survey->top++;
		(survey->survey_measures[survey->top]) = measure;
	}
	return status;
}

// fprintf the survey data structure
void survey_print(Survey *survey) {
	printf("\n________\n");
	printf("Survey data:\n");
	printf("%i - %s\n",survey->serie, survey->name_survey);
	printf("Survey measures:\n");
	
	for(int i=1; i<=survey->top; i++) {
		printf("[%f,%f,%f] ",
			survey->survey_measures[i]->azimuth,
			survey->survey_measures[i]->dip,
			survey->survey_measures[i]->length
		);
	}
	printf("\n________\n");
}

// free memory
void survey_close(Survey *survey) {
	printf("closing survey\n");
	for(survey->top; survey->top>0; survey->top--) {
		free(survey->survey_measures[survey->top]);
	}
	free(survey);
}
*/

/*
int cave_push_survey(Cave *cave, Survey *survey) {
	if (cave->top == (MAX_SURVEYS-1))
		status = 0;
	else
	{
		status = 1;
		cave->top++;
		(cave->surveys[cave->top]) = survey;
		cave->current_survey = survey;
	}
}
*/

void cave_add_survey(Cave *cave, SSurvey *ssurvey) {
	// push pointer to survey to vector v_ssurveys
	vector_push_back(cave->v_ssurveys, *ssurvey);
}



void cave_print(Cave *cave) {
}

void cave_close(Cave *cave) {
/*
	for(int i=1; i<=MAX_SURVEYS; i++) {
		if(cave->surveys[i] != NULL) {
			// FIXME free measurements
			free(cave->surveys[i]);
		}
	}
*/
	free(cave);
}
