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
	cs_imdlcache = pattern_search ( "client.dll", "8B 35 ? ? ? ? 8B CE 8B 06 FF 90 ? ? ? ? 8B 4F" ).add ( 2 ).deref ( ).deref ( ).get< mdl_cache_t* > ( );
	cs_ievents = create_interface ( "engine.dll", "GAMEEVENTSMANAGER002" );
	cs_iinput = pattern_search ( "client.dll", "B9 ? ? ? ? FF 60 60" ).add ( 1 ).deref ( ).get< c_input* > ( );
	cs_icvar = create_interface ( "vstdlib.dll", "VEngineCvar007" );
	cs_imovehelper = **( imovehelper*** )( pattern_search ( "client.dll", "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01" ).add ( 2 ).get< std::uintptr_t > ( ) );
	cs_iclientstate = pattern_search ( "engine.dll", "A1 ? ? ? ? 8B 88 ? ? ? ? 85 C9 75 07" ).add ( 1 ).deref ( ).deref ( ).get< c_clientstate* > ( );
	cs_ibeams = pattern_search ( "client.dll", "A1 ? ? ? ? 56 8B F1 B9 ? ? ? ? FF 50 08" ).add ( 1 ).deref ( ).get< c_view_render_beams* > ( );
	cs_imemalloc = *( c_mem_alloc** ) GetProcAddress ( GetModuleHandleA ( "tier0.dll" ), "g_pMemAlloc" );
	cs_id3ddev = pattern_search ( "shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C" ).add ( 1 ).deref ( ).deref ( ).get< IDirect3DDevice9* > ( );
}

c_mdl_cache_critical_section::c_mdl_cache_critical_section( ) {
	cs::i::mdl_cache->begin_lock( );
}

c_mdl_cache_critical_section::~c_mdl_cache_critical_section( ) {
	cs::i::mdl_cache->end_lock( );
}

bool cs::render::screen_transform( vec3_t& screen, vec3_t& origin ) {
	static auto view_matrix = pattern::search( _( "client.dll" ), _( "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9" ) ).add( 3 ).deref( ).add( 176 ).get< std::uintptr_t >( );

	const auto& world_matrix = *( matrix3x4_t* ) view_matrix;

	screen.x = world_matrix [ 0 ][ 0 ] * origin.x + world_matrix [ 0 ][ 1 ] * origin.y + world_matrix [ 0 ][ 2 ] * origin.z + world_matrix [ 0 ][ 3 ];
	screen.y = world_matrix [ 1 ][ 0 ] * origin.x + world_matrix [ 1 ][ 1 ] * origin.y + world_matrix [ 1 ][ 2 ] * origin.z + world_matrix [ 1 ][ 3 ];
	screen.z = 0.0f;

	const auto w = world_matrix [ 3 ][ 0 ] * origin.x + world_matrix [ 3 ][ 1 ] * origin.y + world_matrix [ 3 ][ 2 ] * origin.z + world_matrix [ 3 ][ 3 ];

	if ( w < 0.001f ) {
		screen.x *= -1.0f / w;
		screen.y *= -1.0f / w;

		return true;
	}

	screen.x *= 1.0f / w;
	screen.y *= 1.0f / w;

	return false;
}

bool cs::render::world_to_screen( vec3_t& screen , vec3_t& origin ) {
	const auto find_point = [ ] ( vec3_t& point , int screen_w , int screen_h , float deg ) {
		const auto x2 = screen_w / 2.0f;
		const auto y2 = screen_h / 2.0f;
		const auto one = point.x - x2;
		const auto two = point.y - y2;
		const auto d = sqrt( one * one + two * two );
		const auto r = deg / d;

		point.x = r * point.x + ( 1.0f - r ) * x2;
		point.y = r * point.y + ( 1.0f - r ) * y2;
	};

	int w , h;
	cs::i::engine->get_screen_size( w , h );

	const bool transform = screen_transform( screen , origin );

	screen.x = ( w * 0.5f ) + ( screen.x * w ) * 0.5f;
	screen.y = ( h * 0.5f ) - ( screen.y * h ) * 0.5f;

	if ( transform ) {
		find_point( screen , w , h , sqrt( w * w + h * h ) );
		return false;
	}

	return true;
}