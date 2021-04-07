#include "sdk.h"

econitem* econitem_static_data ( econitem* this ) {
	static econitem* ( __thiscall* get_static_data )( econitem* ) = NULL;

	if ( !get_static_data )
		get_static_data = pattern_search ( "client.dll", "55 8B EC 51 53 8B D9 8B 0D ? ? ? ? 56 57 8B B9" );

	return get_static_data ( this );
}

void econitem_update_equipped_state ( econitem* this, bool state ) {
	static int ( __thiscall * update_equipped_state )( econitem*, uint32_t ) = NULL;

	if (!update_equipped_state)
		update_equipped_state = pattern_search ( "client.dll", "55 8B EC 8B 45 08 8B D0 C1 EA 10" );

	update_equipped_state ( this, state );
}

/* XREF: "resource/flash/econ/weapons/cached/*.iic" string is inside the function */
void econitem_clean_inventory_image_cache_dir ( econitem* this ) {
	static void ( __thiscall* clean_inventory_image_cache_dir )( econitem* ) = NULL;

	if ( !clean_inventory_image_cache_dir )
		clean_inventory_image_cache_dir = pattern_search ( "client.dll", "55 8B EC 81 EC ? ? ? ? 80 3D ? ? ? ? ? 56 0F 85" );

	clean_inventory_image_cache_dir ( this );
}

void econitem_set_or_add_attribute_by_name ( econitem* this, float val, const char* attribute_name ) {
	static void* set_or_add_attribute_by_name = NULL;
	
	if ( !set_or_add_attribute_by_name )
		set_or_add_attribute_by_name = pattern_search ( "client.dll", "55 8B EC 83 EC 30 53 56 8B F1 F3" );

	__asm {
		mov ecx, this
		movss xmm2, val
		push attribute_name
		call set_or_add_attribute_by_name
	}
}

void econitem_set_custom_name ( econitem* this, const char* name ) {
	static econitem* ( __thiscall * set_custom_name_fn )( econitem*, const char* ) = NULL;

	if (!set_custom_name_fn)
		set_custom_name_fn = pattern_rip( pattern_search ( "client.dll", "E8 ? ? ? ? 8B 46 78 C1 E8 0A A8 01 74 13 8B 46 34" ) );

	set_custom_name_fn ( this, name );
}

void econitem_set_custom_desc ( econitem* this, const char* name ) {
	static econitem* ( __thiscall* set_custom_desc_fn )( econitem*, const char* ) = NULL;

	if ( !set_custom_desc_fn )
		set_custom_desc_fn = pattern_rip ( pattern_search ( "client.dll", "E8 ? ? ? ? 33 DB 39 5E 3C 7E 5E" ) );

	set_custom_desc_fn ( this, name );
}

void econitem_set_attributei ( econitem* this, int index, int val ) {
	static void* ( *get_item_schema )( ) = NULL;
	static int ( __thiscall * set_dynamic_attribute_val )( econitem*, uint32_t, void* ) = NULL;

	if ( !get_item_schema )
		get_item_schema = pattern_search ( "client.dll", "A1 ? ? ? ? 85 C0 75 53" );

	if ( !set_dynamic_attribute_val )
		set_dynamic_attribute_val = pattern_search ( "client.dll", "55 8B EC 83 E4 F8 83 EC 3C 53 8B 5D 08 56 57 6A 00" );

	uint32_t* v15 = ( uint32_t* )( get_item_schema ( ) );
	uint32_t v16 = *( uint32_t* ) ( v15 [ 72 ] + 4 * index );

	set_dynamic_attribute_val ( this, v16, &val );
}

void econitem_set_attributef ( econitem* this, int index, float val ) {
	econitem_set_attributei ( this, index, *( int* ) &val );
}

econitem* econitem_soc_data ( econitem* this ) {
	static econitem* ( __thiscall* get_soc_data )( econitem* ) = NULL;

	if ( !get_soc_data )
		get_soc_data = pattern_search ( "client.dll", "55 8B EC 83 E4 F0 83 EC 18 56 8B F1 57 8B 86" );

	return get_soc_data ( this );
}

weapon_info* weapon_data( weapon* this ) {
	static void* weapon_system = NULL;

	if ( !weapon_system )
		weapon_system = *( void** ) ( pattern_search ( "client.dll", "8B 35 ? ? ? ? FF 10 0F B7 C0" ) + 2 );

	return (( weapon_info * ( __thiscall* )( void*, weapon_idx ) )utils_vfunc( weapon_system, 2 ))( weapon_system, weapon_item_def_idx ( this ) );
}

weapon* weapon_world_mdl ( weapon* this ) {
	return ientlist_get_entity_from_handle ( cs_ientlist, weapon_world_mdl_handle( this ) );
}