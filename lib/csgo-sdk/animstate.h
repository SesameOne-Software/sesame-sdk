#ifndef SDK_ANIMSTATE_H
#define SDK_ANIMSTATE_H

#include "weapon.h"
#include "mat3x4.h"
#include "mdlinfo.h"
#include "entity.h"
#include "utlvec.h"

/* adjust some code in animation update function to make it more straightforward to use */
#define SES_ANIMSTATE_ADJUSTMENTS

#define ANIMATION_LAYER_COUNT 13

#define ANIM_LAYER_ACTIVE 1

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
	invalidate_phys_bits_pos_changed = (1 << 0),
	invalidate_phys_bits_angles_changed = (1 << 1),
	invalidate_phys_bits_vel_changed = (1 << 2),
	invalidate_phys_bits_anim_changed = (1 << 3),
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
	int	seq;
	float previous_cycle;
	float weight;
	float weight_delta_rate;
	float playback_rate;
	float cycle;
	player* owner;
	int invalidate_physics_bit;
} animlayer;

static inline void animlayer_set_seq ( animlayer* this, int seq ) {
	if ( this->owner && this->seq != seq )
		player_invalidate_physics_recursive ( this->owner, invalidate_phys_bits_bounds_changed );

	this->seq = seq;
}

static inline void animlayer_set_playback_rate ( animlayer* this, float rate ) {
	this->playback_rate = rate;
}

static inline void animlayer_set_cycle ( animlayer* this, float cycle ) {
	if ( this->owner && this->cycle != cycle )
		player_invalidate_physics_recursive ( this->owner, invalidate_phys_bits_anim_changed );

	this->cycle = cycle;
}

static inline void animlayer_set_weight ( animlayer* this, float weight ) {
	if ( weight >= 0.0 )
		weight = fminf ( weight, 1.0 );
	else
		weight = 0.0;

	if ( this->owner && ( this->weight != weight && ( !this->weight || !weight ) ) )
		player_invalidate_physics_recursive ( this->owner, invalidate_phys_bits_bounds_changed );

	this->weight = weight;
}

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
	/* these server members are not set on the client, nor do they exist; we need to remember to initialize them ourselves */
	this->flash_start_time = 0.0f;
	this->flash_end_time = 0.0f;
	this->jumping = false;
	this->lby_realign_timer = 0.0f;
	this->deploy_rate_limiting = false;

	/* the reset can be done with the normal client func */
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

static inline void server_animstate_update_layer_order_preset ( server_animstate* this, float weight, animlayer_idx idx, int seq ) {
	animstate* client_animstate = &this->base;
	void* update_layer_order_preset_fn = ( void* ) cs_offsets.animstate_update_layer_order_preset_fn;

	__asm {
		mov ecx, client_animstate
		movss xmm0, weight
		push seq
		push idx
		call update_layer_order_preset_fn
	}
}

static inline bool server_animstate_is_sequence_completed ( server_animstate* this, animlayer_idx idx ) {
	animlayer* layer = &( ( *player_animlayers ( this->base.player ) ) [ idx ] );
	return layer->cycle + this->base.last_update_delta_time * layer->playback_rate >= 1.0f;
}

void server_animstate_set_layer_sequence ( server_animstate* this, animlayer_idx idx, int seq );
void server_animstate_update_animlayer (server_animstate* this, animlayer_idx idx, int seq, float playback_rate, float weight, float cycle);

void server_animstate_update ( server_animstate* this, vec3* ang, bool force );

#endif SDK_ANIMSTATE_H