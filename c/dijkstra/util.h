#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>

void *safeMalloc(size_t bytes);
void *safeCalloc(size_t count, size_t size);
void *safeRealloc(void *p, size_t bytes);
int sameWord(const char *a, const char *b);
void formatDist(double d, char *buf, size_t size);

#endif
