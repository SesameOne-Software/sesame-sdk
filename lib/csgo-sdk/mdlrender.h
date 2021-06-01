#ifndef SDK_MDLRENDER_H
#define SDK_MDLRENDER_H

#include "utils.h"

#include "matsys.h"

typedef struct imdlrender imdlrender;

VIRTUAL( imdlrender, void, force_mat, cs_idx_imdlrender_force_mat, ( this, pmat ), mat* pmat );
VIRTUAL( imdlrender, bool, is_forced_mat_override, cs_idx_imdlrender_is_forced_mat_override, ( this ) );

#endif // !SDK_MDLRENDER_H