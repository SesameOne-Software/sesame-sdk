#include "include/hooks/hooks.h"

void __fastcall hooks_accumulate_layers ( REG, void* setup, vec3* pos, void* q, float time ) {
	typedef void ( __fastcall* hooks_accumulate_layers_fn )( REG, void* setup, vec3* pos, void* q, float time );

	const player* ent = ( player* ) ecx;

	if ( !ent || !entity_is_player((entity*) ent ) || !player_is_alive( ent ) || !*player_animlayers(ent) )
		return ( ( hooks_accumulate_layers_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_accumulate_layers ] ) )( REG_OUT, setup, pos, q, time );

	for ( auto i = 0; i < *player_num_animlayers ( ent ); i++ ) {
		const animlayer* layer = *player_animlayers ( ent ) + i;
		
		if ( layer->weight > 0.0f && layer->order >= 0 && layer->order < *player_num_animlayers ( ent ) )
			(( void ( __fastcall* )( void*, void*, vec3*, void*, int, float, float, float, void* ) )cs_offsets.accumulate_pose_fn) ( *( void** ) setup, NULL, pos, q, layer->sequence, layer->cycle, layer->weight, time, *player_iks(ent) );
	}
}