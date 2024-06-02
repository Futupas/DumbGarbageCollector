#include "dumb_garbage_collector.h"

#include <stdlib.h>

struct dumb_gc {
    struct dgc_node* first_node;
};

struct dgc_node {
    void* ptr;
    struct dgc_node* next_node;
};



struct dumb_gc* dgc_init() {
    struct dumb_gc* dgc = calloc(1, sizeof(struct dumb_gc));
    return dgc;
}

// returns 1 if successfully added a node
static int add_node(struct dumb_gc* dgc, void* ptr) {
    if (ptr == NULL) {
        return 0;
    }
    struct dgc_node* node = calloc(1, sizeof(struct dgc_node));
    if (node == NULL) {
        return 0;
    }
    node->ptr = ptr;
    node->next_node = dgc->first_node;
    dgc->first_node = node;
    return 1;
}

void* dgc_calloc(struct dumb_gc* dgc, size_t num, size_t size) {
    void* ptr = calloc(num, size);
    if (ptr != NULL) {
        if (!add_node(dgc, ptr)) {
            free(ptr);
            return NULL;
        }
    }
    return ptr;
}

void* dgc_malloc(struct dumb_gc* dgc, size_t size) {
    void* ptr = malloc(size);
    if (ptr != NULL) {
        if (!add_node(dgc, ptr)) {
            free(ptr);
            return NULL;
        }
    }
    return ptr;
}


void* dgc_realloc(struct dumb_gc* dgc, void *ptr, size_t new_size) {
    void* new_ptr = realloc(ptr, new_size);
    if (new_ptr == NULL) return NULL; // And do nothing as we don't wanna lose our pointer

    // Find the node corresponding to the old pointer
    struct dgc_node* node = dgc->first_node;
    while (node != NULL) {
        if (node->ptr == ptr) {
            node->ptr = new_ptr; // Update the pointer in the node
            break;
        }
        node = node->next_node;
    }

    // If the old pointer is not found in the list, add the new pointer as a new node
    if (node == NULL) {
        if (!add_node(dgc, ptr)) {
            free(ptr);
            return NULL;
        }
    }

    return new_ptr;
}

static int pointer_exists(struct dumb_gc* dgc, void *ptr) {
    struct dgc_node* curr = dgc->first_node;
    while (curr != NULL) {
        if (curr->ptr == ptr) {
            return 1;
        }
        curr = curr->next_node;
    }
    return 0;
}

int dgc_add(struct dumb_gc* dgc, void *ptr) {
    if (pointer_exists(dgc, ptr))  {
        return 2;
    }
    return add_node(dgc, ptr);
}


void* dgc_free(struct dumb_gc* dgc, void* src) {
    if (src == NULL) return NULL;

    struct dgc_node* prev = NULL;
    struct dgc_node* curr = dgc->first_node;
    while (curr != NULL) {
        if (curr->ptr == src) {
            if (prev == NULL) {
                dgc->first_node = curr->next_node;
            } else {
                prev->next_node = curr->next_node;
            }
            free(curr->ptr);
            free(curr);
            return NULL;
        }
        prev = curr;
        curr = curr->next_node;
    }
    return NULL;
}

void dgc_free_all(struct dumb_gc* dgc) {
    if (dgc == NULL) return;

    struct dgc_node* node = dgc->first_node;
    while (node != NULL) {
        struct dgc_node* next_node = node->next_node;
        free(node->ptr);
        free(node);
        node = next_node;
    }

    free(dgc);
}

