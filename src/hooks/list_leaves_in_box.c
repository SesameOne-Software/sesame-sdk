#include "include/hooks/hooks.h"

#include <intrin.h>

int __fastcall hooks_list_leaves_in_box ( REG, const vec3* mins, const vec3* maxs, const uint16_t* list, int list_max ) {
	typedef int ( __fastcall* hooks_list_leaves_in_box_fn )( REG, const vec3* mins, const vec3* maxs, const uint16_t* list, int list_max );

	typedef struct {
		renderable* renderable;
		void* alpha_property;
		int enum_count;
		int render_frame;
		uint16_t first_shadow;
		uint16_t leaf_list;
		uint16_t area;
		uint16_t flags;
		uint16_t flags2;
		vec3 bloated_abs_mins;
		vec3 bloated_abs_maxs;
		vec3 abs_mins;
		vec3 abs_maxs;
		PAD ( 4 );
	} render_info;

	if ( _ReturnAddress ( ) != cs_offsets.list_leaves_in_box_ret )
		return ( ( hooks_list_leaves_in_box_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_list_leaves_in_box ] ) )( REG_OUT, mins, maxs, list, list_max );

	render_info* info = *( render_info** ) ( ( uintptr_t ) _AddressOfReturnAddress ( ) + 0x14 );

	if ( !info || !info->renderable )
		return ( ( hooks_list_leaves_in_box_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_list_leaves_in_box ] ) )( REG_OUT, mins, maxs, list, list_max );
	
	void* client_unk = ( ( void* ( __fastcall* )( renderable*, void* ) )utils_vfunc ( info->renderable, 0 ) )( info->renderable, NULL );

	if ( !client_unk )
		return ( ( hooks_list_leaves_in_box_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_list_leaves_in_box ] ) )( REG_OUT, mins, maxs, list, list_max );

	entity* ent = ( ( entity* ( __fastcall* )( void*, void* ) )utils_vfunc ( client_unk, 7 ) )( client_unk, NULL );
	
	if ( !ent )
		return ( ( hooks_list_leaves_in_box_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_list_leaves_in_box ] ) )( REG_OUT, mins, maxs, list, list_max );

	client_class* cc = entity_get_client_class( ent );

	if ( !cc || ( cc->class_id != 40 && cc->class_id != 42 ) )
		return ( ( hooks_list_leaves_in_box_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_list_leaves_in_box ] ) )( REG_OUT, mins, maxs, list, list_max );

	info->flags &= ~0x100;
	info->flags2 |= 0x40;
	
	return ( ( hooks_list_leaves_in_box_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_list_leaves_in_box ] ) )( REG_OUT, &( vec3 ) { -16384.0f, -16384.0f, -16384.0f }, & ( vec3 ) { 16384.0f, 16384.0f, 16384.0f }, list, list_max );
}