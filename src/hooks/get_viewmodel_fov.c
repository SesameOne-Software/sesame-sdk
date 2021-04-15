#include "include/hooks/hooks.h"

float __fastcall hooks_get_viewmodel_fov ( REG ) {
	typedef float ( __fastcall* hooks_get_viewmodel_fov_fn )( REG );
	
	return ( ( hooks_get_viewmodel_fov_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_get_viewmodel_fov ] ) )( REG_OUT );
}