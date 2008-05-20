#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "ParsedStrOptNoOffsets.h"
#include "ParsedStrUtil.h"

ParsedStr::ParsedStr()
{
    _count = 0;
    _str = NULL;
}

bool ParsedStr::parse(const char *str)
{
    assert(NULL == _str); /* don't call me twice */
    _str = str;
    _count = 0;
    char *s = (char*)_str;
    while (NULL != delim_str_iter(&s)) {
        ++_count;
    }
    if (_count % 2 != 0) { /* malformed string */
        return false;
    }
    _count = _count / 2;
    return true;
}

ParsedStr::~ParsedStr()
{
}

size_t ParsedStr::count()
{
    return _count;
}

const char* ParsedStr::name(size_t idx)
{
    return get_str(_str, idx * 2);
}

const char* ParsedStr::value(size_t idx)
{
    return get_str(_str, idx * 2 + 1);
}
