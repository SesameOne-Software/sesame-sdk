#include "include/hooks/hooks.h"

#include <intrin.h>

bool __fastcall hooks_is_connected ( REG ) {
	typedef bool ( __fastcall* hooks_is_connected_fn )( REG );

	if ( _ReturnAddress ( ) == cs_offsets.loadout_allowed_ret )
		return false;

	return ( ( hooks_is_connected_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_is_connected ] ) )( REG_OUT );
}