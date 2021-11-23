#include "hooks/hooks.h"

__attribute__( ( thiscall ) ) bool hooks_in_prediction( ipred* this ) {
	typedef __attribute__( ( thiscall ) ) bool ( *hooks_in_prediction_fn )( ipred* this );

    /*
	if ( __builtin_return_address( 0 ) == cs_offsets.maintain_sequence_transitions_ret )
		return true;
    */

	return ( ( hooks_in_prediction_fn )subhook_get_trampoline( hooks_subhooks[ subhook_in_prediction ] ) )( this );
}
