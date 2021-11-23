#include "hooks/hooks.h"

#include "game/animations.h" 

/* prevent the game from skipping bone setups between frames */
__attribute__( ( thiscall ) ) void hooks_animstate_reset( animstate* this ) {
	typedef __attribute__( ( thiscall ) ) void( *hooks_animstate_reset_fn )( animstate* this );

    //utils_print_console( &( uint8_t[ ] ) { 0, 255, 0, 255 }, sdsnew( "Called CCSGOPlayerAnimState::Reset!\n" ) );

    /* reset game animstate */
	( ( hooks_animstate_reset_fn ) subhook_get_trampoline( hooks_subhooks[ subhook_animstate_reset ] ) )( this );

    /* reset server animstate members */
    server_animstate* server_animstate = &game_animation_list[ *entity_idx( ( entity* ) this->player ) ].animstate;

    server_animstate_reset( server_animstate ); 

    /* set our animstate */
    server_animstate->base = *this;
}
