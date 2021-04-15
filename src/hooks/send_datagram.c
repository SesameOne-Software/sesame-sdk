#include "include/hooks/hooks.h"

int __fastcall hooks_send_datagram ( REG, void* datagram ) {
	typedef int ( __fastcall* hooks_send_datagram_fn )( REG, void* datagram );

	nc* nc = iclientstate_get_nc ( cs_iclientstate );
	
	if ( !nc )
		return ( ( hooks_send_datagram_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_send_datagram ] ) )( REG_OUT, datagram );

	return ( ( hooks_send_datagram_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_send_datagram ] ) )( REG_OUT, datagram );
}