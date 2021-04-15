#include "include/hooks/hooks.h"

bool __fastcall hooks_setup_bones ( REG, mat3x4* out, int max_bones, uint32_t mask, float time ) {
	typedef bool ( __fastcall* hooks_setup_bones_fn )( REG, mat3x4* out, int max_bones, uint32_t mask, float time );

	return ( ( hooks_setup_bones_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_setup_bones ] ) )( REG_OUT, out, max_bones, mask, time );
}