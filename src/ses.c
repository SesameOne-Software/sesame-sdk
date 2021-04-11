#include "include/ses.h"
#include "include/utils.h"
#include "include/hooks/hooks.h"

#include <windows.h>
#include <process.h>

static bool ses_should_shutdown = false;

void ses_shutdown( ) {
    ses_should_shutdown = true;
}

static int __stdcall ses_init( HMODULE mod ) {
    /* wait for game to be fully loaded */
    while ( !GetModuleHandleA( "serverbrowser.dll" ) )
        utils_sleep( 100 );

    if ( !cs_init( ) ) goto fail_init;
    if ( !netvars_init( ) ) goto fail_init;
    if ( !hooks_init( ) ) goto fail_init;

    /* wait for self-destruct key */
    while ( !ses_should_shutdown && !GetAsyncKeyState( VK_END ) )
        utils_sleep( 100 );

    if ( !hooks_free( ) ) goto fail_free;

    /* wait for hooks to finish before we free anything they might use */
    utils_sleep( 100 );

    if ( !netvars_free( ) ) goto fail_free;
    if ( !cs_free( ) ) goto fail_free;

    FreeLibraryAndExitThread( mod, EXIT_SUCCESS );
    return EXIT_SUCCESS;

fail_free:
    MessageBoxA( NULL, "Failed to free base.", "Sesame-SDK", MB_OK );
    FreeLibraryAndExitThread( mod, EXIT_FAILURE );
    return EXIT_FAILURE;

fail_init:
    MessageBoxA( NULL, "Failed to initialize base.", "Sesame-SDK", MB_OK );
    FreeLibraryAndExitThread( mod, EXIT_FAILURE );
    return EXIT_FAILURE;
}

/* https://docs.microsoft.com/en-us/windows/win32/dlls/dllmain */
BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved ) {
    if ( fdwReason == DLL_PROCESS_ATTACH ) {
        DisableThreadLibraryCalls( hinstDLL );

        HANDLE ret = _beginthreadex( NULL, 0, ses_init, hinstDLL, 0, NULL );

        if ( ret == INVALID_HANDLE_VALUE ) {
            MessageBoxA( NULL, "Failed to create setup thread.", "Sesame-SDK", MB_OK );
            return FALSE;
        }
        else {
            CloseHandle( ret );
        }

        return TRUE;
    }

    return FALSE;
}