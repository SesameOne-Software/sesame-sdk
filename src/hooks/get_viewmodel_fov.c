#include "hooks/hooks.h"

/* modify viewmodel fov */
__attribute__( ( thiscall ) ) float hooks_get_viewmodel_fov( void* this ) {
	typedef __attribute__( ( thiscall ) ) float( *hooks_get_viewmodel_fov_fn )( void* this );

	return ( ( hooks_get_viewmodel_fov_fn )subhook_get_trampoline( hooks_subhooks[ subhook_get_viewmodel_fov ] ) )( this );
}
