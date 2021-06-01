#include "hooks/hooks.h"

__attribute__( ( thiscall ) ) void hooks_perform_flashbang_effect( ipanel* this, void* view_setup ) {
	typedef __attribute__( ( thiscall ) ) void( *hooks_perform_flashbang_effect_fn )( ipanel* this, void* view_setup );

	( ( hooks_perform_flashbang_effect_fn )subhook_get_trampoline( hooks_subhooks[ subhook_perform_flashbang_effect ] ) )( this, view_setup );
}