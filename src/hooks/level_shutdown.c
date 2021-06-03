#include "hooks/hooks.h"

#include "features/esp.h"

__attribute__( ( thiscall ) ) void hooks_level_shutdown( void* this ) {
	typedef __attribute__( ( thiscall ) ) void( *hooks_level_shutdown_fn )( void* this );

    features_esp_level_shutdown();

    return ( ( hooks_level_shutdown_fn )subhook_get_trampoline( hooks_subhooks[ subhook_level_shutdown ] ) )( this );
}
