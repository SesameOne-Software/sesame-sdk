#include "hooks/hooks.h"
#include "game/animations.h"

__attribute__( ( thiscall ) ) void hooks_player_post_data_update( player* this ) {
	typedef __attribute__( ( thiscall ) ) void( *hooks_player_post_data_update_fn )( player* this );

	( ( hooks_player_post_data_update_fn )subhook_get_trampoline( hooks_subhooks[ subhook_player_post_data_update ] ) )( this );
}
