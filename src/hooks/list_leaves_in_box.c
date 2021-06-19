#include "hooks/hooks.h"

__attribute__( ( thiscall ) ) int hooks_list_leaves_in_box( iengine* this, const vec3* mins, const vec3* maxs, const uint16_t* list, int list_max ) {
	typedef __attribute__( ( thiscall ) ) int( *hooks_list_leaves_in_box_fn )( iengine* this, const vec3* mins, const vec3* maxs, const uint16_t* list, int list_max );

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
		PAD( 4 );
	} render_info;

	if ( __builtin_return_address( 0 ) != cs_offsets.list_leaves_in_box_ret )
		return ( ( hooks_list_leaves_in_box_fn )subhook_get_trampoline( hooks_subhooks[ subhook_list_leaves_in_box ] ) )( this, mins, maxs, list, list_max );

	render_info* info = *( render_info** )( ( uintptr_t )__builtin_frame_address( 0 ) + 0x14 );

	if ( !info || !info->renderable )
		return ( ( hooks_list_leaves_in_box_fn )subhook_get_trampoline( hooks_subhooks[ subhook_list_leaves_in_box ] ) )( this, mins, maxs, list, list_max );

	void* client_unk = ( ( __attribute__( ( thiscall ) ) void* ( * )( renderable* ) )utils_vfunc( info->renderable, 0 ) )( info->renderable );

	if ( !client_unk )
		return ( ( hooks_list_leaves_in_box_fn )subhook_get_trampoline( hooks_subhooks[ subhook_list_leaves_in_box ] ) )( this, mins, maxs, list, list_max );

	entity* ent = ( ( __attribute__( ( thiscall ) ) entity * ( * )( void* ) )utils_vfunc( client_unk, 7 ) )( client_unk );

	if ( !ent )
		return ( ( hooks_list_leaves_in_box_fn )subhook_get_trampoline( hooks_subhooks[ subhook_list_leaves_in_box ] ) )( this, mins, maxs, list, list_max );

	client_class* cc = entity_get_client_class( ent );

	if ( !cc || ( cc->class_id != 40 && cc->class_id != 42 ) )
		return ( ( hooks_list_leaves_in_box_fn )subhook_get_trampoline( hooks_subhooks[ subhook_list_leaves_in_box ] ) )( this, mins, maxs, list, list_max );

	info->flags &= ~0x100;
	info->flags2 |= 0x40;

	return ( ( hooks_list_leaves_in_box_fn )subhook_get_trampoline( hooks_subhooks[ subhook_list_leaves_in_box ] ) )( this, &( vec3 ) { -16384.0f, -16384.0f, -16384.0f }, & ( vec3 ) { 16384.0f, 16384.0f, 16384.0f }, list, list_max );
}
