#include "hooks/hooks.h"

__attribute__( ( thiscall ) ) void hooks_draw_model( imdlrender* this, void* results, void* info, const mat3x4* matrix, float* weights, float* delayed_weights, const vec3* origin, int flags ) {
	typedef __attribute__( ( thiscall ) ) void( *hooks_draw_model_fn )( imdlrender* this, void* results, void* info, const mat3x4* matrix, float* weights, float* delayed_weights, const vec3* origin, int flags );

	renderable* renderable = *( void** )( ( uintptr_t )info + 0x18 );
	player* entity = ( player* )( ( uintptr_t )renderable - 0x4 );



	( ( hooks_draw_model_fn )subhook_get_trampoline( hooks_subhooks[ subhook_draw_model ] ) )( this, results, info, matrix, weights, delayed_weights, origin, flags );
}