#include "include/hooks/hooks.h"

#include <intrin.h>

bool __fastcall hooks_in_prediction ( REG ) {
	typedef bool ( __fastcall* hooks_in_prediction_fn )( REG );
	
	if ( _ReturnAddress ( ) == cs_offsets.maintain_sequence_transitions_ret )
		return true;

	return ( ( hooks_in_prediction_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_in_prediction ] ) )( REG_OUT );
}