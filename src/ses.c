#include "ses.h"
#include "utils.h"
#include "hooks/hooks.h"
#include "gui/config.h"

#include "features/esp.h"
#include "game/animations.h"

#include <windows.h>
#include <process.h>
#include <shlobj.h>
#include <pthread.h>

static pthread_t ses_thread_id; 

ses_ctx_s ses_ctx = { 0 };

static inline int ses_fail( HMODULE mod, sds error_msg ) {
    utils_print_console( &( uint8_t[ ] ) { 255, 0, 0, 255 }, error_msg );
    FreeLibraryAndExitThread( mod, EXIT_FAILURE );
}

static void ses_init( HMODULE mod ) {
    /* wait for game to be fully loaded */
    while ( !GetModuleHandleA( "serverbrowser.dll" ) )
        utils_sleep( 100 );

    utils_print_console( &( uint8_t[ ] ) { 255, 100, 255, 255 }, sdsnew( "Initializing software.\n" ) );

    /* remove breakpoints that are places in the game so we can debug properly */
#ifdef _DEBUG
    hooks_patch_bp( );
#endif

    sds errors_out = NULL;
    
    /* create sesame folders */ {
        char ses_dir[ MAX_PATH ] = { 0 };

        if (!SUCCEEDED( SHGetFolderPathA( NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, ses_dir ) ) ) ses_fail( mod, sdsnew( "Failed to find documents folder.\n" ) );
        else utils_print_console( &( uint8_t[ ] ) { 0, 255, 0, 255 }, sdsnew( "Found documents folder.\n" ) );

        ses_ctx.ses_dir = sdscat( sdsnew( ses_dir ), "\\sesame");

        if ( CreateDirectoryA( ses_ctx.ses_dir, NULL )
            || GetLastError( ) == ERROR_ALREADY_EXISTS )
            utils_print_console( &( uint8_t[ ] ) { 0, 255, 0, 255 }, sdsnew( "Created sesame folder.\n" ) );
        else
            ses_fail(mod,sdsnew("Failed to create sesame folder.\n" ));

        ses_ctx.ses_dir = sdscat( ses_ctx.ses_dir, "\\configs" );

        if ( CreateDirectoryA( ses_ctx.ses_dir, NULL )
            || GetLastError( ) == ERROR_ALREADY_EXISTS )
            utils_print_console( &( uint8_t[ ] ) { 0, 255, 0, 255 }, sdsnew( "Created sesame configs folder.\n" ) );
        else
            ses_fail( mod, sdsnew( "Failed to create sesame configs folder.\n" ) );

        sdsfree( ses_ctx.ses_dir );
        ses_ctx.ses_dir = sdscat( sdsnew( ses_dir ), "\\sesame" );
    }

    /* initialize other stuff */
    if ( !cs_init( &errors_out ) ) ses_fail( mod, errors_out ? errors_out : sdsnew( "Failed to find offsets.\n" ) );
    else iengine_execute_cmd( cs_iengine, "clear" ), utils_print_console( &( uint8_t[ ] ) { 0, 255, 0, 255 }, sdsnew( "Found interfaces.\n" ) );
    if ( !netvars_init( ) ) ses_fail( mod, sdsnew( "Failed to dump netvars.\n" ) );
    else utils_print_console( &( uint8_t[ ] ) { 0, 255, 0, 255 }, sdsnew( "Dumped netvars.\n" ) );
    if ( !ses_cfg_new( &ses_cfg ) ) ses_fail( mod, sdsnew( "Failed to setup config system.\n" ) );
    else utils_print_console( &( uint8_t[ ] ) { 0, 255, 0, 255 }, sdsnew( "Setup config system.\n" ) );

    menu_init( );
    utils_print_console( &( uint8_t[ ] ) { 0, 255, 0, 255 }, sdsnew( "Initialized GUI.\n" ) );

    if ( !hooks_init( ) ) ses_fail( mod, sdsnew( "Failed to install hooks.\n" ) );
    else utils_print_console( &( uint8_t[ ] ) { 0, 255, 0, 255 }, sdsnew( "Installed hooks.\n" ) );
    if ( !features_esp_init( ) ) ses_fail( mod, sdsnew( "Failed to initialize ESP system.\n" ) );
    else utils_print_console( &( uint8_t[ ] ) { 0, 255, 0, 255 }, sdsnew( "Initialized ESP system.\n" ) );
    if ( !game_animation_init( ) ) ses_fail( mod, sdsnew( "Failed to initialize animation system.\n" ) );
    else utils_print_console( &( uint8_t[ ] ) { 0, 255, 0, 255 }, sdsnew( "Initialized animation system.\n" ) );

    utils_print_console( &( uint8_t[ ] ) { 255, 100, 255, 255 }, sdsnew( "Startup was successful!\n" ) );

    /* wait for self-destruct key */
    while ( !ses_ctx.shutdown )
        utils_sleep( 100 );

    iengine_execute_cmd( cs_iengine, "clear" );
    utils_print_console( &( uint8_t[ ] ) { 255, 255, 0, 255 }, sdsnew( "Unloading...\n" ) );

    if ( !hooks_free( ) ) ses_fail( mod, sdsnew( "Failed to uninstall hooks.\n" ) );
    else utils_print_console( &( uint8_t[ ] ) { 0, 255, 0, 255 }, sdsnew( "Uninstalled hooks.\n" ) );

    /* wait for hooks to finish before we free anything they might use */
    utils_sleep( 100 );

    if ( !netvars_free( ) ) ses_fail( mod, sdsnew( "Failed to free netvar map memory allocations.\n" ) );
    else utils_print_console( &( uint8_t[ ] ) { 0, 255, 0, 255 }, sdsnew( "Freed netvar map allocations.\n" ) );
    if ( !cs_free( ) ) ses_fail( mod, sdsnew( "Failed to free SDK memory allocations.\n" ) );
    else utils_print_console( &( uint8_t[ ] ) { 0, 255, 0, 255 }, sdsnew( "Freed SDK allocations.\n" ) );
    if ( !ses_cfg_free( &ses_cfg ) ) ses_fail( mod, sdsnew( "Failed to free config memory allocations.\n" ) );
    else utils_print_console( &( uint8_t[ ] ) { 0, 255, 0, 255 }, sdsnew( "Freed config allocations.\n" ) );
    if ( !features_esp_free( ) ) ses_fail( mod, sdsnew( "Failed to free ESP system allocations.\n" ) );
    else utils_print_console( &( uint8_t[ ] ) { 0, 255, 0, 255 }, sdsnew( "Freed ESP system allocations.\n" ) );
    if ( !game_animation_free( ) ) ses_fail( mod, sdsnew( "Failed to free animation system allocations.\n" ) );
    else utils_print_console( &( uint8_t[ ] ) { 0, 255, 0, 255 }, sdsnew( "Freed animation system allocations.\n" ) );

    utils_print_console( &( uint8_t[ ] ) { 255, 100, 255, 255 }, sdsnew( "Unload was successful!\n" ) );

    FreeLibraryAndExitThread( mod, EXIT_SUCCESS );

    return EXIT_SUCCESS;
}

static HMODULE get_current_module( void ) {
    HMODULE mod = NULL;
    GetModuleHandleEx( GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, ( LPCTSTR )get_current_module, &mod );
    return mod;
}

int __attribute__( ( constructor ) ) sesame_sdk_init( void ) {
    HMODULE mod = get_current_module( );

    if ( !pthread_create( &ses_thread_id, NULL, ses_init, ( void* ) mod ) )
        pthread_detach( ses_thread_id );
    else
        utils_print_console( &( uint8_t[ ] ) { 255, 0, 0, 255 }, sdsnew( "Failed to create setup thread.\n" ) );

    return 0;
}

void __attribute__( ( destructor ) ) sesame_sdk_shutdown( void ) {

}
