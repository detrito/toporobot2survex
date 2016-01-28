/* file: toporobot_reader.h
 * author: detrito
 * date: january 2016
 */

#ifndef TOPOROBOT_READER_H
#define TOPOROBOT_READER_H

#include "functions.h"
#include "survey.h"

#define TOPOROBOT_FIELD_ITEMS 11 // number of fields in a line

extern Cave *cave;
extern Serie *serie;
extern Code *code;
extern SSurvey *ssurvey;
extern Measure *measure;

//int c1; // = 0; // current or last first-column value

void toporobot_process_input_file(const char *filename);
void toporobot_parse_line(char *buf);
void toporobot_parse_name(char **fields);
void toporobot_parse_survey(char **fields);
void toporobot_parse_code(char **fields);
void toporobot_parse_measurement(char **fields);

#endif // survex_writer_h
