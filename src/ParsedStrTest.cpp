#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    printf("Testing ParsedStr implementation.\n");
    ParsedStr p;
    verify(p.parse(STR_TO_PARSE));
    verify(4 == p.count());
    verify(streq("foo", p.name(0)));
    verify(streq("bar", p.value(0)));
    verify(streq("foo2", p.name(1)));
    verify(streq("burgunty", p.value(1)));
    verify(streq("cookie", p.name(2)));
    verify(streq("hello", p.value(2)));
    verify(streq("foo", p.name(3)));
    verify(streq("bargain", p.value(3)));
    if (tests_failed > 0)
        printf("%d out of %d tests failed\n", tests_failed, tests_total);
    else
        printf("All %d tests passed!\n", tests_total);
    return 0;
}