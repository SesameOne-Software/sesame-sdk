#ifndef SDK_SURFACE_H
#define SDK_SURFACE_H

#include "include/utils.h"

typedef struct isurface isurface;

VIRTUAL ( isurface, void, unlock_cursor, cs_idx_isurface_unlock_cursor, (  ) );
VIRTUAL ( isurface, void, lock_cursor, cs_idx_isurface_lock_cursor, (  ) );
VIRTUAL ( isurface, void, get_cursor_pos, cs_idx_isurface_get_cursor_pos, (  x, y ), int* x, int* y );

typedef struct ipanel ipanel;

VIRTUAL ( ipanel, const char*, get_name, cs_idx_ipanel_get_name, ( panel_id ), uint32_t panel_id );

#endif // !SDK_SURFACE_H