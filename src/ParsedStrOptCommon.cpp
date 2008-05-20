#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "ParsedStrOptCommon.h"
#include "ParsedStrUtil.h"

ParsedStr::ParsedStr()
{
    _count = 0;
    _str = NULL;
}

bool ParsedStr::parse(const char *str)
{
    assert(NULL == _str); /* don't call me twice */
    size_t len = strlen(str);
    if (len > sizeof(_buf)-1)
        _str = strdup(str);
    else {
        memcpy(_buf, str, len + 1);
        _str = _buf;
    }

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
    if (_str != _buf)
        free((void*)_str);
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
