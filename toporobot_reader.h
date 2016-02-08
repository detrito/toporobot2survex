/* file: toporobot_reader.h
 * author: detrito
 * date: january 2016
 */

#ifndef TOPOROBOT_READER_H
#define TOPOROBOT_READER_H

#include "functions.h"
#include "container.h"

#define TOPOROBOT_FIELD_ITEMS 11 // number of fields in a line

extern int verbose;
extern Cave *cave;
extern Serie *serie;
extern Code *code;
extern Survey *survey;
extern Measure *measure;

void toporobot_process_input_file(const char *filename);
void toporobot_parse_line(char *buf);
void toporobot_parse_name(char **fields);
void toporobot_parse_coordinates(char **fields);
void toporobot_parse_survey(char **fields);
void toporobot_parse_code(char **fields);
void toporobot_parse_measure(char **fields);

#endif // survex_writer_h
