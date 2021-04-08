#include "sdk.h"

void animstate_reset ( animstate* this ) {
	static void ( __fastcall* fn )( animstate*, void* ) = NULL;

	if ( !fn )
		fn = pattern_search ( "client.dll", "56 6A 01 68 ? ? ? ? 8B F1" );
	
	fn ( this, NULL );
}

const char* animstate_get_weapon_move_animation ( animstate* this ) {
	static const char* ( __fastcall * fn )( animstate* , void* ) = NULL;

	if ( !fn )
		fn = pattern_search ( "client.dll", "53 56 57 8B F9 33 F6 8B 4F 60 8B 01 FF 90" );

	return fn ( this, NULL );
}

void player_get_sequence_linear_motion ( player* this, void* studio_hdr, int sequence, float* poses, vec3* vec ) {
	static void* fn = NULL;

	if ( !fn )
		fn = (void*)pattern_search ( "client.dll", "55 8B EC 83 EC 0C 56 8B F1 57 8B FA 85 F6 75 14" );

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
	vec3 ret;
	get_sequence_linear_motion ( studio_hdr, sequence, poses().data(), &ret );
	return ret.length ( );
}

int player_lookup_sequence ( player* this, const char* seq ) {
	static int ( __fastcall* fn )( player*, void*, const char* ) = NULL;

	if ( !fn )
		fn = pattern_rip(pattern_search ( "client.dll", "E8 ? ? ? ? 5E 83 F8 FF" ));

	return addr ( this, NULL, seq );
}

float player_sequence_duration ( player* this, int sequence ) {
	static float ( __fastcall* fn )( player*, void*, int ) = NULL;

	if ( !fn )
		fn = pattern_search ( "client.dll", "55 8B EC 56 8B F1 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? 83 C4 04 5E 5D C2 04 00 52 68 ? ? ? ? 6A 02" );

	float retval;
	fn ( this, NULL, sequence );
	__asm movss retval, xmm0;
	return retval;
}

float player_get_sequence_cycle_rate_server ( player* this, int sequence ) {
	float t = sequence_duration ( sequence );

	if ( t > 0.0f )
		return 1.0f / t;
		
	return 1.0f / 0.1f;
}

bool player_physics_run_think( player* this, int unk01 ) {
	static bool( __fastcall* fn )( player*, void*, int ) = NULL;

	if ( !fn )
		fn = pattern_search ( "client.dll", "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87 ? ? ? ? C1 E8 16" );

	return fn( this, NULL, unk01 );
}

ehandle player_handle( player* this ) {
	static ehandle( __fastcall* fn )( player*, void* ) = NULL;

	if ( !fn )
		fn = pattern_search ( "client.dll", "8B 51 3C 83 FA FF" );

	return fn( this, NULL );
}

void player_create_animstate( player* this, animstate* state ) {
	static void( __fastcall* fn )( player*, void*, animstate* ) = NULL;

	if ( !fn )
		fn = pattern_search ( "client.dll", "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46" );

	fn( state, NULL, this );
}

void player_set_abs_angles( player* this, vec3* ang ) {
	static void( __fastcall* fn )( player*, void*, vec3* ) = NULL;

	if ( !fn )
		fn = pattern_search ( "client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8" );

	fn( this, NULL, ang );
}

void player_set_abs_origin( player* this, vec3* vec ) {
	static void( __fastcall* fn )( player*, void*, vec3* ) = NULL;

	if ( !fn )
		fn = pattern_search ( "client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8 ? ? ? ? 8B 7D" );
		
	fn( this, NULL, vec );
}

animstate* player_animstate( player* this ) {
	static ptrdiff_t offset = 0;

	if ( !offset )
		offset = *(ptrdiff_t*)(pattern_search ( "client.dll", "8B 8E ? ? ? ? F3 0F 10 48 04 E8 ? ? ? ? E9" ) + 2);

	return *( animstate** )((uintptr_t)this + offset );
}

void player_get_eye_pos( player* this, vec3* pos) {
	static void ( __fastcall* modify_eye_pos )( animstate*, void*, vec3* ) = NULL;

	if ( !modify_eye_pos )
		modify_eye_pos = (void*)pattern_rip(pattern_search ( "client.dll", "57 E8 ? ? ? ? 8B 06 8B CE FF 90" ) + 1);

	vec3_init(pos);

	/* eye position */
	((void( __fastcall* )( player*, void*, vec3* ))utils_vfunc( this, 168 )) ( this, NULL, pos );

	if ( *reinterpret_cast< bool* > ( uintptr_t ( this ) + 0x3AC8 ) && player_animstate (this ) )
		modify_eye_pos ( player_animstate (this ), NULL, pos );
}

uint32_t* player_bone_count( player* this) {
	static ptrdiff_t offset = 0;

	if ( !offset )
		offset = *(ptrdiff_t*)(pattern_search ( "client.dll", "8B 87 ? ? ? ? 8B 4D 0C" ) + 2);

	return ( uint32_t* )( (uintptr_t)entity_get_renderable((entity*)this) + offset );
}

mat3x4* player_bone_cache( player* this) {
	static ptrdiff_t offset = 0;

	if ( !offset )
		offset = *(ptrdiff_t*)(pattern_search ( "client.dll", "FF B7 ? ? ? ? 52" ) + 2);

	return *( mat3x4** )( (uintptr_t)entity_get_renderable((entity*)this) + offset );
}

weapon* player_get_weapon( player* this ) {
	return (weapon*)ientlist_get_entity_from_handle ( cs_ientlist, weapon_handle( ) );
}

vec_weapons player_weapons ( player* this) {
	const ehandle* weapon_handles = player_weapon_handles(this);
	vec_weapons ret = vector_create();

	for ( int i = 0; weapon_handles [ i ] != -1; i++ ) {
		const weapon* cur_weapon = (weapon*)ientlist_get_entity_from_handle ( cs_ientlist, weapon_handles [ i ] );
		
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

	for ( int i = 0; wearable_handles [ i ] != -1; i++ ) {
		const weapon* cur_wearable = (weapon*)ientlist_get_entity_from_handle ( cs_ientlist, wearable_handles [ i ] );
		
		if ( !cur_wearable )
			continue;

		vector_add(&ret, weapon*, cur_wearable);
	}

	/* MAKE SURE TO CALL vector_free */
	return ret;
}