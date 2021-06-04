#ifndef HOOKS_H
#define HOOKS_H

#include "csgo-sdk/sdk.h"
#include "vector/vector.h"

#define SUBHOOK_SEPARATE_SOURCE_FILES
#define SUBHOOK_STATIC
#include "subhook/subhook.h"

#include "gui/menu.h"

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
    subhook_cvar_get_int,
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
    subhook_level_init,
    subhook_level_shutdown,
    subhook_max
} subhooks_list;

extern subhook_t hooks_subhooks[ subhook_max ];
extern WNDPROC hooks_backup_window_proc;

/* hook function prototypes */
__attribute__( ( thiscall ) ) bool hooks_create_move( void* this, float sample_time, usercmd* cmd );
__attribute__( ( stdcall ) ) HRESULT hooks_d3d9_end_scene( IDirect3DDevice9* device );
__attribute__( ( stdcall ) ) HRESULT hooks_d3d9_reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_params );
__attribute__( ( thiscall ) ) void hooks_accumulate_layers( player* this, void* setup, vec3* pos, void* q, float time );
__attribute__( ( thiscall ) ) void hooks_build_transformations( player* this, studiohdr* hdr, vec3* pos, void* quaternion, mat3x4a* matrix, uint32_t mask, bool* computed );
__attribute__( ( stdcall ) ) void hooks_cl_fire_events( );
__attribute__( ( cdecl ) ) void hooks_cs_blood_spray_callback( const void* pdata );
__attribute__( ( thiscall ) ) int hooks_cvar_get_int( cvar* this );
__attribute__( ( thiscall ) ) void hooks_do_extra_bone_processing( player* this, int a2, int a3, int a4, int a5, int a6, int a7 );
__attribute__( ( thiscall ) ) void hooks_draw_model( imdlrender* this, void* results, void* info, const mat3x4* matrix, float* weights, float* delayed_weights, const vec3* origin, int flags );
__attribute__( ( thiscall ) ) int hooks_emit_sound( void* this, void* filter, int ent_idx, int chan, const char* sound_entry, unsigned int sound_entry_hash, const char* sample, float volume, float attenuation, int seed, int flags, int pitch, const vec3* origin, const vec3* dir, vec3* vec_origins, bool update_positions, float sound_time, int speaker_ent, void* sound_params );
__attribute__( ( thiscall ) ) void hooks_frame_stage_notify( iclient* this, int stage );
__attribute__( ( thiscall ) ) float hooks_get_viewmodel_fov( void* this );
__attribute__( ( thiscall ) ) bool hooks_in_prediction( ipred* this );
__attribute__( ( thiscall ) ) bool hooks_is_connected( iengine* this );
__attribute__( ( thiscall ) ) bool hooks_is_hltv( iengine* this );
__attribute__( ( thiscall ) ) int hooks_list_leaves_in_box( iengine* this, const vec3* mins, const vec3* maxs, const uint16_t* list, int list_max );
__attribute__( ( thiscall ) ) void hooks_lock_cursor( isurface* this );
__attribute__( ( thiscall ) ) void hooks_modify_eye_pos( animstate* this, vec3* pos );
__attribute__( ( thiscall ) ) void hooks_notify_on_layer_change_cycle( animstate* this, animlayer* layer, float new_cycle );
__attribute__( ( thiscall ) ) void hooks_notify_on_layer_change_weight( animstate* this, animlayer* layer, float new_weight );
__attribute__( ( thiscall ) ) void hooks_override_view( void* this, void* view_setup );
__attribute__( ( thiscall ) ) void hooks_paint_traverse( ipanel* this, uint32_t panel, bool force_repaint, bool allow_force );
__attribute__( ( thiscall ) ) void hooks_perform_flashbang_effect( ipanel* this, void* view_setup );
__attribute__( ( thiscall ) ) void hooks_run_simulation( ipred* this, int current_command, usercmd* cmd, player* ent );
__attribute__( ( thiscall ) ) void hooks_scene_end( irenderview* this );
__attribute__( ( thiscall ) ) int hooks_send_datagram( void* this, void* datagram );
__attribute__( ( thiscall ) ) bool hooks_send_net_msg( void* this, void* msg, bool force_reliable, bool voice );
__attribute__( ( thiscall ) ) bool hooks_setup_bones( renderable* this, mat3x4* out, int max_bones, uint32_t mask, float time );
__attribute__( ( thiscall ) ) bool hooks_should_skip_anim_frame( renderable* this );
__attribute__( ( thiscall ) ) void hooks_level_init( void* this, const char* map_name ); 
__attribute__( ( thiscall ) ) void hooks_level_shutdown( void* this );

__attribute__( ( stdcall ) ) LRESULT window_proc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#define CREATE_HOOK(target, hook_func, id)\
hooks_subhooks[id] = subhook_new( (void*)(target), hook_func, 0 );\
if ( !hooks_subhooks[id] || !subhook_get_trampoline(hooks_subhooks[id]) || subhook_install( hooks_subhooks[id] ) < 0 )\
    return false;\

static inline bool hooks_patch_bp( ) {
    const void* client_bp = ( void* )pattern_search( "client.dll", "CC F3 0F 10 4D 18" );

    DWORD old_prot = 0;
    VirtualProtect( client_bp, 1, PAGE_EXECUTE_READWRITE, &old_prot );
    memset( client_bp, 0x90, 1 );
    VirtualProtect( client_bp, 1, old_prot, &old_prot );

    const void* server_bp = ( void* )pattern_search( "server.dll", "CC F3 0F 10 4D 18" );

    old_prot = 0;
    VirtualProtect( server_bp, 1, PAGE_EXECUTE_READWRITE, &old_prot );
    memset( server_bp, 0x90, 1 );
    VirtualProtect( server_bp, 1, old_prot, &old_prot );

    const void* engine_bp = ( void* )pattern_search( "engine.dll", "CC FF 15 ? ? ? ? 8B D0 BB" );

    old_prot = 0;
    VirtualProtect( engine_bp, 1, PAGE_EXECUTE_READWRITE, &old_prot );
    memset( engine_bp, 0x90, 1 );
    VirtualProtect( engine_bp, 1, old_prot, &old_prot );

    return true;
}

static inline bool hooks_init( ) {
    hooks_backup_window_proc = ( WNDPROC )SetWindowLongA( cs_window, GWLP_WNDPROC, ( LONG )window_proc );

    /* start menu opened */
    menu_set_opened( true );

    menu_init( );

    /* setup hooks */ {
        CREATE_HOOK( pattern_search( "client.dll", "55 8B EC 8B 0D ? ? ? ? 85 C9 75 06 B0" ), hooks_create_move, subhook_create_move );
        CREATE_HOOK( utils_vfunc( cs_id3ddev, 42 ), hooks_d3d9_end_scene, subhook_d3d9_end_scene );
        CREATE_HOOK( utils_vfunc( cs_id3ddev, 16 ), hooks_d3d9_reset, subhook_d3d9_reset );
        CREATE_HOOK( pattern_search( "client.dll", "55 8B EC 57 8B F9 8B 0D ? ? ? ? 8B 01 8B" ), hooks_accumulate_layers, subhook_accumulate_layers );
        CREATE_HOOK( pattern_search( "client.dll", "55 8B EC 83 E4 F0 81 EC ? ? ? ? 56 57 8B F9 8B 0D ? ? ? ? 89 7C 24 1C" ), hooks_build_transformations, subhook_build_transformations );
        CREATE_HOOK( pattern_rip( pattern_search( "engine.dll", "E8 ? ? ? ? 84 DB 0F 84 ? ? ? ? 8B 0D" ) ), hooks_cl_fire_events, subhook_cl_fire_events );
        CREATE_HOOK( pattern_search( "client.dll", "55 8B EC 8B 4D 08 F3 0F 10 51 ? 8D 51 18" ), hooks_cs_blood_spray_callback, subhook_cs_blood_spray_callback );
        CREATE_HOOK( pattern_search( "client.dll", "8B 51 1C 3B D1 75 06" ), hooks_cvar_get_int, subhook_cvar_get_int );
        CREATE_HOOK( pattern_search( "client.dll", "55 8B EC 83 E4 F8 81 EC FC 00 00 00 53 56 8B F1 57" ), hooks_do_extra_bone_processing, subhook_do_extra_bone_processing );
        CREATE_HOOK( utils_vfunc( cs_imdlrender, 0 ), hooks_draw_model, subhook_draw_model );
        CREATE_HOOK( pattern_rip( pattern_search( "engine.dll", "E8 ? ? ? ? 8B E5 5D C2 3C 00 55" ) ), hooks_emit_sound, subhook_emit_sound );
        CREATE_HOOK( pattern_search( "client.dll", "55 8B EC 8B 0D ? ? ? ? 8B 01 8B 80 74 01 00 00 FF D0 A2" ), hooks_frame_stage_notify, subhook_frame_stage_notify );
        CREATE_HOOK( utils_vfunc( **( void*** )( ( *( uintptr_t** )cs_iclient )[ 10 ] + 5 ), 35 ), hooks_get_viewmodel_fov, subhook_get_viewmodel_fov );
        CREATE_HOOK( utils_vfunc( cs_ipred, 14 ), hooks_in_prediction, subhook_in_prediction );
        CREATE_HOOK( utils_vfunc( cs_iengine, 27 ), hooks_is_connected, subhook_is_connected );
        CREATE_HOOK( utils_vfunc( cs_iengine, 93 ), hooks_is_hltv, subhook_is_hltv );
        //CREATE_HOOK( utils_vfunc( ( ( __attribute__( ( thiscall ) ) void* ( * )( void* ) ) utils_vfunc( cs_iengine, 43 ) )( cs_iengine ), 6 ), hooks_list_leaves_in_box, subhook_list_leaves_in_box );
        CREATE_HOOK( utils_vfunc( cs_isurface, 67 ), hooks_lock_cursor, subhook_lock_cursor );
        CREATE_HOOK( pattern_rip( pattern_search( "client.dll", "57 E8 ? ? ? ? 8B 06 8B CE FF 90" ) + 1 ), hooks_modify_eye_pos, subhook_modify_eye_pos );
        CREATE_HOOK( pattern_search( "client.dll", "F3 0F 11 86 98 00 00 00 5E 5D C2 08 00" ) - 57, hooks_notify_on_layer_change_cycle, subhook_notify_on_layer_change_cycle );
        CREATE_HOOK( pattern_search( "client.dll", "F3 0F 11 86 9C 00 00 00 5E 5D C2 08 00" ) - 57, hooks_notify_on_layer_change_weight, subhook_notify_on_layer_change_weight );
        CREATE_HOOK( pattern_search( "client.dll", "55 8B EC 83 E4 F8 83 EC 58 56 57 8B 3D ? ? ? ? 85 FF" ), hooks_override_view, subhook_override_view );
        CREATE_HOOK( utils_vfunc( cs_ipanel, 41 ), hooks_paint_traverse, subhook_paint_traverse );
        CREATE_HOOK( pattern_search( "client.dll", "55 8B EC 83 EC 48 53 8B 1D" ), hooks_perform_flashbang_effect, subhook_perform_flashbang_effect );
        CREATE_HOOK( pattern_rip( pattern_search( "client.dll", "E8 ? ? ? ? A1 ? ? ? ? F3 0F 10 45 ? F3 0F 11 40" ) ), hooks_run_simulation, subhook_run_simulation );
        CREATE_HOOK( utils_vfunc( cs_irenderview, 9 ), hooks_scene_end, subhook_scene_end );
        CREATE_HOOK( pattern_search( "engine.dll", "55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? 56 57 8B F9 89 7C 24 18" ), hooks_send_datagram, subhook_send_datagram );
        CREATE_HOOK( pattern_search( "engine.dll", "55 8B EC 83 EC 08 56 8B F1 8B 86 ? ? ? ? 85 C0" ), hooks_send_net_msg, subhook_send_net_msg );
        CREATE_HOOK( pattern_search( "client.dll", "55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? 56 57 8B F9" ), hooks_setup_bones, subhook_setup_bones );
        CREATE_HOOK( pattern_search( "client.dll", "57 8B F9 8B 07 8B 80 ? ? ? ? FF D0 84 C0 75 02" ), hooks_should_skip_anim_frame, subhook_should_skip_anim_frame );
        CREATE_HOOK( pattern_search( "client.dll", "55 8B EC 83 E4 F8 83 EC 20 56 57 8B F9 8B 4F" ), hooks_level_init, subhook_level_init );
        CREATE_HOOK( pattern_rip(pattern_search( "client.dll", "E8 ? ? ? ? A1 ? ? ? ? 8D BE" )), hooks_level_shutdown, subhook_level_shutdown );
    }

    return true;
}

static inline bool hooks_free( ) {
    for ( int i = 0; i < subhook_max; i++ ) {
        const subhook_t hook = hooks_subhooks[ i ];

        if ( hook ) {
            subhook_remove( hook );
            subhook_free( hook );
        }
    }

    utils_sleep( 100 );

    menu_free( );
    SetWindowLongA( cs_window, GWLP_WNDPROC, ( LONG )hooks_backup_window_proc );

    return true;
}

#endif // !HOOKS_H
