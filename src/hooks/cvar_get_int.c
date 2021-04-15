#include "include/hooks/hooks.h"

#include <intrin.h>

int __fastcall hooks_cvar_get_int ( REG ) {
	typedef int ( __fastcall* hooks_cvar_get_int_fn )( REG );
	
	if ( !ecx || !iengine_is_in_game ( cs_iengine ) || !iengine_is_connected ( cs_iengine ) )
		return ( ( hooks_cvar_get_int_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_cvar_get_int ] ) )( REG_OUT );

	if ( _ReturnAddress ( ) == cs_offsets.cam_think_ret )
		return true;

	return ( ( hooks_cvar_get_int_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_cvar_get_int ] ) )( REG_OUT );
}