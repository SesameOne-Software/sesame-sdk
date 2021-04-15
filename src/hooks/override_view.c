#include "include/hooks/hooks.h"

void __fastcall hooks_override_view ( REG, void* view_setup ) {
	typedef void ( __fastcall* hooks_override_view_fn )( REG, void* view_setup );

	( ( hooks_override_view_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_override_view ] ) )( REG_OUT, view_setup );
}