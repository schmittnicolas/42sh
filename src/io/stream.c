#define _POSIX_C_SOURCE 200809L
#define BUFFER_SIZE 4096

#include "stream.h"

FILE *create_stream_from_file(const char *path)
{
    FILE *stream = fopen(path, "r");

    if (!stream)
    {
        fprintf(stderr, "Failed to open file %s\n", path);
        return NULL;
    }

    return stream;
}

FILE *create_stream_from_string(const char *input)
{
    size_t len = strlen(input);
    char *buffer = malloc(len + 1);
    strcpy(buffer, input);
    buffer[len] = '\0';

    FILE *stream = open_memstream(&buffer, &len);

    return stream;
}

void print_log(const char *fmt, ...)
{
    if (0)
    {
        va_list args;
        static FILE *logfile = NULL;
        if (logfile == NULL)
            logfile = fopen("logfile", "w");

        va_start(args, fmt);
        vfprintf(logfile, fmt, args);
        va_end(args);
    }
}
