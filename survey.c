/* file: survey.c
 * author: detrito
 * date: january 2016
 */
 
#include <stdio.h>
#include <stdlib.h>

#include "survey.h"

// for array push and pop
int top, status;

// push a dynamic allocated measurement to the end of
// the pointer array
void push_measure(Survey *survey, Measure *measure) {
	if (top == (MAX_MEASURES-1))
		status = 0;
	else
	{
		status = 1;
		top++;
		(survey->survey_measures[top]) = measure;
	}
}


// fprintf the survey data structure
void write_survey(Survey *survey) {
	printf("\n________\n");
	printf("Survey data:\n");
	printf("%i - %s\n",survey->serie, survey->name);
	printf("Survey measures:\n");
	
	for(int i=1; i<=top; i++) {
		printf("[%f,%f,%f] ",
			survey->survey_measures[i]->azimuth,
			survey->survey_measures[i]->dip,
			survey->survey_measures[i]->length
		);
	}
	printf("\n________\n");
}

// free memory
void close_survey(Survey *survey) {
	printf("closing survey\n");
	for(top; top>0; top--) {
		free(survey->survey_measures[top]);
	}
	free(survey);
}