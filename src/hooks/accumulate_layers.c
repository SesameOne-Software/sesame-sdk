#include "hooks/hooks.h"

__attribute__( ( thiscall ) ) void hooks_accumulate_layers( player* this, void* setup, vec3* pos, void* q, float time ) {
	typedef __attribute__( ( thiscall ) ) void( *hooks_accumulate_layers_fn )( player* this, void* setup, vec3* pos, void* q, float time );

	if ( !this || !entity_is_player( ( entity* )this ) || !player_is_alive( this ) || !*player_animlayers( this ) )
		return ( ( hooks_accumulate_layers_fn )subhook_get_trampoline( hooks_subhooks[ subhook_accumulate_layers ] ) )( this, setup, pos, q, time );

	for ( auto i = 0; i < *player_num_animlayers( this ); i++ ) {
		const animlayer* layer = *player_animlayers( this ) + i;

		if ( layer->weight > 0.0f && layer->order >= 0 && layer->order < *player_num_animlayers( this ) )
			( ( __attribute__( ( thiscall ) ) void( * )( void*, vec3*, void*, int, float, float, float, void* ) )cs_offsets.accumulate_pose_fn ) ( *( void** )setup, pos, q, layer->seq, layer->cycle, layer->weight, time, *player_iks( this ) );
	}
}