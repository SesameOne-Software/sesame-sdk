#include "include/hooks/hooks.h"
#include "include/ses.h"

bool __fastcall hooks_create_move( REG, float sample_time, usercmd* cmd ) {
	typedef bool( __fastcall* hooks_create_move_fn )( REG, float sample_time, usercmd* cmd );
	
	const bool ret = ( ( hooks_create_move_fn )subhook_get_trampoline( hooks_subhooks[ subhook_create_move ] ) )( REG_OUT, sample_time, cmd );

	if ( !cmd || !cmd->cmd_num ) {
		/* call is from sample_input */
		if ( cmd ) {
			iengine_set_angles ( cs_iengine, &cmd->angles );
			ipred_set_local_viewangles ( cs_ipred, &cmd->angles );
		}

		return ret;
	}
	
	/* notify other parts of hack that we are in create_move */
	ses_ctx.in_move = true;

	/* run input-related code here */
	player* local = cs_get_local( );

	if ( local && cmd->buttons & in_jump && !( *player_flags( local ) & fl_on_ground ) )
		cmd->buttons &= ~in_jump;
	
	/* clamp viewangles */
	vec3 angles;
	iengine_get_angles ( cs_iengine, &angles );
	iengine_set_angles ( cs_iengine, vec3_clamp_angle ( &angles ) );

	vec3_clamp_angle ( &cmd->angles );

	/* clamp movement */
	cmd->forward_move = clampf ( cmd->forward_move, -450.0f, 450.0f );
	cmd->side_move = clampf ( cmd->side_move, -450.0f, 450.0f );
	cmd->up_move = clampf ( cmd->up_move, -450.0f, 450.0f );

	/* handle choking packets */
	*( bool* ) ( *( uintptr_t* ) ( ( uintptr_t ) _AddressOfReturnAddress ( ) - 0x4 ) - 0x1C ) = ses_ctx.choke;
	
	ses_ctx.num_sent = ses_ctx.choke ? 0 : ( ses_ctx.num_sent + 1 );
	ses_ctx.num_choked = ses_ctx.choke ? ( ses_ctx.num_choked + 1 ) : 0;
	
	/* notify other parts of hack that we are exiting create_move */
	ses_ctx.in_move = false;

	return false;
}