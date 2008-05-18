#ifndef PARSED_STR_H__
#define PARSED_STR_H__

class ParsedStr {
public:
    ParsedStr();
    bool parse(const char* src);
    size_t count();
    const char* name(size_t idx);
    const char* value(size_t idx);
};

#endif
