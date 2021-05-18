#ifndef SDK_ANIMSTATE_H
#define SDK_ANIMSTATE_H

#include "weapon.h"
#include "mat3x4.h"
#include "mdlinfo.h"
#include "entity.h"
#include "utlvec.h"

#define ACT_INVALID -1
#define ANIMATION_LAYER_COUNT 13

#define CSGO_ANIM_LOWER_CATCHUP_IDLE 100.0f
#define CSGO_ANIM_AIM_NARROW_WALK 0.8f
#define CSGO_ANIM_AIM_NARROW_RUN 0.5f
#define CSGO_ANIM_AIM_NARROW_CROUCHMOVING 0.5f
#define CSGO_ANIM_LOWER_CATCHUP_WITHIN 3.0f
#define CSGO_ANIM_LOWER_REALIGN_DELAY 1.1f
#define CSGO_ANIM_READJUST_THRESHOLD 120.0f
#define EIGHT_WAY_WIDTH 22.5f

typedef enum {
	animlayer_aimmatrix = 0,
	animlayer_weapon_action,
	animlayer_weapon_action_recrouch,
	animlayer_adjust,
	animlayer_movement_jump_or_fall,
	animlayer_movement_land_or_climb,
	animlayer_movement_move,
	animlayer_movement_strafechange,
	animlayer_whole_body,
	animlayer_flashed,
	animlayer_flinch,
	animlayer_aliveloop,
	animlayer_lean,
	animlayer_count,
} animlayer_idx;

typedef enum {
	pose_param_first = 0,
	pose_param_lean_yaw = pose_param_first,
	pose_param_speed,
	pose_param_ladder_speed,
	pose_param_ladder_yaw,
	pose_param_move_yaw,
	pose_param_run,
	pose_param_body_yaw,
	pose_param_body_pitch,
	pose_param_death_yaw,
	pose_param_stand,
	pose_param_jump_fall,
	pose_param_aim_blend_stand_idle,
	pose_param_aim_blend_crouch_idle,
	pose_param_strafe_dir,
	pose_param_aim_blend_stand_walk,
	pose_param_aim_blend_stand_run,
	pose_param_aim_blend_crouch_walk,
	pose_param_move_blend_walk,
	pose_param_move_blend_run,
	pose_param_move_blend_crouch_walk,
	pose_param_strafe_cross,
	pose_param_count,
} pose_param_idx;

// Generated using ReClass 2016
typedef struct {
	int init;
	uint32_t idx;
	const char* name;
} animstate_pose_param_cache;

typedef struct {
	float duration_state_valid;
	float duration_state_invalid;
	float blend_in_wait;
	float blend_out_wait;
	float blend_value;
} aimmatrix_transition;

typedef struct {
	vec3 pos_anim;
	vec3 last_pos_anim;
	vec3 pos_plant;
	vec3 plant_vel;
	float lock_amount;
	float last_plant_time;
} procedural_foot;

typedef struct {
	float anim_time;
	float fade_out_time;
	studiohdr* dispatched_studiohdr;
	int dispatched_src;
	int dispatched_dst;
	int	order;
	int	sequence;
	float previous_cycle;
	float weight;
	float weight_delta_rate;
	float playback_rate;
	float cycle;
	void* owner;
	int invalidate_physics_bit;
} animlayer;

/* client animstate */
typedef struct {
	void* vtable;
	bool first_run;
	bool first_foot_plant;
	int last_update_frame;
	float eye_pos_lerp;
	float strafe_change_weight_lerp;
	aimmatrix_transition stand_walk_aim;
	aimmatrix_transition stand_run_aim;
	aimmatrix_transition crouch_walk_aim;
	int model_idx;
	float step_height_left;
	float step_height_right;
	weapon* weapon_last_bone_setup;
	player* player;
	weapon* weapon;
	weapon* last_weapon;
	float last_update_time;
	int last_update_frame;
	float last_update_delta_time;
	float eye_yaw;
	float eye_pitch;
	float foot_yaw;
	float last_foot_yaw;
	float move_yaw;
	float ideal_move_yaw;
	float move_yaw_to_ideal;
	float time_to_align_lby;
	float primary_cycle;
	float move_weight;
	float move_weight_lerp;
	float duck_amount;
	float duck_additional;
	float recrouch_weight;
	vec3 pos;
	vec3 last_pos;
	vec3 vel;
	vec3 vel_norm;
	vec3 vel_norm_nonzero;
	float vel_len2d;
	float vel_lenz;
	float speed_to_run_fraction;
	float speed_to_walk_fraction;
	float speed_to_crouch_fraction;
	float moving_time;
	float still_time;
	bool on_ground;
	bool landing;
	float jump_to_fall;
	float air_time;
	float left_ground_height;
	float land_anim_factor;
	float walk_to_run_fraction;
	bool just_landed;
	bool just_jumped;
	float air_lerp;
	bool on_ladder;
	float ladder_weight;
	float ladder_speed;
	bool walk_to_run_state;
	bool in_defusing;
	bool in_planting;
	bool in_twitching;
	bool in_adjust;
	utlvector activity_modifiers;
	float next_twitch_time;
	float last_injury_time;
	float last_vel_test_time;
	vec3 last_vel;
	vec3 target_accel;
	vec3 accel;
	float accel_weight;
	float aim_matrix_fraction;
	float aim_matrix_fraction_delay;
	bool flashed;
	float strafe_change_weight;
	float strafe_change_target_weight;
	float strafe_change_cycle;
	int strafe_seq;
	bool strafe_changing;
	float strafe_time;
	float foot_lerp;
	bool feet_crossed;
	bool in_accel;
	animstate_pose_param_cache pose_param_map[pose_param_count];
	float move_weight_time_too_high;
	float static_lerp_speed;
	int last_move_state;
	float stutter_step;
	float action_weight_bias_remainder;
	procedural_foot	foot_left;
	procedural_foot	foot_right;
	float camera_smooth_height;
	bool smooth_height_valid;
	float last_time_vel_over_10;
	float aim_yaw_min;
	float aim_yaw_max;
	float aim_pitch_min;
	float aim_pitch_max;
	int animstate_version;
} animstate;

/*
*	@amizu03 ~
*	By doing it this way, we can keep the client animstate when we want the game to use it,
*	as well as keeping the values of the animstate members from the server; we solve two problems at once.
*/
typedef struct {
	/* client animstate */
	animstate base;
	
	/* members that only exist on the server */
	float flash_start_time;
	float flash_end_time;
	bool jumping;
	float lby_realign_timer;
	bool deploy_rate_limiting;
} server_animstate;

static inline bool server_animstate_cache_sequences ( server_animstate* this ) {
	return ( ( bool ( __fastcall* )( animstate*, void* ) )cs_offsets.animstate_cache_sequences_fn ) ( &this->base, NULL );
}

static inline void server_animstate_reset(server_animstate* this) {
	((void(__fastcall*)(animstate*, void*))cs_offsets.animstate_reset_fn) (&this->base, NULL);
}

static inline const char* server_animstate_get_weapon_move_animation(server_animstate* this) {
	return ((const char* (__fastcall*)(animstate*, void*))cs_offsets.animstate_get_weapon_move_animation_fn) (&this->base, NULL);
}

static inline float server_animstate_get_max_desync(server_animstate* this) {
	if (!this || !this->base.player )
		return 0.0f;

	float aim_matrix_width = lerpf ( clampf ( this->base.speed_to_walk_fraction, 0.0f, 1.0f ), 1.0f, lerpf ( this->base.walk_to_run_fraction, CSGO_ANIM_AIM_NARROW_WALK, CSGO_ANIM_AIM_NARROW_RUN ) );

	if ( this->base.duck_amount > 0.0f )
		aim_matrix_width = lerpf ( this->base.duck_amount * clampf ( this->base.speed_to_crouch_fraction, 0.0f, 1.0f ), aim_matrix_width, CSGO_ANIM_AIM_NARROW_CROUCHMOVING );
	
	return aim_matrix_width * this->base.aim_yaw_max;
}



void server_animstate_update ( server_animstate* this, vec3* ang );

#endif SDK_ANIMSTATE_H