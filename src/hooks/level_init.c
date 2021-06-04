#include "hooks/hooks.h"

#include "features/esp.h"

__attribute__( ( thiscall ) ) void hooks_level_init( void* this, const char* map_name ) {
	typedef __attribute__( ( thiscall ) ) void( *hooks_level_init_fn )( void* this );

    features_esp_level_init();
    utils_print_console( &( uint8_t[ ] ) { 0, 255, 0, 255 }, sdsnew( "Called LevelInit!\n" ) );

	return ( ( hooks_level_init_fn )subhook_get_trampoline( hooks_subhooks[ subhook_level_init ] ) )( this );
}
