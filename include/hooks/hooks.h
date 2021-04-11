#ifndef HOOKS_H
#define HOOKS_H

#include "lib/csgo-sdk/sdk.h"
#include "lib/subhook/subhook.h"
#include "lib/vector/vector.h"

typedef enum {
    subhook_create_move
} subhooks_list;

typedef subhook_t* subhook_vec;

extern subhook_vec hooks_subhooks;

/* hook function prototypes */
bool __fastcall hooks_create_move( REG, float sample_time, usercmd* cmd );

#define CREATE_HOOK(target, hook_func)\
subhook_t COMBINE( hook_func, _subhook ) = subhook_new( (void*)(target), hook_func, 0 );\
if ( !COMBINE( hook_func, _subhook ) || subhook_install( COMBINE( hook_func, _subhook ) ) < 0 )\
    return false;\
vector_add(&hooks_subhooks, subhook_t, COMBINE( hook_func, _subhook ) );

static inline bool hooks_init( ) {
    hooks_subhooks = vector_create( );

    CREATE_HOOK( pattern_search( "client.dll", "55 8B EC 8B 0D ? ? ? ? 85 C9 75 06 B0" ), hooks_create_move );

    return true;
}

static inline bool hooks_free( ) {
    for ( int i = 0;i < vector_size( hooks_subhooks ); i++ ) {
        subhook_remove( hooks_subhooks[ i ] );
        subhook_free( hooks_subhooks[ i ] );
    }

    vector_free( hooks_subhooks );

    return true;
}

#endif // !HOOKS_H
