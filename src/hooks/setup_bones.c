#include "hooks/hooks.h"

/* force game to use our "fixed" matrix + this is great for our framerate! */
__attribute__( ( thiscall ) ) bool hooks_setup_bones( renderable* this, mat3x4* out, int max_bones, uint32_t mask, float time ) {
	typedef __attribute__( ( thiscall ) ) bool( *hooks_setup_bones_fn )( renderable* this, mat3x4* out, int max_bones, uint32_t mask, float time );

	return ( ( hooks_setup_bones_fn )subhook_get_trampoline( hooks_subhooks[ subhook_setup_bones ] ) )( this, out, max_bones, mask, time );
}
