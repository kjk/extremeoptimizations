#ifndef PARSED_STRING_H__
#define PARSED_STRING_H__

/* make the size of the buffer a multiple of 8, to match malloc() padding */
#define PARSED_STR_BUF_SIZE 256 - sizeof(char*) - sizeof(size_t)

class ParsedStr
{
protected:
    size_t _count;
    const char *_str;
    char _buf[PARSED_STR_BUF_SIZE];
public:
    ParsedStr();
    ~ParsedStr();
    bool parse(const char *s);
    size_t count();
    const char* name(size_t idx);
    const char* value(size_t idx);
};

#undef PARSED_STR_BUF_SIZE

#endif
