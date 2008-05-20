#include <stdio.h>
#include "ParsedStrUtil.h"

static bool is_delim(char c)
{
    return (('=' == c) || ('&' == c) || (0 == c));
}

void str_skip(char **str)
{
    char *s = *str;
    while (0 != *s)
        ++s;
    *str = s + 1;
}

/* given <str> pointing to zero-terminated strings laid out consecutively in
   memory, returns string number <i> */
const char *get_str(const char *str, int i)
{
    char *start = (char*)str;
    char *s = (char*) str;
    while (i > 0) {
        str_skip(&s);
        --i;
    }
    int idx = (s - start);
    return str + idx;
}

/* Iterates over substrings of <str> delimited by '=' and '&' characters.
   Replaces delimiter characters by 0.
   Returns NULL to signal end of  iteration. */
char *delim_str_iter(char **str)
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
