#ifndef SDK_MEMALLOC_H
#define SDK_MEMALLOC_H

#include "include/utils.h"

typedef struct imemalloc imemalloc;

VIRTUAL ( imemalloc, void*, alloc, cs_idx_imemalloc_alloc, (  size ), size_t size );
VIRTUAL ( imemalloc, void*, realloc, cs_idx_imemalloc_realloc, (  mem, size ), void* mem, size_t size );
VIRTUAL ( imemalloc, void, free, cs_idx_imemalloc_free, (  mem ), void* mem );

#endif // !SDK_MEMALLOC_H