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

bool ParsedStr::parse(const char *str)
{
    assert(NULL == _str); /* don't call me twice */
    _str = strdup(str);
    int str_count = 0;
    char *s = (char*)_str;
    while (NULL != delim_str_iter(&s)) {
        ++str_count;
    }
    if (str_count % 2 != 0) { /* malformed string */
        return false;
    }

    _count = str_count / 2;
    _names = (const char**)malloc(_count * sizeof(_names[0]));
    _values = (const char**)malloc(_count * sizeof(_values[0]));
    
    s = (char*)_str;
    int idx = 0;
    while (*s) {
        _names[idx] = s;
        str_skip(&s);
        _values[idx] = s;
        str_skip(&s);
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
