#ifndef SDK_MDLCACHE_H
#define SDK_MDLCACHE_H

#include "include/utils.h"

typedef struct imdlcache imdlcache;

VIRTUAL ( imdlcache, void, begin_lock, 33, ( this ) );
VIRTUAL ( imdlcache, void, end_lock, 34, ( this ) );

#define MDLCACHE_CRITICAL_SECTION_START imdlcache_begin_lock(cs_mdlcache)
#define MDLCACHE_CRITICAL_SECTION_END imdlcache_end_lock(cs_mdlcache)

#endif // !SDK_MDLCACHE_H