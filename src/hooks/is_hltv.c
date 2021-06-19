#include "hooks/hooks.h"

__attribute__( ( thiscall ) ) bool hooks_is_hltv( iengine* this ) {
	typedef __attribute__( ( thiscall ) ) bool( *hooks_is_hltv_fn )( iengine* this );

    /* stop PVS */
	if ( __builtin_return_address( 0 ) == cs_offsets.reevauluate_anim_lod_ret )
		return true;

	return ( ( hooks_is_hltv_fn )subhook_get_trampoline( hooks_subhooks[ subhook_is_hltv ] ) )( this );
}
