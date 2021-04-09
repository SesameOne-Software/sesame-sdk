#include "sdk.h"
#include "netvars.h"

isurface* cs_isurface = NULL;
ipanel* cs_ipanel= NULL;
iengine* cs_iengine= NULL;
iclient* cs_iclient= NULL;
ientlist* cs_ientlist= NULL;
imatsys* cs_imatsys= NULL;
imdlinfo* cs_imdlinfo= NULL;
imdlrender* cs_imdlrender= NULL;
irenderview* cs_irenderview= NULL;
iglobals* cs_iglobals= NULL;
iphys* cs_iphys= NULL;
itrace* cs_itrace= NULL;
iclientstate* cs_iclientstate= NULL;
imemalloc* cs_imemalloc= NULL;
ipred* cs_ipred= NULL;
imovehelper* cs_imovehelper= NULL;
imove* cs_imove= NULL;
imdlcache* cs_imdlcache= NULL;
iinput* cs_iinput= NULL;
icvar* cs_icvar= NULL;
ievents* cs_ievents= NULL;
ibeams* cs_ibeams= NULL;
IDirect3DDevice9* cs_id3ddev= NULL;

bool cs_init ( ) {
	cs_iglobals = **( iglobals*** )(pattern_search ( "client.dll", "A1 ? ? ? ? 5E 8B 40 10" ) + 1);
	cs_ientlist = create_interface ( "client.dll", "VClientEntityList003" );
	cs_imatsys = create_interface ( "materialsystem.dll", "VMaterialSystem080" );
	cs_imdlinfo = create_interface ( "engine.dll", "VModelInfoClient004" );
	cs_imdlrender = create_interface ( "engine.dll", "VEngineModel016" );
	cs_irenderview = create_interface ( "engine.dll", "VEngineRenderView014" );
	cs_iclient = create_interface ( "client.dll", "VClient018" );
	cs_isurface = create_interface ( "vguimatsurface.dll", "VGUI_Surface031" );
	cs_ipanel = create_interface ( "vgui2.dll", "VGUI_Panel009" );
	cs_iengine = create_interface ( "engine.dll", "VEngineClient014" );
	cs_iphys = create_interface ( "vphysics.dll", "VPhysicsSurfaceProps001" );
	cs_itrace = create_interface( "engine.dll", "EngineTraceClient004" );
	cs_ipred = create_interface ( "client.dll", "VClientPrediction001" );
	cs_imove = create_interface( "client.dll", "GameMovement001" );
	cs_imdlcache = **( imdlcache*** )(pattern_search ( "client.dll", "8B 35 ? ? ? ? 8B CE 8B 06 FF 90 ? ? ? ? 8B 4F" ) + 2);
	cs_ievents = create_interface ( "engine.dll", "GAMEEVENTSMANAGER002" );
	cs_iinput = *( iinput** )(pattern_search ( "client.dll", "B9 ? ? ? ? FF 60 60" ) + 1);
	cs_icvar = create_interface ( "vstdlib.dll", "VEngineCvar007" );
	cs_imovehelper = **( imovehelper*** )( pattern_search ( "client.dll", "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01" ) + 2 );
	cs_iclientstate = **( iclientstate*** )(pattern_search ( "engine.dll", "A1 ? ? ? ? 8B 88 ? ? ? ? 85 C9 75 07" ) + 1);
	cs_ibeams = *( ibeams** )(pattern_search ( "client.dll", "A1 ? ? ? ? 56 8B F1 B9 ? ? ? ? FF 50 08" )+ 1);
	cs_imemalloc = *( imemalloc** ) GetProcAddress ( GetModuleHandleA ( "tier0.dll" ), "g_pMemAlloc" );
	cs_id3ddev = **(IDirect3DDevice9*** )(pattern_search ( "shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C" ) + 1);
}

static inline void cs_screen_transform( vec3* origin, vec3* screen ) {
	static mat3x4* viewmat_ptr = NULL;

	if (!viewmat_ptr)
		viewmat_ptr = ( mat3x4* )(*(uintptr_t*)(pattern_search( "client.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9" ) + 3 ) + 176);

	screen->x = *mat3x4_at(viewmat_ptr, 0, 0) * origin->x + *mat3x4_at(viewmat_ptr, 0, 1) * origin->y + *mat3x4_at(viewmat_ptr, 0, 2) * origin->z + *mat3x4_at(viewmat_ptr, 0, 3);
	screen->y = *mat3x4_at(viewmat_ptr, 1, 0) * origin->x + *mat3x4_at(viewmat_ptr, 1, 1) * origin->y + *mat3x4_at(viewmat_ptr, 1, 2) * origin->z + *mat3x4_at(viewmat_ptr, 1, 3);
	screen->z = 0.0f;

	const float w = abs(*mat3x4_at(viewmat_ptr, 3, 0) * origin->x + *mat3x4_at(viewmat_ptr, 3, 1) * origin->y + *mat3x4_at(viewmat_ptr, 3, 2) * origin->z + *mat3x4_at(viewmat_ptr, 3, 3));

	screen->x *= 1.0f / w;
	screen->y *= 1.0f / w;
}

bool cs_world_to_screen( vec3* origin, vec3* screen ) {
	int w, h;
	iengine_get_screen_size(cs_iengine, &w, &h);

	float wf = (float)w;
	float hf = (float)h;

	screen_transform( origin, screen );

	vec3_zero(screen);

	screen->x = ( wf * 0.5f ) + ( screen->x * wf ) * 0.5f;
	screen->y = ( hf * 0.5f ) - ( screen->y * hf ) * 0.5f;

	return true;
}

bool cs_is_valve_server( ) {
	static uintptr_t* cs_game_rules = NULL;

	if (!cs_game_rules)
		cs_game_rules = *(uintptr_t**)(pattern_search( "client.dll", "A1 ? ? ? ? 74 38" ) + 1);

	return *cs_game_rules && *(bool* ) ( *cs_game_rules + 0x7C );
}

void cs_add_box_overlay ( const vec3* origin, const vec3* mins, const vec3* maxs, const vec3* angles, int r, int g, int b, int a, float duration ) {
	static void* debug_overlay = NULL;

	if (!debug_overlay)
		debug_overlay = **(void***)(pattern_search( "client.dll", "A1 ? ? ? ? F3 0F 11 44 24 ? 8B 30 8B" ) + 1);

	((void(__fastcall*)( void*, void*, const vec3*, const vec3*, const vec3*, const vec3*, int, int, int, int, float ))utils_vfunc ( debug_overlay, 1 ))( debug_overlay, NULL, origin, mins, maxs, angles, r, g, b, a, duration );
}

void cs_free() {

}