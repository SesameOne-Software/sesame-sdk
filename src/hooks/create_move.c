#include "hooks/hooks.h"
#include "ses.h"

#include "features/movement.h"

__attribute__( ( thiscall ) ) bool hooks_create_move( void* this, float sample_time, usercmd* cmd ) {
	typedef __attribute__( ( thiscall ) ) bool( *hooks_create_move_fn )( void* this, float sample_time, usercmd* cmd );

	const bool ret = ( ( hooks_create_move_fn )subhook_get_trampoline( hooks_subhooks[ subhook_create_move ] ) )( this, sample_time, cmd );

	if ( !cmd || !cmd->cmd_num ) {
		/* call is from sample_input */
		if ( cmd ) {
			iengine_set_angles( cs_iengine, &cmd->angles );
			ipred_set_local_viewangles( cs_ipred, &cmd->angles );
		}

		return ret;
	}

	/* notify other parts of hack that we are in create_move */
	ses_ctx.in_move = true;
	/* we should not choke by default */
	ses_ctx.choke = false;

	/* backup original input values */
	ses_ctx.input_angles = cmd->angles;
	ses_ctx.input_forward_move = cmd->forward_move;
	ses_ctx.input_side_move = cmd->side_move;

	/* run input-related code here */
	player* local = cs_get_local( );

	if ( local && player_is_alive( local ) )
		ses_ctx.unpredicted_vel = *player_vel( local );

	features_movement_run( cmd );

	/* clamp viewangles */
	vec3 angles;
	iengine_get_angles( cs_iengine, &angles );
	iengine_set_angles( cs_iengine, vec3_clamp_angle( &angles ) );

	vec3_clamp_angle( &cmd->angles );

	/* clamp movement */
	cmd->forward_move = clamp( cmd->forward_move, -450.0f, 450.0f );
	cmd->side_move = clamp( cmd->side_move, -450.0f, 450.0f );
	cmd->up_move = clamp( cmd->up_move, -450.0f, 450.0f );

	/* fix movement */
	cs_rotate_movement( cmd, ses_ctx.input_side_move, ses_ctx.input_forward_move, &ses_ctx.input_angles );

	/* handle choking packets */
	*( bool* )( *( uintptr_t* )( ( uintptr_t )__builtin_frame_address( 0 ) - 0x4 ) - 0x1C ) = !ses_ctx.choke;

	ses_ctx.num_sent = ses_ctx.choke ? 0 : ++ses_ctx.num_sent;
	ses_ctx.num_choked = ses_ctx.choke ? ++ses_ctx.num_choked : 0;

	/* notify other parts of hack that we are exiting create_move */
	ses_ctx.in_move = false;

	return false;
}
