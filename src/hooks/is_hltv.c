#include "include/hooks/hooks.h"

#include <intrin.h>

bool __fastcall hooks_is_hltv ( REG ) {
	typedef bool ( __fastcall* hooks_is_hltv_fn )( REG );

	if ( _ReturnAddress ( ) == cs_offsets.accumulate_layers_ret )
		return false;

	return ( ( hooks_is_hltv_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_is_hltv ] ) )( REG_OUT );
}