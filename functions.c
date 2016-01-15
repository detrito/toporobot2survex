/* file: functions.c
 * author: detrito
 * date: january 2016
 */


#include <unistd.h>

#include "functions.h"

int file_exists(const char *filename) {
	if (access(filename, F_OK) != -1)
		return 1;
	else
		return 0;
}
