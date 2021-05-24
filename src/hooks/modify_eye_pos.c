#include "include/hooks/hooks.h"
#include "include/ses.h"

void __fastcall hooks_modify_eye_pos ( REG, vec3* pos ) {
	typedef void ( __fastcall* hooks_modify_eye_pos_fn )( REG, vec3* pos );

	animstate* state = ( animstate* ) ecx;

	if ( !state || !ses_ctx.in_move )
		return ( ( hooks_modify_eye_pos_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_modify_eye_pos ] ) )( REG_OUT, pos );

	player* ent = state->player;

	if ( !ent || ent != cs_get_local ( ) || /* TODO: remove this bone cache check after we finish animations and have real matrix saved */ !player_bone_cache ( ent ) )
		return ( ( hooks_modify_eye_pos_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_modify_eye_pos ] ) )( REG_OUT, pos );
	
	if ( state->landing || state->duck_amount || ientlist_get_entity_from_handle ( cs_ientlist, *player_ground_entity_handle ( ent ) ) ) {
		vec3 bone_pos; /*anims::real_matrix [ lookup_bone ( pl, "head_0" ) ].origin ( )*/;
		mat3x4_get_origin ( player_bone_cache ( ent ) + player_lookup_bone ( ent, "head_0" ), &bone_pos );
		
		bone_pos.z += 1.7f;

		if ( pos->z > bone_pos.z ) {
			const float delta = pos->z - bone_pos.z;
			float some_factor = 0.0f;
			float some_offset = ( delta - 4.0f ) / 6.0f;

			if ( some_offset >= 0.0f )
				some_factor = min ( some_offset, 1.0f );

			pos->z += ( ( bone_pos.z - pos->z ) * ( ( ( some_factor * some_factor ) * 3.0f ) - ( ( ( some_factor * some_factor ) * 2.0f ) * some_factor ) ) );
		}
	}
}