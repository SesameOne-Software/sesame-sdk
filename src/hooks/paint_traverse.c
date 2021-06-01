#include "hooks/hooks.h"

__attribute__( ( thiscall ) ) void hooks_paint_traverse( ipanel* this, uint32_t panel, bool force_repaint, bool allow_force ) {
	typedef __attribute__( ( thiscall ) ) void( *hooks_paint_traverse_fn )( ipanel* this, uint32_t panel, bool force_repaint, bool allow_force );

	( ( hooks_paint_traverse_fn )subhook_get_trampoline( hooks_subhooks[ subhook_paint_traverse ] ) )( this, panel, force_repaint, allow_force );
}