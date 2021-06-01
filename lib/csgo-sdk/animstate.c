#include "sdk.h"

float server_animstate_get_layer_ideal_weight_from_seq_cycle( server_animstate* this, animlayer_idx idx ) {
	volatile float ret = 1.0f;

	asm volatile(
		"push %1;"
		"mov ecx, %2;"
		"call %3;"
		"movss %0, xmm0;"
		: "=g"( ret )
		: "g"( idx ), "g"( &this->base ), "g"( cs_offsets.animstate_get_layer_ideal_weight_from_seq_cycle_fn )
		: "ecx", "xmm0"
		);

	return ret;
}

void server_animstate_set_layer_sequence( server_animstate* this, animlayer_idx idx, int seq ) {
	assert( seq > 1 );

	if ( seq > 1 ) {
		MDLCACHE_CRITICAL_SECTION_START;

		animlayer* layer = &( ( *player_animlayers( this->base.player ) )[ idx ] );

		animlayer_set_seq( layer, seq );
		animlayer_set_playback_rate( layer, player_get_layer_sequence_cycle_rate( this->base.player, idx, seq ) );
		animlayer_set_cycle( layer, 0.0f );
		animlayer_set_weight( layer, 0.0f );

		server_animstate_update_layer_order_preset( this, 0.0f, idx, seq );

		MDLCACHE_CRITICAL_SECTION_END;
	}
}

void server_animstate_update_animlayer( server_animstate* this, animlayer_idx idx, int seq, float playback_rate, float weight, float cycle ) {
	assert( weight >= 0.0f && weight <= 1.0f );
	assert( cycle >= 0.0f && cycle <= 1.0f );
	assert( seq > 1 );

	if ( seq > 1 ) {
		MDLCACHE_CRITICAL_SECTION_START;

		animlayer* layer = &( ( *player_animlayers( this->base.player ) )[ idx ] );

		animlayer_set_seq( layer, seq );
		animlayer_set_playback_rate( layer, playback_rate );
		animlayer_set_cycle( layer, clampf( cycle, 0.0f, 1.0f ) );
		animlayer_set_weight( layer, clampf( weight, 0.0f, 1.0f ) );

		server_animstate_update_layer_order_preset( this, weight, idx, seq );

		MDLCACHE_CRITICAL_SECTION_END;
	}
}

void server_animstate_set_up_velocity( server_animstate* this ) {
	MDLCACHE_CRITICAL_SECTION_START;

	vec3 abs_vel = *entity_abs_vel( ( entity* )this->base.player );

	this->base.vel_lenz = abs_vel.z;

	abs_vel.z = 0.0f;

	this->base.in_accel = vec3_len_sqr( &this->base.last_vel ) < vec3_len_sqr( &abs_vel );

	vec3_approach_vec( &abs_vel, &this->base.vel, this->base.last_update_delta_time * 2000.0f, &this->base.vel );
	this->base.vel_norm = this->base.vel;
	vec3_norm( &this->base.vel_norm );

	const float vel_len = vec3_len( &this->base.vel );
	this->base.vel_len2d = min( vel_len, CS_PLAYER_SPEED_RUN );

	if ( this->base.vel_len2d > 0.0f )
		this->base.vel_norm_nonzero = this->base.vel_norm;

	//compute speed in various normalized forms
	const float max_weapon_speed = weapon_max_speed( this->base.weapon );
	const float max_speed_run = this->base.weapon ? max( max_weapon_speed, 0.001f ) : CS_PLAYER_SPEED_RUN;
	assert( max_speed_run > 0.0f );

	this->base.speed_to_run_fraction = clampf( this->base.vel_len2d / max_speed_run, 0.0f, 1.0f );
	this->base.speed_to_walk_fraction = this->base.vel_len2d / ( max_speed_run * CS_PLAYER_SPEED_WALK_MODIFIER );
	this->base.speed_to_crouch_fraction = this->base.vel_len2d / ( max_speed_run * CS_PLAYER_SPEED_DUCK_MODIFIER );

	if ( this->base.speed_to_walk_fraction >= 1.0f )
		this->base.static_lerp_speed = this->base.vel_len2d;
	else if ( this->base.speed_to_walk_fraction < 0.5f )
		this->base.static_lerp_speed = cs_approachf( 80.0f, this->base.static_lerp_speed, this->base.last_update_delta_time * 60.0f );

	bool started_moving = false;
	bool stopped_moving = false;

	if ( this->base.vel_len2d > 0.0f ) {
		started_moving = this->base.moving_time <= 0.0f;
		this->base.still_time = 0.0f;
		this->base.moving_time += this->base.last_update_delta_time;
	}
	else {
		stopped_moving = this->base.still_time <= 0.0f;
		this->base.moving_time = 0.0f;
		this->base.still_time += this->base.last_update_delta_time;
	}

	if ( !this->base.in_adjust && stopped_moving && this->base.on_ground && !this->base.on_ladder && !this->base.landing && this->base.stutter_step < 50.0f ) {
		server_animstate_set_layer_sequence( this, animlayer_adjust, player_select_weighted_seq( this->base.player, act_csgo_idle_adjust_stoppedmoving ) );
		this->base.in_adjust = true;
	}

	if ( server_animstate_get_layer_activity( this, animlayer_adjust ) == act_csgo_idle_adjust_stoppedmoving
		|| server_animstate_get_layer_activity( this, animlayer_adjust ) == act_csgo_idle_turn_balanceadjust ) {
		if ( this->base.in_adjust && this->base.speed_to_crouch_fraction <= 0.25f ) {
			server_animstate_increment_layer_cycle_weight_rate_generic( this, animlayer_adjust );
			this->base.in_adjust = !server_animstate_is_sequence_completed( this, animlayer_adjust );
		}
		else {
			this->base.in_adjust = false;
			const float weight = ( *player_animlayers( this->base.player ) )[ animlayer_adjust ].weight;
			animlayer_set_weight( &( ( *player_animlayers( this->base.player ) )[ animlayer_adjust ] ), cs_approachf( 0.0f, weight, this->base.last_update_delta_time * 5.0f ) );
			server_animstate_set_layer_weight_rate( this, animlayer_adjust, weight );
		}
	}

	// if the player is looking far enough to either side, turn the feet to keep them within the extent of the aim matrix
	this->base.last_foot_yaw = this->base.foot_yaw;
	this->base.foot_yaw = clampf( this->base.foot_yaw, -360.0f, 360.0f );
	const float eye_foot_delta = cs_angle_diff( this->base.eye_yaw, this->base.foot_yaw );

	// narrow the available aim matrix width as speed increases
	float aim_matrix_width = lerpf( clampf( this->base.speed_to_walk_fraction, 0.0f, 1.0f ), 1.0f, lerpf( this->base.walk_to_run_fraction, CSGO_ANIM_AIM_NARROW_WALK, CSGO_ANIM_AIM_NARROW_RUN ) );

	if ( this->base.duck_amount > 0.0f )
		aim_matrix_width = lerpf( this->base.duck_amount * clampf( this->base.speed_to_crouch_fraction, 0.0f, 1.0f ), aim_matrix_width, CSGO_ANIM_AIM_NARROW_CROUCHMOVING );

	const float temp_yaw_max = this->base.aim_yaw_max * aim_matrix_width;
	const float temp_yaw_min = this->base.aim_yaw_min * aim_matrix_width;

	if ( eye_foot_delta > temp_yaw_max )
		this->base.foot_yaw = this->base.eye_yaw - fabsf( temp_yaw_max );
	else if ( eye_foot_delta < temp_yaw_min )
		this->base.foot_yaw = this->base.eye_yaw + fabsf( temp_yaw_min );

	this->base.foot_yaw = cs_norm_rotation( this->base.foot_yaw );

	// pull the lower body direction towards the eye direction, but only when the player is moving
	if ( this->base.on_ground ) {
		if ( this->base.vel_len2d > 0.1f ) {
			this->base.foot_yaw = cs_approachf( this->base.eye_yaw, this->base.foot_yaw, this->base.last_update_delta_time * ( 30.0f + 20.0f * this->base.walk_to_run_fraction ) );

			this->lby_realign_timer = cs_iglobals->curtime + CSGO_ANIM_LOWER_REALIGN_DELAY * 0.2f;
			*player_lby( this->base.player ) = this->base.eye_yaw;
		}
		else {
			this->base.foot_yaw = cs_approachf( *player_lby( this->base.player ), this->base.foot_yaw, this->base.last_update_delta_time * CSGO_ANIM_LOWER_CATCHUP_IDLE );

			if ( cs_iglobals->curtime > this->lby_realign_timer && abs( cs_angle_diff( this->base.foot_yaw, this->base.eye_yaw ) ) > 35.0f ) {
				this->lby_realign_timer = cs_iglobals->curtime + CSGO_ANIM_LOWER_REALIGN_DELAY;
				*player_lby( this->base.player ) = this->base.eye_yaw;
			}
		}
	}

	if ( this->base.vel_len2d <= CS_PLAYER_SPEED_STOPPED && this->base.on_ground && !this->base.on_ladder && !this->base.landing && this->base.last_update_delta_time > 0.0f && abs( cs_angle_diff( this->base.last_foot_yaw, this->base.foot_yaw ) / this->base.last_update_delta_time > CSGO_ANIM_READJUST_THRESHOLD ) ) {
		server_animstate_set_layer_sequence( this, animlayer_adjust, player_select_weighted_seq( this->base.player, act_csgo_idle_turn_balanceadjust ) );
		this->base.in_adjust = true;
	}

	// the final model render yaw is aligned to the foot yaw

	if ( this->base.vel_len2d > 0.0f && this->base.on_ground ) {
		// convert horizontal velocity vec to angular yaw
		float max_yaw_ideal = atan2f( -this->base.vel.y, -this->base.vel.x ) * 180.0f / M_PI;

		if ( max_yaw_ideal < 0.0f )
			max_yaw_ideal += 360.0f;

		this->base.ideal_move_yaw = cs_norm_rotation( cs_angle_diff( max_yaw_ideal, this->base.foot_yaw ) );
	}

	// delta between current yaw and ideal velocity derived target (possibly negative!)
	this->base.move_yaw_to_ideal = cs_norm_rotation( cs_angle_diff( this->base.ideal_move_yaw, this->base.move_yaw ) );


	if ( started_moving && this->base.move_weight <= 0.0f ) {
		this->base.move_yaw = this->base.ideal_move_yaw;

		// select a special starting cycle that's set by the animator in content
		const int move_seq = ( *player_animlayers( this->base.player ) )[ animlayer_movement_move ].seq;

		if ( move_seq != -1 ) {
			const void* seq_desc = studiohdr_seq_desc( this->base.player, move_seq );

			if ( *( int* )( ( uintptr_t )seq_desc + 0xC4 ) > 0 ) {
				if ( fabsf( cs_angle_diff( this->base.move_yaw, 180.0f ) ) <= EIGHT_WAY_WIDTH )
					this->base.primary_cycle = player_get_first_sequence_anim_tag( this->base.player, move_seq, 1, 0.0f, 1.0f );
				else if ( fabsf( cs_angle_diff( this->base.move_yaw, 135.0f ) ) <= EIGHT_WAY_WIDTH )
					this->base.primary_cycle = player_get_first_sequence_anim_tag( this->base.player, move_seq, 2, 0.0f, 1.0f );
				else if ( fabsf( cs_angle_diff( this->base.move_yaw, 90.0f ) ) <= EIGHT_WAY_WIDTH )
					this->base.primary_cycle = player_get_first_sequence_anim_tag( this->base.player, move_seq, 3, 0.0f, 1.0f );
				else if ( fabsf( cs_angle_diff( this->base.move_yaw, 45.0f ) ) <= EIGHT_WAY_WIDTH )
					this->base.primary_cycle = player_get_first_sequence_anim_tag( this->base.player, move_seq, 4, 0.0f, 1.0f );
				else if ( fabsf( cs_angle_diff( this->base.move_yaw, 0.0f ) ) <= EIGHT_WAY_WIDTH )
					this->base.primary_cycle = player_get_first_sequence_anim_tag( this->base.player, move_seq, 5, 0.0f, 1.0f );
				else if ( fabsf( cs_angle_diff( this->base.move_yaw, -45.0f ) ) <= EIGHT_WAY_WIDTH )
					this->base.primary_cycle = player_get_first_sequence_anim_tag( this->base.player, move_seq, 6, 0.0f, 1.0f );
				else if ( fabsf( cs_angle_diff( this->base.move_yaw, -90.0f ) ) <= EIGHT_WAY_WIDTH )
					this->base.primary_cycle = player_get_first_sequence_anim_tag( this->base.player, move_seq, 7, 0.0f, 1.0f );
				else if ( fabsf( cs_angle_diff( this->base.move_yaw, -135.0f ) ) <= EIGHT_WAY_WIDTH )
					this->base.primary_cycle = player_get_first_sequence_anim_tag( this->base.player, move_seq, 8, 0.0f, 1.0f );
			}
		}
	}
	else {
		if ( ( *player_animlayers( this->base.player ) )[ animlayer_movement_strafechange ].weight >= 1.0f ) {
			this->base.move_yaw = this->base.ideal_move_yaw;;
		}
		else {
			const float weight_perms = lerpf( this->base.duck_amount, clampf( this->base.speed_to_walk_fraction, 0.0f, 1.0f ), clampf( this->base.speed_to_crouch_fraction, 0.0f, 1.0f ) );
			const float fraction_waiting = cs_bias( weight_perms, 0.18f ) + 0.1f;

			this->base.move_yaw = cs_norm_rotation( this->base.move_yaw + ( this->base.move_yaw_to_ideal * fraction_waiting ) );
		}
	}

	animstate_pose_param_cache_set_value( &this->base.pose_param_map[ pose_param_move_yaw ], this->base.player, this->base.move_yaw );

	float aim_yaw = cs_angle_diff( this->base.eye_yaw, this->base.foot_yaw );

	if ( aim_yaw >= 0.0f && this->base.aim_yaw_max )
		aim_yaw = ( aim_yaw / this->base.aim_yaw_max ) * 60.0f;
	else if ( this->base.aim_yaw_min )
		aim_yaw = ( aim_yaw / this->base.aim_yaw_min ) * -60.0f;

	animstate_pose_param_cache_set_value( &this->base.pose_param_map[ pose_param_body_yaw ], this->base.player, aim_yaw );

	// we need non-symmetrical arbitrary min/max bounds for vertical aim (pitch) too
	float pitch = cs_angle_diff( this->base.eye_pitch, 0.0f );

	if ( pitch > 0.0f )
		pitch = ( pitch / this->base.aim_pitch_max ) * CSGO_ANIM_AIMMATRIX_DEFAULT_PITCH_MAX;
	else
		pitch = ( pitch / this->base.aim_pitch_min ) * CSGO_ANIM_AIMMATRIX_DEFAULT_PITCH_MIN;

	animstate_pose_param_cache_set_value( &this->base.pose_param_map[ pose_param_body_pitch ], this->base.player, pitch );
	animstate_pose_param_cache_set_value( &this->base.pose_param_map[ pose_param_speed ], this->base.player, this->base.speed_to_walk_fraction );
	animstate_pose_param_cache_set_value( &this->base.pose_param_map[ pose_param_stand ], this->base.player, 1.0f - ( this->base.duck_amount * this->base.air_lerp ) );

	MDLCACHE_CRITICAL_SECTION_END;
}

/* seems to be same on server and client; no problem here! */
void server_animstate_set_up_aim_matrix( server_animstate* this ) {
	( ( __attribute__( ( thiscall ) ) void( * )( animstate* ) ) cs_offsets.animstate_setup_aim_matrix_fn )( &this->base );
}

void server_animstate_set_up_weapon_action( server_animstate* this ) {
	bool do_increment = true;

	/* TODO: for danger zone; let's leave this out for now */
	//if (this->base.weapon && this->deploy_rate_limiting && server_animstate_get_layer_activity(this, animlayer_weapon_action) == act_csgo_deploy) {
	//	//m_pWeapon->ShowWeaponWorldModel(false);
	//
	//	if ((*player_animlayers(this->base.player))[animlayer_weapon_action].cycle >= CSGO_ANIM_DEPLOY_RATELIMIT) {
	//		this->deploy_rate_limiting = false;
	//		server_animstate_set_layer_sequence(this, animlayer_weapon_action, player_select_weighted_seq ( this->base.player, act_csgo_deploy));
	//		animlayer_set_weight(&((*player_animlayers(this->base.player))[animlayer_weapon_action]), 0.0f );
	//		do_increment = false;
	//	}
	//}

	float target_recrouch_weight = 0.0f;

	if ( ( *player_animlayers( this->base.player ) )[ animlayer_weapon_action ].weight > 0.0f ) {
		if ( ( *player_animlayers( this->base.player ) )[ animlayer_weapon_action_recrouch ].seq <= 0 )
			server_animstate_set_layer_sequence( this, animlayer_weapon_action_recrouch, player_lookup_sequence( this->base.player, "recrouch_generic" ) );

		/* ghetto workaround so i dont have to rebuild another function */
		/* THIS IS WRONG!! */
		if (/*LayerSequenceHasActMod( animlayer_weapon_action, "crouch" )*/ this->base.duck_amount > 0.55f ) {
			if ( this->base.duck_amount < 1.0f )
				target_recrouch_weight = ( *player_animlayers( this->base.player ) )[ animlayer_weapon_action ].weight * ( 1.0f - this->base.duck_amount );
		}
		else if ( this->base.duck_amount > 0.0f )
			target_recrouch_weight = ( *player_animlayers( this->base.player ) )[ animlayer_weapon_action ].weight * this->base.duck_amount;
	}
	else {
		if ( ( *player_animlayers( this->base.player ) )[ animlayer_weapon_action_recrouch ].weight > 0.0f )
			target_recrouch_weight = cs_approachf( 0.0f, ( *player_animlayers( this->base.player ) )[ animlayer_weapon_action_recrouch ].weight, this->base.last_update_delta_time * 4.0f );
	}

	animlayer_set_weight( &( ( *player_animlayers( this->base.player ) )[ animlayer_weapon_action_recrouch ] ), target_recrouch_weight );

	if ( do_increment ) {
		// increment the action
		server_animstate_increment_layer_cycle( this, animlayer_weapon_action, false );

		const float previous_weight = ( *player_animlayers( this->base.player ) )[ animlayer_weapon_action ].weight;
		const float desired_weight = server_animstate_get_layer_ideal_weight_from_seq_cycle( this, animlayer_weapon_action );

		animlayer_set_weight( &( ( *player_animlayers( this->base.player ) )[ animlayer_weapon_action ] ), previous_weight );
		server_animstate_set_layer_weight_rate( this, animlayer_weapon_action, previous_weight );
	}
}

void server_animstate_set_up_movement( server_animstate* this ) {
	MDLCACHE_CRITICAL_SECTION_START;

	if ( this->base.walk_to_run_fraction > 0.0f && this->base.walk_to_run_fraction < 1.0f ) {
		if ( this->base.walk_to_run_state == ANIM_TRANSITION_WALK_TO_RUN )
			this->base.walk_to_run_fraction += this->base.last_update_delta_time * CSGO_ANIM_WALK_TO_RUN_TRANSITION_SPEED;
		else
			this->base.walk_to_run_fraction -= this->base.last_update_delta_time * CSGO_ANIM_WALK_TO_RUN_TRANSITION_SPEED;

		this->base.walk_to_run_fraction = clampf( this->base.walk_to_run_fraction, 0.0f, 1.0f );
	}

	if ( this->base.vel_len2d > ( CS_PLAYER_SPEED_RUN * CS_PLAYER_SPEED_WALK_MODIFIER ) && this->base.walk_to_run_state == ANIM_TRANSITION_RUN_TO_WALK ) {
		this->base.walk_to_run_state = ANIM_TRANSITION_WALK_TO_RUN;
		this->base.walk_to_run_fraction = max( 0.01f, this->base.walk_to_run_fraction );
	}
	else if ( this->base.vel_len2d < ( CS_PLAYER_SPEED_RUN * CS_PLAYER_SPEED_WALK_MODIFIER ) && this->base.walk_to_run_state == ANIM_TRANSITION_WALK_TO_RUN ) {
		this->base.walk_to_run_state = ANIM_TRANSITION_RUN_TO_WALK;
		this->base.walk_to_run_fraction = min( 0.99f, this->base.walk_to_run_fraction );
	}

	animstate_pose_param_cache_set_value( &this->base.pose_param_map[ pose_param_move_blend_walk ], this->base.player, ( 1.0f - this->base.walk_to_run_fraction ) * ( 1.0f - this->base.duck_amount ) );
	animstate_pose_param_cache_set_value( &this->base.pose_param_map[ pose_param_move_blend_run ], this->base.player, this->base.walk_to_run_fraction * ( 1.0f - this->base.duck_amount ) );
	animstate_pose_param_cache_set_value( &this->base.pose_param_map[ pose_param_move_blend_crouch_walk ], this->base.player, this->base.duck_amount );

	char weapon_move_seq[ MAX_ANIMSTATE_ANIMNAME_CHARS ];
	sprintf( weapon_move_seq, "move_%s", server_animstate_get_weapon_prefix( this ) );

	int move_seq = player_lookup_sequence( this->base.player, weapon_move_seq );

	if ( move_seq == -1 )
		move_seq = player_lookup_sequence( this->base.player, "move" );

	assert( weapon_move_seq > 0 );

	if ( *player_move_state( this->base.player ) != this->base.last_move_state )
		this->base.stutter_step += 10.0f;

	this->base.last_move_state = *player_move_state( this->base.player );
	this->base.stutter_step = clampf( cs_approachf( 0.0f, this->base.stutter_step, this->base.last_update_delta_time * 40.0f ), 0.0f, 100.0f );

	const float target_move_weight = lerpf( this->base.duck_amount, clampf( this->base.speed_to_walk_fraction, 0.0f, 1.0f ), clampf( this->base.speed_to_crouch_fraction, 0.0f, 1.0f ) );

	if ( this->base.move_weight <= target_move_weight )
		this->base.move_weight = target_move_weight;
	else
		this->base.move_weight = cs_approachf( target_move_weight, this->base.move_weight, this->base.last_update_delta_time * cs_remap_val_clamped( this->base.stutter_step, 0.0f, 100.0f, 2.0f, 20.0f ) );

	vec3 move_yaw_dir = ( vec3 ){ 0.0f, cs_norm_rotation( this->base.foot_yaw + this->base.move_yaw + 180.0f ), 0.0f };
	vec3_to_vec( &move_yaw_dir );
	this->base.move_weight *= cs_bias( fabsf( vec3_dot( &this->base.vel_norm_nonzero, &move_yaw_dir ) ), 0.2f );

	float move_weight_with_air_smooth = this->base.move_weight * this->base.air_lerp;
	move_weight_with_air_smooth *= max( ( 1.0f - ( *player_animlayers( this->base.player ) )[ animlayer_movement_land_or_climb ].weight ), 0.55f );

	float move_cycle_rate = 0.0f;

	if ( this->base.vel_len2d > 0.0f ) {
		move_cycle_rate = player_get_sequence_cycle_rate_server( this->base.player, move_seq );
		const float move_dist_vs_rate = player_get_sequence_move_distance( this->base.player, move_seq ) / ( 1.0f / move_cycle_rate );
		const float seq_ground_speed = max( move_dist_vs_rate, 0.001f );
		move_cycle_rate *= this->base.vel_len2d / seq_ground_speed;
		move_cycle_rate *= lerpf( this->base.walk_to_run_fraction, 1.0f, CSGO_ANIM_RUN_ANIM_PLAYBACK_MULTIPLIER );
	}

	const float cycle_increment = move_cycle_rate * this->base.last_update_delta_time;
	this->base.primary_cycle = cs_clamp_cycle( this->base.primary_cycle + cycle_increment );

	move_weight_with_air_smooth = clampf( move_weight_with_air_smooth, 0.0f, 1.0f );
	server_animstate_update_animlayer( this, animlayer_movement_move, move_seq, cycle_increment, move_weight_with_air_smooth, this->base.primary_cycle );

	if ( player_is_local( this->base.player ) ) {
		/* TODO: UPDATE BUTTONS IN ENGINE PREDICTION */
		bool moving_right = *player_buttons( this->base.player ) & in_moveright;
		bool moving_left = *player_buttons( this->base.player ) & in_moveleft;
		bool moving_forward = *player_buttons( this->base.player ) & in_forward;
		bool moving_backward = *player_buttons( this->base.player ) & in_back;

		vec3 vec_forward;
		vec3 vec_right;

		vec3_to_vecs( &( vec3 ) { 0.0f, this->base.foot_yaw, 0.0f }, & vec_forward, & vec_right, NULL );

		vec3_norm( &vec_forward );
		vec3_norm( &vec_right );

		float right_dot = vec3_dot( &this->base.vel_norm_nonzero, &vec_right );
		float forward_dot = vec3_dot( &this->base.vel_norm_nonzero, &vec_forward );

		bool strafe_right = this->base.speed_to_walk_fraction >= 0.73f && moving_right && !moving_left && right_dot < -0.63f;
		bool strafe_left = this->base.speed_to_walk_fraction >= 0.73f && moving_left && !moving_right && right_dot > 0.63f;
		bool strafe_forward = this->base.speed_to_walk_fraction >= 0.65f && moving_forward && !moving_backward && forward_dot < -0.55f;
		bool strafe_back = this->base.speed_to_walk_fraction >= 0.65f && moving_backward && !moving_forward && forward_dot > 0.55f;

		*player_strafing( this->base.player ) = strafe_right || strafe_left || strafe_forward || strafe_back;
	}

	if ( *player_strafing( this->base.player ) ) {
		if ( !this->base.strafe_changing )
			this->base.strafe_time = 0.0f;

		this->base.strafe_changing = true;

		this->base.strafe_change_weight = cs_approachf( 1.0f, this->base.strafe_change_weight, this->base.last_update_delta_time * 20.0f );
		this->base.strafe_change_cycle = cs_approachf( 0.0f, this->base.strafe_change_cycle, this->base.last_update_delta_time * 10.0f );

		animstate_pose_param_cache_set_value( &this->base.pose_param_map[ pose_param_strafe_dir ], this->base.player, cs_norm_rotation( this->base.move_yaw ) );
	}
	else if ( this->base.strafe_change_weight > 0.0f ) {
		this->base.strafe_time += this->base.last_update_delta_time;

		if ( this->base.strafe_time > 0.08f )
			this->base.strafe_change_weight = cs_approachf( 0.0f, this->base.strafe_change_weight, this->base.last_update_delta_time * 5.0f );

		this->base.strafe_seq = player_lookup_sequence( this->base.player, "strafe" );
		this->base.strafe_change_cycle = clampf( this->base.strafe_change_cycle + this->base.last_update_delta_time * player_get_sequence_cycle_rate_server( this->base.player, this->base.strafe_seq ), 0.0f, 1.0f );
	}

	if ( this->base.strafe_change_weight <= 0.0f )
		this->base.strafe_changing = false;

	const bool was_on_ground = this->base.on_ground;
	this->base.on_ground = *player_flags( this->base.player ) & fl_on_ground;

	this->base.just_landed = !this->base.first_run && was_on_ground != this->base.on_ground && this->base.on_ground;
	this->base.just_jumped = was_on_ground != this->base.on_ground && !this->base.on_ground;

	if ( this->base.just_jumped )
		this->base.left_ground_height = this->base.pos.z;

	if ( this->base.just_landed ) {
		const float distance_fell = fabsf( this->base.left_ground_height - this->base.pos.z );
		const float distance_fell_norm_bias_range = cs_bias( cs_remap_val_clamped( distance_fell, 12.0f, 72.0f, 0.0f, 1.0f ), 0.4f );

		this->base.land_anim_factor = clampf( cs_bias( this->base.air_time, 0.3f ), 0.1f, 1.0f );
		this->base.duck_additional = max( this->base.land_anim_factor, distance_fell_norm_bias_range );
	}
	else {
		this->base.duck_additional = cs_approachf( 0.0f, this->base.duck_additional, this->base.last_update_delta_time * 2.0f );
	}

	this->base.air_lerp = cs_approachf( this->base.on_ground ? 1.0f : 0.0f, this->base.air_lerp, lerpf( this->base.duck_amount, CSGO_ANIM_ONGROUND_FUZZY_APPROACH, CSGO_ANIM_ONGROUND_FUZZY_APPROACH_CROUCH ) * this->base.last_update_delta_time );
	this->base.air_lerp = clampf( this->base.air_lerp, 0.0f, 1.0f );

	this->base.strafe_change_weight *= 1.0f - this->base.duck_amount;
	this->base.strafe_change_weight *= this->base.air_lerp;
	this->base.strafe_change_weight = clampf( this->base.strafe_change_weight, 0.0f, 1.0f );

	if ( this->base.strafe_seq != -1 )
		server_animstate_update_animlayer( this, animlayer_movement_strafechange, this->base.strafe_seq, 0.0f, this->base.strafe_change_weight, this->base.strafe_change_cycle );

	const bool was_on_ladder = this->base.on_ladder;
	this->base.on_ladder = !this->base.on_ground && *player_movetype( this->base.player ) == movetype_ladder;
	const bool just_started_climbing = !was_on_ladder && this->base.on_ladder;
	const bool just_stopped_climbing = was_on_ladder && !this->base.on_ladder;

	if ( this->base.ladder_weight > 0.0f || this->base.on_ladder ) {
		if ( just_started_climbing )
			server_animstate_set_layer_sequence( this, animlayer_movement_land_or_climb, player_select_weighted_seq( this->base.player, act_csgo_climb_ladder ) );

		if ( fabsf( this->base.vel_lenz ) > 100.0f )
			this->base.ladder_speed = cs_approachf( 1.0f, this->base.ladder_speed, this->base.last_update_delta_time * 10.0f );
		else
			this->base.ladder_speed = cs_approachf( 0.0f, this->base.ladder_speed, this->base.last_update_delta_time * 10.0f );

		this->base.ladder_speed = clampf( this->base.ladder_speed, 0.0f, 1.0f );

		if ( this->base.on_ladder )
			this->base.ladder_weight = cs_approachf( 1.0f, this->base.ladder_weight, this->base.last_update_delta_time * 5.0f );
		else
			this->base.ladder_weight = cs_approachf( 0.0f, this->base.ladder_weight, this->base.last_update_delta_time * 10.0f );

		this->base.ladder_weight = clampf( this->base.ladder_weight, 0.0f, 1.0f );

		vec3 ladder_angle = *player_ladder_norm( this->base.player );
		vec3_to_angle( &ladder_angle );

		animstate_pose_param_cache_set_value( &this->base.pose_param_map[ pose_param_ladder_yaw ], this->base.player, cs_angle_diff( ladder_angle.y, this->base.foot_yaw ) );

		float climb_cycle = ( *player_animlayers( this->base.player ) )[ animlayer_movement_land_or_climb ].cycle;
		climb_cycle += ( this->base.pos.z - this->base.last_pos.z ) * lerpf( this->base.ladder_speed, 0.010f, 0.004f );

		animstate_pose_param_cache_set_value( &this->base.pose_param_map[ pose_param_ladder_speed ], this->base.player, this->base.ladder_speed );

		if ( server_animstate_get_layer_activity( this, animlayer_movement_land_or_climb ) == act_csgo_climb_ladder )
			animlayer_set_weight( &( ( *player_animlayers( this->base.player ) )[ animlayer_movement_land_or_climb ] ), this->base.ladder_weight );

		animlayer_set_cycle( &( ( *player_animlayers( this->base.player ) )[ animlayer_movement_land_or_climb ] ), climb_cycle );

		if ( this->base.on_ladder ) {
			float ideal_jump_weight = 1.0f - this->base.ladder_weight;

			if ( ( *player_animlayers( this->base.player ) )[ animlayer_movement_jump_or_fall ].weight > ideal_jump_weight )
				animlayer_set_weight( &( ( *player_animlayers( this->base.player ) )[ animlayer_movement_jump_or_fall ] ), ideal_jump_weight );
		}
	}
	else {
		this->base.ladder_speed = 0.0f;
	}

	if ( this->base.on_ground ) {
		if ( !this->base.landing && ( this->base.just_landed || just_stopped_climbing ) ) {
			server_animstate_set_layer_sequence( this, animlayer_movement_land_or_climb, player_select_weighted_seq( this->base.player, ( this->base.air_time > 1.0f ) ? act_csgo_land_heavy : act_csgo_land_light ) );
			animlayer_set_cycle( &( ( *player_animlayers( this->base.player ) )[ animlayer_movement_land_or_climb ] ), 0.0f );
			this->base.landing = true;
		}

		this->base.air_time = 0.0f;

		if ( this->base.landing && server_animstate_get_layer_activity( this, animlayer_movement_land_or_climb ) != act_csgo_climb_ladder ) {
			this->jumping = false;

			server_animstate_increment_layer_cycle( this, animlayer_movement_land_or_climb, false );
			server_animstate_increment_layer_cycle( this, animlayer_movement_jump_or_fall, false );

			animstate_pose_param_cache_set_value( &this->base.pose_param_map[ pose_param_jump_fall ], this->base.player, 0.0f );

			if ( server_animstate_is_sequence_completed( this, animlayer_movement_land_or_climb ) ) {
				this->base.landing = false;
				animlayer_set_weight( &( ( *player_animlayers( this->base.player ) )[ animlayer_movement_land_or_climb ] ), 0.0f );
				animlayer_set_weight( &( ( *player_animlayers( this->base.player ) )[ animlayer_movement_jump_or_fall ] ), 0.0f );
				this->base.land_anim_factor = 1.0f;
			}
			else {
				float land_weight = server_animstate_get_layer_ideal_weight_from_seq_cycle( this, animlayer_movement_land_or_climb ) * this->base.land_anim_factor;

				land_weight *= clampf( 1.0f - this->base.duck_amount, 0.2f, 1.0f );

				animlayer_set_weight( &( ( *player_animlayers( this->base.player ) )[ animlayer_movement_land_or_climb ] ), land_weight );

				float jump_fall_weight = ( *player_animlayers( this->base.player ) )[ animlayer_movement_jump_or_fall ].weight;
				if ( jump_fall_weight > 0.0f ) {
					jump_fall_weight = cs_approachf( 0.0f, jump_fall_weight, this->base.last_update_delta_time * 10.0f );
					animlayer_set_weight( &( ( *player_animlayers( this->base.player ) )[ animlayer_movement_jump_or_fall ] ), jump_fall_weight );
				}
			}
		}

		if ( !this->base.landing && !this->jumping && this->base.ladder_weight <= 0.0f )
			animlayer_set_weight( &( ( *player_animlayers( this->base.player ) )[ animlayer_movement_land_or_climb ] ), 0.0f );
	}
	else if ( !this->base.on_ladder ) {
		this->base.landing = false;

		if ( this->base.just_jumped || just_stopped_climbing ) {
			if ( !this->jumping )
				server_animstate_set_layer_sequence( this, animlayer_movement_jump_or_fall, player_select_weighted_seq( this->base.player, act_csgo_fall ) );

			this->base.air_time = 0.0f;
		}

		this->base.air_time += this->base.last_update_delta_time;

		server_animstate_increment_layer_cycle( this, animlayer_movement_jump_or_fall, false );

		float jump_weight = ( *player_animlayers( this->base.player ) )[ animlayer_movement_jump_or_fall ].weight;
		float next_jump_weight = server_animstate_get_layer_ideal_weight_from_seq_cycle( this, animlayer_movement_jump_or_fall );

		if ( next_jump_weight > jump_weight )
			animlayer_set_weight( &( ( *player_animlayers( this->base.player ) )[ animlayer_movement_jump_or_fall ] ), next_jump_weight );

		float lingering_land_weight = ( *player_animlayers( this->base.player ) )[ animlayer_movement_land_or_climb ].weight;

		if ( lingering_land_weight > 0.0f ) {
			lingering_land_weight *= cs_smoothstep_bounds( 0.2f, 0.0f, this->base.air_time );
			animlayer_set_weight( &( ( *player_animlayers( this->base.player ) )[ animlayer_movement_land_or_climb ] ), lingering_land_weight );
		}

		animstate_pose_param_cache_set_value( &this->base.pose_param_map[ pose_param_jump_fall ], this->base.player, clampf( cs_smoothstep_bounds( 0.72f, 1.52f, this->base.air_time ), 0.0f, 1.0f ) );
	}

	MDLCACHE_CRITICAL_SECTION_END;
}

void server_animstate_set_up_flashed_reaction( server_animstate* this ) {
	/* part of CCSPlayer::Blind */
	/* we can detect if player was just flashed by increase of flash duration (ghetto) */
	const bool just_flashed = *player_flash_duration( this->base.player ) > this->last_flash_duration;
	this->last_flash_duration = *player_flash_duration( this->base.player );

	if ( just_flashed ) {
		const float hold_time = this->last_flash_duration * 0.7f;
		const float fade_time = ( hold_time * 2.0f ) * 0.7f;

		const float adjusted_hold = hold_time * 0.45f;
		const float adjusted_end = fade_time * 0.7f;

		this->flash_start_time = cs_iglobals->curtime + adjusted_hold;
		this->flash_end_time = cs_iglobals->curtime + adjusted_end;

		if ( adjusted_end < 1.5f )
			this->flash_start_time -= 1.0f;
	}

	/* back to the actual SetUpFlashedReaction function */
	if ( this->flash_end_time < cs_iglobals->curtime ) {
		animlayer_set_weight( &( ( *player_animlayers( this->base.player ) )[ animlayer_flashed ] ), 0.0f );
		this->base.flashed = false;
	}
	else {
		if ( !this->base.flashed ) {
			server_animstate_set_layer_sequence( this, animlayer_flashed, player_select_weighted_seq( this->base.player, act_csgo_flashbang_reaction ) );
			this->base.flashed = true;
		}

		float flash_weight = 0.0f;

		if ( this->flash_start_time == this->flash_end_time ) {
			if ( cs_iglobals->curtime - this->flash_end_time < 0.0f )
				flash_weight = 1.0f;
			else
				flash_weight = 0.0f;
		}
		else {
			const float time_left = ( cs_iglobals->curtime - this->flash_start_time ) / ( this->flash_end_time - this->flash_start_time );
			flash_weight = 1.0f - ( ( time_left >= 0.0f ) ? min( time_left, 1.0f ) : 0.0f );
		}

		animlayer_set_cycle( &( ( *player_animlayers( this->base.player ) )[ animlayer_flashed ] ), 0.0f );
		animlayer_set_playback_rate( &( ( *player_animlayers( this->base.player ) )[ animlayer_flashed ] ), 0.0f );

		float last_weight = ( *player_animlayers( this->base.player ) )[ animlayer_flashed ].weight;
		float weight = ( ( flash_weight * flash_weight ) * 3.0f ) - ( ( ( flash_weight * flash_weight ) * 2.0f ) * flash_weight );

		animlayer_set_weight( &( ( *player_animlayers( this->base.player ) )[ animlayer_flashed ] ), weight );
		server_animstate_set_layer_weight_rate( this, animlayer_flashed, ( weight >= last_weight ) ? 0.0f : last_weight );
	}
}

void server_animstate_set_up_alive_loop( server_animstate* this ) {
	if ( server_animstate_get_layer_activity( this, animlayer_aliveloop ) != act_csgo_alive_loop ) {
		MDLCACHE_CRITICAL_SECTION_START;
		server_animstate_set_layer_sequence( this, animlayer_aliveloop, player_select_weighted_seq( this->base.player, act_csgo_alive_loop ) );
		animlayer_set_cycle( &( ( *player_animlayers( this->base.player ) )[ animlayer_aliveloop ] ), ( float )rand( ) / ( float )RAND_MAX );

		float new_rate = player_get_sequence_cycle_rate_server( this->base.player, ( *player_animlayers( this->base.player ) )[ animlayer_aliveloop ].seq );
		new_rate *= 0.8f + ( ( float )rand( ) / ( float )RAND_MAX ) * 0.3f;
		animlayer_set_playback_rate( &( ( *player_animlayers( this->base.player ) )[ animlayer_aliveloop ] ), new_rate );
		MDLCACHE_CRITICAL_SECTION_END;
	}
	else {
		if ( this->base.weapon && this->base.weapon != this->base.last_weapon ) {
			const float retain_cycle = ( *player_animlayers( this->base.player ) )[ animlayer_aliveloop ].cycle;
			server_animstate_set_layer_sequence( this, animlayer_aliveloop, player_select_weighted_seq( this->base.player, act_csgo_alive_loop ) );
			animlayer_set_cycle( &( ( *player_animlayers( this->base.player ) )[ animlayer_aliveloop ] ), retain_cycle );
		}
		else if ( server_animstate_is_sequence_completed( this, animlayer_aliveloop ) ) {
			MDLCACHE_CRITICAL_SECTION_START;
			float new_rate = player_get_sequence_cycle_rate_server( this->base.player, ( *player_animlayers( this->base.player ) )[ animlayer_aliveloop ].seq );
			new_rate *= 0.8f + ( ( float )rand( ) / ( float )RAND_MAX ) * 0.3f;
			animlayer_set_playback_rate( &( ( *player_animlayers( this->base.player ) )[ animlayer_aliveloop ] ), new_rate );
			MDLCACHE_CRITICAL_SECTION_END;
		}
		else {
			const float weight_out_pose_breaker = cs_remap_val_clamped( this->base.speed_to_run_fraction, 0.55f, 0.9f, 1.0f, 0.0f );
			animlayer_set_weight( &( ( *player_animlayers( this->base.player ) )[ animlayer_aliveloop ] ), weight_out_pose_breaker );
		}
	}
}

/* this stuff deals with server-side events... dont even bother rebuilding them rn */
void server_animstate_set_up_whole_body_action( server_animstate* this ) {
	( ( __attribute__( ( thiscall ) ) void( * )( animstate* ) )cs_offsets.animstate_set_up_whole_body_action_fn )( &this->base );
}

/* this stuff deals with server-side events... dont even bother rebuilding them rn */
void server_animstate_set_up_flinch( server_animstate* this ) {
	( ( __attribute__( ( thiscall ) ) void( * )( animstate* ) )cs_offsets.animstate_set_up_flinch_fn )( &this->base );
}

/* seems to be same on server and client; no problem here! */
void server_animstate_set_up_lean( server_animstate* this ) {
	( ( __attribute__( ( thiscall ) ) void( * )( animstate* ) )cs_offsets.animstate_set_up_lean_fn )( &this->base );
}

void server_animstate_update( server_animstate* this, vec3* ang, bool force ) {
	if ( !this->base.player || !player_is_alive( this->base.player ) || !server_animstate_cache_sequences( this ) )
		return;

#ifdef SES_ANIMSTATE_ADJUSTMENTS

#else
	if ( !force && ( this->base.last_update_time == cs_iglobals->curtime || this->base.last_update_frame == cs_iglobals->framecount ) )
		return;
#endif

	vec3 new_angle = *ang;
	new_angle.x += *player_thirdperson_recoil( this->base.player );
	vec3_clamp_angle( &new_angle );

	this->base.last_update_delta_time = max( 0.0f, cs_iglobals->curtime - this->base.last_update_time );

	cs_allow_invalidate_bone_cache( false );

	this->base.eye_yaw = new_angle.y;
	this->base.eye_pitch = new_angle.x;
	this->base.pos = *player_abs_origin( this );
	this->base.weapon = player_get_weapon( this );

	if ( this->base.weapon != this->base.last_weapon || this->base.first_run ) {
		*player_computed_lod_frame( this->base.player ) = 0;

		const animlayer* layers = *player_animlayers( this->base.player );

		if ( layers ) {
			for ( int i = 0; i < ANIMATION_LAYER_COUNT; i++ ) {
				animlayer* layer = &layers[ i ];

				layer->dispatched_studiohdr = NULL;
				layer->dispatched_src = act_invalid;
				layer->dispatched_dst = act_invalid;
			}
		}
	}

	this->base.duck_amount = clampf( cs_approachf( clampf( *player_duck_amount( this->base.player ) + this->base.duck_additional, 0.0f, 1.0f ), this->base.duck_amount, this->base.last_update_delta_time * 6.0f ), 0.0f, 1.0f );

	MDLCACHE_CRITICAL_SECTION_START;
	player_set_sequence( this->base.player, 0 );
	player_set_playback_rate( this->base.player, 0.0f );
	player_set_cycle( this->base.player, 0.0f );
	MDLCACHE_CRITICAL_SECTION_END;

	server_animstate_set_up_velocity( this );
	server_animstate_set_up_aim_matrix( this );
	server_animstate_set_up_weapon_action( this );
	server_animstate_set_up_movement( this );
	server_animstate_set_up_alive_loop( this );
	server_animstate_set_up_whole_body_action( this );
	server_animstate_set_up_flashed_reaction( this );
	server_animstate_set_up_flinch( this );
	server_animstate_set_up_lean( this );

	const animlayer* layers = *player_animlayers( this->base.player );

	if ( layers ) {
		for ( int i = 0; i < ANIMATION_LAYER_COUNT; i++ ) {
			animlayer* layer = &layers[ i ];

			if ( !layer->seq )
				animlayer_set_weight( layer, 0.0f );
		}
	}

	// force abs angles on client and server to line up hitboxes
	player_set_abs_angles( this->base.player, &( vec3 ){0.0f, this->base.foot_yaw, 0.0f} );

	cs_allow_invalidate_bone_cache( true );

	this->base.last_weapon = this->base.weapon;
	this->base.last_pos = this->base.pos;
	this->base.first_run = false;
	this->base.last_update_time = cs_iglobals->curtime;

#ifdef SES_ANIMSTATE_ADJUSTMENTS
	this->base.last_update_frame = cs_time2ticks( cs_iglobals->curtime );
#else
	this->base.last_update_frame = cs_iglobals->framecount;
#endif
}