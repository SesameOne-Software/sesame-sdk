#include "include/hooks/hooks.h"

void __stdcall hooks_cl_fire_events ( ) {
	typedef void ( __stdcall* hooks_cl_fire_events_fn )( );

	for ( auto ei = iclientstate_get_events( cs_iclientstate ); ei; ei = *( void** )( ( uintptr_t ) ei + 56 ) )
		*( float* ) ( ( uintptr_t ) ei + 4 ) = 0.0f;

	( ( hooks_cl_fire_events_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_cl_fire_events ] ) )( );
}