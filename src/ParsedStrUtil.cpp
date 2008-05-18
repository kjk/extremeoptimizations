#include <stdio.h>
#include "ParsedStrUtil.h"

static int is_delim(char c)
{
    return (('=' == c) || ('&' == c) || (0 == c));
}

char *parsed_str_iter(char **str)
{
    char *s = *str;
    if (!s)
        return s;
    if (0 == *s) {
        *str = NULL;
        return NULL;
    }
    char *start = s;
    while (!is_delim(*s)) {
        ++s;
    }
    if (0 != *s) {
        *s = 0;
        *str = s+1;
    } else {
        *str = NULL;
    }
    return start;
}
