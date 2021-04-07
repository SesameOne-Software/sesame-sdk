#include "player.h"

void animstate_reset ( animstate* this ) {
	static void ( __thiscall* fn )( animstate* ) = NULL;

	if ( !fn )
		fn = pattern_search ( "client.dll", "56 6A 01 68 ? ? ? ? 8B F1" );
	
	fn ( this );
}

const char* animstate_get_weapon_move_animation ( animstate* this ) {
	static const char* ( __thiscall * fn )( animstate* ) = NULL;

	if ( !fn )
		fn = pattern_search ( "client.dll", "53 56 57 8B F9 33 F6 8B 4F 60 8B 01 FF 90" );

	return fn ( this );
}

void player_get_sequence_linear_motion ( player* this, void* studio_hdr, int sequence, float* poses, vec3* vec ) {
	static void ( __fastcall* fn )( void*, int, float*, vec3* ) = NULL;

	if ( !fn )
		fn = pattern_search ( "client.dll", "55 8B EC 83 EC 0C 56 8B F1 57 8B FA 85 F6 75 14" );

	__asm {
		mov edx, sequence
		mov ecx, studio_hdr
		push vec
		push poses
		call fn
		add esp, 8
	}
}

float player_get_sequence_move_distance ( player* this, void* studio_hdr, int sequence ) {
	vec3_t ret;
	get_sequence_linear_motion ( studio_hdr, sequence, poses().data(), &ret );
	return ret.length ( );
}

int player_lookup_sequence ( player* this, const char* seq ) {
	static auto addr = pattern::search ( _ ( "client.dll" ), "E8 ? ? ? ? 5E 83 F8 FF" ).resolve_rip().get<int ( __thiscall* )( player_t*, const char* )> ( );
	return addr ( this, seq );
}

float player_sequence_duration ( player* this, int sequence ) {
	static auto addr = pattern::search ( _ ( "client.dll" ), "55 8B EC 56 8B F1 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? 83 C4 04 5E 5D C2 04 00 52 68 ? ? ? ? 6A 02" ).get<float ( __thiscall* )( player_t*, int )> ( );

	float retval;
	addr ( this, sequence );
	__asm movss retval, xmm0;
	return retval;
}

float player_get_sequence_cycle_rate_server ( player* this, int sequence ) {
	float t = sequence_duration ( sequence );
	if ( t > 0.0f )
		return 1.0f / t;
	else
		return 1.0f / 0.1f;
}

bool player_physics_run_think( player* this, int unk01 ) {
	static auto func = pattern::search( _( "client.dll" ), _( "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87 ? ? ? ? C1 E8 16" ) ).get< bool( __thiscall* )( void*, int ) >( );
	return func( this, unk01 );
}

ehandle player_handle( player* this ) {
	using fn = ehandle( __thiscall* )( void* );
	static auto get_ehandle = pattern::search( _( "client.dll" ), _( "8B 51 3C 83 FA FF" ) ).get< fn >( );
	return get_ehandle( this );
}

void player_create_animstate( player* this, animstate* state ) {
	static auto create_animstate = pattern::search( _( "client.dll" ), _( "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46" ) ).get< void( __thiscall* )( void*, void* ) >( );
	create_animstate( state, this );
}

void player_set_abs_angles( player* this, vec3* ang ) {
	using setabsangles_fn = void( __thiscall* )( void*, vec3* );
	static auto set_abs_angles = pattern::search( _( "client.dll" ), _( "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8" ) ).get< setabsangles_fn >( );
	set_abs_angles( this, ang );
}

void player_set_abs_origin( player* this, vec3* vec ) {
	using set_abs_origin_fn = void( __thiscall* )( void*, vec3* );
	static auto fn = pattern::search( _( "client.dll" ), _( "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8 ? ? ? ? 8B 7D" ) ).get< set_abs_origin_fn >( );
	fn( this, vec );
}

animstate* player_animstate( player* this ) {
	static auto animstate_offset = pattern::search( _( "client.dll" ), _( "8B 8E ? ? ? ? F3 0F 10 48 04 E8 ? ? ? ? E9" ) ).add( 2 ).deref( ).get< std::uintptr_t >( );
	return *( animstate** )( uintptr_t( this ) + animstate_offset );
}

void player_get_eye_pos( player* this, vec3* pos) {
	static auto modify_eye_position = pattern::search( _( "client.dll" ), _( "57 E8 ? ? ? ? 8B 06 8B CE FF 90" ) ).add( 1 ).resolve_rip( ).get<void*>( );

	vec3_init(pos);

	/* eye position */
	vfunc< void( __thiscall* )( player_t*, vec3* ) >( this, 168 ) ( this, pos );

	if ( *reinterpret_cast< bool* > ( uintptr_t ( this ) + 0x3AC8 ) && animstate ( ) )
		reinterpret_cast< void ( __thiscall* )( animstate*, vec3* ) >( modify_eye_position ) ( animstate ( ), pos );
}

uint32_t* player_bone_count( player* this) {
	static auto offset = pattern::search( _( "client.dll" ), _( "8B 87 ? ? ? ? 8B 4D 0C" ) ).add( 2 ).deref( ).get< uint32_t >( );
	return ( uint32_t* )( uintptr_t( renderable( ) ) + offset );
}

mat3x4* player_bone_cache( player* this) {
	static auto offset = pattern::search( _( "client.dll" ), _( "FF B7 ? ? ? ? 52" ) ).add( 2 ).deref( ).get< uint32_t >( );
	return *( mat3x4** )( uintptr_t( renderable( ) ) + offset );
}

weapon* player_get_weapon( player* this ) {
	return cs::i::ent_list->get_by_handle< weapon* >( weapon_handle( ) );
}

vec_weapons player_weapons ( player* this) {
	const ehandle* weapon_handles = player_weapon_handles(this);
	vec_weapons ret = vector_create();

	for ( auto i = 0; weapon_handles [ i ] != -1; i++ ) {
		const weapon* cur_weapon = cs::i::ent_list->get_by_handle<weapon*> ( weapon_handles [ i ] );
		
		if ( !cur_weapon )
			continue;

		vector_add(&ret, weapon*, cur_weapon);
	}

	/* MAKE SURE TO CALL vector_free */
	return ret;
}

vec_wearables player_wearables ( player* this ) {
	const ehandle* wearable_handles = player_wearable_handles(this);
	vec_wearables ret = vector_create();

	for ( auto i = 0; wearable_handles [ i ] != -1; i++ ) {
		const weapon* cur_wearable = cs::i::ent_list->get_by_handle<weapon*> ( wearable_handles [ i ] );
		
		if ( !cur_wearable )
			continue;

		vector_add(&ret, weapon*, cur_wearable);
	}

	/* MAKE SURE TO CALL vector_free */
	return ret;
}