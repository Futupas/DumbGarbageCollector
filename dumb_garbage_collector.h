#ifndef DUMB_GARBAGE_COLLECTOR_H
#define DUMB_GARBAGE_COLLECTOR_H

// https://github.com/Futupas/DumbGarbageCollector

#include <stdlib.h>

struct dumb_gc;

// Very dumb garbage collector. It just stores all the pointers, and allows to free them all not caring about forgotten pointers.
// If you use this, DON'T USE default calloc/malloc/realloc/free. Use only the given ones.
// In case of error free all of YOUR resources, and call `dgc_free_all`.
// Before exit also don't forget to call `dgc_free_all`.
// Be careful: any method can return NULL, it does not free the memory in case of error.
// This is not very performance-efficient as it uses LinkedList, so if you are going to reallocate/free pointers a lot (calloc/malloc don't affect it), be careful.
// Also it doesn't handle some edge cases (cyclic references etc), it's a very basic implementation.

// License: MIT

// Initializes Dumb Garbage Collector
struct dumb_gc* dgc_init();

// Allocates memory for an array of num objects of size and initializes all bytes in the allocated storage to 0. Returns NULL in case of error
void* dgc_calloc(struct dumb_gc* dgc, size_t num, size_t size);

// Allocates size bytes of uninitialized storage. Returns NULL in case of error
void* dgc_malloc(struct dumb_gc* dgc, size_t size);

// Reallocates the given area of memory. Returns NULL in case of error
void* dgc_realloc(struct dumb_gc* dgc, void *ptr, size_t new_size);

// Deallocates the space. Always returns NULL
void* dgc_free(struct dumb_gc* dgc, void* src);

// Frees all the resources
void dgc_free_all(struct dumb_gc* dgc);

#endif // DUMB_GARBAGE_COLLECTOR_H
