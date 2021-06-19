#include "hooks/hooks.h"

__attribute__( ( thiscall ) ) bool hooks_is_connected( iengine* this ) {
	typedef __attribute__( ( thiscall ) ) bool( *hooks_is_connected_fn )( iengine* this );

    /* unlock inventory */
	if ( __builtin_return_address( 0 ) == cs_offsets.loadout_allowed_ret )
		return false;

	return ( ( hooks_is_connected_fn )subhook_get_trampoline( hooks_subhooks[ subhook_is_connected ] ) )( this );
}
