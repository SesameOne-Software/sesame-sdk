#include "hooks/hooks.h"

typedef struct {
	vec3 origin;
	vec3 start;
	vec3 normal;
	vec3 angles;
	uint32_t flags;
	ehandle ent_handle;
	float scale;
	float magnitude;
	float radius;
	int attachment_idx;
	uint16_t surface_prop;
	mat* material;
	int damage_type;
	hitboxes hitbox;
	int	other_ent_idx;
	uint8_t color;
	bool positions_are_relative_to_entity;
	const char* effect_name;
} effect_data;

__attribute__( ( cdecl ) ) void hooks_cs_blood_spray_callback( const void* pdata ) {
	typedef __attribute__( ( cdecl ) ) void( *hooks_cs_blood_spray_callback_fn )( const void* pdata );

	const effect_data* data = ( effect_data* )pdata;

	( ( hooks_cs_blood_spray_callback_fn )subhook_get_trampoline( hooks_subhooks[ subhook_cs_blood_spray_callback ] ) )( pdata );
}