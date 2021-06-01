#include "hooks/hooks.h"

__attribute__( ( thiscall ) ) int hooks_cvar_get_int( cvar* this ) {
	typedef __attribute__( ( thiscall ) ) int( *hooks_cvar_get_int_fn )( cvar* this );

	if ( !this || !iengine_is_in_game( cs_iengine ) || !iengine_is_connected( cs_iengine ) )
		return ( ( hooks_cvar_get_int_fn )subhook_get_trampoline( hooks_subhooks[ subhook_cvar_get_int ] ) )( this );

	if ( __builtin_return_address( 0 ) == cs_offsets.cam_think_ret )
		return true;

	return ( ( hooks_cvar_get_int_fn )subhook_get_trampoline( hooks_subhooks[ subhook_cvar_get_int ] ) )( this );
}