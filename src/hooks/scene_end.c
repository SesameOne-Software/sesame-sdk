#include "include/hooks/hooks.h"

void __fastcall hooks_scene_end ( REG ) {
	typedef void ( __fastcall* hooks_scene_end_fn )( REG );

	( ( hooks_scene_end_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_scene_end ] ) )( REG_OUT );
}