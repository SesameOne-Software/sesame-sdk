#include "include/hooks/hooks.h"

bool __fastcall hooks_send_net_msg ( REG, void* msg, bool force_reliable, bool voice ) {
	typedef bool ( __fastcall* hooks_send_net_msg_fn )( REG, void* msg, bool force_reliable, bool voice );

	if ( ( ( int ( __fastcall* )( REG ) )utils_vfunc ( msg, 8 ) )( msg, NULL ) == 9 )
		voice = true;

	return ( ( hooks_send_net_msg_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_send_net_msg ] ) )( REG_OUT, msg, force_reliable, voice );
}