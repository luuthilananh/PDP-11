#include <stdio.h>
#include <stdarg.h>

#define ERROR 0
#define INFO 1
#define TRACE 2
#define DEBUG 3

int log_level = INFO; // устанавливаем стандартный уровень логирования

int set_log_level(int new_level) {
    int prev_level = log_level;
    log_level = new_level;
    return prev_level;
}

void log(int level, const char *format, ...) {
    if (level > log_level) return; // не выводим сообщение, если уровень логирования ниже заданного

    va_list args;
    va_start(args, format);
    vprintf(format, args); // выводим форматированное сообщение
    va_end(args);
}
