#include "hooks/hooks.h"

#include "gui/menu.h"

__attribute__( ( stdcall ) ) HRESULT hooks_d3d9_reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_params ) {
	typedef __attribute__( ( stdcall ) ) HRESULT( *hooks_d3d9_reset_fn )( IDirect3DDevice9* device );

	menu_release( );

	const HRESULT ret = ( ( hooks_d3d9_reset_fn )subhook_get_trampoline( hooks_subhooks[ subhook_d3d9_reset ] ) )( device );

	if ( SUCCEEDED( ret ) ) {
		menu_reset( );
	}

	return ret;
}