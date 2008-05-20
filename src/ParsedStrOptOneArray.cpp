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
    _names_values = (const char**)malloc(_count * 2 * sizeof(_names_values[0]));
    s = (char*)_str;
    int idx = 0;
    while (*s) {
        _names_values[idx++] = s;
        str_skip(&s);
        _names_values[idx++] = s;
        str_skip(&s);
    }
    assert(idx == _count * 2);
    return true;
}

ParsedStr::~ParsedStr()
{
    free((void*)_str);
    free(_names_values);
}

const char* ParsedStr::name(size_t idx)
{
    return _names_values[idx*2];
}

const char* ParsedStr::value(size_t idx)
{
    return _names_values[idx*2+1];
}

size_t ParsedStr::count()
{
    return _count;
}

