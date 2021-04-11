#include "include/hooks/hooks.h"

bool __fastcall hooks_create_move( REG, float sample_time, usercmd* cmd ) {
	typedef bool( __fastcall* hooks_create_move_fn )( REG, float sample_time, usercmd* cmd );
	const bool ret = ( ( hooks_create_move_fn )subhook_get_trampoline( hooks_subhooks[ subhook_create_move ] ) )( REG_OUT, sample_time, cmd );

	if ( !cmd || !cmd->cmd_num )
		return ret;

	/* run input-related code here */
	const player* local = cs_get_local( );

	if ( local && cmd->buttons & in_jump && !( *player_flags( local ) & fl_on_ground ) )
		cmd->buttons &= ~in_jump;
}