#include "include/ses.h"
#include "include/utils.h"
#include "include/hooks/hooks.h"
#include "include/gui/config.h"

#include <windows.h>
#include <process.h>

static __forceinline int ses_fail ( HMODULE mod, sds error_msg ) {
    utils_print_console ( &( uint8_t [ ] ) { 255, 0, 0, 255 }, error_msg );
    FreeLibraryAndExitThread ( mod, EXIT_FAILURE );
}

static bool ses_should_shutdown = false;

void ses_shutdown( ) {
    ses_should_shutdown = true;
}

static int __stdcall ses_init( HMODULE mod ) {
    /* wait for game to be fully loaded */
    CLEAR_START;
    VM_SHARK_BLACK_START;
    STR_ENCRYPT_START;

    while ( !GetModuleHandleA( "serverbrowser.dll" ) )
        utils_sleep( 100 );

    sds errors_out = NULL;
    if ( !cs_init( &errors_out ) ) ses_fail ( mod, errors_out ? errors_out : sdsnew("Failed to find offsets.\n") );
    if ( !netvars_init( ) ) ses_fail ( mod, sdsnew("Failed to dump netvars.\n") );
    if ( !ses_cfg_new ( &ses_cfg ) ) ses_fail ( mod, sdsnew ( "Failed to create default config.\n" ) );
    if ( !hooks_init( ) ) ses_fail ( mod, sdsnew ( "Failed to install hooks.\n") );

    iengine_execute_cmd ( cs_iengine, "clear" );
    utils_print_console ( &( uint8_t [ ] ) { 0, 255, 0, 255 }, sdsnew("Setup successful!\n") );

    /* wait for self-destruct key */
    while ( !ses_should_shutdown && !GetAsyncKeyState( VK_END ) )
        utils_sleep( 100 );

    iengine_execute_cmd ( cs_iengine, "clear" );
    utils_print_console ( &( uint8_t [ ] ) { 255, 255, 0, 255 }, sdsnew( "Unloading...\n") );

    if ( !hooks_free( ) ) ses_fail ( mod, sdsnew ( "Failed to uninstall hooks.\n") );

    /* wait for hooks to finish before we free anything they might use */
    utils_sleep( 100 );

    if ( !netvars_free( ) ) ses_fail ( mod, sdsnew ( "Failed to free netvar map memory allocations.\n") );
    if ( !cs_free( ) ) ses_fail ( mod, sdsnew ( "Failed to free SDK memory allocations.\n" ));
    if ( !ses_cfg_free ( &ses_cfg ) ) ses_fail ( mod, sdsnew ( "Failed to free config memory allocations.\n" ) );

    iengine_execute_cmd ( cs_iengine, "clear" );
    utils_print_console ( &( uint8_t [ ] ) { 0, 255, 0, 255 }, sdsnew( "Done.\n" ));

    STR_ENCRYPT_END;
    VM_SHARK_BLACK_END;
    CLEAR_END;

    FreeLibraryAndExitThread( mod, EXIT_SUCCESS );

    return EXIT_SUCCESS;
}

/* https://docs.microsoft.com/en-us/windows/win32/dlls/dllmain */
BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved ) {
    if ( fdwReason == DLL_PROCESS_ATTACH ) {
        VM_SHARK_BLACK_START;
        STR_ENCRYPT_START;

        DisableThreadLibraryCalls( hinstDLL );

        HANDLE ret = _beginthreadex( NULL, 0, ses_init, hinstDLL, 0, NULL );

        if ( ret == INVALID_HANDLE_VALUE )
            utils_print_console ( &( uint8_t [ ] ) { 255, 0, 0, 255 }, sdsnew ( "Failed to create setup thread.\n" ) );
        else
            CloseHandle( ret );

        STR_ENCRYPT_END;
        VM_SHARK_BLACK_END;

        return ret != INVALID_HANDLE_VALUE;
    }

    return FALSE;
}