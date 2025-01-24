/* file: survex_writer.h
 * author: detrito
 * date: january 2016
 */

#ifndef SURVEX_WRITER_H
#define SURVEX_WRITER_H

#include "container.h"

extern int verbose;
extern Cave *cave;
extern Serie *serie;
extern Code *code;
extern Survey *survey;
extern Measure *measure;

int write_buffer(char *filename);
void survex_write_cave(const char *folder);
void survex_write_main();
void survex_write_serie(Serie *serie);

#endif // survex_writer_h
