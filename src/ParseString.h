#ifndef IPARSED_STR_H__
#define IPARSED_STR_H__

class IParsedStr {
public:
    IParsedStr(const char* src);
    size_t count();
    const char* name(size_t idx);
    const char* value(size_t idx);
};

#endif
