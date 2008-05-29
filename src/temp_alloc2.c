/* This code is in public domain. 
   Take all the code you want, we'll just write more. */
#include <stdlib.h>
#include <assert.h>
#include <string.h>

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
    /* see if we already have a memory with the same key. Reuse the memory
       if we have and its size is big enough.
       TODO: this is meant to avoid malloc() calls, but maybe it would be faster
       to just malloc() without the check, to avoid traversing the list */
    while (curr) {
        if (key == curr->key)
            break;
        prev = &curr;
        curr = curr->next;
    }
    if (curr && curr->size >= size) {
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

/* Free all temporary allocations that are no longer needed */
void temp_freeall_helper(char *currstacktop)
{
    meminfo *curr, *tmp;
    meminfo **prev;
    /* the stack grows down so I can free all memory whose key address
       is < currstackpos */
    prev = &first;
    curr = first;
    while (curr) {
        char *addr = (char*)curr->key;
        if (currstacktop > addr) {
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

int temp_memdup(void *mem, size_t size, void **key)
{
    int ok = temp_alloc(size, key);
    if (!ok)
        return 0;
    memcpy(*key, mem, size);
    return 1;
}

size_t temp_total_alloced()
{
    return total_alloced;
}

int temp_strdup(const char *txt, char **key)
{
    if (!txt) {
        *key = 0;
        return 1;
    }

    return temp_memdup((void*)txt, strlen(txt)+1, (void**)key);
}

void temp_alloc_dump_stats() {}
