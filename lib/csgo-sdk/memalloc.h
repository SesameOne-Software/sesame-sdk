#ifndef SDK_MEMALLOC_H
#define SDK_MEMALLOC_H

#include "include/utils.h"

typedef struct imemalloc imemalloc;

VIRTUAL ( imemalloc, void*, alloc, 1, ( this, size ), size_t size );
VIRTUAL ( imemalloc, void*, realloc, 2, ( this, mem, size ), void* mem, size_t size );
VIRTUAL ( imemalloc, void, free, 3, ( this, mem ), void* mem );

#endif // !SDK_MEMALLOC_H