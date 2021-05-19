#include "animstate.h"
#include "sdk.h"

/*
int server_animstate_get_weighted_sequence_from_activity(server_animstate* this, anim_activity act) {
	MDLCACHE_CRITICAL_SECTION_START;
	const int new_seq = pBaseEntity->SelectWeightedSequenceFromModifiers(activity, m_ActivityModifiers.Base(), m_ActivityModifiers.Count());
	MDLCACHE_CRITICAL_SECTION_END;
	return new_seq;
}
*/

float server_animstate_get_layer_ideal_weight_from_seq_cycle(server_animstate* this, animlayer_idx idx) {
	animstate* client_animstate = &this->base;
	const void* get_layer_ideal_weight_from_seq_cycle_fn = (void*)cs_offsets.animstate_get_layer_ideal_weight_from_seq_cycle_fn;
	float ret = 1.0f;

	__asm {
		push layer
		mov ecx, client_animstate
		call get_layer_ideal_weight_from_seq_cycle_fn
		movss ret, xmm0
	}

	return ret;
}

void server_animstate_set_layer_sequence ( server_animstate* this, animlayer_idx idx, int seq ) {
	assert ( seq > 1 );

	if ( seq > 1 ) {
		MDLCACHE_CRITICAL_SECTION_START;

		animlayer* layer = &( ( *player_animlayers ( this->base.player ) ) [ idx ] );

		animlayer_set_seq ( layer, seq );
		animlayer_set_playback_rate ( layer, player_get_layer_sequence_cycle_rate ( this->base.player, idx, seq ) );
		animlayer_set_cycle ( layer, 0.0f );
		animlayer_set_weight ( layer, 0.0f );
		
		server_animstate_update_layer_order_preset ( this, 0.0f, idx, seq );

		MDLCACHE_CRITICAL_SECTION_END;
	}
}

void server_animstate_update_animlayer(server_animstate* this, animlayer_idx idx, int seq, float playback_rate, float weight, float cycle) {
	assert(weight >= 0.0f && weight <= 1.0f);
	assert(cycle >= 0.0f && cycle <= 1.0f);
	assert(seq > 1);

	if (seq > 1) {
		MDLCACHE_CRITICAL_SECTION_START;

		animlayer* layer = &( ( *player_animlayers ( this->base.player ) ) [ idx ] );
		
		animlayer_set_seq ( layer, seq );
		animlayer_set_playback_rate( layer, playback_rate);
		animlayer_set_cycle( layer, clampf(cycle, 0.0f, 1.0f));
		animlayer_set_weight( layer, clampf (weight, 0.0f, 1.0f));

		server_animstate_update_layer_order_preset ( this, weight, idx, seq );

		MDLCACHE_CRITICAL_SECTION_END;
	}
}

void server_animstate_set_up_velocity ( server_animstate* this ) {
	MDLCACHE_CRITICAL_SECTION_START;

	vec3 abs_vel = *entity_abs_vel((entity*) this->base.player );

	this->base.vel_lenz = abs_vel.z;

	abs_vel.z = 0.0f;

	this->base.in_accel = vec3_len_sqr( &this->base.last_vel) < vec3_len_sqr ( &abs_vel );
	
	vec3_approach_vec ( &abs_vel, &this->base.vel, this->base.last_update_delta_time * 2000.0f, &this->base.vel );
	this->base.vel_norm = this->base.vel;
	vec3_norm ( &this->base.vel_norm );

	const float vel_len = vec3_len ( &this->base.vel );
	this->base.vel_len2d = min ( vel_len, CS_PLAYER_SPEED_RUN );

	if ( this->base.vel_len2d > 0.0f )
		this->base.vel_norm_nonzero = this->base.vel_norm;

	//compute speed in various normalized forms
	const float max_weapon_speed = weapon_max_speed ( this->base.weapon );
	const float max_speed_run = this->base.weapon ? max ( max_weapon_speed, 0.001f ) : CS_PLAYER_SPEED_RUN;
	assert ( max_speed_run > 0.0f );

	this->base.speed_to_run_fraction = clampf ( this->base.vel_len2d / max_speed_run, 0.0f, 1.0f );
	this->base.speed_to_walk_fraction = this->base.vel_len2d / ( max_speed_run * CS_PLAYER_SPEED_WALK_MODIFIER );
	this->base.speed_to_crouch_fraction = this->base.vel_len2d / ( max_speed_run * CS_PLAYER_SPEED_DUCK_MODIFIER );
	
	if ( this->base.speed_to_walk_fraction >= 1.0f )
		this->base.static_lerp_speed = this->base.vel_len2d;
	else if ( this->base.speed_to_walk_fraction < 0.5f )
		this->base.static_lerp_speed = cs_approachf ( 80.0f, this->base.static_lerp_speed, this->base.last_update_delta_time * 60.0f );

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
		server_animstate_set_layer_sequence( this, animlayer_adjust, 5 );
		this->base.in_adjust = true;
	}

	if (server_animstate_get_layer_activity( this, animlayer_adjust ) == act_csgo_idle_adjust_stoppedmoving
		|| server_animstate_get_layer_activity(this, animlayer_adjust ) == act_csgo_idle_turn_balanceadjust ) {
		if ( this->base.in_adjust && this->base.speed_to_crouch_fraction <= 0.25f ) {
			server_animstate_increment_layer_cycle_weight_rate_generic( this, animlayer_adjust);
			this->base.in_adjust = !server_animstate_is_sequence_completed(this, animlayer_adjust);
		}
		else {
			this->base.in_adjust = false;
			const float weight = (*player_animlayers(this->base.player))[animlayer_adjust].weight;
			animlayer_set_weight ( animlayer_adjust, cs_approachf ( 0.0f, weight, this->base.last_update_delta_time * 5.0f ) );
			server_animstate_set_layer_weight_rate( this, animlayer_adjust, weight );
		}
	}

	// if the player is looking far enough to either side, turn the feet to keep them within the extent of the aim matrix
	this->base.last_foot_yaw = this->base.foot_yaw;
	this->base.foot_yaw = clampf ( this->base.foot_yaw, -360.0f, 360.0f );
	const float eye_foot_delta = cs_angle_diff ( this->base.eye_yaw, this->base.foot_yaw );

	// narrow the available aim matrix width as speed increases
	float aim_matrix_width = lerpf ( clampf ( this->base.speed_to_walk_fraction, 0.0f, 1.0f ), 1.0f, lerpf ( this->base.walk_to_run_fraction, CSGO_ANIM_AIM_NARROW_WALK, CSGO_ANIM_AIM_NARROW_RUN ) );

	if ( this->base.duck_amount > 0.0f )
		aim_matrix_width = lerpf ( this->base.duck_amount * clampf ( this->base.speed_to_crouch_fraction, 0.0f, 1.0f ), aim_matrix_width, CSGO_ANIM_AIM_NARROW_CROUCHMOVING );

	const float temp_yaw_max = this->base.aim_yaw_max * aim_matrix_width;
	const float temp_yaw_min = this->base.aim_yaw_min * aim_matrix_width;

	if ( eye_foot_delta > temp_yaw_max )
		this->base.foot_yaw = this->base.eye_yaw - fabsf ( temp_yaw_max );
	else if ( eye_foot_delta < temp_yaw_min )
		this->base.foot_yaw = this->base.eye_yaw + fabsf ( temp_yaw_min );

	this->base.foot_yaw = cs_normalize ( this->base.foot_yaw );

	// pull the lower body direction towards the eye direction, but only when the player is moving
	if ( this->base.on_ground ) {
		if ( this->base.vel_len2d > 0.1f ) {
			this->base.foot_yaw = cs_approachf( this->base.eye_yaw, this->base.foot_yaw, this->base.last_update_delta_time * ( 30.0f + 20.0f * this->base.walk_to_run_fraction ) );
			
			this->lby_realign_timer = cs_iglobals->curtime + CSGO_ANIM_LOWER_REALIGN_DELAY * 0.2f;
			*player_lby ( this->base.player ) = this->base.eye_yaw;
		}
		else {
			this->base.foot_yaw = cs_approachf ( *player_lby ( this->base.player ), this->base.foot_yaw, this->base.last_update_delta_time * CSGO_ANIM_LOWER_CATCHUP_IDLE );

			if ( cs_iglobals->curtime > this->lby_realign_timer && abs ( cs_angle_diff ( this->base.foot_yaw, this->base.eye_yaw ) ) > 35.0f ) {
				this->lby_realign_timer = cs_iglobals->curtime + CSGO_ANIM_LOWER_REALIGN_DELAY;
				*player_lby ( this->base.player ) = this->base.eye_yaw;
			}
		}
	}

	if ( this->base.vel_len2d <= CS_PLAYER_SPEED_STOPPED && this->base.on_ground && !this->base.on_ladder && !this->base.landing && this->base.last_update_delta_time > 0.0f && abs ( AngleDiff ( this->base.last_foot_yaw, this->base.foot_yaw ) / this->base.last_update_delta_time > CSGO_ANIM_READJUST_THRESHOLD ) ) {
		server_animstate_set_layer_sequence (this, animlayer_adjust, 4 );
		this->base.in_adjust = true;
	}

	// the final model render yaw is aligned to the foot yaw

	if ( this->base.vel_len2d > 0.0f && this->base.on_ground ) {
		// convert horizontal velocity vec to angular yaw
		float max_yaw_ideal = atan2f ( -this->base.vel.y, -this->base.vel.x ) * 180.0f / M_PI;
		
		if ( max_yaw_ideal < 0.0f )
			max_yaw_ideal += 360.0f;

		this->base.ideal_move_yaw = cs_norm_rotation ( AngleDiff ( max_yaw_ideal, this->base.foot_yaw ) );
	}

	// delta between current yaw and ideal velocity derived target (possibly negative!)
	this->base.move_yaw_to_ideal = cs_norm_rotation ( AngleDiff ( this->base.ideal_move_yaw, this->base.move_yaw ) );
	

	if ( started_moving && this->base.move_weight <= 0.0f ) {
		this->base.move_yaw = this->base.ideal_move_yaw;

		// select a special starting cycle that's set by the animator in content
		const int move_seq = (*player_animlayers(this->base.player))[animlayer_movement_move].seq;

		if (move_seq != -1 ) {
			const void* seq_desc = player_seq_desc(this->base.player, move_seq);

			if (*(int*)((uintptr_t)seq_desc + 0xC4) > 0 ) {
				if ( fabsf ( cs_angle_diff ( this->base.move_yaw, 180.0f ) ) <= EIGHT_WAY_WIDTH )
					this->base.primary_cycle = player_get_first_sequence_anim_tag(this->base.player, move_seq, 1, 0.0f, 1.0f );
				else if ( fabsf ( cs_angle_diff ( this->base.move_yaw, 135.0f ) ) <= EIGHT_WAY_WIDTH )
					this->base.primary_cycle = player_get_first_sequence_anim_tag(this->base.player, move_seq, 2, 0.0f, 1.0f);
				else if ( fabsf ( cs_angle_diff ( this->base.move_yaw, 90.0f ) ) <= EIGHT_WAY_WIDTH )
					this->base.primary_cycle = player_get_first_sequence_anim_tag(this->base.player, move_seq, 3, 0.0f, 1.0f);
				else if ( fabsf ( cs_angle_diff ( this->base.move_yaw, 45.0f ) ) <= EIGHT_WAY_WIDTH )
					this->base.primary_cycle = player_get_first_sequence_anim_tag(this->base.player, move_seq, 4, 0.0f, 1.0f);
				else if ( fabsf ( cs_angle_diff ( this->base.move_yaw, 0.0f ) ) <= EIGHT_WAY_WIDTH )
					this->base.primary_cycle = player_get_first_sequence_anim_tag(this->base.player, move_seq, 5, 0.0f, 1.0f);
				else if ( fabsf ( cs_angle_diff ( this->base.move_yaw, -45.0f ) ) <= EIGHT_WAY_WIDTH )
					this->base.primary_cycle = player_get_first_sequence_anim_tag(this->base.player, move_seq, 6, 0.0f, 1.0f);
				else if ( fabsf ( cs_angle_diff ( this->base.move_yaw, -90.0f ) ) <= EIGHT_WAY_WIDTH )
					this->base.primary_cycle = player_get_first_sequence_anim_tag(this->base.player, move_seq, 7, 0.0f, 1.0f);
				else if ( fabsf ( cs_angle_diff ( this->base.move_yaw, -135.0f ) ) <= EIGHT_WAY_WIDTH )
					this->base.primary_cycle = player_get_first_sequence_anim_tag(this->base.player, move_seq, 8, 0.0f, 1.0f);
			}
		}
	}
	else {
		if ((*player_animlayers(this->base.player))[animlayer_movement_strafechange].weight >= 1.0f ) {
			this->base.move_yaw = this->base.ideal_move_yaw;;
		}
		else {
			const float weight_perms = lerpf ( this->base.duck_amount, clampf ( this->base.speed_to_walk_fraction, 0.0f, 1.0f ), clampf ( this->base.speed_to_crouch_fraction, 0.0f, 1.0f ) );
			const float fraction_waiting = cs_bias ( weight_perms, 0.18f ) + 0.1f;

			this->base.move_yaw = cs_norm_rotation ( this->base.move_yaw + ( this->base.move_yaw_to_ideal * fraction_waiting ) );
		}
	}

	animstate_pose_param_cache_set_value ( &this->base.pose_param_map[pose_param_move_yaw], this->base.player, this->base.move_yaw );

	float aim_yaw = cs_angle_diff ( this->base.eye_yaw, this->base.foot_yaw );

	if (aim_yaw >= 0.0f && this->base.aim_yaw_max )
		aim_yaw = (aim_yaw / this->base.aim_yaw_max ) * 60.0f;
	else if ( this->base.aim_yaw_min )
		aim_yaw = (aim_yaw / this->base.aim_yaw_min ) * -60.0f;

	animstate_pose_param_cache_set_value(&this->base.pose_param_map[pose_param_body_yaw], this->base.player, aim_yaw);

	// we need non-symmetrical arbitrary min/max bounds for vertical aim (pitch) too
	float pitch = cs_angle_diff ( this->base.eye_pitch, 0.0f );

	if ( pitch > 0.0f )
		pitch = ( pitch / this->base.aim_pitch_max ) * CSGO_ANIM_AIMMATRIX_DEFAULT_PITCH_MAX;
	else
		pitch = ( pitch / this->base.aim_pitch_min ) * CSGO_ANIM_AIMMATRIX_DEFAULT_PITCH_MIN;

	animstate_pose_param_cache_set_value(&this->base.pose_param_map[pose_param_body_pitch], this->base.player, pitch );
	animstate_pose_param_cache_set_value(&this->base.pose_param_map[pose_param_speed], this->base.player, this->base.speed_to_walk_fraction );
	animstate_pose_param_cache_set_value(&this->base.pose_param_map[pose_param_stand], this->base.player, 1.0f - ( this->base.duck_amount * this->base.air_lerp ) );

	MDLCACHE_CRITICAL_SECTION_END;
}

/* seems to be same on server and client; no problem here! */
void server_animstate_set_up_aim_matrix(server_animstate* this) {
	void(__fastcall * animstate_set_up_aim_matrix)(animstate*, void*) = (void*)cs_offsets.animstate_setup_aim_matrix_fn;
	animstate_set_up_aim_matrix(&this->base, NULL);
}

void server_animstate_set_up_weapon_action(server_animstate* this) {
	bool do_increment = true;

	if (this->base.weapon && this->deploy_rate_limiting && server_animstate_get_layer_activity(this, animlayer_weapon_action) == act_csgo_deploy) {
		m_pWeapon->ShowWeaponWorldModel(false);

		if ((*player_animlayers(this->base.player))[animlayer_weapon_action].cycle >= CSGO_ANIM_DEPLOY_RATELIMIT) {
			this->deploy_rate_limiting = false;
			server_animstate_set_layer_sequence(this, animlayer_weapon_action, SelectSequenceFromActMods(ACT_CSGO_DEPLOY));
			animlayer_set_weight(&((*player_animlayers(this->base.player))[animlayer_weapon_action]), 0.0f );
			do_increment = false;
		}
	}

	float target_recrouch_weight = 0.0f;

	if ((*player_animlayers(this->base.player))[animlayer_weapon_action].weight > 0.0f ) {
		if ((*player_animlayers(this->base.player))[animlayer_weapon_action_recrouch].seq <= 0)
			server_animstate_set_layer_sequence(this, animlayer_weapon_action_recrouch, player_lookup_sequence(this->base.player, "recrouch_generic"));

		if (LayerSequenceHasActMod(nLayer, "crouch")) {
			if (this->base.duck_amount < 1.0f)
				target_recrouch_weight = (*player_animlayers(this->base.player))[animlayer_weapon_action].weight * (1.0f - this->base.duck_amount);
		}
		else if (this->base.duck_amount > 0.0f)
			target_recrouch_weight = (*player_animlayers(this->base.player))[animlayer_weapon_action].weight * this->base.duck_amount;
	}
	else {
		if ((*player_animlayers(this->base.player))[animlayer_weapon_action_recrouch].weight > 0.0f )
			target_recrouch_weight = cs_approachf(0.0f, (*player_animlayers(this->base.player))[animlayer_weapon_action_recrouch].weight, m_flLastUpdateIncrement * 4);
	}

	animlayer_set_weight(&((*player_animlayers(this->base.player))[animlayer_weapon_action_recrouch]), target_recrouch_weight);

	if (do_increment) {
		// increment the action
		server_animstate_increment_layer_cycle(this, animlayer_weapon_action, false);

		const float previous_weight = (*player_animlayers(this->base.player))[animlayer_weapon_action].weight;
		const float desired_weight = server_animstate_get_layer_ideal_weight_from_seq_cycle(this, animlayer_weapon_action);
		
		animlayer_set_weight(&((*player_animlayers(this->base.player))[animlayer_weapon_action]), previous_weight);
		server_animstate_set_layer_weight_rate(this, animlayer_weapon_action, previous_weight);
	}

}

void server_animstate_update ( server_animstate* this, vec3* ang, bool force ) {
	if ( !this->base.player || !player_is_alive ( this->base.player ) || !server_animstate_cache_sequences( this ) )
		return;

#ifdef SES_ANIMSTATE_ADJUSTMENTS
	
#else
	if ( !force && ( this->base.last_update_time == cs_iglobals->curtime || this->base.last_update_frame == cs_iglobals->framecount ) )
		return;
#endif

	vec3 new_angle = *ang;
	new_angle.x += *player_thirdperson_recoil( this->base.player );
	vec3_clamp_angle ( &new_angle );

	this->base.last_update_delta_time = max ( 0.0f, cs_iglobals->curtime - this->base.last_update_time );

	cs_allow_invalidate_bone_cache ( false );

	this->base.eye_yaw = new_angle.y;
	this->base.eye_pitch = new_angle.x;
	this->base.pos = *player_abs_origin ( this );
	this->base.weapon = player_get_weapon ( this );

	if ( this->base.weapon != this->base.last_weapon || this->base.first_run ) {
		*player_computed_lod_frame ( this->base.player ) = 0;

		const animlayer* layers = *player_animlayers ( this->base.player );

		if ( layers ) {
			for ( int i = 0; i < ANIMATION_LAYER_COUNT; i++ ) {
				animlayer* layer = &layers [ i ];

				layer->dispatched_studiohdr = NULL;
				layer->dispatched_src = act_invalid;
				layer->dispatched_dst = act_invalid;
			}
		}
	}
	
	this->base.duck_amount = clampf ( cs_approachf ( clampf ( *player_duck_amount( this->base.player ) + this->base.duck_additional, 0.0f, 1.0f ), this->base.duck_amount, this->base.last_update_delta_time * 6.0f ), 0.0f, 1.0f );

	MDLCACHE_CRITICAL_SECTION_START;
	player_set_sequence ( this->base.player, 0 );
	player_set_playback_rate ( this->base.player, 0.0f );
	player_set_cycle ( this->base.player, 0.0f );
	MDLCACHE_CRITICAL_SECTION_END;

	// all the layers get set up here
	server_animstate_set_up_velocity(this );			// calculate speed and set up body yaw values
	server_animstate_set_up_velocity( this );			// aim matrices are full body, so they not only point the weapon down the eye dir, they can crouch the idle player
	SetUpWeaponAction ( );		// firing, reloading, silencer-swapping, deploying
	SetUpMovement ( );			// jumping, climbing, ground locomotion, post-weapon crouch-stand
	SetUpAliveloop ( );			// breathe and fidget
	SetUpWholeBodyAction ( );		// defusing, planting, whole-body custom events
	SetUpFlashedReaction ( );		// shield eyes from flashbang
	SetUpFlinch ( );				// flinch when shot
	SetUpLean ( );				// lean into acceleration

	const animlayer* layers = *player_animlayers ( this->base.player );

	if ( layers ) {
		for ( int i = 0; i < ANIMATION_LAYER_COUNT; i++ ) {
			animlayer* layer = &layers [ i ];
			
			if ( !layer->seq )
				animlayer_set_weight ( layer, 0.0f );
		}
	}

	// force abs angles on client and server to line up hitboxes
	player_set_abs_angles ( this->base.player, &( vec3 ){0.0f, this->base.foot_yaw, 0.0f} );

	cs_allow_invalidate_bone_cache ( true );
	
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