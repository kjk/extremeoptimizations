/* Public domain code. Take all the code you want, we'll just write more. */
#ifndef TEMP_ALLOC_H__
#define TEMP_ALLOC_H__

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

/* core APIs */
int temp_alloc(size_t size, void **key);
int temp_realloc(size_t size, void **key);
void temp_freeall_helper(char *currstacktop);

#define temp_freeall() { \
    char dummy; \
    temp_freeall_helper(&dummy); \
   }

#include "temp_alloc_helpers.h"

#endif
