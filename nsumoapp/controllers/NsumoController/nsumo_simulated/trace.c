#include "common/trace.h"
#include <stdio.h>
#include <stdarg.h>

void trace(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void trace_init()
{
}
