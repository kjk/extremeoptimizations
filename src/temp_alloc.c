
#include <stdlib.h>
#include <assert.h>

#include "temp_alloc.h"

#ifdef NDEBUG
static void verify_on_stack(void *addr)
{
    /* TODO: assert()/crash if addr is not on stack */
}
#else
#define verify_on_stack(addr)
#endif

typedef struct meminfo {
    struct meminfo *next;
    void **key;
    size_t size;
    /* data follows */
} meminfo;

static meminfo *first = NULL;
size_t total_alloced = 0;

#define FREE_THRESHOLD 4*1024

static void *ptr_from_meminfo(meminfo *mi)
{
    char *d = (char*)mi + sizeof(meminfo);
    return (void*)d;
}

/* Allocate */
int temp_alloc(size_t size, void **key)
{
    meminfo *curr;
    meminfo **prev;
    verify_on_stack(key);
    curr = first;
    prev = &first;
    while (curr) {
        if (key == curr->key)
            break;
        prev = &curr;
        curr = curr->next;
    }
    if (curr && curr->size > size) {
        /* TODO: should periodically free curr to prevent unbound grow of cache.
           On the other hand, if we call temp_freeall() frequently enough,
           that shouldn't be the problem */
        *key = ptr_from_meminfo(curr);
        return 1;
    }
    /* free and unlink */
    if (curr) {
        *prev = curr->next;
        assert(curr->size >= total_alloced);
        total_alloced -= curr->size;
        free((void*)curr);
    }
    curr = (meminfo*)malloc(sizeof(meminfo) + size);
    if (!curr) {
        *key = 0;
        return 0;
    }
    /* TODO: change it based on the number of allocations since last freall()
       to avoid worst case of always calling temp_freeall()? */
    if (total_alloced + size > FREE_THRESHOLD) {
        temp_freeall();
    }

    curr->key = key;
    curr->size = size;
    curr->next = first;
    first = curr;
    total_alloced += size;
    *key = ptr_from_meminfo(curr);
    return 1;
}

int temp_realloc(size_t size, void **key)
{
    return temp_alloc(size, key); /* it really is the same thing */
}

void temp_freeall()
{
    meminfo *curr, *tmp;
    meminfo **prev;
    /* the stack grows down so I can free all memory whose key address
       is > currstackpos */
    char *currstackpos = (char*)&curr;
    prev = &first;
    curr = first;
    while (curr) {
        char *addr = (char*)curr->key;
        if (currstackpos > addr) {
            /* free it */
            *prev = curr->next;
            tmp = curr->next;
            total_alloced -= curr->size;
            free((void*)curr);
            curr = tmp;
        } else {
            prev = &curr;
            curr = curr->next;
        }
    }

}

size_t temp_total_alloced()
{
    return total_alloced;
}

