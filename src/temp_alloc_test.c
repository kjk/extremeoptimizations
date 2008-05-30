/* Public domain code. Take all the code you want, we'll just write more. */
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "temp_alloc.h"

static void foo()
{
    void *tmp;
    void *m1;
    char *s;

    temp_alloc(200, &m1);
    memset(m1, 0, 200);
    assert(temp_total_alloced() == 200);
    tmp = m1;
    temp_freeall();
    assert(temp_total_alloced() == 200);

    /* should return the same address */
    temp_alloc(150, &m1);
    assert(tmp == m1);
    temp_freeall();
    assert(temp_total_alloced() == 200);

    /* should allocate new address now */
    temp_alloc(300, &m1);
    assert(temp_total_alloced() == 300);
    temp_freeall();
    assert(temp_total_alloced() == 300);

    temp_strdup("foo", &s);
    assert(0 == strcmp("foo", s));
}

int main(int argc, char **argv)
{
    temp_freeall();
    foo();
    temp_freeall();
    size_t alloced = temp_total_alloced();
    printf("Still alloced for temp values: %d\n", (int)alloced);
    temp_alloc_dump_stats();
    return 0;
}
