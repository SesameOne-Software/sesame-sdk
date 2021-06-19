#include "hooks/hooks.h"

#include "game/animations.h" 

/* prevent the game from skipping bone setups between frames */
__attribute__( ( thiscall ) ) void hooks_animstate_update( animstate* this /*, float pitch<xmm1>, float yaw<xmm2> */, bool force_update ) {
	typedef __attribute__( ( thiscall ) ) bool( *hooks_animstate_update_fn )( animstate* this /*, float pitch<xmm1>, float yaw<xmm2> */, bool force_update );

    volatile float pitch, yaw;
    asm volatile(
		"movss %0, xmm1;"
		"movss %1, xmm2;"
		: "=g"( pitch ), "=g"( yaw )
		:: "xmm1", "xmm2"
		);

    utils_print_console( &( uint8_t[ ] ) { 0, 255, 0, 255 }, sdsnew( "Called CCSGOPlayerAnimState::Update!\n" ) );

    /*
    server_animstate* server_animstate = &game_animation_list[ *entity_idx( ( entity* ) this->player ) ].animstate;

    server_animstate_update( server_animstate, &( vec3 ) { pitch, yaw, 0.0f }, force_update );  

    *this = server_animstate->base;
	*/

    asm volatile(
		"movss xmm1, %0;"
		"movss xmm2, %1;"
		: "=g"( pitch ), "=g"( yaw )
		:: "xmm1", "xmm2"
		);

    ( ( hooks_animstate_update_fn ) subhook_get_trampoline( hooks_subhooks[ subhook_animstate_update ] ) )( this, force_update );
}
