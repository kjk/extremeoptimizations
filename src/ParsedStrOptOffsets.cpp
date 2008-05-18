#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "ParsedStrOptOffsets.h"
#include "ParsedStrUtil.h"

ParsedStr::ParsedStr()
{
    _count = 0;
    _str = NULL;
    _names_values_offsets = NULL;
}

static void skip_one(char **str)
{
    char *s = *str;
    while (0 != *s)
        ++s;
    *str = s + 1;
}

bool ParsedStr::parse(const char *str)
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
        return false;
    }

    _count = count / 2;
    _names_values_offsets = (unsigned short *)malloc(_count * 2 * sizeof(_names_values_offsets[0]));
    
    s = (char*)_str;
    int idx = 0;
    while (*s) {
        _names_values_offsets[idx++] = s - _str;
        skip_one(&s);
        _names_values_offsets[idx++] = s - _str;
        skip_one(&s);
    }

    assert(idx == _count * 2);
    return true;
}

ParsedStr::~ParsedStr()
{
    free((void*)_str);
    free(_names_values_offsets);
}

size_t ParsedStr::count()
{
    return _count;
}

const char* ParsedStr::name(size_t idx)
{
    return _str + _names_values_offsets[idx*2];
}

const char* ParsedStr::value(size_t idx)
{
    return _str + _names_values_offsets[idx*2+1];
}
