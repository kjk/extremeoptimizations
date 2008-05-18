#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "ParsedStrOptOneArray.h"
#include "ParsedStrUtil.h"

ParsedStr::ParsedStr()
{
    _count = 0;
    _str = NULL;
    _names_values = NULL;
}

static void skip_one(char **str)
{
    char *s = *str;
    while (0 != *s)
        ++s;
    *str = s + 1;
}

int ParsedStr::parse(const char *str)
{
    _str = strdup(str);
    int count = 0;
    /* first, let's calculate the count of strings */
    char *s = (char*)_str;
    while (NULL != parsed_str_iter(&s)) {
        ++count;
    }
    /* if count is not even => malformed string */
    if (count % 2 != 0) {
        return 0;
    }

    _count = count / 2;
    _names_values = (const char**)malloc(_count * 2 * sizeof(_names_values[0]));
    
    s = (char*)_str;
    int idx = 0;
    while (*s) {
        _names_values[idx++] = s;
        skip_one(&s);
        _names_values[idx++] = s;
        skip_one(&s);
    }

    assert(idx == _count * 2);
    return 1;
}

ParsedStr::~ParsedStr()
{
    free((void*)_str);
    free(_names_values);
}

size_t ParsedStr::count()
{
    return _count;
}

const char* ParsedStr::name(size_t idx)
{
    return _names_values[idx*2];
}

const char* ParsedStr::value(size_t idx)
{
    return _names_values[idx*2+1];
}
