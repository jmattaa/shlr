#include "utils.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int shlr_utils_inStrArr(char *str, char **arr, int len)
{
    int idx = -1;
    for (int i = 0; i < SH_KEYWORDS_COUNT; i++)
        if (strcmp(str, shlr_keywords[i]) == 0)
        {
            idx = i;
            break;
        }

    return idx;
}

const char *shlr_utils_readFile(char *path, size_t *len)
{
    FILE *file = fopen(path, "rb");
    if (file == NULL)
        shlr_logger_fatal(1, "failed to open file at '%s'", path);

    fseek(file, 0, SEEK_END);
    *len = ftell(file);
    if (*len == 0)
        shlr_logger_fatal(1, "'%s' is empty", path);

    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(*len + 1);
    if (buffer == NULL)
        shlr_logger_fatal(1, "memmory allocation failed!");

    size_t bytesRead = fread(buffer, 1, *len, file);
    fclose(file);

    if (bytesRead != *len)
        shlr_logger_fatal(1, "failed to read file at '%s'", path);

    buffer[*len] = '\0';
    return buffer;
}
