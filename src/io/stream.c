#define _POSIX_C_SOURCE 200809L
#define BUFFER_SIZE 4096

#include "stream.h"

char *get_user_input(FILE* stream)
{
    char buf[BUFFER_SIZE];
    char *ptr = NULL;
    char ptrlen = 0;

    while(fgets(buf, BUFFER_SIZE, stream))
    {
        int buflen = strlen(buf);

        if(!ptr)
        {
            ptr = malloc(buflen+1);
        }
        else
        {
            char *ptr2 = realloc(ptr, ptrlen+buflen+1);

            if(ptr2)
            {
                ptr = ptr2;
            }
            else
            {
                free(ptr);
                ptr = NULL;
            }
        }

        if(!ptr)
        {
            fprintf(stderr, "error: failed to alloc buffer: %s\n", 
                    strerror(errno));
            return NULL;
        }

        strcpy(ptr+ptrlen, buf);

        if(buf[buflen-1] == '\n')
        {
            if(buflen == 1 || buf[buflen-2] != '\\')
            {
                return ptr;
            }

            ptr[ptrlen+buflen-2] = '\0';
            buflen -= 2;
        }

        ptrlen += buflen;
    }

    return ptr;
}

FILE* create_stream_from_file(const char *path)
{
    FILE* stream = fopen(path, "r");

    if (!stream)
    {
        fprintf(stderr, "Failed to open file %s\n", path);
        return NULL;
    }

    return stream;
}

FILE* create_stream_from_string(const char *input)
{
    size_t len = strlen(input);
    char* buffer = malloc(len + 1);
    strcpy(buffer, input);
    buffer[len] = '\0';

    FILE* stream = open_memstream(&buffer, &len);

    return stream;
}
