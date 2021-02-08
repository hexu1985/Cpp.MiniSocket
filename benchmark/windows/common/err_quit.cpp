#include "err_quit.hpp"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void err_quit(const char *format, ...)
{
    va_list args;

    va_start(args, format);
    fflush(stdout);
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(1);
}

