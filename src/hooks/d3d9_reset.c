#include "include/hooks/hooks.h"

#include "include/gui/menu.h"

HRESULT __stdcall hooks_d3d9_reset ( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_params ) {
	typedef HRESULT ( __stdcall* hooks_d3d9_reset_fn )( IDirect3DDevice9* device );
	
	menu_release ( );

	const HRESULT ret = ( ( hooks_d3d9_reset_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_d3d9_reset ] ) )( device );

	if (SUCCEEDED(ret) ) {
		menu_reset ( );
	}

	return ret;
}