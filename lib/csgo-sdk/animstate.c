#include "animstate.h"
#include "sdk.h"

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
	assert ( max_speed_run > 0 );

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
		SetLayerSequence ( animlayer_adjust, SelectSequenceFromActMods ( act_csgo_idle_adjust_stoppedmoving ) );
		this->base.in_adjust = true;
	}

	if ( GetLayerActivity ( animlayer_adjust ) == act_csgo_idle_adjust_stoppedmoving
		|| GetLayerActivity ( animlayer_adjust ) == act_csgo_idle_turn_balanceadjust ) {
		if ( this->base.in_adjust && this->base.speed_to_crouch_fraction <= 0.25f ) {
			IncrementLayerCycleWeightRateGeneric ( 3 );
			this->base.in_adjust = !( IsLayerSequenceCompleted ( cs_approachf ) );
		}
		else {
			this->base.in_adjust = false;
			const float weight = GetLayerWeight ( animlayer_adjust );
			SetLayerWeight ( animlayer_adjust, cs_approachf ( 0, weight, this->base.last_update_delta_time * 5.0f ) );
			SetLayerWeightRate ( animlayer_adjust, weight );
		}
	}

	// if the player is looking far enough to either side, turn the feet to keep them within the extent of the aim matrix
	this->base.last_foot_yaw = this->base.foot_yaw;
	this->base.foot_yaw = clampf ( this->base.foot_yaw, -360, 360 );
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
			this->base.foot_yaw = ApproachAngle ( this->base.eye_yaw, this->base.foot_yaw, this->base.last_update_delta_time * ( 30.0f + 20.0f * this->base.walk_to_run_fraction ) );
			
			this->base.time_to_align_lby = cs_iglobals->curtime + ( CSGO_ANIM_LOWER_REALIGN_DELAY * 0.2f );
			*player_lby ( this->base.player ) = this->base.eye_yaw;
		}
		else
		{
			this->base.foot_yaw = cs_approachf ( *player_lby ( this->base.player ), this->base.foot_yaw, this->base.last_update_delta_time * CSGO_ANIM_LOWER_CATCHUP_IDLE );

			if ( cs_iglobals->curtime > this->base.time_to_align_lby && abs ( cs_angle_diff ( this->base.foot_yaw, this->base.eye_yaw ) ) > 35.0f ) {
				this->base.time_to_align_lby = cs_iglobals->curtime + CSGO_ANIM_LOWER_REALIGN_DELAY;
				*player_lby ( this->base.player ) = this->base.eye_yaw;
			}
		}
	}

	if ( this->base.vel_len2d <= CS_PLAYER_SPEED_STOPPED && this->base.on_ground && !this->base.on_ladder && !this->base.landing && this->base.last_update_delta_time > 0.0f && abs ( AngleDiff ( this->base.last_foot_yaw, m_flFootYaw ) / m_flLastUpdateIncrement > CSGO_ANIM_READJUST_THRESHOLD ) ) {
		SetLayerSequence ( animation_layer_adjust, SelectSequenceFromActMods ( act_csgo_idle_turn_balanceadjust ) );
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
		int nMoveSeq = GetLayerSequence ( animation_layer_movement_move );

		if ( nMoveSeq != -1 ) {
			mstudioseqdesc_t& seqdesc = m_pPlayer->GetModelPtr ( )->pSeqdesc ( nMoveSeq );

			if ( seqdesc.numanimtags > 0 ) {
				if ( fabsf ( cs_angle_diff ( this->base.move_yaw, 180.0f ) ) <= EIGHT_WAY_WIDTH )
					this->base.primary_cycle = m_pPlayer->GetFirstSequenceAnimTag ( nMoveSeq, ANIMTAG_STARTCYCLE_N, 0, 1 );
				else if ( fabsf ( cs_angle_diff ( this->base.move_yaw, 135.0f ) ) <= EIGHT_WAY_WIDTH )
					this->base.primary_cycle = m_pPlayer->GetFirstSequenceAnimTag ( nMoveSeq, ANIMTAG_STARTCYCLE_NE, 0, 1 );
				else if ( fabsf ( cs_angle_diff ( this->base.move_yaw, 90.0f ) ) <= EIGHT_WAY_WIDTH )
					this->base.primary_cycle = m_pPlayer->GetFirstSequenceAnimTag ( nMoveSeq, ANIMTAG_STARTCYCLE_E, 0, 1 );
				else if ( fabsf ( cs_angle_diff ( this->base.move_yaw, 45.0f ) ) <= EIGHT_WAY_WIDTH ) //SE
					this->base.primary_cycle = m_pPlayer->GetFirstSequenceAnimTag ( nMoveSeq, ANIMTAG_STARTCYCLE_SE, 0, 1 );
				else if ( fabsf ( cs_angle_diff ( this->base.move_yaw, 0.0f ) ) <= EIGHT_WAY_WIDTH ) //S
					this->base.primary_cycle = m_pPlayer->GetFirstSequenceAnimTag ( nMoveSeq, ANIMTAG_STARTCYCLE_S, 0, 1 );
				else if ( fabsf ( cs_angle_diff ( this->base.move_yaw, -45.0f ) ) <= EIGHT_WAY_WIDTH ) //SW
					this->base.primary_cycle = m_pPlayer->GetFirstSequenceAnimTag ( nMoveSeq, ANIMTAG_STARTCYCLE_SW, 0, 1 );
				else if ( fabsf ( cs_angle_diff ( this->base.move_yaw, -90.0f ) ) <= EIGHT_WAY_WIDTH ) //W
					this->base.primary_cycle = m_pPlayer->GetFirstSequenceAnimTag ( nMoveSeq, ANIMTAG_STARTCYCLE_W, 0, 1 );
				else if ( fabsf ( cs_angle_diff ( this->base.move_yaw, -135.0f ) ) <= EIGHT_WAY_WIDTH ) //NW
					this->base.primary_cycle = m_pPlayer->GetFirstSequenceAnimTag ( nMoveSeq, ANIMTAG_STARTCYCLE_NW, 0, 1 );
			}
		}
	}
	else {
		if ( GetLayerWeight ( animation_layer_movement_strafechange ) >= 1 ) {
			this->base.move_yaw = this->base.ideal_move_yaw;;
		}
		else {
			const float weight_perms = lerpf ( this->base.duck_amount, clampf ( this->base.speed_to_walk_fraction, 0.0f, 1.0f ), clampf ( this->base.speed_to_crouch_fraction, 0.0f, 1.0f ) );
			const float fraction_waiting = cs_bias ( weight_perms, 0.18f ) + 0.1f;

			this->base.move_yaw = cs_norm_rotation ( this->base.move_yaw + ( this->base.move_yaw_to_ideal * fraction_waiting ) );
		}
	}

	m_tPoseParamMappings [ PLAYER_POSE_PARAM_MOVE_YAW ].SetValue ( m_pPlayer, this->base.move_yaw );

	float flAimYaw = cs_angle_diff ( this->base.eye_yaw, this->base.foot_yaw );

	if ( flAimYaw >= 0 && this->base.aim_yaw_max )
		flAimYaw = ( flAimYaw / this->base.aim_yaw_max ) * 60.0f;
	else if ( this->base.aim_yaw_min )
		flAimYaw = ( flAimYaw / this->base.aim_yaw_min ) * -60.0f;

	m_tPoseParamMappings [ PLAYER_POSE_PARAM_BODY_YAW ].SetValue ( m_pPlayer, flAimYaw );

	// we need non-symmetrical arbitrary min/max bounds for vertical aim (pitch) too
	float pitch = cs_angle_diff ( this->base.eye_pitch, 0.0f );

	if ( pitch > 0.0f )
		pitch = ( pitch / this->base.aim_pitch_max ) * CSGO_ANIM_AIMMATRIX_DEFAULT_PITCH_MAX;
	else
		pitch = ( pitch / this->base.aim_pitch_min ) * CSGO_ANIM_AIMMATRIX_DEFAULT_PITCH_MIN;

	m_tPoseParamMappings [ PLAYER_POSE_PARAM_BODY_PITCH ].SetValue ( m_pPlayer, pitch );
	m_tPoseParamMappings [ PLAYER_POSE_PARAM_SPEED ].SetValue ( m_pPlayer, this->base.speed_to_walk_fraction );
	m_tPoseParamMappings [ PLAYER_POSE_PARAM_STAND ].SetValue ( m_pPlayer, 1.0f - ( this->base.duck_amount * this->base.air_lerp ) );

	MDLCACHE_CRITICAL_SECTION_END;
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
				layer->dispatched_src = ACT_INVALID;
				layer->dispatched_dst = ACT_INVALID;
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
	SetUpVelocity ( );			// calculate speed and set up body yaw values
	SetUpAimMatrix ( );			// aim matrices are full body, so they not only point the weapon down the eye dir, they can crouch the idle player
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