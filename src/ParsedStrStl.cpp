#include "ParsedStrStl.h"
#include "ParsedStrUtil.h"

ParsedStr::ParsedStr()
{
}

int ParsedStr::parse(const char *s)
{
    int result;

    char *scopy = strdup(s);
    for(;;) {
        char *name = parsed_str_iter(&scopy);
        if (NULL == name) {
             /* finished parsing */
            result = 1;
            break;
        }

        char *value = parsed_str_iter(&scopy);
        if (NULL == value) {
            /* malformed string */
            result = 0;
            break;
        }
        _names.push_back(std::string(name));
        _values.push_back(std::string(value));
    }
    free(scopy);
    return result;
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
