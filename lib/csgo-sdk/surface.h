#ifndef SDK_SURFACE_H
#define SDK_SURFACE_H

#include "include/utils.h"

typedef struct isurface isurface;

VIRTUAL ( isurface, void, unlock_cursor, 66, ( this ) );
VIRTUAL ( isurface, void, lock_cursor, 67, ( this ) );
VIRTUAL ( isurface, void, get_cursor_pos, 100, ( this, x, y ), int* x, int* y );

typedef struct ipanel ipanel;

VIRTUAL ( ipanel, const char*, get_name, 36, ( this, panel_id ), uint32_t panel_id );

#endif // !SDK_SURFACE_H