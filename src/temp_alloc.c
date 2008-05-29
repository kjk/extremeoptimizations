/* Public domain code. Take all the code you want, we'll just write more. */
#include "temp_alloc.h"

typedef struct meminfo {
    struct meminfo *next;
    void **key;
    size_t size;
    /* data follows */
} meminfo;

static meminfo *first = NULL;
size_t total_alloced = 0;

int total_allocs = 0;
int allocs_from_cache = 0;

static void *ptr_from_meminfo(meminfo *mi)
{
    char *d = (char*)mi + sizeof(meminfo);
    return (void*)d;
}

/* Allocate temporary space of a given <size> and put in under <key>.
   Returns 0 if failed to allocate, 1 otherwise.
   Assumptions: number of unique allocation points (i.e. unique values of
   <key>) is relatively small so linear search is ok. */
static int temp_alloc_helper(size_t size, void **key, int copyold)
{
    /* TODO: implement copyold */
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

    ++total_allocs;

    /* reuse the memory if we have it and its size is big enough. */
    if (curr && curr->size >= size) {
        /* on the other hand, a cache with a bad policy is another name for a memory leak
           (http://blogs.msdn.com/oldnewthing/archive/2006/05/02/588350.aspx)
           so if the last allocation was significantly bigger than the
           size we ask for (heuristic: twice as big) we free it anyway to prevent
           growing the cache infinitely */
        if (curr->size < size * 2) {
            *key = ptr_from_meminfo(curr);
            ++allocs_from_cache;
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

int temp_alloc(size_t size, void **key)
{
    return temp_alloc_helper(size, key, 0);
}

int temp_realloc(size_t size, void **key)
{
    return temp_alloc_helper(size, key, 1);
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
