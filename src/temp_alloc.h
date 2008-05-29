#ifndef TEMP_ALLOC_H__
#define TEMP_ALLOC_H__

int temp_alloc(size_t size, void **key);
int temp_realloc(size_t size, void **key);
void temp_freeall();
size_t temp_total_alloced();

int temp_memdup(void *mem, size_t size, void **key);
int temp_strdup(const char *txt, char **key);

#endif
