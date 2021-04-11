#ifndef SDK_MDLCACHE_H
#define SDK_MDLCACHE_H

#include "include/utils.h"

typedef struct imdlcache imdlcache;

VIRTUAL ( imdlcache, void, begin_lock, 33, ( ) );
VIRTUAL ( imdlcache, void, end_lock, 34, (  ) );

#define MDLCACHE_CRITICAL_SECTION_START imdlcache_begin_lock(cs_imdlcache)
#define MDLCACHE_CRITICAL_SECTION_END imdlcache_end_lock(cs_imdlcache)

#endif // !SDK_MDLCACHE_H