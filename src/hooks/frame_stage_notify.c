#include "include/hooks/hooks.h"

void __fastcall hooks_frame_stage_notify ( REG, int stage ) {
	typedef void ( __fastcall* hooks_frame_stage_notify_fn )( REG, int stage );

	/**/

	( ( hooks_frame_stage_notify_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_frame_stage_notify ] ) )( REG_OUT, stage );

	/**/
}