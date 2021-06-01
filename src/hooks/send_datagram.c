#include "hooks/hooks.h"

__attribute__( ( thiscall ) ) int hooks_send_datagram( void* this, void* datagram ) {
	typedef __attribute__( ( thiscall ) ) int( *hooks_send_datagram_fn )( void* this, void* datagram );

	nc* nc = iclientstate_get_nc( cs_iclientstate );

	if ( !nc )
		return ( ( hooks_send_datagram_fn )subhook_get_trampoline( hooks_subhooks[ subhook_send_datagram ] ) )( this, datagram );

	return ( ( hooks_send_datagram_fn )subhook_get_trampoline( hooks_subhooks[ subhook_send_datagram ] ) )( this, datagram );
}