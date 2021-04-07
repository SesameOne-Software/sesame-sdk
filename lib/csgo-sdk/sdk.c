#include "sdk.h"
#include "netvars.h"

surface* cs_surface = NULL;
panel* cs_panel = NULL;
engine* cs_engine = NULL;
client* cs_client = NULL;
entlist* cs_entlist = NULL;
matsys* cs_matsys = NULL;
mdlinfo* cs_mdlinfo = NULL;
mdlrender* cs_mdlrender = NULL;
renderview* cs_renderview = NULL;
globals* cs_globals = NULL;
phys* cs_phys = NULL;
engine_trace* cs_trace = NULL;
clientstate* cs_clientstate = NULL;
mem_alloc* cs_memalloc = NULL;
prediction* cs_pred = NULL;
move_helper* cs_movehelper = NULL;
movement* cs_move = NULL;
mdl_cache_t* cs_mdlcache = NULL;
input* cs_input = NULL;
cvars* cs_cvar = NULL;
game_event_mgr* cs_events = NULL;
view_render_beams* cs_beams = NULL;
IDirect3DDevice9* cs_dev = NULL;

bool cs_init ( ) {
	cs_globals = **( globals*** )(pattern_search ( "client.dll", "A1 ? ? ? ? 5E 8B 40 10" ) + 1);
	cs_entlist = create_interface< c_entlist* > ( "client.dll", "VClientEntityList003" );
	cs_matsys = create_interface< c_matsys* > ( "materialsystem.dll", "VMaterialSystem080" );
	cs_mdlinfo = create_interface< c_mdlinfo* > ( "engine.dll", "VModelInfoClient004" );
	cs_mdlrender = create_interface< c_mdlrender* > ( "engine.dll", "VEngineModel016" );
	cs_renderview = create_interface< c_renderview* > ( "engine.dll", "VEngineRenderView014" );
	cs_client = create_interface< c_client* > ( "client.dll", "VClient018" );
	cs_surface = create_interface< c_surface* > ( "vguimatsurface.dll", "VGUI_Surface031" );
	cs_panel = create_interface< c_panel* > ( "vgui2.dll", "VGUI_Panel009" );
	cs_engine = create_interface< c_engine* > ( "engine.dll", "VEngineClient014" );
	cs_phys = create_interface< c_phys* > ( "vphysics.dll", "VPhysicsSurfaceProps001" );
	cs_trace = create_interface< c_engine_trace* > ( "engine.dll", "EngineTraceClient004" );
	cs_pred = create_interface< c_prediction* > ( "client.dll", "VClientPrediction001" );
	cs_move = create_interface< c_movement* > ( "client.dll", "GameMovement001" );
	cs_mdlcache = pattern_search ( "client.dll", "8B 35 ? ? ? ? 8B CE 8B 06 FF 90 ? ? ? ? 8B 4F" ).add ( 2 ).deref ( ).deref ( ).get< mdl_cache_t* > ( );
	cs_events = create_interface< c_game_event_mgr* > ( "engine.dll", "GAMEEVENTSMANAGER002" );
	cs_input = pattern_search ( "client.dll", "B9 ? ? ? ? FF 60 60" ).add ( 1 ).deref ( ).get< c_input* > ( );
	cs_cvar = create_interface< c_cvar* > ( "vstdlib.dll", "VEngineCvar007" );
	cs_movehelper = **reinterpret_cast< c_move_helper*** >( pattern_search ( "client.dll", "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01" ).add ( 2 ).get< std::uintptr_t > ( ) );
	cs_clientstate = pattern_search ( "engine.dll", "A1 ? ? ? ? 8B 88 ? ? ? ? 85 C9 75 07" ).add ( 1 ).deref ( ).deref ( ).get< c_clientstate* > ( );
	cs_beams = pattern_search ( "client.dll", "A1 ? ? ? ? 56 8B F1 B9 ? ? ? ? FF 50 08" ).add ( 1 ).deref ( ).get< c_view_render_beams* > ( );
	cs_memalloc = *( c_mem_alloc** ) GetProcAddress ( GetModuleHandleA ( "tier0.dll" ), "g_pMemAlloc" );
	cs_dev = pattern_search ( "shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C" ).add ( 1 ).deref ( ).deref ( ).get< IDirect3DDevice9* > ( );
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