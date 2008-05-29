#include "temp_alloc.h"

extern size_t total_alloced;
extern int total_allocs;
extern int allocs_from_cache;

#ifdef DEBUG
static void verify_on_stack(void *addr)
{
    /* TODO: assert()/crash if addr is not on stack */
}
#endif

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

void temp_alloc_dump_stats()
{
    printf("Total alllocs:     %d\n", total_allocs);
    printf("Allocs from cache: %d\n", allocs_from_cache);
}
