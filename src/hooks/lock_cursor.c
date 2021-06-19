#include "hooks/hooks.h"

#include "gui/menu.h"

__attribute__( ( thiscall ) ) void hooks_lock_cursor( isurface* this ) {
	typedef __attribute__( ( thiscall ) ) void( *hooks_lock_cursor_fn )( isurface* this );

	if ( menu_is_open( ) )
		return isurface_unlock_cursor( this );

	( ( hooks_lock_cursor_fn ) subhook_get_trampoline( hooks_subhooks[ subhook_lock_cursor ] ) )( this );
}
