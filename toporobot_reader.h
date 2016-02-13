/* file: toporobot_reader.h
 * author: detrito
 * date: january 2016
 */

#ifndef TOPOROBOT_READER_H
#define TOPOROBOT_READER_H

#include "container.h"

#define FIELD_ITEMS 13 // number of fields in a line
#define FIELD_SIZE 128

extern int verbose;
extern Cave *cave;
extern Serie *serie;
extern Code *code;
extern Survey *survey;
extern Measure *measure;

void toporobot_process_input_file(const char *filename);
int toporobot_parse_line();
void toporobot_parse_name();
void toporobot_parse_coordinates();
void toporobot_parse_survey();
void toporobot_parse_code();
void toporobot_parse_measure();

#endif // survex_writer_h
