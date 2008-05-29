/* Public domain code. Take all the code you want, we'll just write more. */
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "temp_alloc.h"

#define STATS

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

#ifdef STATS
int total_allocs = 0;
int allocs_from_cache = 0;
#endif

static void *ptr_from_meminfo(meminfo *mi)
{
    char *d = (char*)mi + sizeof(meminfo);
    return (void*)d;
}

/* Allocate temporary space of a given <size> and put in under <key>.
   Returns 0 if failed to allocate, 1 otherwise.
   Assumptions: number of unique allocation points (i.e. unique values of
   <key>) is relatively small so linear search is ok. */
int temp_alloc(size_t size, void **key)
{
    meminfo *curr;
    meminfo **prev;
    verify_on_stack(key);
    curr = first;
    prev = &first;
    /* see if we already have allocation with the same key. */
    while (curr) {
        if (key == curr->key)
            break;
        prev = &curr;
        curr = curr->next;
    }

#ifdef STATS
    ++total_allocs;
#endif

    /* reuse the memory if we have it and its size is big enough. */
    if (curr && curr->size >= size) {
        /* on the other hand, a cache with a bad policy is another name for a memory leak
           (http://blogs.msdn.com/oldnewthing/archive/2006/05/02/588350.aspx)
           so if the last allocation was significantly bigger than the
           size we ask for (heuristic: twice as big) we free it anyway to prevent
           growing the cache infinitely */
        if (curr->size < size * 2) {
            *key = ptr_from_meminfo(curr);
#ifdef STATS
            ++allocs_from_cache;
#endif
            return 1;
        }
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
    /* in this implementation it's exactly the same as temp_alloc() */
    return temp_alloc(size, key);
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

int temp_strdup(const char *txt, char **key)
{
    if (!txt) {
        *key = 0;
        return 1;
    }

    return temp_memdup((void*)txt, strlen(txt)+1, (void**)key);
}

/* Return total amount of allocated memory. For diagnostics mostly */
size_t temp_total_alloced()
{
    return total_alloced;
}

#ifdef STATS
void temp_alloc_dump_stats()
{
    printf("Total alllocs:     %d\n", total_allocs);
    printf("Allocs from cache: %d\n", allocs_from_cache);
}
#else
void temp_alloc_dump_stats() {}
#endif
