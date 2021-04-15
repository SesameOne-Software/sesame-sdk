#ifndef SDK_OFFSETS_H
#define SDK_OFFSETS_H

#include <stdint.h>

typedef enum {
	/* beams.h */
	cs_idx_ibeams_init = 1,
	cs_idx_ibeams_shutdown = 3,
	cs_idx_ibeams_clear = 3,
	cs_idx_ibeams_draw = 5,
	cs_idx_ibeams_create_points = 13,
	/* client.h */
	cs_idx_iclient_get_classes = 8,
	/* cvar.h */
	cs_idx_cvar_set_valuesz = 14,
	cs_idx_cvar_set_valuef = 15,
	cs_idx_cvar_set_valuei = 16,
	cs_idx_icvar_find = 14,
	/* engine.h */
	cs_idx_nci_name = 0,
	cs_idx_nci_address = 1,
	cs_idx_nci_time = 2,
	cs_idx_nci_time_connected = 3,
	cs_idx_nci_buffer_size = 4,
	cs_idx_nci_data_rate = 5,
	cs_idx_nci_is_loopback = 6,
	cs_idx_nci_is_timing_out = 7,
	cs_idx_nci_is_playback = 8,
	cs_idx_nci_latency = 9,
	cs_idx_nci_avg_latency = 10,
	cs_idx_iengine_get_screen_size = 5,
	cs_idx_iengine_server_cmd = 6,
	cs_idx_iengine_client_cmd = 7,
	cs_idx_iengine_get_player_info = 8,
	cs_idx_iengine_get_local_player = 12,
	cs_idx_iengine_get_angles = 18,
	cs_idx_iengine_set_angles = 19,
	cs_idx_iengine_is_in_game = 26,
	cs_idx_iengine_is_connected = 27,
	cs_idx_iengine_get_nci = 78,
	cs_idx_iengine_execute_cmd = 108,
	/* entity.h */
	cs_idx_entity_get_client_class = 2,
	/* entlist.h */
	cs_idx_ientlist_get_entity = 3,
	cs_idx_ientlist_get_entity_from_handle = 4,
	cs_idx_ientlist_get_highest_idx = 6,
	/* events.h */
	cs_idx_event_get_name = 1,
	cs_idx_event_is_empty = 4,
	cs_idx_event_get_bool = 5,
	cs_idx_event_get_int = 6,
	cs_idx_event_get_int64 = 7,
	cs_idx_event_get_float = 8,
	cs_idx_event_get_string = 9,
	cs_idx_event_get_wstring = 10,
	cs_idx_event_set_bool = 11,
	cs_idx_event_set_int = 12,
	cs_idx_event_set_int64 = 13,
	cs_idx_event_set_float = 14,
	cs_idx_event_set_string = 15,
	cs_idx_event_set_wstring = 16,
	cs_idx_ievents_add_listener = 3,
	cs_idx_ievents_find_listener = 4,
	cs_idx_ievents_remove_listener = 5,
	/* matsys.h */
	cs_idx_mat_get_name = 0,
	cs_idx_mat_get_texture_group_name = 1,
	cs_idx_mat_find_var = 11,
	cs_idx_mat_increment_ref_count = 12,
	cs_idx_mat_alpha_modulate = 27,
	cs_idx_mat_color_modulate = 28,
	cs_idx_mat_set_mat_var_flag = 29,
	cs_idx_mat_is_error_material = 42,
	cs_idx_imatsys_create_mat = 83,
	cs_idx_imatsys_find_mat = 84,
	cs_idx_imatsys_get_first_mat = 86,
	cs_idx_imatsys_get_next_mat = 87,
	cs_idx_imatsys_get_invalid_mat = 88,
	cs_idx_imatsys_get_mat_from_handle = 89,
	/* mdlcache.h */
	cs_idx_imdlcache_begin_lock = 33,
	cs_idx_imdlcache_end_lock = 34,
	/* mdlinfo.h */
	cs_idx_imdlinfo_get_mdl = 1,
	cs_idx_imdlinfo_get_mdl_idx = 2,
	cs_idx_imdlinfo_get_mdl_name = 3,
	cs_idx_imdlinfo_get_studio_mdl = 32,
	/* mdlrender.h */
	cs_idx_imdlrender_force_mat = 1,
	cs_idx_imdlrender_is_forced_mat_override = 2,
	/* memalloc.h */
	cs_idx_imemalloc_alloc = 1,
	cs_idx_imemalloc_realloc = 2,
	cs_idx_imemalloc_free = 3,
	/* physics.h */
	cs_idx_get_surface = 5,
	/* player.h */
	cs_idx_player_set_local_viewangles = 371,
	cs_idx_player_think = 138,
	cs_idx_player_pre_think = 317,
	cs_idx_player_post_think = 318,
	cs_idx_player_world_space = 17,
	cs_idx_player_abs_origin = 10,
	cs_idx_player_abs_angles = 11,
	cs_idx_player_mdl = 8,
	cs_idx_player_studio_mdl = 32,
	cs_idx_player_render_origin = 1,
	cs_idx_player_get_shoot_pos = 168,
	/* pred.h */
	cs_idx_imovehelper_set_host = 1,
	cs_idx_ipred_get_local_viewangles = 12,
	cs_idx_ipred_set_local_viewangles = 13,
	cs_idx_ipred_update = 3,
	cs_idx_ipred_check_moving_ground = 18,
	cs_idx_ipred_setup_move = 20,
	cs_idx_ipred_finish_move = 21,

	cs_idx_imove_process_movement = 1,
	cs_idx_imove_reset = 2,
	cs_idx_imove_start_track_pred_errors = 3,
	cs_idx_imove_finish_track_pred_errors = 4,
	/* renderview.h */
	cs_idx_irenderview_set_alpha = 4,
	cs_idx_irenderview_set_color = 6,
	/* sdk.h */
	cs_idx_idbgoverlay_add_box_overlay = 1,
	/* surface.h */
	cs_idx_isurface_unlock_cursor = 66,
	cs_idx_isurface_lock_cursor = 67,
	cs_idx_isurface_get_cursor_pos = 100,
	cs_idx_ipanel_get_name = 36,
	/* trace.h */
	cs_idx_itrace_get_point_contents = 0,
	cs_idx_itrace_get_point_contents_world_only = 1,
	cs_idx_itrace_get_point_contents_collideable = 2,
	cs_idx_itrace_clip_ray_to_entity = 3,
	cs_idx_itrace_clip_ray_to_collideable = 4,
	cs_idx_itrace_trace_ray = 5,
	/* weapon.h */
	cs_idx_econitem_inventory_img = 5,
	cs_idx_weapon_update_accuracy = 483,
	cs_idx_weapon_innacuracy = 482,
	cs_idx_weapon_spread = 452,
	cs_idx_weapon_max_speed = 441,
	cs_idx_weaponsys_data = 2,
} cs_vfunc_idx;

typedef struct {
	ptrdiff_t iclientstate_nc;
	ptrdiff_t iclientstate_choked;
	ptrdiff_t iclientstate_last_cmd_ack;
	ptrdiff_t iclientstate_last_out_cmd;
	ptrdiff_t iclientstate_server_tick;
	ptrdiff_t iclientstate_delta_tick;
	ptrdiff_t iclientstate_next_cmd_time;
	ptrdiff_t iclientstate_out_seq_num;
	ptrdiff_t iclientstate_events;

	ptrdiff_t entity_idx;
	ptrdiff_t entity_dormant;
	ptrdiff_t entity_eflags;
	ptrdiff_t entity_effects;
	ptrdiff_t entity_abs_vel;
	ptrdiff_t entity_studiohdr;

	ptrdiff_t player_movetype;
	ptrdiff_t player_iks;
	ptrdiff_t player_spawn_time;
	ptrdiff_t player_bones;
	ptrdiff_t player_animlayers;
	ptrdiff_t player_num_animlayers;
	ptrdiff_t player_poses;
	ptrdiff_t player_jiggle_bones;
	ptrdiff_t player_group_hdr;

	ptrdiff_t animstate_reset_fn;
	ptrdiff_t animstate_get_weapon_move_animation_fn;
	ptrdiff_t player_get_sequence_linear_motion_fn;
	ptrdiff_t player_lookup_sequence_fn;
	ptrdiff_t player_lookup_bone_fn;
	ptrdiff_t player_sequence_duration_fn;
	ptrdiff_t player_physics_run_think_fn;
	ptrdiff_t player_handle_fn;
	ptrdiff_t player_create_animstate_fn;
	ptrdiff_t player_set_abs_angles_fn;
	ptrdiff_t player_set_abs_origin_fn;
	ptrdiff_t player_animstate;
	ptrdiff_t player_get_eye_pos_fn;
	ptrdiff_t player_bone_count;
	ptrdiff_t player_bone_cache;
	ptrdiff_t player_is_local;

	ptrdiff_t sdk_view_matrix;
	ptrdiff_t sdk_game_rules;
	ptrdiff_t sdk_dbgview;
	ptrdiff_t sdk_is_valve_ds;

	ptrdiff_t econitem_account_id;
	ptrdiff_t econitem_icon_name;
	ptrdiff_t econitem_mdl_name;
	ptrdiff_t econitem_world_mdl_name;
	ptrdiff_t econitem_item_id;
	ptrdiff_t econitem_original_id;
	ptrdiff_t econitem_item_definition_index;
	ptrdiff_t econitem_inventory;
	ptrdiff_t econitem_flags;
	ptrdiff_t econitem_equipped_position;
	ptrdiff_t econitem_definition_name;

	ptrdiff_t econitem_static_data_fn;
	ptrdiff_t econitem_update_equipped_state_fn;
	ptrdiff_t econitem_clean_inventory_image_cache_dir_fn;
	ptrdiff_t econitem_set_or_add_attribute_by_name_fn;
	ptrdiff_t econitem_set_custom_name_fn;
	ptrdiff_t econitem_set_custom_desc_fn;
	ptrdiff_t econitem_get_item_schema_fn;
	ptrdiff_t econitem_set_dynamic_attribute_val_fn;
	ptrdiff_t econitem_soc_data_fn;
	ptrdiff_t weapon_sys;

	ptrdiff_t accumulate_pose_fn;

	ptrdiff_t cam_think_ret;
	ptrdiff_t maintain_sequence_transitions_ret;
	ptrdiff_t loadout_allowed_ret;
	ptrdiff_t accumulate_layers_ret;
	ptrdiff_t list_leaves_in_box_ret;
} cs_offsets_s;

extern cs_offsets_s cs_offsets;

#endif // !SDK_OFFSETS_H