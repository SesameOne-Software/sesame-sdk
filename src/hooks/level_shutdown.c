#include "hooks/hooks.h"

#include "features/esp.h"

__attribute__( ( thiscall ) ) void hooks_level_shutdown( void* this ) {
	typedef __attribute__( ( thiscall ) ) void( *hooks_level_shutdown_fn )( void* this );

    features_esp_level_shutdown();
    utils_print_console( &( uint8_t[ ] ) { 0, 255, 0, 255 }, sdsnew( "Called LevelShutdown!\n" ) );

    return ( ( hooks_level_shutdown_fn )subhook_get_trampoline( hooks_subhooks[ subhook_level_shutdown ] ) )( this );
}
