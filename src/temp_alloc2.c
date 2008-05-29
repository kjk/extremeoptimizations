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

#define ENTRIES_START_COUNT 128

typedef char * key_t; /* for readability */
typedef struct meminfo {
    size_t size;
    void *mem;
} meminfo;

/* Store keys and rest of the info in separate arrays for cache efficiency.
   We traverse keys often, so we want the array to be as small as possible */
static key_t *keys_start = NULL;
static key_t *keys_end = NULL;
static meminfo *allocs_info = NULL;

int keys_alloced = 0;

size_t total_alloced = 0;

#ifdef STATS
int total_allocs = 0;
int allocs_from_cache = 0;
#endif

/* Allocate temporary space of a given <size> and put in under <key>.
   Returns 0 if failed to allocate, 1 otherwise.
   Assumptions: number of unique allocation points (i.e. unique values of
   <key>) is relatively small so linear search is ok. 
   Note: the speed could have been probably improved (especially for large
   numbers of unique keys) if we did insert new values in sorted order and
   did a binary search instead of linear. Since we never remove keys, number
   of insertions would be very small. */
int temp_alloc_helper(size_t size, void **key, int copyold)
{
    int idx;
    meminfo *mi;
    void *old_mem;
    size_t old_size;
    size_t to_copy;
    key_t k = (key_t)key;
    key_t *curr = keys_start;

    verify_on_stack(key);

    /* linear search in arrays of keys */
    while (curr < keys_end) {
        if (k == *curr)
            break;
        ++curr;
    }

    idx = curr - keys_start;
    if (curr >= keys_end) {
        /* didn't find */
        int keys_count = keys_end - keys_start;
        if (keys_alloced  <= keys_count) {
            /* neds to grow arrays for keys and meminfo */
            int new_keys_alloced = keys_alloced * 2;
            if (0 == new_keys_alloced)
                new_keys_alloced = ENTRIES_START_COUNT;
            /* optimisticallya assume that allocations won't fail */
            keys_alloced = new_keys_alloced;
            keys_start = (key_t*)realloc(keys_start, new_keys_alloced * sizeof(key_t));
            allocs_info = (meminfo*)realloc(allocs_info, new_keys_alloced * sizeof(meminfo));
            keys_end = keys_start + keys_alloced;
            curr = &keys_start[keys_count];
        }
        /* insert at the end */
        *curr = k;
        idx = curr - keys_start;
        allocs_info[idx].mem = 0;
        ++keys_end;
    }

    assert(*curr = k);
    mi = &allocs_info[idx];
    if (mi->mem && mi->size > size && mi->size < 2 * size) {
        /* reuse the memory */
        *key = mi->mem;
#ifdef STATS
        ++allocs_from_cache;
#endif
        return 1;
    }

    old_size = 0;
    old_mem = mi->mem;
    if (old_mem)
        old_size = mi->size;

    mi->mem = malloc(size);
    *key = mi->mem;
    mi->size = size;
    if (mi->mem) {
        assert(total_alloced >= mi->size);
        total_alloced += size;
#ifdef STATS
        ++total_allocs;
#endif

        if (old_mem) {
            if (copyold) {
                to_copy = old_size;
                if (to_copy > size)
                    to_copy = size;
                memcpy(mi->mem, old_mem, to_copy);
            }
            free(old_mem);
        }

        return 1;
    }
    return 0;
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
    key_t *curr = keys_start;
    meminfo *mi_curr = allocs_info;

    /* linear search in arrays of keys */
    while (curr < keys_end) {
        char *addr = *curr;
        if (currstacktop > addr && mi_curr->mem) {
            free(mi_curr->mem);
            mi_curr->mem = 0;
            assert(total_alloced >= mi_curr->size);
            total_alloced -= mi_curr->size;
        }
        ++curr;
        ++mi_curr;
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

#ifdef STATS
void temp_alloc_dump_stats()
{
    printf("Total alllocs:     %d\n", total_allocs);
    printf("Allocs from cache: %d\n", allocs_from_cache);
}
#else
void temp_alloc_dump_stats() {}
#endif