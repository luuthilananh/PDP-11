#pragma once

#define ERROR 0
#define INFO 1
#define TRACE 2
#define DEBUG 3

void trace(int level, const char *format, ...);
