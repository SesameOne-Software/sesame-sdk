#include "include/hooks/hooks.h"

void __fastcall hooks_draw_model ( REG, void* results, void* info, const mat3x4* matrix, float* weights, float* delayed_weights, const vec3* origin, int flags ) {
	typedef void ( __fastcall* hooks_draw_model_fn )( REG, void* results, void* info, const mat3x4* matrix, float* weights, float* delayed_weights, const vec3* origin, int flags );
	
	renderable* renderable = *( void** )( ( uintptr_t ) info + 0x18 );
	player* entity = ( player* )( (uintptr_t) renderable - 0x4 );
	


	( ( hooks_draw_model_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_draw_model ] ) )( REG_OUT, results, info, matrix, weights, delayed_weights, origin, flags );
}