#include "hooks/hooks.h"

__attribute__( ( thiscall ) ) void hooks_frame_stage_notify( iclient* this, int stage ) {
	typedef __attribute__( ( thiscall ) ) void( *hooks_frame_stage_notify_fn )( iclient* this, int stage );

	/**/

	( ( hooks_frame_stage_notify_fn )subhook_get_trampoline( hooks_subhooks[ subhook_frame_stage_notify ] ) )( this, stage );

	/**/
}