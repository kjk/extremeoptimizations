#include "ParsedStrStl.h"
#include "ParsedStrUtil.h"
#include <assert.h>

ParsedStr::ParsedStr()
{
}

bool ParsedStr::parse(const char *s)
{
    assert(0 == _names.size()); /* don't call me twice */
    char *scopy = strdup(s);
    for(;;) {
        char *name = delim_str_iter(&scopy);
        if (NULL == name) { /* finished parsing */
            free(scopy);
            return true;
        }

        char *value = delim_str_iter(&scopy);
        if (NULL == value) { /* malformed string */
            free(scopy);
            return false;
        }
        _names.push_back(std::string(name));
        _values.push_back(std::string(value));
    }
}

size_t ParsedStr::count()
{
    return _names.size();
}

const char* ParsedStr::name(size_t idx)
{
    return _names[idx].c_str();
}

const char* ParsedStr::value(size_t idx)
{
    return _values[idx].c_str();
}
