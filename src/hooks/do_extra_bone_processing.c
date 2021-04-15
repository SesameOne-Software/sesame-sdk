#include "include/hooks/hooks.h"

void __fastcall hooks_do_extra_bone_processing ( REG, int a2, int a3, int a4, int a5, int a6, int a7 ) {
	typedef void ( __fastcall* hooks_do_extra_bone_processing_fn )( REG, int a2, int a3, int a4, int a5, int a6, int a7 );

	animstate* state = player_animstate( ( player* ) ecx );
	
	if ( !state )
		return ( ( hooks_do_extra_bone_processing_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_do_extra_bone_processing ] ) )( REG_OUT, a2, a3, a4, a5, a6, a7 );

	const auto backup_on_ground = state->on_ground;
	state->on_ground = false;
	( ( hooks_do_extra_bone_processing_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_do_extra_bone_processing ] ) )( REG_OUT, a2, a3, a4, a5, a6, a7 );
	state->on_ground = backup_on_ground;
}