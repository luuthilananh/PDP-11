#pragma once

/*#define ERROR 0
#define INFO 1
#define TRACE 2
#define DEBUG 3
*/
enum lognum
{
	ERROR = 50,
	INFO = 40,
	TRACE = 30,
	DEBUG = 20,
	MORE_DEBUG = 10,
};

int set_log_level(int new_level);
void trace(int level, const char *format, ...);
