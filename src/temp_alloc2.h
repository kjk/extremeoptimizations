/* This code is in public domain. 
   Take all the code you want, we'll just write more. */
#ifndef TEMP_ALLOC_H__
#define TEMP_ALLOC_H__

int temp_alloc(size_t size, void **key);

void temp_freeall_helper(char *currstacktop);

#define temp_freeall() { \
    char dummy; \
    temp_freeall_helper(&dummy); \
   }

int temp_realloc(size_t size, void **key);
size_t temp_total_alloced();

int temp_memdup(void *mem, size_t size, void **key);
int temp_strdup(const char *txt, char **key);

#endif
