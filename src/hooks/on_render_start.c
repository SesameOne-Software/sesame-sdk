#include "hooks/hooks.h"

__attribute__( ( cdecl ) ) void hooks_on_render_start( void ) {
	typedef __attribute__( ( cdecl ) ) void( *hooks_on_render_start_fn )( void );

	( ( hooks_on_render_start_fn ) subhook_get_trampoline( hooks_subhooks[ subhook_on_render_start ] ) )( );
}
