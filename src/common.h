#ifndef QCOMMON_H
#define QCOMMON_H

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

typedef enum {
	LOG_DEBUG,
	LOG_ERROR,
	LOG_WARNING
} logLevel_t;

void		com_printf(logLevel_t, const char* format, ...);

#endif
