#include "hooks/hooks.h"

__attribute__( ( thiscall ) ) void hooks_run_simulation( ipred* this, int current_command, usercmd* cmd, player* ent ) {
	typedef __attribute__( ( thiscall ) ) void( *hooks_run_simulation_fn )( ipred* this, int current_command, usercmd* cmd, player* ent );

	if ( !ent || !cs_get_local( ) || ent != cs_get_local( ) )
		return ( ( hooks_run_simulation_fn )subhook_get_trampoline( hooks_subhooks[ subhook_run_simulation ] ) )( this, current_command, cmd, ent );
	/**/

	( ( hooks_run_simulation_fn )subhook_get_trampoline( hooks_subhooks[ subhook_run_simulation ] ) )( this, current_command, cmd, ent );

	/**/
}