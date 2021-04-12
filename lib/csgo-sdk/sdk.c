#include "sdk.h"
#include "netvars.h"

isurface* cs_isurface = NULL;
ipanel* cs_ipanel = NULL;
iengine* cs_iengine = NULL;
iclient* cs_iclient = NULL;
ientlist* cs_ientlist = NULL;
imatsys* cs_imatsys = NULL;
imdlinfo* cs_imdlinfo = NULL;
imdlrender* cs_imdlrender = NULL;
irenderview* cs_irenderview = NULL;
iglobals* cs_iglobals = NULL;
iphys* cs_iphys = NULL;
itrace* cs_itrace = NULL;
iclientstate* cs_iclientstate = NULL;
imemalloc* cs_imemalloc = NULL;
ipred* cs_ipred = NULL;
imovehelper* cs_imovehelper = NULL;
imove* cs_imove = NULL;
imdlcache* cs_imdlcache = NULL;
iinput* cs_iinput = NULL;
icvar* cs_icvar = NULL;
ievents* cs_ievents = NULL;
ibeams* cs_ibeams = NULL;
IDirect3DDevice9* cs_id3ddev = NULL;
player** cs_local_ptr = NULL;
HWND cs_window = NULL;

static inline void cs_screen_transform( vec3* origin, vec3* screen ) {
	const mat3x4* viewmat_ptr = ( mat3x4* ) cs_offsets.sdk_view_matrix;

	screen->x = *mat3x4_at( viewmat_ptr, 0, 0 ) * origin->x + *mat3x4_at( viewmat_ptr, 0, 1 ) * origin->y + *mat3x4_at( viewmat_ptr, 0, 2 ) * origin->z + *mat3x4_at( viewmat_ptr, 0, 3 );
	screen->y = *mat3x4_at( viewmat_ptr, 1, 0 ) * origin->x + *mat3x4_at( viewmat_ptr, 1, 1 ) * origin->y + *mat3x4_at( viewmat_ptr, 1, 2 ) * origin->z + *mat3x4_at( viewmat_ptr, 1, 3 );
	screen->z = 0.0f;

	const float w = abs( *mat3x4_at( viewmat_ptr, 3, 0 ) * origin->x + *mat3x4_at( viewmat_ptr, 3, 1 ) * origin->y + *mat3x4_at( viewmat_ptr, 3, 2 ) * origin->z + *mat3x4_at( viewmat_ptr, 3, 3 ) );

	screen->x *= 1.0f / w;
	screen->y *= 1.0f / w;
}

bool cs_world_to_screen( vec3* origin, vec3* screen ) {
	int w, h;
	iengine_get_screen_size( cs_iengine, &w, &h );

	float wf = ( float )w;
	float hf = ( float )h;

	cs_screen_transform( origin, screen );

	vec3_zero( screen );

	screen->x = ( wf * 0.5f ) + ( screen->x * wf ) * 0.5f;
	screen->y = ( hf * 0.5f ) - ( screen->y * hf ) * 0.5f;

	return true;
}