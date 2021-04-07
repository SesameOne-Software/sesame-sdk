#ifndef SDK_MDLRENDER_H
#define SDK_MDLRENDER_H

#include "include/utils.h"

#include "matsys.h"

typedef struct imdlrender imdlrender;

VIRTUAL ( imdlrender, void, force_mat, 1, ( this, pmat ), mat* pmat );
VIRTUAL ( imdlrender, bool, is_forced_mat_override, 2, ( this ) );

#endif // !SDK_MDLRENDER_H