#ifndef STREAM_H
#define STREAM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

char *get_user_input(FILE *stream);
FILE* create_stream_from_file(const char *path);
FILE* create_stream_from_string(const char *input);

#endif /* STREAM_H */
