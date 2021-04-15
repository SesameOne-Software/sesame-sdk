#include "include/hooks/hooks.h"

void __fastcall hooks_build_transformations ( REG, studiohdr* hdr, vec3* pos, void* quaternion, mat3x4a* matrix, uint32_t mask, bool* computed ) {
	typedef void ( __fastcall* hooks_build_transformations_fn )( REG, studiohdr* hdr, vec3* pos, void* quaternion, mat3x4a* matrix, uint32_t mask, bool* computed );
	
	const auto ent = ( player* ) ecx;

	if ( !ent || !entity_is_player ( ( entity* ) ent ) )
		return ( ( hooks_build_transformations_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_build_transformations ] ) )( REG_OUT, hdr, pos, quaternion, matrix, mask, computed );
	
	const auto backup_jiggle_bones = *player_jiggle_bones ( ent );
	*player_jiggle_bones ( ent ) = 0;

	( ( hooks_build_transformations_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_build_transformations ] ) )( REG_OUT, hdr, pos, quaternion, matrix, mask, computed );
	
	*player_jiggle_bones ( ent ) = backup_jiggle_bones;
}