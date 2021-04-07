#ifndef CREATEMOVE_H
#define CREATEMOVE_H

#include "../utils.h"

static bool( __fastcall* hooks_old_createmove )( REG, float, ucmd_t* );
static bool __fastcall hooks_createmove ( REG, float sampletime, ucmd_t* ucmd ) {
	const bool ret = hooks_old_createmove ( REG_OUT, sampletime, ucmd );

	if ( !ucmd || !ucmd->m_cmdnum )
		return ret;

	/* run input-related code here */
}

#endif // !CREATEMOVE_H