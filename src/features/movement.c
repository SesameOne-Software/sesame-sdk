#include "include/features/movement.h"
#include "include/gui/config.h"

void features_movement_run ( usercmd* cmd ) {
	if ( !iengine_is_in_game ( cs_iengine ) || !iengine_is_connected ( cs_iengine ) )
		return;

	player* local = cs_get_local ( );

	if ( !local || !player_is_alive( local ) || *player_movetype ( local ) == movetype_noclip || *player_movetype ( local ) == movetype_fly )
		return;

	/* in air stuff */
	if ( !( *player_flags ( local ) & fl_on_ground ) ) {
		if ( *ses_cfg_get_item ( &ses_cfg, misc, movement, autojump, bool ) && cmd->buttons & in_jump )
			cmd->buttons &= ~in_jump;

		enum e_strafe_dirs {
			qd_front,
			qd_left,
			qd_right,
			qd_back
		};

		switch ( *ses_cfg_get_item( &ses_cfg, misc, movement, autostrafer, int ) ) {
		case 0:
		default: /* no autostrafer */ {
		} break;
		case 1: /* legit autostrafer */ {
			
		} break;
		case 2: /* directional autostrafer */ {
		} break;
		case 3: /*  */ {
		} break;
		}

		if ( *ses_cfg_get_item ( &ses_cfg, misc, movement, autostrafer, int ) > 0 ) {
			if ( abs ( cmd->mouse_dx ) > 2 ) {
				cmd->side_move = cmd->mouse_dx < 0 ? -450.0f : 450.0f;
				return;
			}
		}
	}
	/* on ground stuff */
	else {

	}
}