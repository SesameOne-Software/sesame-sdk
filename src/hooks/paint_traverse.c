#include "include/hooks/hooks.h"

void __fastcall hooks_paint_traverse ( REG, uint32_t panel, bool force_repaint, bool allow_force ) {
	typedef void ( __fastcall* hooks_paint_traverse_fn )( REG, uint32_t panel, bool force_repaint, bool allow_force );

	( ( hooks_paint_traverse_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_paint_traverse ] ) )( REG_OUT, panel, force_repaint, allow_force );
}