#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "dumb_garbage_collector.h"

// Usage example

int main(void)
{
    // Init DGC
    struct dumb_gc* dgc = dgc_init();

    // CAlloc example
    int32_t* ints1 = dgc_calloc(dgc, sizeof(int32_t), 100);
    printf("CAlloc: %d, %d\n", ints1[4], ints1[20]);

    // MAlloc example
    int32_t* ints2 = dgc_malloc(dgc, sizeof(int32_t) * 100);
    ints2[4] = 6;
    ints2[20] = 9;
    printf("MAlloc: %d, %d\n", ints2[4], ints2[20]);

    // Free example
    ints2 = dgc_free(dgc, ints2);
    printf("Ints2 (freed): %p\n", (void*)ints2);

    // ReAlloc example
    ints1 = dgc_realloc(dgc, ints1, sizeof(int32_t) * 200);
    if (ints1 == NULL) {
        fprintf(stderr, "Error reallocating memory\n");
        // Don't forget to free the resources!
        dgc_free_all(dgc);
        exit(EXIT_FAILURE);
    }
    ints1[104] = 6;
    ints1[120] = 9;
    printf("ReAlloc: %d, %d\n", ints1[104], ints1[120]);

    // Final free
    dgc_free_all(dgc);

    printf("All resources are freed\n");

    return EXIT_SUCCESS;
}
