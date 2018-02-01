/* file: functions.h
 * author: detrito
 * date: january 2016
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

int file_exists(const char *filename);
int create_path(char *filename);
int init_output_file(char *filename);
int append_to_str(char *target, size_t targetSize, const char * restrict format, ...);

char *clean_string(char *p);
char *remove_quote(char *p);
char *remove_newline(char *p);

#endif // functions_h
