#include "features/movement.h"
#include "gui/config.h"

typedef enum {
	strafe_dir_front,
	strafe_dir_left,
	strafe_dir_right,
	strafe_dir_back
} strafe_dir;

/* credits: @cbrs#9210 */
static void features_movement_handle_dir( bool dir, bool odir, strafe_dir qd, strafe_dir* dirs ) {
	const strafe_dir old_qd_v = dirs[ qd ];

	if ( !dir ) {
		dirs[ qd ] = -1;

		if ( odir ) {
			for ( strafe_dir i = strafe_dir_front; i <= strafe_dir_back; i++ ) {
				if ( dirs[ i ] >= old_qd_v )
					dirs[ i ] = max( dirs[ i ] - 1, -1 );
			}
		}
	}

	if ( dir && !odir )
		dirs[ qd ] = strafe_dir_back;
}

void features_movement_run( usercmd* cmd ) {
	if ( !iengine_is_in_game( cs_iengine ) || !iengine_is_connected( cs_iengine ) )
		return;

	player* local = cs_get_local( );

	if ( !local || !player_is_alive( local ) || *player_movetype( local ) == movetype_noclip || *player_movetype( local ) == movetype_fly )
		return;

	const float speed_2d = vec3_len2d( &ses_ctx.unpredicted_vel );

	/* in air stuff */
	if ( !( *player_flags( local ) & fl_on_ground ) ) {
		if ( *ses_cfg_get_item( &ses_cfg, misc, movement, auto_jump, bool ) && cmd->buttons & in_jump )
			cmd->buttons &= ~in_jump;

		switch ( *ses_cfg_get_item( &ses_cfg, misc, movement, auto_strafer, int ) ) {
		case 0: /* no autostrafer */ {
		} break;
		case 1: /* legit autostrafer */ {
			if ( abs( cmd->mouse_dx ) > 2 )
				ses_ctx.input_side_move = copysignf( 450.0f, ( float )cmd->mouse_dx );
		} break;
		case 2: /* directional autostrafer */ {
			strafe_dir dirs[ 4 ] = { 0 };
			static bool of = false, ort = false, ob = false, ol = false;

			features_movement_handle_dir( cmd->buttons & in_forward, of, strafe_dir_front, dirs );
			features_movement_handle_dir( cmd->buttons & in_moveleft, ol, strafe_dir_left, dirs );
			features_movement_handle_dir( cmd->buttons & in_moveright, ort, strafe_dir_right, dirs );
			features_movement_handle_dir( cmd->buttons & in_back, ob, strafe_dir_back, dirs );

			strafe_dir best = strafe_dir_front;
			strafe_dir chosen = strafe_dir_front, val = -1;

			for ( strafe_dir i = strafe_dir_front; i <= strafe_dir_back; i++ ) {
				if ( dirs[ i ] > val ) {
					val = dirs[ i ];
					chosen = i;
				}
			}

			best |= 1 << chosen;

			val = -1;
			chosen = -1;

			for ( strafe_dir i = strafe_dir_front; i <= strafe_dir_back; i++ ) {
				if ( dirs[ i ] > val && !( best & ( 1 << i ) ) ) {
					val = dirs[ i ];
					chosen = i;
				}
			}

			if ( chosen >= 0 )
				best |= 1 << chosen;

			const bool il = best & ( 1 << strafe_dir_left );
			const bool ir = best & ( 1 << strafe_dir_right );
			const bool ib = best & ( 1 << strafe_dir_back );
			const bool ifr = best & ( 1 << strafe_dir_front );

			if ( ifr && il ) ses_ctx.input_angles.y += 45.0f;
			else if ( ib && il ) ses_ctx.input_angles.y += 135.0f;
			else if ( ifr && ir ) ses_ctx.input_angles.y += -45.0f;
			else if ( ib && ir ) ses_ctx.input_angles.y += -135.0f;
			else if ( ib ) ses_ctx.input_angles.y += 180.0f;
			else if ( ir ) ses_ctx.input_angles.y += -90.0f;
			else if ( il ) ses_ctx.input_angles.y += 90.0f;

			ses_ctx.input_angles.y = cs_norm_rotation( ses_ctx.input_angles.y );

			of = cmd->buttons & in_forward;
			ob = cmd->buttons & in_back;
			ol = cmd->buttons & in_moveleft;
			ort = cmd->buttons & in_moveright;

			const float vel_dir = cs_rad2deg( atan2f( ses_ctx.unpredicted_vel.y, ses_ctx.unpredicted_vel.x ) );
			const float target_vel_diff = cs_norm_rotation( ses_ctx.input_angles.y - vel_dir );
			const float max_angle_change = clampf( cs_rad2deg( atan2f( 15.0f, speed_2d ) ), 0.0f, 45.0f );

			ses_ctx.input_angles.y = cs_norm_rotation( vel_dir - copysignf( max_angle_change, -target_vel_diff ) );
			ses_ctx.input_side_move = copysignf( 450.0f, -target_vel_diff );
			ses_ctx.input_forward_move = fabsf( target_vel_diff ) >= max_angle_change ? 0.0f : 450.0f;
		} break;
		case 3: /* rage autostrafer */ {
			ses_ctx.input_forward_move = 450.0f;

			if ( abs( cmd->mouse_dx ) > 2 )
				ses_ctx.input_side_move = copysignf( 450.0f, ( float )cmd->mouse_dx );
		} break;
		default:
			break;
		}
	}
	/* on ground stuff */
	else {
		if ( ses_cfg_get_item( &ses_cfg, misc, movement, fast_stop, bool )
			&& !( cmd->buttons & ( in_forward | in_back | in_moveleft | in_moveright ) )
			&& speed_2d > 1.11f ) {
			vec3 vec_fwd = ses_ctx.input_angles;
			vec3_to_angle( &vec_fwd );
			vec3 vec_right = vec_fwd;
			vec3_cross( &vec_right, &( vec3 ){0.0f, 0.0f, 1.0f} );

			vec3 wish_vel;
			vec3_zero( &wish_vel );

			if ( speed_2d > 13.0f ) {
				wish_vel = ses_ctx.unpredicted_vel;
				vec3_mulf( &wish_vel, -1.0f );
			}

			ses_ctx.input_forward_move = ( wish_vel.y - ( vec_right.y / vec_right.x ) * wish_vel.x ) / ( vec_fwd.y - ( vec_right.y / vec_right.x ) * vec_fwd.x );
			ses_ctx.input_side_move = ( wish_vel.x - vec_fwd.x * ses_ctx.input_forward_move ) / vec_right.x;

			cmd->buttons &= ~in_walk;
		}
	}
}