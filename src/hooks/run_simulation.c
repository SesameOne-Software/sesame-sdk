#include "include/hooks/hooks.h"

void __fastcall hooks_run_simulation ( REG, int current_command, usercmd* cmd, player* ent ) {
	typedef void ( __fastcall* hooks_run_simulation_fn )( REG, int current_command, usercmd* cmd, player* ent );

	if (!ent || !cs_get_local ( ) || ent != cs_get_local ( ) )
		return ( ( hooks_run_simulation_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_run_simulation ] ) )( REG_OUT, current_command, cmd, ent );
	/**/

	( ( hooks_run_simulation_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_run_simulation ] ) )( REG_OUT, current_command, cmd, ent );

	/**/
}