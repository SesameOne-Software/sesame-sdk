#ifndef SDK_PLAYER_H
#define SDK_PLAYER_H

#include "weapon.h"
#include "mat3x4.h"
#include "mdlinfo.h"
#include "entity.h"
#include "client.h"

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
	PAD ( 4 );
	bool force_update; //0x0005
	PAD ( 90 );
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
	PAD ( 4 );
	float feet_cycle; //0x0098
	float feet_yaw_rate; //0x009C
	PAD ( 4 );
	float duck_amount; //0x00A4
	float landing_duck_additive; //0x00A8
	PAD ( 4 );
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
	PAD ( 4 );
	float time_in_air; //0x0110
	PAD ( 6 );
	float ground_fraction; //0x011C
	PAD ( 2 );
	float unk_fraction; //0x0124
	PAD ( 12 );
	bool moving; //0x0134
	PAD ( 123 );
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
	PAD ( 4 );
	float vel_unk; //0x02A4
	PAD ( 134 );
	float min_yaw; //0x0330
	float max_yaw; //0x0334
	float max_pitch; //0x0338
	float min_pitch; //0x033C
} animstate; //Size=0x0340

void animstate_reset ( animstate* this );
const char* animstate_get_weapon_move_animation ( animstate* this );

struct animlayer {
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
};

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
NETVAR( player, vec3 , aim_punch_vel , "DT_BasePlayer->m_aimPunchAngleVel" );
NETVAR( player, vec3, vel, "DT_BasePlayer->m_vecVelocity[0]" );
NETVAR( player, bool, animate, "DT_BaseAnimating->m_bClientSideAnimation" );
NETVAR( player, ehandle, weapon_handle, "DT_BaseCombatCharacter->m_hActiveWeapon" );
NETVAR ( player,ehandle*, weapon_handles, "DT_BaseCombatCharacter->m_hMyWeapons[0]" );
NETVAR( player, vec3, view_offset, "DT_BasePlayer->m_vecViewOffset[0]" );
NETVAR( player, vec3 , base_vel , "DT_BasePlayer->m_vecBaseVelocity" );
NETVAR( player, float , fall_vel , "DT_BasePlayer->m_flFallVelocity" );
NETVAR( player, float, simtime, "DT_BaseEntity->m_flSimulationTime" );
NETVAR( player, vec3, mins, "DT_CSPlayer->m_vecMins" );
NETVAR( player, vec3, maxs, "DT_CSPlayer->m_vecMaxs" );
NETVAR( player, float , velocity_modifier , "DT_CSPlayer->m_flVelocityModifier" );
NETVAR( player, uint32_t, observer_mode, "DT_CSPlayer->m_iObserverMode" );
NETVAR( player, ehandle, ragdoll_handle, "DT_CSPlayer->m_hRagdoll" );
NETVAR( player, ehandle, viewmodel_handle, "DT_BasePlayer->m_hViewModel[0]" );
NETVAR( player, vec3, force, "DT_CSRagdoll->m_vecForce" );
NETVAR( player, vec3, ragdoll_vel, "DT_CSRagdoll->m_vecRagdollVelocity" );
NETVAR( player, float, next_attack, "DT_CSPlayer->m_flNextAttack" );
NETVAR ( player,vec3, ladder_norm, "DT_CSPlayer->m_vecLadderNormal" );
NETVAR ( player,ehandle*, wearable_handles, "DT_BaseCombatCharacter->m_hMyWearables" );
NETVAR ( player,bool, strafing, "DT_CSPlayer->m_bStrafing" );
NETVAR ( player,int, body, "DT_CSPlayer->m_nBody" );
OFFSET( player, uint8_t, movetype, 0x25C );
OFFSET( player,void*, iks, 0x266C );
OFFSET( player,float, spawn_time, 0xA370 );
OFFSET( player,mat3x4a*, bones, 0x26A8 );
OFFSET( player,animlayer*, animlayers, 0x2980 );
OFFSET( player,uint32_t, num_animlayers, 0x298C );
OFFSET( player,float, poses, 0x2774 );

static inline void* player_seq_desc( player* this, int seq ) {
	void* group_hdr = *(void** )( (uintptr_t)this + 0xA53 );
	int i = seq;

	if ( seq < 0 || seq >= *(uint32_t* )( (uintptr_t)group_hdr + 0xBC ) )
		i = 0;

	return (void* )( (uintptr_t)group_hdr + *(uintptr_t* )( (uintptr_t)group_hdr + 0xC0 ) + 0xD4 * i );
}

VIRTUAL ( player, void, set_local_viewangles, 371, ( this, angles ), vec3* angles );
VIRTUAL ( player, void, think, 138, ( this ) );
VIRTUAL ( player, void, pre_think, 317, ( this ) );
VIRTUAL ( player, void, post_think, 318, ( this ) );

bool player_physics_run_think( player* this, int unk01 );

static inline vec3 player_world_space(player* this, ) {
	vec3 wspace;
	vec3 va, vb;

	((void( __thiscall* )( renderable*, vec3*, vec3* ))utils_get_vfunc( entity_renderable((entity*)this), 17 ))( entity_renderable((entity*)this), &va, &vb );
	
	wspace = *entity_origin((entity*)this );
	wspace.z += ( va.z + vb.z ) * 0.5f;

	return wspace;
}

static inline float* player_old_simtime(player* this) {
	return (float*)((uintptr_t)player_simtime(this) + sizeof(float)));
}

animstate* player_animstate( player* this );

static inline void* player_mdl(player* this ) {
	return ((void* ( __thiscall* )( renderable* ))utils_get_vfunc( entity_renderable((entity*)this ), 8 ))( entity_renderable((entity*)this ) );
}

static inline studiohdr_t* player_studio_mdl( player* this, void* mdl ) {
	return ((studiohdr*( __thiscall* )( player*, void* ))utils_get_vfunc( this, 32 ))( this, mdl );
}

static inline bool player_is_alive(player* this){
	return player_health(this) > 0;
}

VIRTUAL ( player, vec3*, abs_origin, 10, ( this ) );
VIRTUAL ( player, vec3*, abs_angles, 11, ( this ) );

ehandle player_handle( player* this );

static inline vec3* player_mdl(player* this ) {
	return ((vec3* ( __thiscall* )( renderable* ))utils_get_vfunc( entity_renderable((entity*)this ), 1 ))( entity_renderable((entity*)this ) );
}

void player_create_animstate( player* this, animstate* state );
void player_set_abs_angles( player* this, vec3* ang );
void player_set_abs_origin( player* this, vec3* vec );
void player_get_eye_pos( player* this, vec3* pos);
uint32_t* player_bone_count( player* this);
mat3x4* player_bone_cache( player* this);
weapon* player_get_weapon( player* this );

typedef weapon** vec_weapons; 
typedef weapon** vec_wearables; 

vec_weapons player_weapons (player* this );
vec_wearables player_wearables ( player* this);

class player_t : public entity_t {
public:
	float desync_amount( ) {
		auto state = animstate( );

		if ( !state )
			return 0.0f;

		auto running_speed = std::clamp( state->m_run_speed, 0.0f, 1.0f );
		auto yaw_modifier = ( ( ( state->m_ground_fraction * -0.3f ) - 0.2f ) * running_speed ) + 1.0f;

		if ( state->m_duck_amount > 0.0f ) {
			auto speed_factor = std::clamp( state->m_unk_feet_speed_ratio, 0.0f, 1.0f );
			yaw_modifier += ( ( state->m_duck_amount * speed_factor ) * ( 0.5f - yaw_modifier ) );
		}

		return yaw_modifier * state->m_max_yaw;
	}

	void get_sequence_linear_motion ( void* studio_hdr, int sequence, float* poses, vec3_t* vec );
	float get_sequence_move_distance ( void* studio_hdr, int sequence );
	int lookup_sequence ( const char* seq );
	float sequence_duration ( int sequence );
	float get_sequence_cycle_rate_server ( int sequence );
};

#endif // !SDK_PLAYER_H