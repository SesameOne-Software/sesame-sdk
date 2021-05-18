#include "animstate.h"
#include "sdk.h"

void server_animstate_update_layer(server_animstate* this, int idx, int seq, float playback_rate, float weight, float cycle) {
	assert(weight >= 0.0f && weight <= 1.0f);
	assert(cycle >= 0.0f && cycle <= 1.0f);
	assert(seq > 1);

	if (seq > 1) {
		MDLCACHE_CRITICAL_SECTION_START;

		animlayer* layer = &((*player_animlayers(this->base.player))[idx]);

		if (layer->owner && layer->seq != seq)
			player_invalidate_physics_recursive(this->base.player, 16);

		layer->seq = seq;
		set_rate(anim_state, idx, playback_rate);
		set_cycle(anim_state, idx, std::clamp(cycle, 0.0f, 1.0f));
		set_weight(anim_state, idx, std::clamp(weight, 0.0f, 1.0f));

		void* update_layer_order_preset_fn = (void*)cs_offsets.animstate_update_layer_order_preset_fn;

		__asm {
			mov ecx, anim_state
			movss xmm0, weight
			push seq
			push idx
			call update_layer_order_preset_fn
		}

		MDLCACHE_CRITICAL_SECTION_END;
	}
}

void server_animstate_update ( server_animstate* this, vec3* ang ) {
	if ( !this->base.player || !player_is_alive ( this->base.player ) || !server_animstate_cache_sequences( this ) )
		return;

	vec3 new_angle = *ang;
	new_angle.x += *player_thirdperson_recoil( this->base.player );
	vec3_clamp_angle ( &new_angle );

	this->base.last_update_delta_time = max ( 0.0f, cs_iglobals->curtime - this->base.last_update_time );

#ifdef CLIENT_DLL
	// suspend bonecache invalidation
	C_BaseAnimating::EnableInvalidateBoneCache ( false );
#endif

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
	m_pPlayer->SetSequence ( 0 );
	m_pPlayer->SetPlaybackRate ( 0 );
	m_pPlayer->SetCycle ( 0 );
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
			
			if ( !layer->sequence )
				pLayer->SetWeight ( 0 );
		}
	}

	// force abs angles on client and server to line up hitboxes
	player_set_abs_angles ( this->base.player, &( vec3 ){0.0f, this->base.foot_yaw, 0.0f} );

#ifdef CLIENT_DLL
	// restore bonecache invalidation
	C_BaseAnimating::EnableInvalidateBoneCache ( true );
#endif

	m_pWeaponLast = m_pWeapon;
	m_vecPositionLast = m_vecPositionCurrent;
	m_bFirstRunSinceInit = false;
	m_flLastUpdateTime = gpGlobals->curtime;
	m_nLastUpdateFrame = gpGlobals->framecount;
}