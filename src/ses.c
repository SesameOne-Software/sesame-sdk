#include "include/ses.h"
#include "include/utils.h"

#include "lib/csgo-sdk/sdk.h"

#include <windows.h>
#include <process.h>

static bool ses_should_shutdown = false;

inline void ses_shutdown ( ) {
    ses_should_shutdown = true;
}

int __stdcall ses_init ( HMODULE mod ) {
    /* wait for game to be fully loaded */
    while ( !GetModuleHandleA ( "serverbrowser.dll" ) )
        utils_sleep ( 100 );

    cs_init();
    netvars_init();

    /* wait for self-destruct key */
    while ( !ses_should_shutdown )
        utils_sleep ( 100 );
        
    netvars_free();
    cs_free();

    FreeLibraryAndExitThread ( mod, EXIT_SUCCESS );
    return 0;

fail:
    MessageBoxA ( NULL, "Failed to initialize base.", "Sesame-SDK", MB_OK );
    FreeLibraryAndExitThread ( mod, 1 );
    return 1;
}

/* https://docs.microsoft.com/en-us/windows/win32/dlls/dllmain */
BOOL WINAPI DllMain ( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved ) {
    if ( fdwReason == DLL_THREAD_ATTACH ) {
        DisableThreadLibraryCalls ( hinstDLL );

        HANDLE ret = _beginthreadex ( NULL, 0, ses_init, hinstDLL, 0, NULL );
        
        if ( ret == INVALID_HANDLE_VALUE ) {
            MessageBoxA ( NULL, "Failed to create setup thread.", "Sesame-SDK", MB_OK );
        }
        else {
            CloseHandle ( ret );
        }

        return TRUE;
    }

    return FALSE;
}