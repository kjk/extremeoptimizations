#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "ParsedStrUnopt.h"
#include "ParsedStrUtil.h"

ParsedStr::ParsedStr()
{
    _count = 0;
    _names = _values = NULL;
}

bool ParsedStr::parse(const char *s)
{
    assert(NULL == _names); /* don't call me twice */
    char *scopy = strdup(s);
    int str_count = 0;
    char *stmp = scopy;
    while (NULL != delim_str_iter(&stmp)) {
        ++str_count;
    }
    if (str_count % 2 != 0) { /* malformed string */
        free(scopy);
        return false;
    }

    _count = str_count / 2;
    _names = (const char**)malloc(_count * sizeof(_names[0]));
    _values = (const char**)malloc(_count * sizeof(_values[0]));

    stmp = scopy;
    for (int i=0; i<_count; i++) {
        _names[i] = strdup(stmp);
        str_skip(&stmp);
        _values[i] = strdup(stmp);
        str_skip(&stmp);
    }
    free(scopy);
    return true;
}

ParsedStr::~ParsedStr()
{
    for (int i=0; i < _count; i++) {
        free((void*)_names[i]);
        free((void*)_values[i]);
    }
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
