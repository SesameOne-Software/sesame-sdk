#ifndef SDK_H
#define SDK_H

#include <windows.h>
#include <d3d9.h>
#include <assert.h>
#include <stdio.h>

#include "utils.h"

/* sdk classes */
#include "beams.h"
#include "client.h"
#include "crc32.h"
#include "cvar.h"
#include "engine.h"
#include "entity.h"
#include "entlist.h"
#include "events.h"
#include "globals.h"
#include "input.h"
#include "mat3x4.h"
#include "matsys.h"
#include "mdlcache.h"
#include "mdlinfo.h"
#include "mdlrender.h"
#include "memalloc.h"
#include "netvars.h"
#include "physics.h"
#include "player.h"
#include "pred.h"
#include "quat.h"
#include "renderview.h"
#include "surface.h"
#include "trace.h"
#include "usercmd.h"
#include "vec3.h"
#include "weapon.h"
#include "utlvec.h"

extern isurface* cs_isurface;
extern ipanel* cs_ipanel;
extern iengine* cs_iengine;
extern iclient* cs_iclient;
extern ientlist* cs_ientlist;
extern imatsys* cs_imatsys;
extern imdlinfo* cs_imdlinfo;
extern imdlrender* cs_imdlrender;
extern irenderview* cs_irenderview;
extern iglobals* cs_iglobals;
extern iphys* cs_iphys;
extern itrace* cs_itrace;
extern iclientstate* cs_iclientstate;
extern imemalloc* cs_imemalloc;
extern ipred* cs_ipred;
extern imovehelper* cs_imovehelper;
extern imove* cs_imove;
extern imdlcache* cs_imdlcache;
extern iinput* cs_iinput;
extern icvar* cs_icvar;
extern ievents* cs_ievents;
extern ibeams* cs_ibeams;
extern IDirect3DDevice9* cs_id3ddev;
extern player** cs_local_ptr;
extern HWND cs_window;

static inline player* cs_get_local( void ) {
	return *cs_local_ptr;
}

static inline void* cs_create_interface( const char* module, const char* name ) {
	void* ( *create_interface_export )( const char*, int ) = ( void* ( * )( const char*, int ) )GetProcAddress( GetModuleHandleA( module ), "CreateInterface" );
	return create_interface_export( name, 0 );
}

static inline int cs_time_to_ticks( float t ) {
	return ( int )( t / cs_iglobals->tick_interval + 0.5f );
}

static inline float cs_ticks_to_time( int t ) {
	return ( float )t * cs_iglobals->tick_interval;
}

/* https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/shared/shareddefs.h */
#define MAX_PLAYERS 64

#define cs_for_each_player() \
for ( struct { int idx; player* player; } iter = { 0 }; iter.idx <= cs_iglobals->max_clients - 1; iter.player = (player*) ientlist_get_entity(cs_ientlist, ++iter.idx) )

static inline void cs_rotate_movement( usercmd* cmd, float backup_side_move, float backup_forward_move, const vec3* old_angles ) {
	float dv = 0.0f;
	float f1 = 0.0f;
	float f2 = 0.0f;

	if ( old_angles->y < 0.0f )
		f1 = 360.0f + old_angles->y;
	else
		f1 = old_angles->y;

	if ( cmd->angles.y < 0.0f )
		f2 = 360.0f + cmd->angles.y;
	else
		f2 = cmd->angles.y;

	if ( f2 < f1 )
		dv = abs( f2 - f1 );
	else
		dv = 360.0f - abs( f1 - f2 );

	dv = 360.0f - dv;

	cmd->forward_move = cosf( cs_deg2rad( dv ) ) * backup_forward_move + cosf( cs_deg2rad( dv + 90.0f ) ) * backup_side_move;
	cmd->side_move = sinf( cs_deg2rad( dv ) ) * backup_forward_move + sinf( cs_deg2rad( dv + 90.0f ) ) * backup_side_move;
}

static inline mat3x4* cs_angle_matrix( const vec3* angles, mat3x4* mat ) {
	float sr, sp, sy, cr, cp, cy;

	cs_sincos( cs_deg2rad( angles->y ), &sy, &cy );
	cs_sincos( cs_deg2rad( angles->x ), &sp, &cp );
	cs_sincos( cs_deg2rad( angles->z ), &sr, &cr );

	const float crcy = cr * cy;
	const float crsy = cr * sy;
	const float srcy = sr * cy;
	const float srsy = sr * sy;

	*mat3x4_at( mat, 0, 0 ) = cp * cy;
	*mat3x4_at( mat, 1, 0 ) = cp * sy;
	*mat3x4_at( mat, 2, 0 ) = -sp;
	*mat3x4_at( mat, 0, 1 ) = sp * srcy - crsy;
	*mat3x4_at( mat, 1, 1 ) = sp * srsy + crcy;
	*mat3x4_at( mat, 2, 1 ) = sr * cp;
	*mat3x4_at( mat, 0, 2 ) = ( sp * crcy + srsy );
	*mat3x4_at( mat, 1, 2 ) = ( sp * crsy - srcy );
	*mat3x4_at( mat, 2, 2 ) = cr * cp;
	*mat3x4_at( mat, 0, 3 ) = 0.0f;
	*mat3x4_at( mat, 1, 3 ) = 0.0f;
	*mat3x4_at( mat, 2, 3 ) = 0.0f;

	return mat;
}

static inline void cs_angle_matrix_pos( const vec3* angles, const vec3* origin, mat3x4* mat ) {
	cs_angle_matrix( angles, mat );
	mat3x4_set_origin( mat, origin );
}

static inline void cs_vector_transform( const vec3* in, const mat3x4* mat, vec3* out ) {
	vec3_add( mat3x4_get_origin( mat, vec3_zero( out ) ),
		&( vec3 ) {
		vec3_dot( in, ( vec3* )mat3x4_at( mat, 0, 0 ) ),
			vec3_dot( in, ( vec3* )mat3x4_at( mat, 1, 0 ) ),
			vec3_dot( in, ( vec3* )mat3x4_at( mat, 2, 0 ) )
	} );
}

static inline bool cs_is_valve_server( void ) {
	return *( uintptr_t* )cs_offsets.sdk_game_rules && *( bool* )( *( uintptr_t* )cs_offsets.sdk_game_rules + cs_offsets.sdk_is_valve_ds );
}

static inline void cs_allow_invalidate_bone_cache( bool enable ) {
	*( bool* )cs_offsets.enable_invalidate_bone_cache = enable;
}

static inline void cs_add_box_overlay( const vec3* origin, const vec3* mins, const vec3* maxs, const vec3* angles, int r, int g, int b, int a, float duration ) {
	( ( __attribute__( ( thiscall ) ) void( * )( void*, const vec3*, const vec3*, const vec3*, const vec3*, int, int, int, int, float ) )utils_vfunc( ( void* )cs_offsets.sdk_dbgview, cs_idx_idbgoverlay_add_box_overlay ) )( ( void* )cs_offsets.sdk_dbgview, origin, mins, maxs, angles, r, g, b, a, duration );
}

bool cs_world_to_screen( vec3* origin, vec3* screen );

#define cs_dump_offset(var, offset)\
var = offset;\
if (!var) {\
	sds new_error = sdstrim(sdscatprintf(sdsempty(), "\t%s was null.\n", #var ), "cs_offsets.");\
	*errors = sdscat(*errors, new_error);\
	sdsfree(new_error);\
	return false;\
}

static inline bool cs_offsets_dump( sds* errors ) {
	cs_dump_offset( cs_offsets.iclientstate_nc, 0x9C );
	cs_dump_offset( cs_offsets.iclientstate_choked, 0x4D30 );
	cs_dump_offset( cs_offsets.iclientstate_last_cmd_ack, 0x4D34 );
	cs_dump_offset( cs_offsets.iclientstate_last_out_cmd, 0x4D2C );
	cs_dump_offset( cs_offsets.iclientstate_server_tick, 0x164 );
	cs_dump_offset( cs_offsets.iclientstate_delta_tick, 0x174 );
	cs_dump_offset( cs_offsets.iclientstate_next_cmd_time, 0x114 );
	cs_dump_offset( cs_offsets.iclientstate_out_seq_num, 0x4D24 );
	cs_dump_offset( cs_offsets.iclientstate_events, *( ptrdiff_t* )( pattern_search( "engine.dll", "8B BB ? ? ? ? 85 FF 0F 84" ) + 2 ) );

	cs_dump_offset( cs_offsets.entity_idx, 0x64 );
	cs_dump_offset( cs_offsets.entity_dormant, 0xED );
	cs_dump_offset( cs_offsets.entity_eflags, 0xE8 );
	cs_dump_offset( cs_offsets.entity_effects, 0xF0 );
	cs_dump_offset( cs_offsets.entity_abs_vel, 0x94 );
	cs_dump_offset( cs_offsets.entity_model_ptr, *( ptrdiff_t* )( pattern_search( "client.dll", "8B 86 ? ? ? ? 85 C0 74 05 83 38 00 75 02 33 C0 5E" ) + 2 ) );
	cs_dump_offset( cs_offsets.entity_can_use_fast_path, *( ptrdiff_t* )( pattern_search( "client.dll", "88 87 ? ? ? ? 84 C0 74 39" ) + 2 ) - 4 /* have to sub 4 because offset is off renderable */ );

	cs_dump_offset( cs_offsets.player_playback_rate, 0xA18 );
	cs_dump_offset( cs_offsets.player_cycle, 0xA14 );
	cs_dump_offset( cs_offsets.player_movetype, 0x25C );
	cs_dump_offset( cs_offsets.player_iks, *( ptrdiff_t* )( pattern_search( "client.dll", "83 BF ? ? ? ? ? 75 3C 8B" ) + 2 ) + 4 );
	cs_dump_offset( cs_offsets.player_spawn_time, *( ptrdiff_t* )( pattern_search( "client.dll", "89 86 ? ? ? ? E8 ? ? ? ? 80 BE" ) + 2 ) );
	cs_dump_offset( cs_offsets.player_bones, ( *( ptrdiff_t* )( pattern_search( "client.dll", "8B B1 ? ? ? ? C7 81 ? ? ? ? ? ? ? ? C7" ) + 2 ) - 4 ) + 4 );
	cs_dump_offset( cs_offsets.player_animlayers, *( ptrdiff_t* )( pattern_search( "client.dll", "8B 80 ? ? ? ? 03 C1 74" ) + 2 ) );
	cs_dump_offset( cs_offsets.player_num_animlayers, cs_offsets.player_animlayers + 0xC );
	cs_dump_offset( cs_offsets.player_jiggle_bones, *( ptrdiff_t* )( pattern_search( "client.dll", "C7 87 ? ? ? ? ? ? ? ? 8B 54" ) + 2 ) + 4 );
	cs_dump_offset( cs_offsets.player_poses, *( ptrdiff_t* )( pattern_search( "client.dll", "F3 0F 11 8C BE ? ? ? ? 5F" ) + 5 ) );
	cs_dump_offset( cs_offsets.player_thirdperson_recoil, *( ptrdiff_t* )( pattern_search( "client.dll", "F3 0F 10 86 ? ? ? ? F3 0F 58 44 24 0C" ) + 4 ) );
	cs_dump_offset( cs_offsets.player_computed_lod_frame, *( ptrdiff_t* )( pattern_search( "client.dll", "C7 80 ? ? ? ? ? ? ? ? 66 66" ) + 2 ) );
	cs_dump_offset( cs_offsets.player_move_state, *( ptrdiff_t* )( pattern_search( "client.dll", "8B 81 ? ? ? ? 3B 86 ? ? ? ? 74" ) + 2 ) );

	cs_dump_offset( cs_offsets.studiohdr_findmapping, pattern_search( "client.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 56 57 8B F9 8B 17 83 BA ? ? ? ? ? 74 34 83 7F 04 00 75 2E 83 BA ? ? ? ? ? 75 04 33 C0 EB 0C 8B 0D ? ? ? ? 52 8B 01 FF 50 68" ) );
	cs_dump_offset( cs_offsets.studiohdr_seq_desc, pattern_search( "client.dll", "55 8B EC 83 79 04 00 75" ) );
	cs_dump_offset( cs_offsets.studiohdr_select_weighted_seq, pattern_search( "client.dll", "51 A1 ? ? ? ? 53 8B DA" ) );

	cs_dump_offset( cs_offsets.animstate_set_up_whole_body_action_fn, pattern_search( "client.dll", "55 8B EC 83 EC 08 56 57 8B F9 8B 77" ) );
	cs_dump_offset( cs_offsets.animstate_set_up_flinch_fn, pattern_search( "client.dll", "55 8B EC 51 56 8B 71 60 83 BE ? ? ? ? 00 0F 84 ? ? ? ? 8B B6 ? ? ? ? 81 C6 30 02 00 00" ) );
	cs_dump_offset( cs_offsets.animstate_set_up_lean_fn, pattern_search( "client.dll", "55 8B EC 83 E4 F8 A1 ? ? ? ? 83 EC 20 F3" ) );

	cs_dump_offset( cs_offsets.animstate_get_weapon_prefix_fn, pattern_search( "client.dll", "53 56 57 8B F9 33 F6 8B 4F 60" ) );
	cs_dump_offset( cs_offsets.animstate_setup_aim_matrix_fn, pattern_search( "client.dll", "55 8B EC 81 EC ? ? ? ? 53 56 57 8B 3D" ) );
	cs_dump_offset( cs_offsets.animstate_pose_param_cache_set_value_fn, pattern_rip( pattern_search( "client.dll", "E8 ? ? ? ? D9 45 08 5F" ) ) );
	cs_dump_offset( cs_offsets.animstate_get_layer_activity_fn, pattern_search( "client.dll", "55 8B EC 83 EC 08 53 56 8B 35 ? ? ? ? 57 8B F9 8B CE 8B 06 FF 90 84 00 00 00 8B 7F 60 83 BF ? ? ? ? 00" ) );
	cs_dump_offset( cs_offsets.animstate_get_first_sequence_anim_tag_fn, pattern_rip( pattern_search( "client.dll", "E8 ? ? ? ? F3 0F 11 86 98 00 00 00 0F 57 DB F3 0F 10 86 24 01 00 00" ) ) );
	cs_dump_offset( cs_offsets.animstate_get_layer_ideal_weight_from_seq_cycle_fn, pattern_search( "client.dll", "55 8B EC 83 EC 08 53 56 8B 35 ? ? ? ? 57 8B F9 8B CE 8B 06 FF 90 84 00 00 00 8B 7F 60 0F 57 DB" ) );
	cs_dump_offset( cs_offsets.animstate_cache_sequences_fn, pattern_search( "client.dll", "55 8B EC 83 E4 F8 83 EC 34 53 56 8B F1 57 8B" ) );
	cs_dump_offset( cs_offsets.animstate_reset_fn, pattern_search( "client.dll", "56 6A 01 68 ? ? ? ? 8B F1" ) );
	cs_dump_offset( cs_offsets.animstate_get_weapon_move_animation_fn, pattern_search( "client.dll", "53 56 57 8B F9 33 F6 8B 4F 60 8B 01 FF 90" ) );
	cs_dump_offset( cs_offsets.animstate_update_layer_order_preset_fn, pattern_search( "client.dll", "55 8B EC 51 53 56 57 8B F9 83 7F 60" ) );
	cs_dump_offset( cs_offsets.player_get_layer_sequence_cycle_rate_fn, pattern_search( "client.dll", "55 8B EC 56 57 FF 75 0C 8B 7D 08 8B F1 57 E8" ) );
	cs_dump_offset( cs_offsets.player_set_sequence_fn, pattern_search( "client.dll", "55 8B EC 80 B9 ? ? ? ? ? 74 16" ) );
	cs_dump_offset( cs_offsets.player_get_sequence_linear_motion_fn, pattern_search( "client.dll", "55 8B EC 83 EC 0C 56 8B F1 57 8B FA 85 F6 75 14" ) );
	cs_dump_offset( cs_offsets.player_invalidate_physics_recursive_fn, pattern_search( "client.dll", "55 8B EC 83 E4 F8 83 EC 0C 53 8B 5D 08 8B C3 56" ) );
	cs_dump_offset( cs_offsets.player_lookup_sequence_fn, pattern_rip( pattern_search( "client.dll", "E8 ? ? ? ? 5E 83 F8 FF" ) ) );
	cs_dump_offset( cs_offsets.player_lookup_bone_fn, pattern_search( "client.dll", "55 8B EC 53 56 8B F1 57 83 BE ? ? ? ? ? 75" ) );
	cs_dump_offset( cs_offsets.player_sequence_duration_fn, pattern_search( "client.dll", "55 8B EC 56 8B F1 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? 83 C4 04 5E 5D C2 04 00 52 68 ? ? ? ? 6A 02" ) );
	cs_dump_offset( cs_offsets.player_physics_run_think_fn, pattern_search( "client.dll", "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87 ? ? ? ? C1 E8 16" ) );
	cs_dump_offset( cs_offsets.player_handle_fn, pattern_search( "client.dll", "8B 51 3C 83 FA FF" ) );
	cs_dump_offset( cs_offsets.player_create_animstate_fn, pattern_search( "client.dll", "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46" ) );
	cs_dump_offset( cs_offsets.player_set_abs_angles_fn, pattern_search( "client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8" ) );
	cs_dump_offset( cs_offsets.player_set_abs_origin_fn, pattern_search( "client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8 ? ? ? ? 8B 7D" ) );
	cs_dump_offset( cs_offsets.player_baseclass_post_data_update_fn, pattern_search( "client.dll", "55 8B EC 83 E4 F0 83 EC 1C 56 83 EC 08" ) );
	cs_dump_offset( cs_offsets.player_animstate, *( ptrdiff_t* )( pattern_search( "client.dll", "8B 8E ? ? ? ? F3 0F 10 48 04 E8 ? ? ? ? E9" ) + 2 ) );
	//cs_dump_offset ( cs_offsets.player_weapon_shootposition, pattern_rip ( pattern_search ( "client.dll", "57 E8 ? ? ? ? 8B 06 8B CE FF 90" ) + 1 ) );
	cs_dump_offset( cs_offsets.player_bone_count, *( ptrdiff_t* )( pattern_search( "client.dll", "8B 87 ? ? ? ? 8B 4D 0C" ) + 2 ) );
	cs_dump_offset( cs_offsets.player_bone_cache, *( ptrdiff_t* )( pattern_search( "client.dll", "FF B7 ? ? ? ? 52" ) + 2 ) );
	cs_dump_offset( cs_offsets.player_use_new_animstate, *( ptrdiff_t* )( pattern_search( "client.dll", "80 BE ? ? ? ? ? 74 36 8B" ) + 2 ) );
	cs_dump_offset( cs_offsets.player_is_local, *(ptrdiff_t*)(pattern_search("client.dll", "74 4C 8A 86") + 4));
	cs_dump_offset( cs_offsets.player_is_strafing, *( ptrdiff_t* )( pattern_search( "client.dll", "F3 0F 10 4C 24 ? 80 B9" ) + 8 ) );
	cs_dump_offset( cs_offsets.player_buttons, *( ptrdiff_t* )( pattern_search( "client.dll", "89 96 ? ? ? ? 23 C2" ) + 2 ) );

	cs_dump_offset( cs_offsets.sdk_view_matrix, *( ptrdiff_t* )( pattern_search( "client.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9" ) + 3 ) + 17 );
	cs_dump_offset( cs_offsets.sdk_game_rules, *( ptrdiff_t* )( pattern_search( "client.dll", "A1 ? ? ? ? 74 38" ) + 1 ) );
	cs_dump_offset( cs_offsets.sdk_dbgview, **( ptrdiff_t** )( pattern_search( "client.dll", "A1 ? ? ? ? F3 0F 11 44 24 ? 8B 30 8B" ) + 1 ) );
	cs_dump_offset( cs_offsets.sdk_is_valve_ds, 0x7C );

	cs_dump_offset( cs_offsets.econitem_account_id, 0x1C );
	cs_dump_offset( cs_offsets.econitem_icon_name, 0x198 );
	cs_dump_offset( cs_offsets.econitem_mdl_name, 0x94 );
	cs_dump_offset( cs_offsets.econitem_world_mdl_name, 0x9C );
	cs_dump_offset( cs_offsets.econitem_item_id, 0x8 );
	cs_dump_offset( cs_offsets.econitem_original_id, 0x10 );
	cs_dump_offset( cs_offsets.econitem_item_definition_index, 0x24 );
	cs_dump_offset( cs_offsets.econitem_inventory, 0x20 );
	cs_dump_offset( cs_offsets.econitem_flags, 0x30 );
	cs_dump_offset( cs_offsets.econitem_equipped_position, 0x248 );
	cs_dump_offset( cs_offsets.econitem_definition_name, 0x1BC );
	cs_dump_offset( cs_offsets.econitem_static_data_fn, pattern_search( "client.dll", "55 8B EC 51 53 8B D9 8B 0D ? ? ? ? 56 57 8B B9" ) );
	cs_dump_offset( cs_offsets.econitem_update_equipped_state_fn, pattern_search( "client.dll", "55 8B EC 8B 45 08 8B D0 C1 EA 10" ) );
	cs_dump_offset( cs_offsets.econitem_clean_inventory_image_cache_dir_fn, pattern_search( "client.dll", "55 8B EC 81 EC ? ? ? ? 80 3D ? ? ? ? ? 56 0F 85" ) );
	cs_dump_offset( cs_offsets.econitem_set_or_add_attribute_by_name_fn, pattern_search( "client.dll", "55 8B EC 83 EC 30 53 56 8B F1 F3" ) )
    cs_dump_offset( cs_offsets.econitem_set_custom_name_fn, pattern_rip( pattern_search( "client.dll", "E8 ? ? ? ? 8B 46 78 C1 E8 0A A8 01 74 13 8B 46 34" ) ) );
	cs_dump_offset( cs_offsets.econitem_set_custom_desc_fn, pattern_rip( pattern_search( "client.dll", "E8 ? ? ? ? 33 DB 39 5E 3C 7E 5E" ) ) );
	cs_dump_offset( cs_offsets.econitem_get_item_schema_fn, pattern_search( "client.dll", "A1 ? ? ? ? 85 C0 75 53" ) );
	cs_dump_offset( cs_offsets.econitem_set_dynamic_attribute_val_fn, pattern_search( "client.dll", "55 8B EC 83 E4 F8 83 EC 3C 53 8B 5D 08 56 57 6A 00" ) );
	cs_dump_offset( cs_offsets.econitem_soc_data_fn, pattern_search( "client.dll", "55 8B EC 83 E4 F0 83 EC 18 56 8B F1 57 8B 86" ) );

	cs_dump_offset( cs_offsets.weapon_sys, *( ptrdiff_t* )( pattern_search( "client.dll", "8B 35 ? ? ? ? FF 10 0F B7 C0" ) + 2 ) );

	cs_dump_offset( cs_offsets.accumulate_pose_fn, pattern_rip( pattern_search( "server.dll", "E8 ? ? ? ? 83 BF ? ? ? ? ? 0F 84 ? ? ? ? 8D" ) ) );

	cs_dump_offset( cs_offsets.cam_think_ret, pattern_search( "client.dll", "85 C0 75 30 38 86" ) );
	cs_dump_offset( cs_offsets.maintain_sequence_transitions_ret, pattern_search( "client.dll", "84 C0 74 17 8B 87" ) );
	cs_dump_offset( cs_offsets.loadout_allowed_ret, pattern_search( "client.dll", "75 04 B0 01 5F" ) - 2 );
	cs_dump_offset( cs_offsets.accumulate_layers_ret, pattern_search( "client.dll", "84 C0 75 0D F6 87" ) );
	cs_dump_offset( cs_offsets.reevauluate_anim_lod_ret, pattern_search( "client.dll", "84 C0 0F 85 ? ? ? ? A1 ? ? ? ? 8B B7" ) );
	cs_dump_offset( cs_offsets.list_leaves_in_box_ret, pattern_search( "client.dll", "56 52 FF 50 18" ) + 5 );

	cs_dump_offset( cs_offsets.enable_invalidate_bone_cache, *( ptrdiff_t* )( pattern_search( "client.dll", "C6 05 ? ? ? ? 00 F3 0F 5F 05 ? ? ? ? F3 0F 11 47 74" ) + 2 ) );

    return true;
}

static inline bool cs_init( sds* errors ) {
	*errors = sdsnew( "Cheat is outdated:\n" );

	memset( &cs_offsets, 0, sizeof( cs_offsets_s ) );

    /* offsets */
	cs_offsets_dump( errors );

	cs_dump_offset( cs_window, FindWindowA( NULL, "Counter-Strike: Global Offensive" ) );

	DWORD pid = 0;
	GetWindowThreadProcessId( cs_window, &pid );
	if ( pid != GetCurrentProcessId( ) ) {
		*errors = sdscat( *errors, "\tAttached to incorrect process.\n" );
		return false;
	}

    /* interfaces */
	cs_dump_offset( cs_iglobals, **( iglobals*** )( pattern_search( "client.dll", "A1 ? ? ? ? 5E 8B 40 10" ) + 1 ) );
	cs_dump_offset( cs_ientlist, cs_create_interface( "client.dll", "VClientEntityList003" ) );
	cs_dump_offset( cs_imatsys, cs_create_interface( "materialsystem.dll", "VMaterialSystem080" ) );
	cs_dump_offset( cs_imdlinfo, cs_create_interface( "engine.dll", "VModelInfoClient004" ) );
	cs_dump_offset( cs_imdlrender, cs_create_interface( "engine.dll", "VEngineModel016" ) );
	cs_dump_offset( cs_irenderview, cs_create_interface( "engine.dll", "VEngineRenderView014" ) );
	cs_dump_offset( cs_iclient, cs_create_interface( "client.dll", "VClient018" ) );
	cs_dump_offset( cs_isurface, cs_create_interface( "vguimatsurface.dll", "VGUI_Surface031" ) );
	cs_dump_offset( cs_ipanel, cs_create_interface( "vgui2.dll", "VGUI_Panel009" ) );
	cs_dump_offset( cs_iengine, cs_create_interface( "engine.dll", "VEngineClient014" ) );
	cs_dump_offset( cs_iphys, cs_create_interface( "vphysics.dll", "VPhysicsSurfaceProps001" ) );
	cs_dump_offset( cs_itrace, cs_create_interface( "engine.dll", "EngineTraceClient004" ) );
	cs_dump_offset( cs_ipred, cs_create_interface( "client.dll", "VClientPrediction001" ) );
	cs_dump_offset( cs_imove, cs_create_interface( "client.dll", "GameMovement001" ) );
	cs_dump_offset( cs_imdlcache, **( imdlcache*** )( pattern_search( "client.dll", "8B 35 ? ? ? ? 8B CE 8B 06 FF 90 ? ? ? ? 8B 4F" ) + 2 ) );
	cs_dump_offset( cs_ievents, cs_create_interface( "engine.dll", "GAMEEVENTSMANAGER002" ) );
	cs_dump_offset( cs_iinput, *( iinput** )( pattern_search( "client.dll", "B9 ? ? ? ? FF 60 60" ) + 1 ) );
	cs_dump_offset( cs_icvar, cs_create_interface( "vstdlib.dll", "VEngineCvar007" ) );
	cs_dump_offset( cs_imovehelper, **( imovehelper*** )( pattern_search( "client.dll", "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01" ) + 2 ) );
	cs_dump_offset( cs_iclientstate, **( iclientstate*** )( pattern_search( "engine.dll", "A1 ? ? ? ? 8B 88 ? ? ? ? 85 C9 75 07" ) + 1 ) );
	cs_dump_offset( cs_ibeams, *( ibeams** )( pattern_search( "client.dll", "A1 ? ? ? ? 56 8B F1 B9 ? ? ? ? FF 50 08" ) + 1 ) );
	cs_dump_offset( cs_imemalloc, *( imemalloc** )GetProcAddress( GetModuleHandleA( "tier0.dll" ), "g_pMemAlloc" ) );
	cs_dump_offset( cs_id3ddev, **( IDirect3DDevice9*** )( pattern_search( "shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C" ) + 1 ) );
	cs_dump_offset( cs_local_ptr, ( player** )( *( uintptr_t* )( pattern_search( "client.dll", "8D 34 85 ? ? ? ? 89 15 ? ? ? ? 8B 41 08 8B 48 04 83 F9 FF" ) + 3 ) + 4 ) );

    return true;
}

static inline bool cs_free( void ) {
	return true;
}

#endif // !SDK_H
