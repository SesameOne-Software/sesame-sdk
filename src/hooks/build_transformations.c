#include "hooks/hooks.h"

__attribute__( ( thiscall ) ) void hooks_build_transformations( player* this, studiohdr* hdr, vec3* pos, void* quaternion, mat3x4a* matrix, uint32_t mask, bool* computed ) {
	typedef __attribute__( ( thiscall ) ) void( *hooks_build_transformations_fn )( player* this, studiohdr* hdr, vec3* pos, void* quaternion, mat3x4a* matrix, uint32_t mask, bool* computed );

	if ( !this || !entity_is_player( ( entity* )this ) )
		return ( ( hooks_build_transformations_fn )subhook_get_trampoline( hooks_subhooks[ subhook_build_transformations ] ) )( this, hdr, pos, quaternion, matrix, mask, computed );

	const void* backup_jiggle_bones = *player_jiggle_bones( this );
	*player_jiggle_bones( this ) = NULL;

	( ( hooks_build_transformations_fn )subhook_get_trampoline( hooks_subhooks[ subhook_build_transformations ] ) )( this, hdr, pos, quaternion, matrix, mask, computed );

	/* make sure to restore... causes memory leaks if we dont? */
	*player_jiggle_bones( this ) = backup_jiggle_bones;
}
