#include "ParsedStrStl.h"
#include "ParsedStrUtil.h"

ParsedStr::ParsedStr()
{
}

bool ParsedStr::parse(const char *s)
{
    bool ok;

    char *scopy = strdup(s);
    for(;;) {
        char *name = parsed_str_iter(&scopy);
        if (NULL == name) {
             /* finished parsing */
            ok = true;
            break;
        }

        char *value = parsed_str_iter(&scopy);
        if (NULL == value) {
            /* malformed string */
            ok = false;
            break;
        }
        _names.push_back(std::string(name));
        _values.push_back(std::string(value));
    }
    free(scopy);
    return ok;
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
