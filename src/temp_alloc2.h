/* Public domain code. Take all the code you want, we'll just write more. */
#ifndef TEMP_ALLOC_H__
#define TEMP_ALLOC_H__

/* core APIs */
int temp_alloc(size_t size, void **key);
int temp_realloc(size_t size, void **key);
void temp_freeall_helper(char *currstacktop);

#define temp_freeall() { \
    char dummy; \
    temp_freeall_helper(&dummy); \
   }

/* diagnostics */
size_t temp_total_alloced();
void temp_alloc_dump_stats();

#include "temp_alloc_helpers.h"

#endif
