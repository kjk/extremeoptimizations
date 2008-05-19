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

#ifdef VER_OPT_ALLOC
#include "ParsedStrOptAlloc.h"
#endif

#ifdef VER_OPT_ONE_ARRAY
#include "ParsedStrOptOneArray.h"
#endif

#ifdef VER_OPT_OFFSETS
#include "ParsedStrOptOffsets.h"
#endif

#ifdef VER_OPT_NO_OFFSETS
#include "ParsedStrOptNoOffsets.h"
#endif

#ifdef VER_OPT_NO_OFFSETS_NO_DUP
#include "ParsedStrOptNoOffsetsNoDup.h"
#endif

#ifdef VER_OPT_COMMON
#include "ParsedStrOptCommon.h"
#endif

#define STR_TO_PARSE "foo=bar&foo2=burgunty&cookie=hello&foo=bargain"

int tests_failed = 0;
int tests_total = 0;

void test_failed(const char *expr) 
{
    printf("Expression '%s' failed\n", expr);
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
        test_failed(#expr);

int streq(const char *s1, const char* s2)
{
    if ((NULL == s1) && (NULL == s2))
        return 1;
    if ((NULL == s1) || (NULL == s2))
        return 0;
    return 0 == strcmp(s1, s2);
}

int main(int argc, char **argv)
{
    printf("Testing ParsedStr implementation.\n");
    printf("sizeof(ParsedStr)=%d\n", sizeof(ParsedStr));
    ParsedStr p;
    char *s = strdup(STR_TO_PARSE);
    verify(p.parse(s));
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
    free(s);
    return 0;
}
