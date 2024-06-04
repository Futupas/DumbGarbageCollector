#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "dumb_garbage_collector.h"

// Usage example

static void assert_pointer_is_not_null(struct dumb_gc* dgc, void* ptr) {
    if (ptr != NULL) return;

    fprintf(stderr, "Error [re]allocating memory\n");
    dgc_free_all(dgc); // Don't forget to free the resources!
    exit(EXIT_FAILURE);
}

static void assert_equals(struct dumb_gc* dgc, int expected, int actual) {
    if (expected != actual) {
        fprintf(stderr, "Assertion error. Expected %d, got %d.\n", expected, actual);
        dgc_free_all(dgc); // Don't forget to free the resources!
        exit(EXIT_FAILURE);
    }
}

int main(void)
{
    // Init DGC
    struct dumb_gc* dgc = dgc_init();

    // CAlloc example
    int32_t* ints1 = dgc_calloc(dgc, sizeof(int32_t), 100);
    assert_pointer_is_not_null(dgc, ints1);
    printf("CAlloc: %d, %d\n", ints1[4], ints1[20]);

    // MAlloc example
    int32_t* ints2 = dgc_malloc(dgc, sizeof(int32_t) * 100);
    assert_pointer_is_not_null(dgc, ints2);
    ints2[4] = 6;
    ints2[20] = 9;
    printf("MAlloc: %d, %d\n", ints2[4], ints2[20]);

    // Free example
    ints2 = dgc_free(dgc, ints2);
    printf("Ints2 (freed): %p\n", (void*)ints2);

    // ReAlloc example
    ints1 = dgc_realloc(dgc, ints1, sizeof(int32_t) * 200);
    assert_pointer_is_not_null(dgc, ints1);
    ints1[104] = 6;
    ints1[120] = 9;
    printf("ReAlloc: %d, %d\n", ints1[104], ints1[120]);

    {
        int res = dgc_add(dgc, ints1);
        assert_equals(dgc, 2, res);
        printf("Add existing pointer: OK\n");
    }
    {
        int* ptr = malloc(5); // it can be e.g. `strdup` or something that allocates memory inside
        assert_pointer_is_not_null(dgc, ptr);
        int res = dgc_add(dgc, ptr);
        assert_equals(dgc, 1, res);
        printf("Add non-existing pointer: OK\n");
    }
    {
        int res = dgc_add(dgc, NULL);
        assert_equals(dgc, 0, res);
        printf("Add NULL pointer: OK\n");
    }
    {
        int* ptr = dgc_realloc(dgc, NULL, 5);
        assert_pointer_is_not_null(dgc, ptr);
        printf("Realloc NULL-pointer: OK\n");
    }

    // Final free
    dgc_free_all(dgc);

    printf("All resources are freed\n");

    return EXIT_SUCCESS;
}
