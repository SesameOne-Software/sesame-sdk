#include "include/hooks/hooks.h"

void __fastcall hooks_perform_flashbang_effect ( REG, void* view_setup ) {
	typedef void ( __fastcall* hooks_perform_flashbang_effect_fn )( REG, void* view_setup );

	( ( hooks_perform_flashbang_effect_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_perform_flashbang_effect ] ) )( REG_OUT, view_setup );
}