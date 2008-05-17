#include <stdio.h>

#ifdef VER_DUMMY
#include "ParsedStrDummy.h"
#endif

#ifdef VER_STL
#include "ParsedStrStl.h"
#endif

#ifdef VER_UNOPT
#include "ParsedStrUnopt.h"
#endif

#ifdef VER_ALLOC_OPT
#include "ParsedStrAllocOpt.h"
#endif

#define STR_TO_PARSE "foo=bar&foo2=burgunty&cookie=hello&foo=bargain"

int tests_failed = 0;
int tests_total = 0;

void test_failed() 
{
    ++tests_failed;
    ++tests_total;
}

void test_passed() 
{
    ++tests_total;
}

#define verify(expr) \
    if (expr) \
        test_passed(); \
    else \
        test_failed();

#define streq(s1, s2) (0 == strcmp(s1, s2))

int main(int argc, char **argv)
{
    printf("Testing ParseStr implementation\n");
    ParsedStr p(STR_TO_PARSE);
    verify(4 == p.count());
    
    printf("");
    return 0;
}