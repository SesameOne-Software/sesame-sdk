#ifndef SDK_ENTLIST_H
#define SDK_ENTLIST_H

#include "entity.h"

#include "include/utils.h"

typedef struct ientlist ientlist;

VIRTUAL ( ientlist, entity*, get_entity, cs_idx_ientlist_get_entity, ( idx ), int idx );
VIRTUAL ( ientlist, entity*, get_entity_from_handle, cs_idx_ientlist_get_entity_from_handle, (  handle ), ehandle handle );
VIRTUAL ( ientlist, int, get_highest_idx, cs_idx_ientlist_get_highest_idx, ( ) );

#endif // !SDK_ENTLIST_H