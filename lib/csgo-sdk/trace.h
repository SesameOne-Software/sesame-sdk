#ifndef SDK_TRACE_H
#define SDK_TRACE_H

#include "mat3x4.h"
#include "entity.h"

typedef enum {
	contents_empty = 0,
	contents_solid = 0x1,
	contents_window = 0x2,
	contents_aux = 0x4,
	contents_grate = 0x8,
	contents_slime = 0x10,
	contents_water = 0x20,
	contents_blocklos = 0x40,
	contents_opaque = 0x80,
	last_visible_contents = contents_opaque,
	all_visible_contents = ( last_visible_contents | ( last_visible_contents - 1 ) ),
	contents_testfogvolume = 0x100,
	contents_unused = 0x200,
	contents_blocklight = 0x400,
	contents_team1 = 0x800,
	contents_team2 = 0x1000,
	contents_ignore_nodraw_opaque = 0x2000,
	contents_moveable = 0x4000,
	contents_areaportal = 0x8000,
	contents_playerclip = 0x10000,
	contents_monsterclip = 0x20000,
	contents_current_0 = 0x40000,
	contents_current_90 = 0x80000,
	contents_current_180 = 0x100000,
	contents_current_270 = 0x200000,
	contents_current_up = 0x400000,
	contents_current_down = 0x800000,
	contents_origin = 0x1000000,
	contents_monster = 0x2000000,
	contents_debris = 0x4000000,
	contents_detail = 0x8000000,
	contents_translucent = 0x10000000,
	contents_ladder = 0x20000000,
	contents_hitbox = 0x40000000
} trace_contents;

typedef enum {
	surf_light = 0x0001,
	surf_sky2d = 0x0002,
	surf_sky = 0x0004,
	surf_warp = 0x0008,
	surf_trans = 0x0010,
	surf_noportal = 0x0020,
	surf_trigger = 0x0040,
	surf_nodraw = 0x0080,
	surf_hint = 0x0100,
	surf_skip = 0x0200,
	surf_nolight = 0x0400,
	surf_bumplight = 0x0800,
	surf_noshadows = 0x1000,
	surf_nodecals = 0x2000,
	surf_nopaint = surf_nodecals,
	surf_nochop = 0x4000,
	surf_hitbox = 0x8000
} surf;

typedef enum {
	mask_all = ( 0xFFFFFFFF ),
	mask_solid = ( contents_solid | contents_moveable | contents_window | contents_monster | contents_grate ),
	mask_playersolid = ( contents_solid | contents_moveable | contents_playerclip | contents_window | contents_monster | contents_grate ),
	mask_npcsolid = ( contents_solid | contents_moveable | contents_monsterclip | contents_window | contents_monster | contents_grate ),
	mask_npcfluid = ( contents_solid | contents_moveable | contents_monsterclip | contents_window | contents_monster ),
	mask_water = ( contents_water | contents_moveable | contents_slime ),
	mask_opaque = ( contents_solid | contents_moveable | contents_opaque ),
	mask_opaque_and_npcs = ( mask_opaque | contents_monster ),
	mask_blocklos = ( contents_solid | contents_moveable | contents_blocklos ),
	mask_blocklos_and_npcs = ( mask_blocklos | contents_monster ),
	mask_visible = ( mask_opaque | contents_ignore_nodraw_opaque ),
	mask_visible_and_npcs = ( mask_opaque_and_npcs | contents_ignore_nodraw_opaque ),
	mask_shot = ( contents_solid | contents_moveable | contents_monster | contents_window | contents_debris | contents_hitbox ),
	mask_shot_brushonly = ( contents_solid | contents_moveable | contents_window | contents_debris ),
	mask_shot_hull = ( contents_solid | contents_moveable | contents_monster | contents_window | contents_debris | contents_grate ),
	mask_shot_portal = ( contents_solid | contents_moveable | contents_window | contents_monster ),
	mask_solid_brushonly = ( contents_solid | contents_moveable | contents_window | contents_grate ),
	mask_playersolid_brushonly = ( contents_solid | contents_moveable | contents_window | contents_playerclip | contents_grate ),
	mask_npcsolid_brushonly = ( contents_solid | contents_moveable | contents_window | contents_monsterclip | contents_grate ),
	mask_npcworldstatic = ( contents_solid | contents_window | contents_monsterclip | contents_grate ),
	mask_npcworldstatic_fluid = ( contents_solid | contents_window | contents_monsterclip ),
	mask_splitareaportal = ( contents_water | contents_slime ),
	mask_current = ( contents_current_0 | contents_current_90 | contents_current_180 | contents_current_270 | contents_current_up | contents_current_down ),
	mask_deadsolid = ( contents_solid | contents_playerclip | contents_window | contents_grate )
} trace_mask;

typedef enum {
	hitgroup_generic = 0,
	hitgroup_head = 1,
	hitgroup_chest = 2,
	hitgroup_stomach = 3,
	hitgroup_leftarm = 4,
	hitgroup_rightarm = 5,
	hitgroup_leftleg = 6,
	hitgroup_rightleg = 7,
	hitgroup_gear = 10
} hitgroups;

typedef enum {
	dispsurf_flag_surface = ( 1 << 0 ),
	dispsurf_flag_walkable = ( 1 << 1 ),
	dispsurf_flag_buildable = ( 1 << 2 ),
	dispsurf_flag_surfprop1 = ( 1 << 3 ),
	dispsurf_flag_surfprop2 = ( 1 << 4 )
} dispsurf;

typedef enum {
	trace_everything = 0,
	trace_world_only,
	trace_entities_only,
	trace_everything_filter_props
} trace_type;

typedef struct {
	vec3 norm;
	float dist;
	uint8_t type;
	uint8_t sign_bits;
	PAD( 2 );
} trace_plane;

typedef struct {
	const char* name;
	short surface_props;
	uint8_t flags;
} trace_surface;

typedef struct {
	vec3 start_pos;
	vec3 end_pos;
	trace_plane plane;
	float fraction;
	trace_contents contents;
	uint16_t disp_flags;
	bool all_solid;
	bool start_solid;
} base_trace;

typedef struct {
	base_trace base;
	float fraction_left_solid;
	trace_surface surface;
	int hitgroup;
	short physics_bone;
	uint16_t world_surface_index;
	entity* hit_entity;
	int hitbox;
} trace;

static inline bool trace_did_hit( trace* this ) {
	return ( ( base_trace* )this )->fraction < 1.0f || ( ( base_trace* )this )->all_solid || ( ( base_trace* )this )->start_solid;
}

static inline bool trace_is_visible( trace* this ) {
	return ( ( base_trace* )this )->fraction > 0.97f;
}

typedef struct itrace_filter itrace_filter;

struct itrace_filter {
	__attribute__( ( thiscall ) ) bool ( *should_hit_entity ) ( itrace_filter*, entity*, int );
	__attribute__( ( thiscall ) ) trace_type( *get_trace_type ) ( itrace_filter*, entity*, int );
};

typedef struct {
	itrace_filter base;
	void* skip;
	const char* ignore;
} trace_filter;

__attribute__( ( thiscall ) ) static bool trace_filter_should_hit_entity_override( trace_filter* this, entity* ent, int contents_mask ) {
	client_class* ent_cc = entity_get_client_class( ent );

	if ( ent_cc && strcmp( this->ignore, "" ) ) {
		if ( ent_cc->network_name == this->ignore )
			return false;
	}

	return !( ent == this->skip );
}

__attribute__( ( thiscall ) ) static trace_type trace_filter_get_trace_type_override( trace_filter* this ) {
	return trace_everything;
}

static inline void trace_filter_set_ignore_class( trace_filter* this, char* cc ) {
	this->ignore = cc;
}

static inline trace_filter* trace_filter_init( trace_filter* this, entity* to_skip ) {
	( ( itrace_filter* ) this )->should_hit_entity = trace_filter_should_hit_entity_override;
	( ( itrace_filter* ) this )->get_trace_type = trace_filter_get_trace_type_override;
	this->skip = to_skip;
	return this;
}

typedef struct {
	vec3a start;
	vec3a delta;
	vec3a start_offset;
	vec3a extents;
	mat3x4* world_axis_transform;
	bool is_ray;
	bool is_swept;
} ray;

static inline ray* ray_init( ray* ray, vec3* src, vec3* dst ) {
	const vec3a src_aligned = { src->x, src->y, src->z };
	const vec3a dst_aligned = { dst->x, dst->y, dst->z };

	ray->delta = dst_aligned;
	vec3a_sub( &ray->delta, &src_aligned );
	ray->is_swept = vec3a_len_sqr( &ray->delta ) != 0.0f;
	vec3a_init( &ray->extents );
	ray->world_axis_transform = NULL;
	ray->is_ray = true;
	vec3a_init( &ray->start_offset );
	ray->start = src_aligned;

	return ray;
}

static inline ray* ray_bbox_init( ray* ray, vec3* src, vec3* dst, vec3* mins, vec3* maxs ) {
	const vec3a src_aligned = { src->x, src->y, src->z };
	const vec3a dst_aligned = { dst->x, dst->y, dst->z };
	const vec3a mins_aligned = { mins->x, mins->y, mins->z };
	const vec3a maxs_aligned = { maxs->x, maxs->y, maxs->z };

	ray->delta = dst_aligned;
	vec3a_sub( &ray->delta, &src_aligned );
	ray->world_axis_transform = NULL;
	ray->is_swept = vec3a_len( &ray->delta ) != 0.0f;

	ray->extents = maxs_aligned;
	vec3a_sub( &ray->extents, &mins_aligned );
	vec3a_mulf( &ray->extents, 0.5f );
	ray->is_ray = vec3a_len_sqr( &ray->extents ) < 1e-6f;

	ray->start_offset = maxs_aligned;
	vec3a_add( &ray->start_offset, &mins_aligned );
	vec3a_mulf( &ray->start_offset, 0.5f );
	ray->start = src_aligned;
	vec3a_add( &ray->start, &ray->start_offset );
	vec3a_mulf( &ray->start_offset, -1.0f );

	return ray;
}

typedef struct itrace itrace;

VIRTUAL( itrace, int, get_point_contents, cs_idx_itrace_get_point_contents, ( this, pos, mask, ent ), vec3* pos, trace_mask mask, entity* ent );
VIRTUAL( itrace, int, get_point_contents_world_only, 1, ( this, pos, mask ), vec3* pos, trace_mask mask );
VIRTUAL( itrace, int, get_point_contents_collideable, 2, ( this, collideable, pos ), void* collideable, vec3* pos );
VIRTUAL( itrace, void, clip_ray_to_entity, 3, ( this, r, mask, ent, tr ), ray* r, trace_mask mask, entity* ent, trace* tr );
VIRTUAL( itrace, void, clip_ray_to_collideable, 4, ( this, r, mask, collideable, tr ), ray* r, trace_mask mask, void* collideable, trace* tr );
VIRTUAL( itrace, void, trace_ray, 5, ( this, r, mask, filter, tr ), ray* r, trace_mask mask, trace_filter* filter, trace* tr );

#endif // !SDK_TRACE_H
