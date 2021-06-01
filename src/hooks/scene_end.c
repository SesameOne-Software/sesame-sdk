#include "hooks/hooks.h"

__attribute__( ( thiscall ) ) void hooks_scene_end( irenderview* this ) {
	typedef __attribute__( ( thiscall ) ) void( *hooks_scene_end_fn )( irenderview* this );

	( ( hooks_scene_end_fn )subhook_get_trampoline( hooks_subhooks[ subhook_scene_end ] ) )( this );
}