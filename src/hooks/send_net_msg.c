#include "hooks/hooks.h"

__attribute__( ( thiscall ) ) bool hooks_send_net_msg( void* this, void* msg, bool force_reliable, bool voice ) {
	typedef __attribute__( ( thiscall ) ) bool( *hooks_send_net_msg_fn )( void* this, void* msg, bool force_reliable, bool voice );

	if ( ( ( __attribute__( ( thiscall ) ) int( * )( void* ) )utils_vfunc( msg, 8 ) )( msg ) == 9 )
		voice = true;

	return ( ( hooks_send_net_msg_fn )subhook_get_trampoline( hooks_subhooks[ subhook_send_net_msg ] ) )( this, msg, force_reliable, voice );
}