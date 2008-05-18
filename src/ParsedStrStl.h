#ifndef PARSED_STRING_STL_H__
#define PARSED_STRING_STL_H__

#include <vector>
#include <string>

class ParsedStr
{
protected:
    std::string _str;
    std::vector<std::string> _names;
    std::vector<std::string> _values;
public:
    ParsedStr();
    int parse(const char *s);
    size_t count();
    const char* name(size_t idx);
    const char* value(size_t idx);
};

#endif