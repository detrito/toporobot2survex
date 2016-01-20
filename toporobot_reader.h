/* file: toporobot_reader.h
 * author: detrito
 * date: january 2016
 */

#ifndef TOPOROBOT_READER_H
#define TOPOROBOT_READER_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "functions.h"
#include "survey.h"

#define TOPOROBOT_FIELD_ITEMS 11

extern Cave *cave;
Survey *current_survey;
Measure *current_measure;
//int c1; // = 0; // current or last first-column value

void toporobot_parse_measurement(char **fields);
void toporobot_parse_name(char **fields);
void toporobot_parse_line(char *buf);
void toporobot_process_input_file(const char *filename);

#endif // survex_writer_h
