#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "ParsedStrOptAlloc.h"
#include "ParsedStrUtil.h"

ParsedStr::ParsedStr()
{
    _count = 0;
    _str = NULL;
    _names = _values = NULL;
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
    _names = (const char**)malloc(_count * sizeof(_names[0]));
    _values = (const char**)malloc(_count * sizeof(_values[0]));
    
    s = (char*)_str;
    int idx = 0;
    while (*s) {
        _names[idx] = s;
        skip_one(&s);
        _values[idx] = s;
        skip_one(&s);
        ++idx;
    }

    assert(idx == _count);
    return true;
}

ParsedStr::~ParsedStr()
{
    free((void*)_str);
    free(_names);
    free(_values);
}

size_t ParsedStr::count()
{
    return _count;
}

const char* ParsedStr::name(size_t idx)
{
    return _names[idx];
}

const char* ParsedStr::value(size_t idx)
{
    return _values[idx];
}
