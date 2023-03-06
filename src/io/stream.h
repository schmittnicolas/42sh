#ifndef STREAM_H
#define STREAM_H

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_user_input(FILE *stream);
FILE *create_stream_from_file(const char *path);
FILE *create_stream_from_string(const char *input);
void print_log(const char *fmt, ...);

#endif /* STREAM_H */
