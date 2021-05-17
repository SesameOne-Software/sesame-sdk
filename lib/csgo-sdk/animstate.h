#ifndef SDK_ANIMSTATE_H
#define SDK_ANIMSTATE_H

#include "weapon.h"
#include "mat3x4.h"
#include "mdlinfo.h"
#include "entity.h"
#include "utlvec.h"

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
	player* cur_player;
	weapon* cur_weapon;
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

	float	m_flPrimaryCycle;
	float	m_flMoveWeight;
	float	m_flMoveWeightSmoothed;
	float	m_flAnimDuckAmount;
	float	m_flDuckAdditional;
	float	m_flRecrouchWeight;

	vec3	m_vecPositionCurrent;
	vec3	m_vecPositionLast;

	vec3	m_vecVelocity;
	vec3	m_vecVelocityNormalized;
	vec3	m_vecVelocityNormalizedNonZero;
	float	m_flVelocityLengthXY;
	float	m_flVelocityLengthZ;

	float	m_flSpeedAsPortionOfRunTopSpeed;
	float	m_flSpeedAsPortionOfWalkTopSpeed;
	float	m_flSpeedAsPortionOfCrouchTopSpeed;

	float	m_flDurationMoving;
	float	m_flDurationStill;

	bool	m_bOnGround;
	bool	m_bLanding;
	float	m_flJumpToFall;
	float	m_flDurationInAir;
	float	m_flLeftGroundHeight;
	float	m_flLandAnimMultiplier;

	float	m_flWalkToRunTransition;

	bool	m_bLandedOnGroundThisFrame;
	bool	m_bLeftTheGroundThisFrame;
	float	m_flInAirSmoothValue;

	bool	m_bOnLadder;
	float	m_flLadderWeight;
	float	m_flLadderSpeed;

	bool	m_bWalkToRunTransitionState;

	bool	m_bDefuseStarted;
	bool	m_bPlantAnimStarted;
	bool	m_bTwitchAnimStarted;
	bool	m_bAdjustStarted;

	utlvector	m_ActivityModifiers;

	float					m_flNextTwitchTime;

	float					m_flTimeOfLastKnownInjury;

	float					m_flLastVelocityTestTime;
	vec3					m_vecVelocityLast;
	vec3					m_vecTargetAcceleration;
	vec3					m_vecAcceleration;
	float					m_flAccelerationWeight;

	float					m_flAimMatrixTransition;
	float					m_flAimMatrixTransitionDelay;

	bool					m_bFlashed;

	float					m_flStrafeChangeWeight;
	float					m_flStrafeChangeTargetWeight;
	float					m_flStrafeChangeCycle;
	int						m_nStrafeSequence;
	bool					m_bStrafeChanging;
	float					m_flDurationStrafing;

	float					m_flFootLerp;

	bool					m_bFeetCrossed;

	bool					m_bPlayerIsAccelerating;

	animstate_pose_param_cache m_tPoseParamMappings[pose_param_count];

	float					m_flDurationMoveWeightIsTooHigh;
	float					m_flStaticApproachSpeed;

	int						m_nPreviousMoveState;
	float					m_flStutterStep;

	float					m_flActionWeightBiasRemainder;

	procedural_foot	m_footLeft;
	procedural_foot	m_footRight;

	float					m_flCameraSmoothHeight;
	bool					m_bSmoothHeightValid;
	float					m_flLastTimeVelocityOverTen;

	float					m_flAimYawMin;
	float					m_flAimYawMax;
	float					m_flAimPitchMin;
	float					m_flAimPitchMax;
	int						m_nAnimstateModelVersion;
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
	float m_flFlashedAmountEaseOutStart;
	float m_flFlashedAmountEaseOutEnd;
	bool m_bJumping;
	float m_flLowerBodyRealignTimer;
	bool m_bDeployRateLimiting;
} server_animstate;

static inline void server_animstate_reset(server_animstate* this) {
	((void(__fastcall*)(animstate*, void*))cs_offsets.animstate_reset_fn) (&this->base, NULL);
}

static inline const char* server_animstate_get_weapon_move_animation(server_animstate* this) {
	return ((const char* (__fastcall*)(animstate*, void*))cs_offsets.animstate_get_weapon_move_animation_fn) (&this->base, NULL);
}

static inline float server_animstate_get_max_desync(server_animstate* this) {
	if (!this || !this->player_ptr)
		return 0.0f;

	const float running_speed = clampf(this->run_speed, 0.0f, 1.0f);
	float yaw_modifier = (((this->ground_fraction * -0.3f) - 0.2f) * running_speed) + 1.0f;

	if (this->duck_amount > 0.0f) {
		const float speed_factor = clampf(this->unk_feet_speed_ratio, 0.0f, 1.0f);
		yaw_modifier += ((this->duck_amount * speed_factor) * (0.5f - yaw_modifier));
	}

	return yaw_modifier * this->max_yaw;
}

#endif SDK_ANIMSTATE_H