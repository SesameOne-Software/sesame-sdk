#ifndef HOOKS_H
#define HOOKS_H

#include "lib/csgo-sdk/sdk.h"
#include "lib/vector/vector.h"

#define SUBHOOK_SEPARATE_SOURCE_FILES
#define SUBHOOK_STATIC
#include "lib/subhook/subhook.h"

#include "include/gui/menu.h"

typedef enum {
    subhook_create_move,
    subhook_d3d9_end_scene,
    subhook_d3d9_reset,
    subhook_accumulate_layers,
    subhook_build_transformations,
    subhook_cl_fire_events,
    subhook_cs_blood_spray_callback,
    subhook_do_extra_bone_processing,
    subhook_draw_model,
    subhook_emit_sound,
    subhook_entity_listener,
    subhook_event_listener,
    subhook_frame_stage_notify,
    subhook_cvar_get_bool,
    subhook_get_viewmodel_fov,
    subhook_in_prediction,
    subhook_is_connected,
    subhook_is_hltv,
    subhook_list_leaves_in_box,
    subhook_lock_cursor,
    subhook_modify_eye_pos,
    subhook_notify_on_layer_change_cycle,
    subhook_notify_on_layer_change_weight,
    subhook_override_view,
    subhook_paint_traverse,
    subhook_perform_flashbang_effect,
    subhook_run_simulation,
    subhook_scene_end,
    subhook_send_datagram,
    subhook_send_net_msg,
    subhook_setup_bones,
    subhook_should_skip_anim_frame,
    subhook_max
} subhooks_list;

extern subhook_t hooks_subhooks [ subhook_max ];
extern WNDPROC backup_window_proc;

/* hook function prototypes */
bool __fastcall hooks_create_move( REG, float sample_time, usercmd* cmd );
HRESULT __stdcall hooks_d3d9_end_scene ( IDirect3DDevice9* device );
HRESULT __stdcall hooks_d3d9_reset ( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_params );
void __fastcall hooks_accumulate_layers ( REG, void* setup, vec3* pos, void* q, float time );

LRESULT __stdcall window_proc ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#define CREATE_HOOK(target, hook_func, id)\
hooks_subhooks[id] = subhook_new( (void*)(target), hook_func, 0 );\
if ( !hooks_subhooks[id] || subhook_install( hooks_subhooks[id] ) < 0 )\
    return false;\

static inline bool hooks_init( ) {
    /* remove breakpoints */ {
        const void* client_bp = ( void* ) pattern_search ( "client.dll", "CC F3 0F 10 4D 18" );

        DWORD old_prot = 0;
        VirtualProtect ( client_bp, 1, PAGE_EXECUTE_READWRITE, &old_prot );
        memset ( client_bp, 0x90, 1 );
        VirtualProtect ( client_bp, 1, old_prot, &old_prot );

        const void* server_bp = ( void* ) pattern_search ( "server.dll", "CC F3 0F 10 4D 18" );

        old_prot = 0;
        VirtualProtect ( server_bp, 1, PAGE_EXECUTE_READWRITE, &old_prot );
        memset ( server_bp, 0x90, 1 );
        VirtualProtect ( server_bp, 1, old_prot, &old_prot );

        const void* engine_bp = ( void* ) pattern_search ( "engine.dll", "CC FF 15 ? ? ? ? 8B D0 BB" );

        old_prot = 0;
        VirtualProtect ( engine_bp, 1, PAGE_EXECUTE_READWRITE, &old_prot );
        memset ( engine_bp, 0x90, 1 );
        VirtualProtect ( engine_bp, 1, old_prot, &old_prot );
    }

    backup_window_proc = ( WNDPROC ) SetWindowLongA ( cs_window, GWLP_WNDPROC, ( LONG ) window_proc );

    /* start menu opened */
    menu_set_opened ( true );

    menu_init ( );

    /* setup hooks */ {
        CREATE_HOOK ( pattern_search ( "client.dll", "55 8B EC 8B 0D ? ? ? ? 85 C9 75 06 B0" ), hooks_create_move, subhook_create_move );
        CREATE_HOOK ( utils_vfunc ( cs_id3ddev, 42 ), hooks_d3d9_end_scene, subhook_d3d9_end_scene );
        CREATE_HOOK ( utils_vfunc ( cs_id3ddev, 16 ), hooks_d3d9_reset, subhook_d3d9_reset );
        CREATE_HOOK ( pattern_search ( "client.dll", "55 8B EC 57 8B F9 8B 0D ? ? ? ? 8B 01 8B" ), hooks_accumulate_layers, subhook_accumulate_layers );
    }

    return true;
}

static inline bool hooks_free( ) {
    for ( int i = 0; i < subhook_max; i++ ) {
        const subhook_t hook = hooks_subhooks [ i ];

        if ( hook ) {
            subhook_remove ( hook );
            subhook_free ( hook );
        }
    }

    utils_sleep ( 100 );

    menu_free ( );
    SetWindowLongA ( cs_window, GWLP_WNDPROC, ( LONG ) backup_window_proc );

    return true;
}

#endif // !HOOKS_H
