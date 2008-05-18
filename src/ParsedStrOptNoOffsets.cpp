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
}

static void skip_one(char **str)
{
    char *s = *str;
    while (0 != *s)
        ++s;
    *str = s + 1;
}

static const char *get_str(const char *str, int i)
{
    char *start = (char*)str;
    char *s = (char*) str;
    while (i > 0) {
        skip_one(&s);
        --i;
    }
    int idx = (s - start);
    return str + idx;
}

int ParsedStr::parse(const char *str)
{
    _str = strdup(str);
    _count = 0;
    char *s = (char*)_str;
    while (NULL != parsed_str_iter(&s)) {
        ++_count;
    }
    /* if count is not even => malformed string */
    if (_count % 2 != 0) {
        return 0;
    }
    _count = _count / 2;
    return 1;
}

ParsedStr::~ParsedStr()
{
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
