#include "hooks/hooks.h"

__attribute__( ( thiscall ) ) void hooks_override_view( void* this, void* view_setup ) {
	typedef __attribute__( ( thiscall ) ) void( *hooks_override_view_fn )( void* this, void* view_setup );

	( ( hooks_override_view_fn )subhook_get_trampoline( hooks_subhooks[ subhook_override_view ] ) )( this, view_setup );
}