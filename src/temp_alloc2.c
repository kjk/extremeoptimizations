/* Public domain code. Take all the code you want, we'll just write more. */
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "temp_alloc.h"

#define ENTRIES_COUNT 128

typedef char * key_type; /* for readability */
typedef struct meminfo {
    size_t size;
    void *mem;
} meminfo;

/* Store keys and rest of the info in separate arrays for cache efficiency.
   We traverse keys often, so we want the array to be as small as possible */
static key_type keys[ENTRIES_COUNT] = {0};
static meminfo allocs_info[ENTRIES_COUNT] = {0};

static int keys_count = 0;

size_t total_alloced = 0;
int total_allocs = 0;
int allocs_from_cache = 0;

/* Allocate temporary space of a given <size> and put in under <key>.
   Returns 0 if failed to allocate, 1 otherwise.
   Assumptions: number of unique allocation points (i.e. unique values of
   <key>) is relatively small so linear search is ok. 
   Note: the speed could have been probably improved (especially for large
   numbers of unique keys) if we did insert new values in sorted order and
   did a binary search instead of linear. Since we never remove keys, number
   of insertions would be very small. */
static int temp_alloc_helper(size_t size, void **key, int copyold)
{
    int idx;
    meminfo *mi;
    void *old_mem;
    size_t old_size;
    size_t to_copy;
    key_type k = (key_type)key;
    key_type *curr = &keys[0];
    key_type *keys_end = curr + keys_count;

    verify_on_stack(key);

    /* linear search in arrays of keys */
    while (curr < keys_end) {
        if (k == *curr)
            break;
        ++curr;
    }

    idx = curr - keys;
    if (idx >= keys_count) {
        /* didn't find */
        if (keys_count >= ENTRIES_COUNT) {
            /* not enough space in array. need to recompile with bigger limit */
            assert(0);
            *key = malloc(size); /* memory leak */
            return *key != 0;
        }
        /* insert at the end */
        *curr = k;
        allocs_info[idx].mem = 0;
        ++keys_count;
    }

    assert(*curr = k);
    mi = &allocs_info[idx];
    if (mi->mem && mi->size > size && (mi->size < 2 * size || copyold)) {
        /* reuse the memory */
        *key = mi->mem;
        ++allocs_from_cache;
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
        ++total_allocs;

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
    key_type *curr = &keys[0];
    key_type *keys_end = &keys[keys_count];
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
