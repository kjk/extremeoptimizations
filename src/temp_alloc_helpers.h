#ifndef TEMP_ALLOC_HELPERS_H__
#define TEMP_ALLOC_HELPERS_H__

#ifdef DEBUG
void verify_on_stack(void *addr);
#else
#define verify_on_stack(addr)
#endif

/* utility functions built on top of core APIs */
int temp_memdup(void *mem, size_t size, void **key);
int temp_strdup(const char *txt, char **key);

/* diagnostics */
size_t temp_total_alloced();
void temp_alloc_dump_stats();

#endif
