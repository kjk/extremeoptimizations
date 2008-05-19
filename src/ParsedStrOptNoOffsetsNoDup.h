#ifndef PARSED_STRING_H__
#define PARSED_STRING_H__

class ParsedStr
{
protected:
    size_t _count;
    const char *_str;
public:
    ParsedStr();
    ~ParsedStr();
    bool parse(const char *s);
    size_t count();
    const char* name(size_t idx);
    const char* value(size_t idx);
};

#endif
