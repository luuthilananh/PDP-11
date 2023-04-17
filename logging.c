#include <stdio.h>
#include <stdarg.h>

#include "logging.h"


int log_level = TRACE; // устанавливаем стандартный уровень логирования

int set_log_level(int new_level) {
    int prev_level = log_level;
    log_level = new_level;
    return prev_level;
}

void trace(int level, const char *format, ...) {
    if (level > log_level) return; // не выводим сообщение, если уровень логирования ниже заданного

    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args); // выводим форматированное сообщение
    va_end(args);
}
