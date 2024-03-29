﻿#ifndef SDK_PLAYER_H
#define SDK_PLAYER_H

#include "weapon.h"
#include "mat3x4.h"
#include "mdlinfo.h"
#include "entity.h"
#include "client.h"
#include "sdk.h"
#include "utlvec.h"

#include "utils.h"

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

/* adjust some code in animation update function to make it more straightforward to use */
#define SES_ANIMSTATE_ADJUSTMENTS

#define ANIMATION_LAYER_COUNT 13

#define ANIM_LAYER_ACTIVE 1
#define ANIM_TRANSITION_WALK_TO_RUN 0.0f
#define ANIM_TRANSITION_RUN_TO_WALK 1.0f

#define CS_PLAYER_SPEED_STOPPED 1.0f
#define CS_PLAYER_SPEED_RUN 260.0f
#define CS_PLAYER_SPEED_WALK_MODIFIER 0.52f
#define CS_PLAYER_SPEED_DUCK_MODIFIER 0.34f

#define CSGO_ANIM_AIMMATRIX_DEFAULT_YAW_MAX 58.0f
#define CSGO_ANIM_AIMMATRIX_DEFAULT_YAW_MIN -58.0f
#define CSGO_ANIM_AIMMATRIX_DEFAULT_PITCH_MAX 90.0f
#define CSGO_ANIM_AIMMATRIX_DEFAULT_PITCH_MIN -90.0f

#define CSGO_ANIM_LOWER_CATCHUP_IDLE 100.0f
#define CSGO_ANIM_AIM_NARROW_WALK 0.8f
#define CSGO_ANIM_AIM_NARROW_RUN 0.5f
#define CSGO_ANIM_AIM_NARROW_CROUCHMOVING 0.5f
#define CSGO_ANIM_LOWER_CATCHUP_WITHIN 3.0f
#define CSGO_ANIM_LOWER_REALIGN_DELAY 1.1f
#define CSGO_ANIM_READJUST_THRESHOLD 120.0f
#define EIGHT_WAY_WIDTH 22.5f

#define CSGO_ANIM_DEPLOY_RATELIMIT 0.15f

#define CSGO_ANIM_DUCK_APPROACH_SPEED_DOWN 3.1f
#define CSGO_ANIM_DUCK_APPROACH_SPEED_UP 6.0f

#define CSGO_ANIM_WALK_TO_RUN_TRANSITION_SPEED 2.0f
#define CSGO_ANIM_ONGROUND_FUZZY_APPROACH 8.0f
#define CSGO_ANIM_ONGROUND_FUZZY_APPROACH_CROUCH 16.0f
#define CSGO_ANIM_LADDER_CLIMB_COVERAGE 100.0f
#define CSGO_ANIM_RUN_ANIM_PLAYBACK_MULTIPLIER 0.85f

#define MAX_ANIMSTATE_ANIMNAME_CHARS 64

typedef enum {
	act_invalid = -1,			// so we have something more succint to check for than '-1'
	act_reset = 0,				// set m_activity to this invalid value to force a reset to m_idealactivity
	act_idle,
	act_transition,
	act_cover,					// fixme: obsolete? redundant with act_cover_low?
	act_cover_med,				// fixme: unsupported?
	act_cover_low,				// fixme: rename act_idle_crouch?
	act_walk,
	act_walk_aim,
	act_walk_crouch,
	act_walk_crouch_aim,
	act_run,
	act_run_aim,
	act_run_crouch,
	act_run_crouch_aim,
	act_run_protected,
	act_script_custom_move,
	act_range_attack1,
	act_range_attack2,
	act_range_attack1_low,		// fixme: not used yet, crouched versions of the range attack
	act_range_attack2_low,		// fixme: not used yet, crouched versions of the range attack
	act_diesimple,
	act_diebackward,
	act_dieforward,
	act_dieviolent,
	act_dieragdoll,
	act_fly,				// fly (and flap if appropriate)
	act_hover,
	act_glide,
	act_swim,
	act_jump,
	act_hop,				// vertical jump
	act_leap,				// long forward jump
	act_land,
	act_climb_up,
	act_climb_down,
	act_climb_dismount,
	act_shipladder_up,
	act_shipladder_down,
	act_strafe_left,
	act_strafe_right,
	act_roll_left,			// tuck and roll, left
	act_roll_right,			// tuck and roll, right
	act_turn_left,			// turn quickly left (stationary)
	act_turn_right,			// turn quickly right (stationary)
	act_crouch,				// fixme: obsolete? only used be soldier (the act of crouching down from a standing position)
	act_crouchidle,			// fixme: obsolete? only used be soldier (holding body in crouched position (loops))
	act_stand,				// fixme: obsolete? should be transition (the act of standing from a crouched position)
	act_use,
	act_alien_burrow_idle,
	act_alien_burrow_out,

	act_signal1,
	act_signal2,
	act_signal3,

	act_signal_advance,		// squad handsignals, specific.
	act_signal_forward,
	act_signal_group,
	act_signal_halt,
	act_signal_left,
	act_signal_right,
	act_signal_takecover,

	act_lookback_right,		// look back over shoulder without turning around.
	act_lookback_left,
	act_cower,				// fixme: unused, should be more extreme version of crouching
	act_small_flinch,		// fixme: needed? shouldn't flinching be down with overlays?
	act_big_flinch,
	act_melee_attack1,
	act_melee_attack2,
	act_reload,
	act_reload_start,
	act_reload_finish,
	act_reload_low,
	act_arm,				// pull out gun, for instance
	act_disarm,				// reholster gun
	act_drop_weapon,
	act_drop_weapon_shotgun,
	act_pickup_ground,		// pick up something in front of you on the ground
	act_pickup_rack,		// pick up something from a rack or shelf in front of you.
	act_idle_angry,			// fixme: being used as an combat ready idle?  alternate idle animation in which the monster is clearly agitated. (loop)

	act_idle_relaxed,
	act_idle_stimulated,
	act_idle_agitated,
	act_idle_stealth,
	act_idle_hurt,

	act_walk_relaxed,
	act_walk_stimulated,
	act_walk_agitated,
	act_walk_stealth,

	act_run_relaxed,
	act_run_stimulated,
	act_run_agitated,
	act_run_stealth,

	act_idle_aim_relaxed,
	act_idle_aim_stimulated,
	act_idle_aim_agitated,
	act_idle_aim_stealth,

	act_walk_aim_relaxed,
	act_walk_aim_stimulated,
	act_walk_aim_agitated,
	act_walk_aim_stealth,

	act_run_aim_relaxed,
	act_run_aim_stimulated,
	act_run_aim_agitated,
	act_run_aim_stealth,

	act_crouchidle_stimulated,
	act_crouchidle_aim_stimulated,
	act_crouchidle_agitated,

	act_walk_hurt,			// limp  (loop)
	act_run_hurt,			// limp  (loop)
	act_special_attack1,	// very monster specific special attacks.
	act_special_attack2,
	act_combat_idle,		// fixme: unused?  agitated idle.
	act_walk_scared,
	act_run_scared,
	act_victory_dance,		// killed a player, do a victory dance.
	act_die_headshot,		// die, hit in head. 
	act_die_chestshot,		// die, hit in chest
	act_die_gutshot,		// die, hit in gut
	act_die_backshot,		// die, hit in back
	act_flinch_head,
	act_flinch_chest,
	act_flinch_stomach,
	act_flinch_leftarm,
	act_flinch_rightarm,
	act_flinch_leftleg,
	act_flinch_rightleg,
	act_flinch_physics,
	act_flinch_head_back,
	act_flinch_head_left,
	act_flinch_head_right,
	act_flinch_chest_back,
	act_flinch_stomach_back,
	act_flinch_crouch_front,
	act_flinch_crouch_back,
	act_flinch_crouch_left,
	act_flinch_crouch_right,

	act_idle_on_fire,		// on fire animations
	act_walk_on_fire,
	act_run_on_fire,

	act_rappel_loop,		// rappel down a rope!

	act_180_left,			// 180 degree left turn
	act_180_right,

	act_90_left,			// 90 degree turns
	act_90_right,

	act_step_left,			// single steps
	act_step_right,
	act_step_back,
	act_step_fore,

	act_gesture_range_attack1,
	act_gesture_range_attack2,
	act_gesture_melee_attack1,
	act_gesture_melee_attack2,
	act_gesture_range_attack1_low,	// fixme: not used yet, crouched versions of the range attack
	act_gesture_range_attack2_low,	// fixme: not used yet, crouched versions of the range attack

	act_melee_attack_swing_gesture,

	act_gesture_small_flinch,
	act_gesture_big_flinch,
	act_gesture_flinch_blast,			// startled by an explosion
	act_gesture_flinch_blast_shotgun,
	act_gesture_flinch_blast_damaged,	// damaged by an explosion
	act_gesture_flinch_blast_damaged_shotgun,
	act_gesture_flinch_head,
	act_gesture_flinch_chest,
	act_gesture_flinch_stomach,
	act_gesture_flinch_leftarm,
	act_gesture_flinch_rightarm,
	act_gesture_flinch_leftleg,
	act_gesture_flinch_rightleg,

	act_gesture_turn_left,
	act_gesture_turn_right,
	act_gesture_turn_left45,
	act_gesture_turn_right45,
	act_gesture_turn_left90,
	act_gesture_turn_right90,
	act_gesture_turn_left45_flat,
	act_gesture_turn_right45_flat,
	act_gesture_turn_left90_flat,
	act_gesture_turn_right90_flat,

	// half-life 1 compatability stuff goes here. temporary!
	act_barnacle_hit,		// barnacle tongue hits a monster
	act_barnacle_pull,		// barnacle is lifting the monster ( loop )
	act_barnacle_chomp,		// barnacle latches on to the monster
	act_barnacle_chew,		// barnacle is holding the monster in its mouth ( loop )

	// sometimes, you just want to set an npc's sequence to a sequence that doesn't actually
	// have an activity. the ai will reset the npc's sequence to whatever its ideal activity
	// is, though. so if you set ideal activity to do_not_disturb, the ai will not interfere
	// with the npc's current sequence. (sjb)
	act_do_not_disturb,

	act_specific_sequence,

	// viewmodel (weapon) activities
	// fixme: move these to the specific viewmodels, no need to make global
	act_vm_draw,
	act_vm_holster,
	act_vm_idle,
	act_vm_fidget,
	act_vm_pullback,
	act_vm_pullback_high,
	act_vm_pullback_low,
	act_vm_throw,
	act_vm_pullpin,
	act_vm_primaryattack,		// fire
	act_vm_secondaryattack,		// alt. fire
	act_vm_reload,
	act_vm_dryfire,				// fire with no ammo loaded.
	act_vm_hitleft,				// bludgeon, swing to left - hit (primary attk)
	act_vm_hitleft2,			// bludgeon, swing to left - hit (secondary attk)
	act_vm_hitright,			// bludgeon, swing to right - hit (primary attk)
	act_vm_hitright2,			// bludgeon, swing to right - hit (secondary attk)
	act_vm_hitcenter,			// bludgeon, swing center - hit (primary attk)
	act_vm_hitcenter2,			// bludgeon, swing center - hit (secondary attk)
	act_vm_missleft,			// bludgeon, swing to left - miss (primary attk)
	act_vm_missleft2,			// bludgeon, swing to left - miss (secondary attk)
	act_vm_missright,			// bludgeon, swing to right - miss (primary attk)
	act_vm_missright2,			// bludgeon, swing to right - miss (secondary attk)
	act_vm_misscenter,			// bludgeon, swing center - miss (primary attk)
	act_vm_misscenter2,			// bludgeon, swing center - miss (secondary attk)
	act_vm_haulback,			// bludgeon, haul the weapon back for a hard strike (secondary attk)
	act_vm_swinghard,			// bludgeon, release the hard strike (secondary attk)
	act_vm_swingmiss,
	act_vm_swinghit,
	act_vm_idle_to_lowered,
	act_vm_idle_lowered,
	act_vm_lowered_to_idle,
	act_vm_recoil1,
	act_vm_recoil2,
	act_vm_recoil3,
	act_vm_pickup,
	act_vm_release,

	act_vm_attach_silencer,
	act_vm_detach_silencer,

	act_vm_empty_fire,			// fire last round in magazine
	act_vm_empty_reload,        // reload from an empty state
	act_vm_empty_draw,			// deploy an empty weapon
	act_vm_empty_idle,			// idle in an empty state

//===========================
// hl2 specific activities
//===========================
	// slam	specialty activities
	act_slam_stickwall_idle,
	act_slam_stickwall_nd_idle,
	act_slam_stickwall_attach,
	act_slam_stickwall_attach2,
	act_slam_stickwall_nd_attach,
	act_slam_stickwall_nd_attach2,
	act_slam_stickwall_detonate,
	act_slam_stickwall_detonator_holster,
	act_slam_stickwall_draw,
	act_slam_stickwall_nd_draw,
	act_slam_stickwall_to_throw,
	act_slam_stickwall_to_throw_nd,
	act_slam_stickwall_to_tripmine_nd,
	act_slam_throw_idle,
	act_slam_throw_nd_idle,
	act_slam_throw_throw,
	act_slam_throw_throw2,
	act_slam_throw_throw_nd,
	act_slam_throw_throw_nd2,
	act_slam_throw_draw,
	act_slam_throw_nd_draw,
	act_slam_throw_to_stickwall,
	act_slam_throw_to_stickwall_nd,
	act_slam_throw_detonate,
	act_slam_throw_detonator_holster,
	act_slam_throw_to_tripmine_nd,
	act_slam_tripmine_idle,
	act_slam_tripmine_draw,
	act_slam_tripmine_attach,
	act_slam_tripmine_attach2,
	act_slam_tripmine_to_stickwall_nd,
	act_slam_tripmine_to_throw_nd,
	act_slam_detonator_idle,
	act_slam_detonator_draw,
	act_slam_detonator_detonate,
	act_slam_detonator_holster,
	act_slam_detonator_stickwall_draw,
	act_slam_detonator_throw_draw,

	// shotgun specialty activities
	act_shotgun_reload_start,
	act_shotgun_reload_finish,
	act_shotgun_pump,

	// smg2 special activities
	act_smg2_idle2,
	act_smg2_fire2,
	act_smg2_draw2,
	act_smg2_reload2,
	act_smg2_dryfire2,
	act_smg2_toauto,
	act_smg2_toburst,

	// physcannon special activities
	act_physcannon_upgrade,

	// weapon override activities
	act_range_attack_ar1,
	act_range_attack_ar2,
	act_range_attack_ar2_low,
	act_range_attack_ar2_grenade,
	act_range_attack_hmg1,
	act_range_attack_ml,
	act_range_attack_smg1,
	act_range_attack_smg1_low,
	act_range_attack_smg2,
	act_range_attack_shotgun,
	act_range_attack_shotgun_low,
	act_range_attack_pistol,
	act_range_attack_pistol_low,
	act_range_attack_slam,
	act_range_attack_tripwire,
	act_range_attack_throw,
	act_range_attack_sniper_rifle,
	act_range_attack_rpg,
	act_melee_attack_swing,

	act_range_aim_low,
	act_range_aim_smg1_low,
	act_range_aim_pistol_low,
	act_range_aim_ar2_low,

	act_cover_pistol_low,
	act_cover_smg1_low,

	// weapon override activities
	act_gesture_range_attack_ar1,
	act_gesture_range_attack_ar2,
	act_gesture_range_attack_ar2_grenade,
	act_gesture_range_attack_hmg1,
	act_gesture_range_attack_ml,
	act_gesture_range_attack_smg1,
	act_gesture_range_attack_smg1_low,
	act_gesture_range_attack_smg2,
	act_gesture_range_attack_shotgun,
	act_gesture_range_attack_pistol,
	act_gesture_range_attack_pistol_low,
	act_gesture_range_attack_slam,
	act_gesture_range_attack_tripwire,
	act_gesture_range_attack_throw,
	act_gesture_range_attack_sniper_rifle,
	act_gesture_melee_attack_swing,

	act_idle_rifle,
	act_idle_smg1,
	act_idle_angry_smg1,
	act_idle_pistol,
	act_idle_angry_pistol,
	act_idle_angry_shotgun,
	act_idle_stealth_pistol,

	act_idle_package,
	act_walk_package,
	act_idle_suitcase,
	act_walk_suitcase,

	act_idle_smg1_relaxed,
	act_idle_smg1_stimulated,
	act_walk_rifle_relaxed,
	act_run_rifle_relaxed,
	act_walk_rifle_stimulated,
	act_run_rifle_stimulated,

	act_idle_aim_rifle_stimulated,
	act_walk_aim_rifle_stimulated,
	act_run_aim_rifle_stimulated,

	act_idle_shotgun_relaxed,
	act_idle_shotgun_stimulated,
	act_idle_shotgun_agitated,

	// policing activities
	act_walk_angry,
	act_police_harass1,
	act_police_harass2,

	// manned guns
	act_idle_mannedgun,

	// melee weapon
	act_idle_melee,
	act_idle_angry_melee,

	// rpg activities
	act_idle_rpg_relaxed,
	act_idle_rpg,
	act_idle_angry_rpg,
	act_cover_low_rpg,
	act_walk_rpg,
	act_run_rpg,
	act_walk_crouch_rpg,
	act_run_crouch_rpg,
	act_walk_rpg_relaxed,
	act_run_rpg_relaxed,

	act_walk_rifle,
	act_walk_aim_rifle,
	act_walk_crouch_rifle,
	act_walk_crouch_aim_rifle,
	act_run_rifle,
	act_run_aim_rifle,
	act_run_crouch_rifle,
	act_run_crouch_aim_rifle,
	act_run_stealth_pistol,

	act_walk_aim_shotgun,
	act_run_aim_shotgun,

	act_walk_pistol,
	act_run_pistol,
	act_walk_aim_pistol,
	act_run_aim_pistol,
	act_walk_stealth_pistol,
	act_walk_aim_stealth_pistol,
	act_run_aim_stealth_pistol,

	// reloads
	act_reload_pistol,
	act_reload_pistol_low,
	act_reload_smg1,
	act_reload_smg1_low,
	act_reload_shotgun,
	act_reload_shotgun_low,

	act_gesture_reload,
	act_gesture_reload_pistol,
	act_gesture_reload_smg1,
	act_gesture_reload_shotgun,

	// busy animations
	act_busy_lean_left,
	act_busy_lean_left_entry,
	act_busy_lean_left_exit,
	act_busy_lean_back,
	act_busy_lean_back_entry,
	act_busy_lean_back_exit,
	act_busy_sit_ground,
	act_busy_sit_ground_entry,
	act_busy_sit_ground_exit,
	act_busy_sit_chair,
	act_busy_sit_chair_entry,
	act_busy_sit_chair_exit,
	act_busy_stand,
	act_busy_queue,

	// dodge animations
	act_duck_dodge,

	// for npcs being lifted/eaten by barnacles:
	// being swallowed by a barnacle
	act_die_barnacle_swallow,
	// being lifted by a barnacle
	act_gesture_barnacle_strangle,

	act_physcannon_detach,	// an activity to be played if we're picking this up with the physcannon
	act_physcannon_animate, // an activity to be played by an object being picked up with the physcannon, but has different behavior to detach
	act_physcannon_animate_pre,	// an activity to be played by an object being picked up with the physcannon, before playing the act_physcannon_animate
	act_physcannon_animate_post,// an activity to be played by an object being picked up with the physcannon, after playing the act_physcannon_animate

	act_die_frontside,
	act_die_rightside,
	act_die_backside,
	act_die_leftside,

	act_die_crouch_frontside,
	act_die_crouch_rightside,
	act_die_crouch_backside,
	act_die_crouch_leftside,

	act_open_door,

	// dynamic interactions
	act_di_alyx_zombie_melee,
	act_di_alyx_zombie_torso_melee,
	act_di_alyx_headcrab_melee,
	act_di_alyx_antlion,

	act_di_alyx_zombie_shotgun64,
	act_di_alyx_zombie_shotgun26,

	act_readiness_relaxed_to_stimulated,
	act_readiness_relaxed_to_stimulated_walk,
	act_readiness_agitated_to_stimulated,
	act_readiness_stimulated_to_relaxed,

	act_readiness_pistol_relaxed_to_stimulated,
	act_readiness_pistol_relaxed_to_stimulated_walk,
	act_readiness_pistol_agitated_to_stimulated,
	act_readiness_pistol_stimulated_to_relaxed,

	act_idle_carry,
	act_walk_carry,

	//===========================
	// tf2 specific activities
	//===========================
	act_startdying,
	act_dyingloop,
	act_dyingtodead,

	act_ride_manned_gun,

	// all viewmodels
	act_vm_sprint_enter,
	act_vm_sprint_idle,
	act_vm_sprint_leave,

	// looping weapon firing
	act_fire_start,
	act_fire_loop,
	act_fire_end,

	act_crouching_grenadeidle,
	act_crouching_grenadeready,
	act_crouching_primaryattack,
	act_overlay_grenadeidle,
	act_overlay_grenadeready,
	act_overlay_primaryattack,
	act_overlay_shield_up,
	act_overlay_shield_down,
	act_overlay_shield_up_idle,
	act_overlay_shield_attack,
	act_overlay_shield_knockback,
	act_shield_up,
	act_shield_down,
	act_shield_up_idle,
	act_shield_attack,
	act_shield_knockback,
	act_crouching_shield_up,
	act_crouching_shield_down,
	act_crouching_shield_up_idle,
	act_crouching_shield_attack,
	act_crouching_shield_knockback,

	// turning in place
	act_turnright45,
	act_turnleft45,

	act_turn,

	act_obj_assembling,
	act_obj_dismantling,
	act_obj_startup,
	act_obj_running,
	act_obj_idle,
	act_obj_placing,
	act_obj_deteriorating,
	act_obj_upgrading,

	// deploy
	act_deploy,
	act_deploy_idle,
	act_undeploy,

	// crossbow
	act_crossbow_draw_unloaded,

	// gauss
	act_gauss_spinup,
	act_gauss_spincycle,

	//===========================
	// csport specific activities
	//===========================

	act_vm_primaryattack_silenced,		// fire
	act_vm_reload_silenced,
	act_vm_dryfire_silenced,				// fire with no ammo loaded.
	act_vm_idle_silenced,
	act_vm_draw_silenced,
	act_vm_idle_empty_left,
	act_vm_dryfire_left,

	// new for cs2
	act_vm_is_draw,
	act_vm_is_holster,
	act_vm_is_idle,
	act_vm_is_primaryattack,

	act_player_idle_fire,
	act_player_crouch_fire,
	act_player_crouch_walk_fire,
	act_player_walk_fire,
	act_player_run_fire,

	act_idletorun,
	act_runtoidle,

	act_vm_draw_deployed,

	act_hl2mp_idle_melee,
	act_hl2mp_run_melee,
	act_hl2mp_idle_crouch_melee,
	act_hl2mp_walk_crouch_melee,
	act_hl2mp_gesture_range_attack_melee,
	act_hl2mp_gesture_reload_melee,
	act_hl2mp_jump_melee,

	// portal!
	act_vm_fizzle,

	// multiplayer
	act_mp_stand_idle,
	act_mp_crouch_idle,
	act_mp_crouch_deployed_idle,
	act_mp_crouch_deployed,
	act_mp_deployed_idle,
	act_mp_run,
	act_mp_walk,
	act_mp_airwalk,
	act_mp_crouchwalk,
	act_mp_sprint,
	act_mp_jump,
	act_mp_jump_start,
	act_mp_jump_float,
	act_mp_jump_land,
	act_mp_jump_impact_n,
	act_mp_jump_impact_e,
	act_mp_jump_impact_w,
	act_mp_jump_impact_s,
	act_mp_jump_impact_top,
	act_mp_doublejump,
	act_mp_swim,
	act_mp_deployed,
	act_mp_swim_deployed,
	act_mp_vcd,

	act_mp_attack_stand_primaryfire,
	act_mp_attack_stand_primaryfire_deployed,
	act_mp_attack_stand_secondaryfire,
	act_mp_attack_stand_grenade,
	act_mp_attack_crouch_primaryfire,
	act_mp_attack_crouch_primaryfire_deployed,
	act_mp_attack_crouch_secondaryfire,
	act_mp_attack_crouch_grenade,
	act_mp_attack_swim_primaryfire,
	act_mp_attack_swim_secondaryfire,
	act_mp_attack_swim_grenade,
	act_mp_attack_airwalk_primaryfire,
	act_mp_attack_airwalk_secondaryfire,
	act_mp_attack_airwalk_grenade,
	act_mp_reload_stand,
	act_mp_reload_stand_loop,
	act_mp_reload_stand_end,
	act_mp_reload_crouch,
	act_mp_reload_crouch_loop,
	act_mp_reload_crouch_end,
	act_mp_reload_swim,
	act_mp_reload_swim_loop,
	act_mp_reload_swim_end,
	act_mp_reload_airwalk,
	act_mp_reload_airwalk_loop,
	act_mp_reload_airwalk_end,
	act_mp_attack_stand_prefire,
	act_mp_attack_stand_postfire,
	act_mp_attack_stand_startfire,
	act_mp_attack_crouch_prefire,
	act_mp_attack_crouch_postfire,
	act_mp_attack_swim_prefire,
	act_mp_attack_swim_postfire,

	// multiplayer - primary
	act_mp_stand_primary,
	act_mp_crouch_primary,
	act_mp_run_primary,
	act_mp_walk_primary,
	act_mp_airwalk_primary,
	act_mp_crouchwalk_primary,
	act_mp_jump_primary,
	act_mp_jump_start_primary,
	act_mp_jump_float_primary,
	act_mp_jump_land_primary,
	act_mp_swim_primary,
	act_mp_deployed_primary,
	act_mp_swim_deployed_primary,

	act_mp_attack_stand_primary,		// run, walk
	act_mp_attack_stand_primary_deployed,
	act_mp_attack_crouch_primary,		// crouchwalk
	act_mp_attack_crouch_primary_deployed,
	act_mp_attack_swim_primary,
	act_mp_attack_airwalk_primary,

	act_mp_reload_stand_primary,		// run, walk
	act_mp_reload_stand_primary_loop,
	act_mp_reload_stand_primary_end,
	act_mp_reload_crouch_primary,		// crouchwalk
	act_mp_reload_crouch_primary_loop,
	act_mp_reload_crouch_primary_end,
	act_mp_reload_swim_primary,
	act_mp_reload_swim_primary_loop,
	act_mp_reload_swim_primary_end,
	act_mp_reload_airwalk_primary,
	act_mp_reload_airwalk_primary_loop,
	act_mp_reload_airwalk_primary_end,

	act_mp_attack_stand_grenade_primary,		// run, walk
	act_mp_attack_crouch_grenade_primary,		// crouchwalk
	act_mp_attack_swim_grenade_primary,
	act_mp_attack_airwalk_grenade_primary,


	// secondary
	act_mp_stand_secondary,
	act_mp_crouch_secondary,
	act_mp_run_secondary,
	act_mp_walk_secondary,
	act_mp_airwalk_secondary,
	act_mp_crouchwalk_secondary,
	act_mp_jump_secondary,
	act_mp_jump_start_secondary,
	act_mp_jump_float_secondary,
	act_mp_jump_land_secondary,
	act_mp_swim_secondary,

	act_mp_attack_stand_secondary,		// run, walk
	act_mp_attack_crouch_secondary,		// crouchwalk
	act_mp_attack_swim_secondary,
	act_mp_attack_airwalk_secondary,

	act_mp_reload_stand_secondary,		// run, walk
	act_mp_reload_stand_secondary_loop,
	act_mp_reload_stand_secondary_end,
	act_mp_reload_crouch_secondary,		// crouchwalk
	act_mp_reload_crouch_secondary_loop,
	act_mp_reload_crouch_secondary_end,
	act_mp_reload_swim_secondary,
	act_mp_reload_swim_secondary_loop,
	act_mp_reload_swim_secondary_end,
	act_mp_reload_airwalk_secondary,
	act_mp_reload_airwalk_secondary_loop,
	act_mp_reload_airwalk_secondary_end,

	act_mp_attack_stand_grenade_secondary,		// run, walk
	act_mp_attack_crouch_grenade_secondary,		// crouchwalk
	act_mp_attack_swim_grenade_secondary,
	act_mp_attack_airwalk_grenade_secondary,

	// melee
	act_mp_stand_melee,
	act_mp_crouch_melee,
	act_mp_run_melee,
	act_mp_walk_melee,
	act_mp_airwalk_melee,
	act_mp_crouchwalk_melee,
	act_mp_jump_melee,
	act_mp_jump_start_melee,
	act_mp_jump_float_melee,
	act_mp_jump_land_melee,
	act_mp_swim_melee,

	act_mp_attack_stand_melee,		// run, walk
	act_mp_attack_stand_melee_secondary,
	act_mp_attack_crouch_melee,		// crouchwalk
	act_mp_attack_crouch_melee_secondary,
	act_mp_attack_swim_melee,
	act_mp_attack_airwalk_melee,

	act_mp_attack_stand_grenade_melee,		// run, walk
	act_mp_attack_crouch_grenade_melee,		// crouchwalk
	act_mp_attack_swim_grenade_melee,
	act_mp_attack_airwalk_grenade_melee,

	// item1
	act_mp_stand_item1,
	act_mp_crouch_item1,
	act_mp_run_item1,
	act_mp_walk_item1,
	act_mp_airwalk_item1,
	act_mp_crouchwalk_item1,
	act_mp_jump_item1,
	act_mp_jump_start_item1,
	act_mp_jump_float_item1,
	act_mp_jump_land_item1,
	act_mp_swim_item1,

	act_mp_attack_stand_item1,		// run, walk
	act_mp_attack_stand_item1_secondary,
	act_mp_attack_crouch_item1,		// crouchwalk
	act_mp_attack_crouch_item1_secondary,
	act_mp_attack_swim_item1,
	act_mp_attack_airwalk_item1,

	// item2
	act_mp_stand_item2,
	act_mp_crouch_item2,
	act_mp_run_item2,
	act_mp_walk_item2,
	act_mp_airwalk_item2,
	act_mp_crouchwalk_item2,
	act_mp_jump_item2,
	act_mp_jump_start_item2,
	act_mp_jump_float_item2,
	act_mp_jump_land_item2,
	act_mp_swim_item2,

	act_mp_attack_stand_item2,		// run, walk
	act_mp_attack_stand_item2_secondary,
	act_mp_attack_crouch_item2,		// crouchwalk
	act_mp_attack_crouch_item2_secondary,
	act_mp_attack_swim_item2,
	act_mp_attack_airwalk_item2,

	// flinches
	act_mp_gesture_flinch,
	act_mp_gesture_flinch_primary,
	act_mp_gesture_flinch_secondary,
	act_mp_gesture_flinch_melee,
	act_mp_gesture_flinch_item1,
	act_mp_gesture_flinch_item2,

	act_mp_gesture_flinch_head,
	act_mp_gesture_flinch_chest,
	act_mp_gesture_flinch_stomach,
	act_mp_gesture_flinch_leftarm,
	act_mp_gesture_flinch_rightarm,
	act_mp_gesture_flinch_leftleg,
	act_mp_gesture_flinch_rightleg,

	// team fortress specific - medic heal, medic infect, etc.....
	act_mp_grenade1_draw,
	act_mp_grenade1_idle,
	act_mp_grenade1_attack,
	act_mp_grenade2_draw,
	act_mp_grenade2_idle,
	act_mp_grenade2_attack,

	act_mp_primary_grenade1_draw,
	act_mp_primary_grenade1_idle,
	act_mp_primary_grenade1_attack,
	act_mp_primary_grenade2_draw,
	act_mp_primary_grenade2_idle,
	act_mp_primary_grenade2_attack,

	act_mp_secondary_grenade1_draw,
	act_mp_secondary_grenade1_idle,
	act_mp_secondary_grenade1_attack,
	act_mp_secondary_grenade2_draw,
	act_mp_secondary_grenade2_idle,
	act_mp_secondary_grenade2_attack,

	act_mp_melee_grenade1_draw,
	act_mp_melee_grenade1_idle,
	act_mp_melee_grenade1_attack,
	act_mp_melee_grenade2_draw,
	act_mp_melee_grenade2_idle,
	act_mp_melee_grenade2_attack,

	act_mp_item1_grenade1_draw,
	act_mp_item1_grenade1_idle,
	act_mp_item1_grenade1_attack,
	act_mp_item1_grenade2_draw,
	act_mp_item1_grenade2_idle,
	act_mp_item1_grenade2_attack,

	act_mp_item2_grenade1_draw,
	act_mp_item2_grenade1_idle,
	act_mp_item2_grenade1_attack,
	act_mp_item2_grenade2_draw,
	act_mp_item2_grenade2_idle,
	act_mp_item2_grenade2_attack,

	// building
	act_mp_stand_building,
	act_mp_crouch_building,
	act_mp_run_building,
	act_mp_walk_building,
	act_mp_airwalk_building,
	act_mp_crouchwalk_building,
	act_mp_jump_building,
	act_mp_jump_start_building,
	act_mp_jump_float_building,
	act_mp_jump_land_building,
	act_mp_swim_building,

	act_mp_attack_stand_building,		// run, walk
	act_mp_attack_crouch_building,		// crouchwalk
	act_mp_attack_swim_building,
	act_mp_attack_airwalk_building,

	act_mp_attack_stand_grenade_building,		// run, walk
	act_mp_attack_crouch_grenade_building,		// crouchwalk
	act_mp_attack_swim_grenade_building,
	act_mp_attack_airwalk_grenade_building,

	act_mp_stand_pda,
	act_mp_crouch_pda,
	act_mp_run_pda,
	act_mp_walk_pda,
	act_mp_airwalk_pda,
	act_mp_crouchwalk_pda,
	act_mp_jump_pda,
	act_mp_jump_start_pda,
	act_mp_jump_float_pda,
	act_mp_jump_land_pda,
	act_mp_swim_pda,

	act_mp_attack_stand_pda,
	act_mp_attack_swim_pda,

	act_mp_gesture_vc_handmouth,
	act_mp_gesture_vc_fingerpoint,
	act_mp_gesture_vc_fistpump,
	act_mp_gesture_vc_thumbsup,
	act_mp_gesture_vc_nodyes,
	act_mp_gesture_vc_nodno,

	act_mp_gesture_vc_handmouth_primary,
	act_mp_gesture_vc_fingerpoint_primary,
	act_mp_gesture_vc_fistpump_primary,
	act_mp_gesture_vc_thumbsup_primary,
	act_mp_gesture_vc_nodyes_primary,
	act_mp_gesture_vc_nodno_primary,

	act_mp_gesture_vc_handmouth_secondary,
	act_mp_gesture_vc_fingerpoint_secondary,
	act_mp_gesture_vc_fistpump_secondary,
	act_mp_gesture_vc_thumbsup_secondary,
	act_mp_gesture_vc_nodyes_secondary,
	act_mp_gesture_vc_nodno_secondary,

	act_mp_gesture_vc_handmouth_melee,
	act_mp_gesture_vc_fingerpoint_melee,
	act_mp_gesture_vc_fistpump_melee,
	act_mp_gesture_vc_thumbsup_melee,
	act_mp_gesture_vc_nodyes_melee,
	act_mp_gesture_vc_nodno_melee,

	act_mp_gesture_vc_handmouth_item1,
	act_mp_gesture_vc_fingerpoint_item1,
	act_mp_gesture_vc_fistpump_item1,
	act_mp_gesture_vc_thumbsup_item1,
	act_mp_gesture_vc_nodyes_item1,
	act_mp_gesture_vc_nodno_item1,

	act_mp_gesture_vc_handmouth_item2,
	act_mp_gesture_vc_fingerpoint_item2,
	act_mp_gesture_vc_fistpump_item2,
	act_mp_gesture_vc_thumbsup_item2,
	act_mp_gesture_vc_nodyes_item2,
	act_mp_gesture_vc_nodno_item2,

	act_mp_gesture_vc_handmouth_building,
	act_mp_gesture_vc_fingerpoint_building,
	act_mp_gesture_vc_fistpump_building,
	act_mp_gesture_vc_thumbsup_building,
	act_mp_gesture_vc_nodyes_building,
	act_mp_gesture_vc_nodno_building,

	act_mp_gesture_vc_handmouth_pda,
	act_mp_gesture_vc_fingerpoint_pda,
	act_mp_gesture_vc_fistpump_pda,
	act_mp_gesture_vc_thumbsup_pda,
	act_mp_gesture_vc_nodyes_pda,
	act_mp_gesture_vc_nodno_pda,


	act_vm_unusable,
	act_vm_unusable_to_usable,
	act_vm_usable_to_unusable,

	// specific viewmodel activities for weapon roles
	act_primary_vm_draw,
	act_primary_vm_holster,
	act_primary_vm_idle,
	act_primary_vm_pullback,
	act_primary_vm_primaryattack,
	act_primary_vm_secondaryattack,
	act_primary_vm_reload,
	act_primary_vm_dryfire,
	act_primary_vm_idle_to_lowered,
	act_primary_vm_idle_lowered,
	act_primary_vm_lowered_to_idle,

	act_secondary_vm_draw,
	act_secondary_vm_holster,
	act_secondary_vm_idle,
	act_secondary_vm_pullback,
	act_secondary_vm_primaryattack,
	act_secondary_vm_secondaryattack,
	act_secondary_vm_reload,
	act_secondary_vm_dryfire,
	act_secondary_vm_idle_to_lowered,
	act_secondary_vm_idle_lowered,
	act_secondary_vm_lowered_to_idle,

	act_melee_vm_draw,
	act_melee_vm_holster,
	act_melee_vm_idle,
	act_melee_vm_pullback,
	act_melee_vm_primaryattack,
	act_melee_vm_secondaryattack,
	act_melee_vm_reload,
	act_melee_vm_dryfire,
	act_melee_vm_idle_to_lowered,
	act_melee_vm_idle_lowered,
	act_melee_vm_lowered_to_idle,

	act_pda_vm_draw,
	act_pda_vm_holster,
	act_pda_vm_idle,
	act_pda_vm_pullback,
	act_pda_vm_primaryattack,
	act_pda_vm_secondaryattack,
	act_pda_vm_reload,
	act_pda_vm_dryfire,
	act_pda_vm_idle_to_lowered,
	act_pda_vm_idle_lowered,
	act_pda_vm_lowered_to_idle,

	act_item1_vm_draw,
	act_item1_vm_holster,
	act_item1_vm_idle,
	act_item1_vm_pullback,
	act_item1_vm_primaryattack,
	act_item1_vm_secondaryattack,
	act_item1_vm_reload,
	act_item1_vm_dryfire,
	act_item1_vm_idle_to_lowered,
	act_item1_vm_idle_lowered,
	act_item1_vm_lowered_to_idle,

	act_item2_vm_draw,
	act_item2_vm_holster,
	act_item2_vm_idle,
	act_item2_vm_pullback,
	act_item2_vm_primaryattack,
	act_item2_vm_secondaryattack,
	act_item2_vm_reload,
	act_item2_vm_dryfire,
	act_item2_vm_idle_to_lowered,
	act_item2_vm_idle_lowered,
	act_item2_vm_lowered_to_idle,

	// infested activities
	act_reload_succeed,
	act_reload_fail,
	// autogun
	act_walk_aim_autogun,
	act_run_aim_autogun,
	act_idle_autogun,
	act_idle_aim_autogun,
	act_reload_autogun,
	act_crouch_idle_autogun,
	act_range_attack_autogun,
	act_jump_autogun,
	// pistol
	act_idle_aim_pistol,
	// pdw
	act_walk_aim_dual,
	act_run_aim_dual,
	act_idle_dual,
	act_idle_aim_dual,
	act_reload_dual,
	act_crouch_idle_dual,
	act_range_attack_dual,
	act_jump_dual,
	// shotgun
	act_idle_shotgun,
	act_idle_aim_shotgun,
	act_crouch_idle_shotgun,
	act_jump_shotgun,
	// rifle
	act_idle_aim_rifle,
	act_reload_rifle,
	act_crouch_idle_rifle,
	act_range_attack_rifle,
	act_jump_rifle,

	// infested general ai
	act_sleep,
	act_wake,

	// shield bug
	act_flick_left,
	act_flick_left_middle,
	act_flick_right_middle,
	act_flick_right,
	act_spinaround,

	// mortar bug
	act_prep_to_fire,
	act_fire,
	act_fire_recover,

	// shaman
	act_spray,

	// boomer
	act_prep_explode,
	act_explode,

	///******************
	///dota animations
	///******************

	act_dota_idle,
	act_dota_run,
	act_dota_attack,
	act_dota_attack_event,
	act_dota_die,
	act_dota_flinch,
	act_dota_disabled,
	act_dota_cast_ability_1,
	act_dota_cast_ability_2,
	act_dota_cast_ability_3,
	act_dota_cast_ability_4,
	act_dota_override_ability_1,
	act_dota_override_ability_2,
	act_dota_override_ability_3,
	act_dota_override_ability_4,
	act_dota_channel_ability_1,
	act_dota_channel_ability_2,
	act_dota_channel_ability_3,
	act_dota_channel_ability_4,
	act_dota_channel_end_ability_1,
	act_dota_channel_end_ability_2,
	act_dota_channel_end_ability_3,
	act_dota_channel_end_ability_4,


	// portal2
	act_mp_run_speedpaint,  // running on speed paint
	act_mp_long_fall, // falling a long way
	act_mp_tractorbeam_float, // floating in a tractor beam
	act_mp_death_crush,

	act_mp_run_speedpaint_primary, // player with portalgun running on speed paint
	act_mp_drowning_primary, // drowning while holding portalgun
	act_mp_long_fall_primary, // falling a long way while holding portalgun
	act_mp_tractorbeam_float_primary, // floating in a tractor beam while holding portalgun 
	act_mp_death_crush_primary,


	// csgo death anims that don't require direction (direction is pose-param driven for more granularity)
	act_die_stand,
	act_die_stand_headshot,
	act_die_crouch,
	act_die_crouch_headshot,



	// csgo action activities
	act_csgo_null,
	act_csgo_defuse,
	act_csgo_defuse_with_kit,
	act_csgo_flashbang_reaction,
	act_csgo_fire_primary,
	act_csgo_fire_primary_opt_1,
	act_csgo_fire_primary_opt_2,
	act_csgo_fire_secondary,
	act_csgo_fire_secondary_opt_1,
	act_csgo_fire_secondary_opt_2,
	act_csgo_reload,
	act_csgo_reload_start,
	act_csgo_reload_loop,
	act_csgo_reload_end,
	act_csgo_operate,
	act_csgo_deploy,
	act_csgo_catch,
	act_csgo_silencer_detach,
	act_csgo_silencer_attach,
	act_csgo_twitch,
	act_csgo_twitch_buyzone,
	act_csgo_plant_bomb,
	act_csgo_idle_turn_balanceadjust,
	act_csgo_idle_adjust_stoppedmoving,
	act_csgo_alive_loop,
	act_csgo_flinch,
	act_csgo_flinch_head,
	act_csgo_flinch_molotov,
	act_csgo_jump,
	act_csgo_fall,
	act_csgo_climb_ladder,
	act_csgo_land_light,
	act_csgo_land_heavy,
	act_csgo_exit_ladder_top,
	act_csgo_exit_ladder_bottom,

	// this is the end of the global activities, private per-monster activities start here.
	last_shared_activity,
} anim_activity;

typedef enum {
	invalidate_phys_bits_pos_changed = ( 1 << 0 ),
	invalidate_phys_bits_angles_changed = ( 1 << 1 ),
	invalidate_phys_bits_vel_changed = ( 1 << 2 ),
	invalidate_phys_bits_anim_changed = ( 1 << 3 ),
	invalidate_phys_bits_bounds_changed = ( 1 << 4 ),
	invalidate_phys_bits_seq_changed = ( 1 << 5 ),
} invalidate_phys_bits;

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
	pose_param_count,
} pose_param_idx;

// Generated using ReClass 2016
typedef struct {
	bool init;
	int idx;
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
	int	seq;
	float previous_cycle;
	float weight;
	float weight_delta_rate;
	float playback_rate;
	float cycle;
	player* owner;
	int invalidate_physics_bit;
} animlayer;

/* client animstate */
typedef struct {
	void* vtable;
	bool first_run;
	bool first_foot_plant;
	int last_update_frame1;
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
	animstate_pose_param_cache pose_param_map[ pose_param_count ];
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

	/* extra info that is just for us to do some ghetto workarounds */
	float last_flash_duration;
} server_animstate;

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
NETVAR( player, float, duck_amount, "DT_BasePlayer->m_flDuckAmount" );
NETVAR( player, float, duck_speed, "DT_BasePlayer->m_flDuckSpeed" );
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
//NETVAR( player, bool, strafing, "DT_CSPlayer->m_bStrafing" );
NETVAR( player, int, body, "DT_CSPlayer->m_nBody" );
OFFSET( player, uint8_t, movetype, cs_offsets.player_movetype );
OFFSET( player, void*, iks, cs_offsets.player_iks );
OFFSET( player, float, spawn_time, cs_offsets.player_spawn_time );
OFFSET( player, mat3x4a*, bones, cs_offsets.player_bones );
OFFSET( player, animlayer*, animlayers, cs_offsets.player_animlayers );
OFFSET( player, uint32_t, num_animlayers, cs_offsets.player_num_animlayers );
OFFSET( player, float, poses, cs_offsets.player_poses );
OFFSET( player, void*, jiggle_bones, cs_offsets.player_jiggle_bones );
OFFSET( player, float, thirdperson_recoil, cs_offsets.player_thirdperson_recoil );
OFFSET( player, int, computed_lod_frame, cs_offsets.player_computed_lod_frame );
OFFSET( player, bool, strafing, cs_offsets.player_is_strafing );
OFFSET( player, uint32_t*, bone_count, cs_offsets.player_bone_count );
OFFSET( player, mat3x4*, bone_cache, cs_offsets.player_bone_cache );
OFFSET( player, in_buttons, buttons, cs_offsets.player_buttons );

VIRTUAL( player, void, set_local_viewangles, cs_idx_player_set_local_viewangles, ( this, angles ), vec3* angles );
VIRTUAL( player, void, think, cs_idx_player_think, ( this ) );
VIRTUAL( player, void, pre_think, cs_idx_player_pre_think, ( this ) );
VIRTUAL( player, void, post_think, cs_idx_player_post_think, ( this ) );

static inline bool player_physics_run_think( player* this, int unk01 ) {
	return ( ( __attribute__( ( thiscall ) ) bool( * )( player*, int ) )cs_offsets.player_physics_run_think_fn )( this, unk01 );
}

static inline vec3* player_world_space( player* this, vec3* out ) {
	vec3 va, vb;

	( ( __attribute__( ( thiscall ) ) void( * )( renderable*, vec3*, vec3* ) )utils_vfunc( entity_renderable( ( entity* )this ), cs_idx_player_world_space ) )( entity_renderable( ( entity* )this ), &va, &vb );

	vec3_zero( out );
	*out = *entity_origin( ( entity* )this );
	out->z += ( va.z + vb.z ) * 0.5f;

	return out;
}

static inline float* player_old_simtime( player* this ) {
	return ( float* )( ( uintptr_t )player_simtime( this ) + sizeof( float ) );
}

static inline bool player_is_alive( player* this ) {
	return player_health( this ) > 0;
}

static inline bool player_is_local( player* this ) {
	return *( bool* )( ( uintptr_t )this + cs_offsets.player_is_local );
}

VIRTUAL( player, vec3*, abs_origin, cs_idx_player_abs_origin, ( this ) );
VIRTUAL( player, vec3*, abs_angles, cs_idx_player_abs_angles, ( this ) );

static inline ehandle player_handle( player* this ) {
	return ( ( __attribute__( ( thiscall ) ) ehandle( * )( player* ) ) cs_offsets.player_handle_fn )( this );
}

static inline vec3* player_render_origin( player* this ) {
	return ( ( __attribute__( ( thiscall ) ) vec3 * ( * )( renderable* ) )utils_vfunc( entity_renderable( ( entity* )this ), cs_idx_player_render_origin ) )( entity_renderable( ( entity* )this ) );
}

static inline void player_set_abs_angles( player* this, vec3* ang ) {
	( ( __attribute__( ( thiscall ) ) void( * )( player*, vec3* ) )cs_offsets.player_set_abs_angles_fn )( this, ang );
}

static inline void player_set_abs_origin( player* this, vec3* vec ) {
	( ( __attribute__( ( thiscall ) ) void( * )( player*, vec3* ) )cs_offsets.player_set_abs_origin_fn )( this, vec );
}

static inline animstate* player_animstate( player* this ) {
	return *( animstate** )( ( uintptr_t )this + cs_offsets.player_animstate );
}

static inline void player_create_animstate( player* this, animstate* state ) {
	( ( __attribute__( ( thiscall ) ) void( * )( player*, animstate* ) )cs_offsets.player_create_animstate_fn )( state, this );
}

static inline void player_weapon_shootposition( player* this, vec3* pos ) {
	vec3_init( pos );

	/* eye position */
	( ( __attribute__( ( thiscall ) ) void( * )( player*, vec3* ) )utils_vfunc( this, cs_idx_player_get_shoot_pos ) ) ( this, pos );

	if ( *( bool* )( ( uintptr_t )this + cs_offsets.player_use_new_animstate ) && player_animstate( this ) )
		( ( __attribute__( ( thiscall ) ) void( * )( animstate*, vec3* ) ) cs_offsets.animstate_modifyeyepos ) ( player_animstate( this ), pos );
}

static inline weapon* player_get_weapon( player* this ) {
	extern struct ientlist* cs_ientlist;
	return ( weapon* )ientlist_get_entity_from_handle( cs_ientlist, player_weapon_handle( this ) );
}

typedef weapon** vec_weapons;
typedef weapon** vec_wearables;

static inline vec_weapons player_weapons( player* this ) {
	const ehandle* weapon_handles = player_weapon_handles( this );
	vec_weapons ret = vector_create( );

	for ( int i = 0; weapon_handles[ i ] != 0xFFFFFFFF; i++ ) {
		extern struct ientlist* cs_ientlist;
		const weapon* cur_weapon = ( weapon* )ientlist_get_entity_from_handle( cs_ientlist, weapon_handles[ i ] );

		if ( !cur_weapon )
			continue;

		vector_add( &ret, cur_weapon );
	}

	/* MAKE SURE TO CALL vector_free */
	return ret;
}

static inline vec_wearables player_wearables( player* this ) {
	const ehandle* wearable_handles = player_wearable_handles( this );
	vec_wearables ret = vector_create( );

	for ( int i = 0; wearable_handles[ i ] != -1; i++ ) {
		extern struct ientlist* cs_ientlist;
		const weapon* cur_wearable = ( weapon* )ientlist_get_entity_from_handle( cs_ientlist, wearable_handles[ i ] );

		if ( !cur_wearable )
			continue;

		vector_add( &ret, cur_wearable );
	}

	/* MAKE SURE TO CALL vector_free */
	return ret;
}

static inline void player_get_sequence_linear_motion( player* this, studiohdr* studio_hdr, int seq, float* poses, vec3* vec ) {
	asm volatile(
		"mov edx, %0;"
		"mov ecx, %1;"
		"push %2;"
		"push %3;"
		"call %4;"
		"add esp, 8;"
		:: "g"( seq ), "g"( studio_hdr ), "g"( vec ), "g"( poses ), "g"( cs_offsets.player_get_sequence_linear_motion_fn )
		: "edx", "ecx"
		);
}

static inline float player_get_sequence_move_distance( player* this, int seq ) {
	vec3 ret;
	player_get_sequence_linear_motion( this, *entity_model_ptr( ( entity* )this ), seq, player_poses( this ), &ret );
	return vec3_len( &ret );
}

static inline int player_lookup_sequence( player* this, const char* seq ) {
	return ( ( __attribute__( ( thiscall ) ) int( * )( player*, const char* ) ) cs_offsets.player_lookup_sequence_fn ) ( this, seq );
}

static inline int player_lookup_bone( player* this, const char* bone ) {
	return ( ( __attribute__( ( thiscall ) ) int( * )( player*, const char* ) ) cs_offsets.player_lookup_bone_fn ) ( this, bone );
}

static inline float player_sequence_duration( player* this, int seq ) {
	volatile float ret = 0.0f;

	asm volatile(
		"mov ecx, %1;"
		"push %2;"
		"call %3;"
		"movss %0, xmm0;"
		: "=g"( ret )
		: "g"( this ), "g"( seq ), "g"( cs_offsets.player_sequence_duration_fn )
		: "ecx", "xmm0"
		);

	return ret;
}

static inline float player_get_sequence_cycle_rate_server( player* this, int seq ) {
	float t = player_sequence_duration( this, seq );

	if ( t > 0.0f )
		return 1.0f / t;

	return 1.0f / 0.1f;
}

static inline void player_invalidate_physics_recursive( player* this, int flags ) {
	( ( __attribute__( ( thiscall ) ) void( * )( player*, int ) )cs_offsets.player_invalidate_physics_recursive_fn ) ( this, flags );
}

static inline void player_set_sequence( player* this, int seq ) {
	( ( __attribute__( ( thiscall ) ) void( * )( player*, int ) )cs_offsets.player_set_sequence_fn ) ( this, seq );
}

static inline void player_set_playback_rate( player* this, float rate ) {
	*( float* )( ( uintptr_t )this + cs_offsets.player_playback_rate ) = rate;
}

static inline void player_set_cycle( player* this, float cycle ) {
	*( float* )( ( uintptr_t )this + cs_offsets.player_cycle ) = cycle;
}

static inline float player_get_layer_sequence_cycle_rate( player* this, animlayer_idx idx, int seq ) {
	return ( ( __attribute__( ( thiscall ) ) float( * )( player*, animlayer_idx, int ) )cs_offsets.player_get_layer_sequence_cycle_rate_fn ) ( this, idx, seq );
}

static inline float player_get_first_sequence_anim_tag( player* this, int seq, int tag, float start, float end ) {
	volatile float ret = 0.0f;

	asm volatile(
		"push %1;"
		"push %2;"
		"push %3;"
		"mov ecx, %4;"
		"call %5;"
		"movss %0, xmm0;"
		: "=g"( ret )
		: "g"( *entity_model_ptr( ( entity* )this ) ), "g"( tag ), "g"( seq ), "g"( this ), "g"( cs_offsets.animstate_get_first_sequence_anim_tag_fn )
		: "ecx", "xmm0"
		);

	return ret;
}

static inline int* player_move_state( player* this ) {
	return ( int* )( ( uintptr_t )this + cs_offsets.player_move_state );
}

/* ty @cbrs */
static inline int player_select_weighted_seq( player* this, anim_activity act ) {
	animstate* animstate = player_animstate( this );
	weapon* weapon = player_get_weapon( this );
	weapon_info* data = weapon ? weapon_data( weapon ) : NULL;

	if ( !animstate )
		return -1;

	int seq = -1;

	const bool crouching = animstate->duck_amount > 0.55f;
	const bool moving = animstate->speed_to_walk_fraction > 0.25f;

	switch ( act ) {
	case act_csgo_land_heavy:
		seq = 23;
		if ( crouching )
			seq = 24;
		break;
	case act_csgo_fall:
		seq = 14;
		break;
	case act_csgo_jump:
		seq = moving + 17;
		if ( !crouching )
			seq = moving + 15;
		break;
	case act_csgo_climb_ladder:
		seq = 13;
		break;
	case act_csgo_land_light:
		seq = 2 * moving + 20;
		if ( crouching ) {
			seq = 21;
			if ( moving )
				seq = 19;
		}
		break;
	case act_csgo_idle_turn_balanceadjust:
		seq = 4;
		break;
	case act_csgo_idle_adjust_stoppedmoving:
		seq = 5;
		break;
	case act_csgo_flashbang_reaction:
		seq = 224;
		if ( crouching )
			seq = 225;
		break;
	case act_csgo_alive_loop:
		seq = 8;
		if ( data && data->type == weapontype_knife )
			seq = 9;
		break;
	default:
		return -1;
	}

	if ( seq < 2 )
		return -1;

	return seq;
}

vec_weapons player_weapons( player* this );
vec_wearables player_wearables( player* this );

static inline void animstate_pose_param_cache_set_value( animstate_pose_param_cache* this, player* player, float value ) {
	asm volatile(
		"push %0;"
		"movss xmm2, %1;"
		"mov ecx, %2;"
		"call %3;"
		:: "g"( this ), "g"( value ), "g"( player ), "g"( cs_offsets.animstate_pose_param_cache_set_value_fn )
		: "xmm2", "ecx"
		);
}

static inline void animlayer_set_seq( animlayer* this, int seq ) {
	if ( this->owner && this->seq != seq )
		player_invalidate_physics_recursive( this->owner, invalidate_phys_bits_bounds_changed );

	this->seq = seq;
}

static inline void animlayer_set_playback_rate( animlayer* this, float rate ) {
	this->playback_rate = rate;
}

static inline void animlayer_set_cycle( animlayer* this, float cycle ) {
	if ( this->owner && this->cycle != cycle )
		player_invalidate_physics_recursive( this->owner, invalidate_phys_bits_anim_changed );

	this->cycle = cycle;
}

static inline void animlayer_set_weight( animlayer* this, float weight ) {
	if ( weight >= 0.0f )
		weight = min( weight, 1.0f );
	else
		weight = 0.0f;

	if ( this->owner && ( this->weight != weight && ( !this->weight || !weight ) ) )
		player_invalidate_physics_recursive( this->owner, invalidate_phys_bits_bounds_changed );

	this->weight = weight;
}

static inline bool server_animstate_cache_sequences( server_animstate* this ) {
	return ( ( __attribute__( ( thiscall ) ) bool( * )( animstate* ) )cs_offsets.animstate_cache_sequences_fn ) ( &this->base );
}

static inline void server_animstate_reset( server_animstate* this ) {
	/* these server members are not set on the client, nor do they exist; we need to remember to initialize them ourselves */
	this->flash_start_time = 0.0f;
	this->flash_end_time = 0.0f;
	this->jumping = false;
	this->lby_realign_timer = 0.0f;
	this->deploy_rate_limiting = false;
	this->last_flash_duration = 0.0f;

	/* the reset can be done with the normal client func */
	//( ( __attribute__( ( thiscall ) ) void( * )( animstate* ) )cs_offsets.animstate_reset_fn ) ( &this->base );
}

static inline const char* server_animstate_get_weapon_move_animation( server_animstate* this ) {
	return ( ( __attribute__( ( thiscall ) ) const char* ( * )( animstate* ) )cs_offsets.animstate_get_weapon_move_animation_fn ) ( &this->base );
}

static inline float server_animstate_get_max_desync( server_animstate* this ) {
	if ( !this || !this->base.player )
		return 0.0f;

	float aim_matrix_width = lerp( 1.0f, lerp( this->base.walk_to_run_fraction, CSGO_ANIM_AIM_NARROW_WALK, CSGO_ANIM_AIM_NARROW_RUN ), clamp( this->base.speed_to_walk_fraction, 0.0f, 1.0f ) );

	if ( this->base.duck_amount > 0.0f )
		aim_matrix_width = lerp( aim_matrix_width, CSGO_ANIM_AIM_NARROW_CROUCHMOVING, this->base.duck_amount * clamp( this->base.speed_to_crouch_fraction, 0.0f, 1.0f ));

	return aim_matrix_width * this->base.aim_yaw_max;
}

static inline void server_animstate_update_layer_order_preset( server_animstate* this, float weight, animlayer_idx idx, int seq ) {
	asm volatile(
		"mov ecx, %0;"
		"movss xmm0, %1;"
		"push %2;"
		"push %3;"
		"call %4;"
		:: "g"( &this->base ), "g"( weight ), "g"( seq ), "g"( idx ), "g"( cs_offsets.animstate_update_layer_order_preset_fn )
		: "ecx", "xmm0"
		);
}

static inline bool server_animstate_is_sequence_completed( server_animstate* this, animlayer_idx idx ) {
	animlayer* layer = &( ( *player_animlayers( this->base.player ) )[ idx ] );
	return layer->cycle + this->base.last_update_delta_time * layer->playback_rate >= 1.0f;
}

static inline void server_animstate_increment_layer_cycle( server_animstate* this, animlayer_idx idx, bool allow_loop ) {
	animlayer* layer = &( ( *player_animlayers( this->base.player ) )[ idx ] );

	if ( fabsf( layer->playback_rate ) <= 0.0f )
		return;

	float cur_cycle = layer->cycle;
	cur_cycle += this->base.last_update_delta_time * layer->playback_rate;

	if ( !allow_loop && cur_cycle >= 1.0f )
		cur_cycle = 0.999f;

	animlayer_set_cycle( layer, cur_cycle );
}

float server_animstate_get_layer_ideal_weight_from_seq_cycle( server_animstate* this, animlayer_idx idx );

static inline void server_animstate_increment_layer_cycle_weight_rate_generic( server_animstate* this, animlayer_idx idx ) {
	const animlayer* layer = &( ( *player_animlayers( this->base.player ) )[ idx ] );
	const float last_weight = layer->weight;
	server_animstate_increment_layer_cycle( this, idx, false );
	animlayer_set_weight( layer, server_animstate_get_layer_ideal_weight_from_seq_cycle( this, idx ) );
	animlayer_set_playback_rate( layer, last_weight );
}

static inline const char* server_animstate_get_weapon_prefix( server_animstate* this ) {
	return ( ( __attribute__( ( thiscall ) ) const char* ( * )( animstate* ) )cs_offsets.animstate_get_weapon_prefix_fn )( &this->base );
}

static inline void server_animstate_set_layer_weight_rate( server_animstate* this, animlayer_idx idx, float previous ) {
	if ( !this->base.last_update_delta_time )
		return;

	animlayer* layer = &( ( *player_animlayers( this->base.player ) )[ idx ] );

	layer->weight_delta_rate = ( layer->weight - previous ) / this->base.last_update_delta_time;
}

static inline anim_activity server_animstate_get_layer_activity( server_animstate* this, animlayer_idx idx ) {
	return ( ( __attribute__( ( thiscall ) ) anim_activity( * )( animstate*, animlayer_idx ) )cs_offsets.animstate_get_layer_activity_fn )( &this->base, idx );
}

void server_animstate_set_layer_sequence( server_animstate* this, animlayer_idx idx, int seq );
void server_animstate_update_animlayer( server_animstate* this, animlayer_idx idx, int seq, float playback_rate, float weight, float cycle );

void server_animstate_set_up_velocity( server_animstate* this );
void server_animstate_set_up_aim_matrix( server_animstate* this );
void server_animstate_set_up_weapon_action( server_animstate* this );
void server_animstate_set_up_movement( server_animstate* this );
void server_animstate_set_up_flashed_reaction( server_animstate* this );
void server_animstate_set_up_whole_body_action( server_animstate* this );
void server_animstate_set_up_flinch( server_animstate* this );
void server_animstate_set_up_lean( server_animstate* this );

void server_animstate_update( server_animstate* this, vec3* ang, bool force );

#endif // !SDK_PLAYER_H
