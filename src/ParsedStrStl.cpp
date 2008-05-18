#include "ParsedStrStl.h"
#include "ParsedStrUtil.h"

ParsedStr::ParsedStr()
{
}

int ParsedStr::parse(const char *s)
{
    char *scopy = strdup(s);
    for(;;) {
        char *name = parsed_str_iter(&scopy);
        if (NULL == name)
            return 1; /* finished parsing */

        char *value = parsed_str_iter(&scopy);
        if (NULL == value)
            return 0; /* malformed string */
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
