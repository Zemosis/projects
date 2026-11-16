#include "util.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// malloc that exits the program if memory runs out
void *safeMalloc(size_t bytes) {
    void *p = malloc(bytes ? bytes : 1);
    if (p == NULL) {
        fprintf(stderr, "error: out of memory (requested %zu bytes)\n", bytes);
        exit(EXIT_FAILURE);
    }
    return p;
}

// calloc that exits the program if memory runs out
void *safeCalloc(size_t count, size_t size) {
    void *p = calloc(count ? count : 1, size ? size : 1);
    if (p == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

// realloc that exits the program if memory runs out
void *safeRealloc(void *p, size_t bytes) {
    void *grown = realloc(p, bytes ? bytes : 1);
    if (grown == NULL) {
        fprintf(stderr, "error: out of memory (requested %zu bytes)\n", bytes);
        exit(EXIT_FAILURE);
    }
    return grown;
}

// Case-insensitive string compare, returns 1 if equal
int sameWord(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) {
            return 0;
        }
        a++;
        b++;
    }
    return *a == *b;
}

// "inf" for unreachable nodes, otherwise the shortest exact form (7, 2.5, ...)
void formatDist(double d, char *buf, size_t size) {
    if (isinf(d)) {
        snprintf(buf, size, "inf");
    } else {
        snprintf(buf, size, "%.10g", d);
    }
}
