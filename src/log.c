#include "common.h"

#include <stdarg.h>

const char* log_level[3] = {
	"[DEBUG] ",
	"[ERROR] ",
	"[WARNING] "
};

void com_printf(logLevel_t level, const char* format, ...) {
	va_list args;
	va_start(args, format);
	
	printf(log_level[level]);
	
	vprintf(format, args);
	
	va_end(args);
	
	printf("\n");
	
	if (level == LOG_ERROR)
		exit(-1);
}
