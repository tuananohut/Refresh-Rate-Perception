#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <assert.h>
#include <stdlib.h>

#define DA_INIT_CAP 256

#define DA_APPEND(da, item) \
do { \
    if ((da)->count >= (da)->capacity) { \
        (da)->capacity = (da)->capacity == 0 ? DA_INIT_CAP : (da)->capacity*2; \
        (da)->items = realloc((da)->items, (da)->capacity*sizeof(*(da)->items)); \
        assert((da)->items != NULL); \
    } \
    (da)->items[(da)->count++] = (item); \
} while(0)

#endif
