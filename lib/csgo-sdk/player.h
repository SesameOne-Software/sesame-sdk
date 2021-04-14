#ifndef SDK_PLAYER_H
#define SDK_PLAYER_H

#include "weapon.h"
#include "mat3x4.h"
#include "mdlinfo.h"
#include "entity.h"
#include "client.h"
#include "sdk.h"

#include "include/utils.h"

typedef struct player player;

typedef enum {
	fl_on_ground = ( 1 << 0 ),
	fl_ducking = ( 1 << 1 ),
	fl_water_jump = ( 1 << 2 ),
	fl_on_train = ( 1 << 3 ),
	fl_in_rain = ( 1 << 4 ),
	fl_frozen = ( 1 << 5 ),
	fl_at_controls = ( 1 << 6 ),
	fl_client = ( 1 << 7 ),
	fl_fake_client = ( 1 << 8 )
} move_flags;

typedef enum {
	hitbox_head = 0,
	hitbox_neck,
	hitbox_pelvis,
	hitbox_stomach,
	hitbox_thorax,
	hitbox_l_chest,
	hitbox_u_chest,
	hitbox_r_thigh,
	hitbox_l_thigh,
	hitbox_r_calf,
	hitbox_l_calf,
	hitbox_r_foot,
	hitbox_l_foot,
	hitbox_r_hand,
	hitbox_l_hand,
	hitbox_r_upperarm,
	hitbox_r_forearm,
	hitbox_l_upperarm,
	hitbox_l_forearm,
	hitbox_max_hitbox,
} hitboxes;

typedef enum {
	movetype_none = 0,
	movetype_isometric,
	movetype_walk,
	movetype_step,
	movetype_fly,
	movetype_flygravity,
	movetype_vphysics,
	movetype_push,
	movetype_noclip,
	movetype_ladder,
	movetype_observer,
	movetype_custom
} move_types;

// Generated using ReClass 2016
typedef struct {
	int init;
	uint32_t idx; //0x4
	char* name; //0x8
} animstate_pose_param_cache;

typedef struct {
	PAD( 4 );
	bool force_update; //0x0005
	PAD( 90 );
	player* player_ptr; //0x0060
	weapon* weapon_ptr; //0x0064
	weapon* last_weapon_ptr; //0x0068
	float last_clientside_anim_update; //0x006C
	uint32_t last_clientside_anim_framecount; //0x0070
	float last_clientside_anim_update_time_delta; //0x0074
	float eye_yaw; //0x0078
	float pitch; //0x007C
	float abs_yaw; //0x0080
	float feet_yaw; //0x0084
	float body_yaw; //0x0088
	float body_yaw_clamped; //0x008C
	float feet_vel_dir_delta; //0x0090
	PAD( 4 );
	float feet_cycle; //0x0098
	float feet_yaw_rate; //0x009C
	PAD( 4 );
	float duck_amount; //0x00A4
	float landing_duck_additive; //0x00A8
	PAD( 4 );
	vec3 origin; //0x00B0
	vec3 old_origin; //0x00BC
	vec3 vel; //0x00C8
	vec3 vel_norm; //0x00D4
	vec3 vel_norm_nonzero; //0x00E0
	float speed2d; //0x00EC
	float up_vel; //0x00F0
	float speed_normalized; //0x00F4
	float run_speed; //0x00F8
	float unk_feet_speed_ratio; //0x00FC
	float time_since_move; //0x0100
	float time_since_stop; //0x0104
	bool on_ground; //0x0108
	bool hit_ground; //0x0109
	PAD( 4 );
	float time_in_air; //0x0110
	PAD( 6 );
	float ground_fraction; //0x011C
	PAD( 2 );
	float unk_fraction; //0x0124
	PAD( 12 );
	bool moving; //0x0134
	PAD( 123 );
	animstate_pose_param_cache lean_yaw_pose; //0x1B0
	animstate_pose_param_cache speed_pose; //0x01BC
	animstate_pose_param_cache ladder_speed_pose; //0x01C8
	animstate_pose_param_cache ladder_yaw_pose; //0x01D4
	animstate_pose_param_cache move_yaw_pose; //0x01E0
	animstate_pose_param_cache run_pose; //0x01EC
	animstate_pose_param_cache body_yaw_pose; //0x01F8
	animstate_pose_param_cache body_pitch_pose; //0x0204
	animstate_pose_param_cache dead_yaw_pose; //0x0210
	animstate_pose_param_cache stand_pose; //0x021C
	animstate_pose_param_cache jump_fall_pose; //0x0228
	animstate_pose_param_cache aim_blend_stand_idle_pose; //0x0234
	animstate_pose_param_cache aim_blend_crouch_idle_pose; //0x0240
	animstate_pose_param_cache strafe_yaw_pose; //0x024C
	animstate_pose_param_cache aim_blend_stand_walk_pose; //0x0258
	animstate_pose_param_cache aim_blend_stand_run_pose; //0x0264
	animstate_pose_param_cache aim_blend_crouch_walk_pose; //0x0270
	animstate_pose_param_cache move_blend_walk_pose; //0x027C
	animstate_pose_param_cache move_blend_run_pose; //0x0288
	animstate_pose_param_cache move_blend_crouch_pose; //0x0294
	PAD( 4 );
	float vel_unk; //0x02A4
	PAD( 134 );
	float min_yaw; //0x0330
	float max_yaw; //0x0334
	float max_pitch; //0x0338
	float min_pitch; //0x033C
} animstate; //Size=0x0340

static inline void animstate_reset ( animstate* this ) {
	( ( void ( __fastcall* )( animstate*, void* ) )cs_offsets.animstate_reset_fn ) ( this, NULL );
}

static inline const char* animstate_get_weapon_move_animation ( animstate* this ) {
	return ( ( const char* ( __fastcall* )( animstate*, void* ) )cs_offsets.animstate_get_weapon_move_animation_fn ) ( this, NULL );
}

typedef struct {
	PAD( 20 );
	int	order;
	int	sequence;
	float previous_cycle;
	float weight;
	float weight_delta_rate;
	float playback_rate;
	float cycle;
	void* owner;
	PAD( 4 );
} animlayer;

NETVAR( player, ehandle, ground_entity_handle, "DT_BasePlayer->m_hGroundEntity" );
NETVAR( player, move_flags, flags, "DT_BasePlayer->m_fFlags" );
NETVAR( player, bool, has_defuser, "DT_CSPlayer->m_bHasDefuser" );
NETVAR( player, bool, immune, "DT_CSPlayer->m_bGunGameImmunity" );
NETVAR( player, vec3, angles, "DT_CSPlayer->m_angEyeAngles[0]" );
NETVAR( player, bool, has_heavy_armor, "DT_CSPlayer->m_bHasHeavyArmor" );
NETVAR( player, int, health, "DT_BasePlayer->m_iHealth" );
NETVAR( player, int, armor, "DT_CSPlayer->m_ArmorValue" );
NETVAR( player, bool, has_helmet, "DT_CSPlayer->m_bHasHelmet" );
NETVAR( player, bool, scoped, "DT_CSPlayer->m_bIsScoped" );
NETVAR( player, float, lby, "DT_CSPlayer->m_flLowerBodyYawTarget" );
NETVAR( player, float, flash_duration, "DT_CSPlayer->m_flFlashDuration" );
NETVAR( player, float, flash_alpha, "DT_CSPlayer->m_flFlashMaxAlpha" );
NETVAR( player, uint8_t, life_state, "DT_BasePlayer->m_lifeState" );
NETVAR( player, uint32_t, tick_base, "DT_BasePlayer->m_nTickBase" );
NETVAR( player, float, crouch_amount, "DT_BasePlayer->m_flDuckAmount" );
NETVAR( player, float, crouch_speed, "DT_BasePlayer->m_flDuckSpeed" );
NETVAR( player, vec3, view_punch, "DT_BasePlayer->m_viewPunchAngle" );
NETVAR( player, vec3, aim_punch, "DT_BasePlayer->m_aimPunchAngle" );
NETVAR( player, vec3, aim_punch_vel, "DT_BasePlayer->m_aimPunchAngleVel" );
NETVAR( player, vec3, vel, "DT_BasePlayer->m_vecVelocity[0]" );
NETVAR( player, bool, animate, "DT_BaseAnimating->m_bClientSideAnimation" );
NETVAR( player, ehandle, weapon_handle, "DT_BaseCombatCharacter->m_hActiveWeapon" );
NETVAR( player, ehandle*, weapon_handles, "DT_BaseCombatCharacter->m_hMyWeapons[0]" );
NETVAR( player, vec3, view_offset, "DT_BasePlayer->m_vecViewOffset[0]" );
NETVAR( player, vec3, base_vel, "DT_BasePlayer->m_vecBaseVelocity" );
NETVAR( player, float, fall_vel, "DT_BasePlayer->m_flFallVelocity" );
NETVAR( player, float, simtime, "DT_BaseEntity->m_flSimulationTime" );
NETVAR( player, vec3, mins, "DT_CSPlayer->m_vecMins" );
NETVAR( player, vec3, maxs, "DT_CSPlayer->m_vecMaxs" );
NETVAR( player, float, velocity_modifier, "DT_CSPlayer->m_flVelocityModifier" );
NETVAR( player, uint32_t, observer_mode, "DT_CSPlayer->m_iObserverMode" );
NETVAR( player, ehandle, ragdoll_handle, "DT_CSPlayer->m_hRagdoll" );
NETVAR( player, ehandle, viewmodel_handle, "DT_BasePlayer->m_hViewModel[0]" );
NETVAR( player, vec3, force, "DT_CSRagdoll->m_vecForce" );
NETVAR( player, vec3, ragdoll_vel, "DT_CSRagdoll->m_vecRagdollVelocity" );
NETVAR( player, float, next_attack, "DT_CSPlayer->m_flNextAttack" );
NETVAR( player, vec3, ladder_norm, "DT_CSPlayer->m_vecLadderNormal" );
NETVAR( player, ehandle*, wearable_handles, "DT_BaseCombatCharacter->m_hMyWearables" );
NETVAR( player, bool, strafing, "DT_CSPlayer->m_bStrafing" );
NETVAR( player, int, body, "DT_CSPlayer->m_nBody" );
OFFSET( player, uint8_t, movetype, cs_offsets.player_movetype );
OFFSET( player, void*, iks, cs_offsets.player_iks );
OFFSET( player, float, spawn_time, cs_offsets.player_spawn_time );
OFFSET( player, mat3x4a*, bones, cs_offsets.player_bones );
OFFSET( player, animlayer*, animlayers, cs_offsets.player_animlayers );
OFFSET( player, uint32_t, num_animlayers, cs_offsets.player_num_animlayers );
OFFSET( player, float, poses, cs_offsets.player_poses );

static inline void* player_seq_desc( player* this, int seq ) {
	void* group_hdr = *( void** )( ( uintptr_t )this + cs_offsets.player_group_hdr );
	int i = seq;

	if ( seq < 0 || seq >= *( uint32_t* )( ( uintptr_t )group_hdr + 0xBC ) )
		i = 0;

	return ( void* )( ( uintptr_t )group_hdr + *( uintptr_t* )( ( uintptr_t )group_hdr + 0xC0 ) + 0xD4 * i );
}

VIRTUAL( player, void, set_local_viewangles, cs_idx_player_set_local_viewangles, ( angles ), vec3* angles );
VIRTUAL( player, void, think, cs_idx_player_think, ( ) );
VIRTUAL( player, void, pre_think, cs_idx_player_pre_think, ( ) );
VIRTUAL( player, void, post_think, cs_idx_player_post_think, ( ) );

static inline bool player_physics_run_think ( player* this, int unk01 ) {
	return ( ( bool ( __fastcall* )( player*, void*, int ) )cs_offsets.player_physics_run_think_fn )( this, NULL, unk01 );
}

static inline vec3* player_world_space( player* this, vec3* out ) {
	vec3 va, vb;

	( ( void( __fastcall* )( renderable*, void*, vec3*, vec3* ) )utils_vfunc( entity_renderable( ( entity* )this ), cs_idx_player_world_space ) )( entity_renderable( ( entity* )this ), NULL, &va, &vb );

	vec3_zero( out );
	*out = *entity_origin( ( entity* )this );
	out->z += ( va.z + vb.z ) * 0.5f;

	return out;
}

static inline float* player_old_simtime( player* this ) {
	return ( float* )( ( uintptr_t )player_simtime( this ) + sizeof( float ) );
}

animstate* player_animstate( player* this );

static inline void* player_mdl( player* this ) {
	return ( ( void* ( __fastcall* )( renderable*, void* ) )utils_vfunc( entity_renderable( ( entity* )this ), cs_idx_player_mdl ) )( entity_renderable( ( entity* )this ), NULL );
}

static inline studiohdr* player_studio_mdl( player* this, void* mdl ) {
	return ( ( studiohdr * ( __fastcall* )( player*, void*, void* ) )utils_vfunc( this, player_studio_mdl ) )( this, NULL, mdl );
}

static inline bool player_is_alive( player* this ) {
	return player_health( this ) > 0;
}

VIRTUAL( player, vec3*, abs_origin, cs_idx_player_abs_origin, ( ) );
VIRTUAL( player, vec3*, abs_angles, cs_idx_player_abs_angles, ( ) );

static inline ehandle player_handle ( player* this ) {
	return ( ( ehandle ( __fastcall* )( player*, void* ) ) cs_offsets.player_handle_fn )( this, NULL );
}

static inline vec3* player_render_origin( player* this ) {
	return ( ( vec3 * ( __fastcall* )( renderable*, void* ) )utils_vfunc( entity_renderable( ( entity* )this ), cs_idx_player_render_origin ) )( entity_renderable( ( entity* )this ), NULL );
}

static inline void player_create_animstate ( player* this, animstate* state ) {
	( ( void ( __fastcall* )( player*, void*, animstate* ) )cs_offsets.player_create_animstate_fn )( state, NULL, this );
}

static inline void player_set_abs_angles ( player* this, vec3* ang ) {
	( ( void ( __fastcall* )( player*, void*, vec3* ) )cs_offsets.player_set_abs_angles_fn )( this, NULL, ang );
}

static inline void player_set_abs_origin ( player* this, vec3* vec ) {
	( ( void ( __fastcall* )( player*, void*, vec3* ) )cs_offsets.player_set_abs_origin_fn )( this, NULL, vec );
}

static inline animstate* player_animstate ( player* this ) {
	return *( animstate** ) ( ( uintptr_t ) this + cs_offsets.player_animstate );
}

static inline void player_get_eye_pos ( player* this, vec3* pos ) {
	vec3_init ( pos );

	/* eye position */
	( ( void ( __fastcall* )( player*, void*, vec3* ) )utils_vfunc ( this, cs_idx_player_get_shoot_pos ) ) ( this, NULL, pos );

	if ( *( bool* ) ( ( uintptr_t ) this + cs_offsets.player_is_local ) && player_animstate ( this ) )
		( ( void ( __fastcall* )( animstate*, void*, vec3* ) ) cs_offsets.player_get_eye_pos_fn ) ( player_animstate ( this ), NULL, pos );
}

static inline uint32_t* player_bone_count ( player* this ) {
	return ( uint32_t* ) ( ( uintptr_t ) entity_renderable ( ( entity* ) this ) + cs_offsets.player_bone_count );
}

static inline mat3x4* player_bone_cache ( player* this ) {
	return *( mat3x4** ) ( ( uintptr_t ) entity_renderable ( ( entity* ) this ) + cs_offsets.player_bone_cache );
}

static inline weapon* player_get_weapon ( player* this ) {
	extern struct ientlist* cs_ientlist;
	return ( weapon* ) ientlist_get_entity_from_handle ( cs_ientlist, player_weapon_handle ( this ) );
}

typedef weapon** vec_weapons;
typedef weapon** vec_wearables;

static inline vec_weapons player_weapons ( player* this ) {
	const ehandle* weapon_handles = player_weapon_handles ( this );
	vec_weapons ret = vector_create ( );

	for ( int i = 0; weapon_handles [ i ] != 0xFFFFFFFF; i++ ) {
		extern struct ientlist* cs_ientlist;
		const weapon* cur_weapon = ( weapon* ) ientlist_get_entity_from_handle ( cs_ientlist, weapon_handles [ i ] );

		if ( !cur_weapon )
			continue;

		vector_add ( &ret, weapon*, cur_weapon );
	}

	/* MAKE SURE TO CALL vector_free */
	return ret;
}

static inline vec_wearables player_wearables ( player* this ) {
	const ehandle* wearable_handles = player_wearable_handles ( this );
	vec_wearables ret = vector_create ( );

	for ( int i = 0; wearable_handles [ i ] != -1; i++ ) {
		extern struct ientlist* cs_ientlist;
		const weapon* cur_wearable = ( weapon* ) ientlist_get_entity_from_handle ( cs_ientlist, wearable_handles [ i ] );

		if ( !cur_wearable )
			continue;

		vector_add ( &ret, weapon*, cur_wearable );
	}

	/* MAKE SURE TO CALL vector_free */
	return ret;
}

static inline void player_get_sequence_linear_motion ( player* this, void* studio_hdr, int sequence, float* poses, vec3* vec ) {
	const ptrdiff_t fn = cs_offsets.player_get_sequence_linear_motion_fn;

	__asm {
		mov edx, sequence
		mov ecx, studio_hdr
		push vec
		push poses
		call fn
		add esp, 8
	}
}

static inline float player_get_sequence_move_distance ( player* this, void* studio_hdr, int sequence ) {
	vec3 ret;
	player_get_sequence_linear_motion ( this, studio_hdr, sequence, player_poses ( this ), &ret );
	return vec3_len ( &ret );
}

static inline int player_lookup_sequence ( player* this, const char* seq ) {
	return ( ( int ( __fastcall* )( player*, void*, const char* ) ) cs_offsets.player_lookup_sequence_fn ) ( this, NULL, seq );
}

static inline float player_sequence_duration ( player* this, int sequence ) {
	float retval;
	( ( float ( __fastcall* )( player*, void*, int ) )cs_offsets.player_sequence_duration_fn ) ( this, NULL, sequence );
	__asm movss retval, xmm0;
	return retval;
}

static inline float player_get_sequence_cycle_rate ( player* this, int sequence ) {
	float t = player_sequence_duration ( this, sequence );

	if ( t > 0.0f )
		return 1.0f / t;

	return 1.0f / 0.1f;
}

vec_weapons player_weapons( player* this );
vec_wearables player_wearables( player* this );

static inline float player_get_max_desync( player* this ) {
	animstate* state = player_animstate( this );

	if ( !state )
		return 0.0f;

	const float running_speed = clamp( state->run_speed, 0.0f, 1.0f );
	float yaw_modifier = ( ( ( state->ground_fraction * -0.3f ) - 0.2f ) * running_speed ) + 1.0f;

	if ( state->duck_amount > 0.0f ) {
		const float speed_factor = clamp( state->unk_feet_speed_ratio, 0.0f, 1.0f );
		yaw_modifier += ( ( state->duck_amount * speed_factor ) * ( 0.5f - yaw_modifier ) );
	}

	return yaw_modifier * state->max_yaw;
}

#endif // !SDK_PLAYER_H