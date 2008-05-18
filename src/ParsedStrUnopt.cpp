#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ParsedStrUnopt.h"
#include "ParsedStrUtil.h"

ParsedStr::ParsedStr()
{
    _count = 0;
    _names = _values = NULL;
}

enum {CALC_COUNT, BUILD};

int ParsedStr::parse(const char *s)
{
    int count;
    for (int phase=CALC_COUNT; phase <= BUILD; phase++) {
        char *scopy = strdup(s);
        count = 0;
        for(;;) {
            char *name = parsed_str_iter(&scopy);
            if (NULL == name) {
                /* finished parsing */
                free(scopy);
                break;
            }
            char *value = parsed_str_iter(&scopy);
            if (NULL == value) {
                /* malformed string */
                free(scopy);
                return 0;
            }
            if (BUILD == phase) {
                _names[count] = strdup(name);
                _values[count] = strdup(value);                
            }
            ++count;
        }
        if (CALC_COUNT == phase) {
            _count = count;
            if (count > 0) {
                _names = (const char**)malloc(count * sizeof(_names[0]));
                _values = (const char**)malloc(count * sizeof(_values[0]));
            }
        }
    }
    return 1;
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
