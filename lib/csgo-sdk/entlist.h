#ifndef SDK_ENTLIST_H
#define SDK_ENTLIST_H

#include "entity.h"

#include "include/utils.h"

typedef struct ientlist ientlist;

VIRTUAL ( ientlist, entity*, get_entity, 3, ( idx ), int idx );
VIRTUAL ( ientlist, entity*, get_entity_from_handle, 4, (  handle ), ehandle handle );
VIRTUAL ( ientlist, int, get_highest_idx, 6, ( ) );

#endif // !SDK_ENTLIST_H