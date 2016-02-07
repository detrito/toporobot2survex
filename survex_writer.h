/* file: survex_writer.h
 * author: detrito
 * date: january 2016
 */
 
#ifndef SURVEX_WRITER_H
#define SURVEX_WRITER_H

#include "container.h"

//extern Cave *cave;
extern Serie *serie;
extern Code *code;
extern SSurvey *ssurvey;
extern Measure *measure;

char buffer[1024*10];
char filename[128];

int write_buffer(char *filename);
void survex_write_main(Cave *cave);
void survex_write_cave(Cave *cave);
void survex_write_serie(Serie *serie);

#endif // survex_writer_h