#include "hooks/hooks.h"

__attribute__( ( stdcall ) ) void hooks_cl_fire_events( ) {
	typedef __attribute__( ( stdcall ) ) void( *hooks_cl_fire_events_fn )( );

	for ( void* ei = iclientstate_get_events( cs_iclientstate ); ei; ei = *( void** )( ( uintptr_t )ei + 56 ) )
		*( float* )( ( uintptr_t )ei + 4 ) = 0.0f;

	( ( hooks_cl_fire_events_fn )subhook_get_trampoline( hooks_subhooks[ subhook_cl_fire_events ] ) )( );
}