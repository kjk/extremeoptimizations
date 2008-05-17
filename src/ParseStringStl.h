#ifndef PARSE_STRING_STL_H__
#define PARSE_STRING_STL_H__

class ParsedStrStl : public IParseStr
{
protected:
    std::string _str;
    std::vector<std::string> _names;
    std::vector<std::string> _values;
public:
    ParsedStrStl(const char *s);
};

#endif