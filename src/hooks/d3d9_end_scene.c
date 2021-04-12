#include "include/hooks/hooks.h"

#include "include/gui/menu.h"

#include <intrin.h>

HRESULT __stdcall hooks_d3d9_end_scene ( IDirect3DDevice9* device ) {
	typedef HRESULT ( __stdcall* hooks_d3d9_end_scene_fn )( IDirect3DDevice9* device );
	static void* ret = NULL;

	if ( !ret )
		ret = _ReturnAddress ( );

	if ( ret != _ReturnAddress ( ) )
		return ( ( hooks_d3d9_end_scene_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_d3d9_end_scene ] ) )( device );

	utils_update_key_states ( );

	menu_draw ( );

	return ( ( hooks_d3d9_end_scene_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_d3d9_end_scene ] ) )( device );
}