#include "include/hooks/hooks.h"

#include "include/gui/menu.h"

void __fastcall hooks_lock_cursor ( REG ) {
	typedef void ( __fastcall* hooks_lock_cursor_fn )( REG );

	if ( menu_is_open() )
		return isurface_unlock_cursor ( cs_isurface );

	( ( hooks_lock_cursor_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_lock_cursor ] ) )( REG_OUT );
}