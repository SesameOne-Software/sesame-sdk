#ifndef SDK_MDLINFO_H
#define SDK_MDLINFO_H

#include "vec3.h"
#include "quat.h"
#include "mat3x4.h"

#include "include/utils.h"

typedef struct {
	int name_idx;
	int parent;
	int bone_controller [ 6 ];
	vec3 pos;
	quat q;
	vec3 rot;
	vec3 pos_scale;
	vec3 rot_scale;
	mat3x4 pose_to_bone;
	quat alignment;
	int flags;
	int proc_type;
	int proc_idx;
	int physics_bone;
	int surface_prop_idx;
	int contents;
	PAD( 32 );
} mstudiobone;

typedef struct {
	int bone;
	int group;
	vec3 bbmin;
	vec3 bbmax;
	int hitbox_name_idx;
	PAD( 12 );
	float radius;
	PAD( 16 );
} mstudiobbox;

typedef struct {
	int name_idx;
	int num_hitboxes;
	int hitbox_idx;
} mstudiohitboxset;

static inline mstudiobbox* mstudiohitboxset_hitbox ( mstudiohitboxset* this, int i ) {
	if ( i > this->num_hitboxes )
		return NULL;

	return ( mstudiobbox* )( ( uintptr_t ) this + this->hitbox_idx ) + i;
}

typedef struct studiohdr studiohdr;

struct studiohdr {
	int id;
	int version;
	long checksum;
	char name [ 64 ];
	int length;
	vec3 eye_pos;
	vec3 illumination_pos;
	vec3 hull_min;
	vec3 hull_max;
	vec3 min;
	vec3 max;
	int flags;
	int num_bones;
	int bone_idx;
	int num_bone_controllers;
	int bone_controller_idx;
	int num_hitbox_sets;
	int hitbox_set_idx;
	int num_local_anim;
	int local_anim_idx;
	int num_local_seq;
	int local_seq_idx;
	int activity_list_version;
	int events_indexed;
	int num_textures;
	int texture_idx;
};

static inline mstudiohitboxset* studiohdr_hitbox_set ( studiohdr* this, int i ) {
	if ( i > this->num_hitbox_sets )
		return NULL;

	return ( mstudiohitboxset* ) ( ( uintptr_t ) this + this->hitbox_set_idx ) + i;
}

static inline mstudiobone* studiohdr_bone ( studiohdr* this, int i ) {
	if ( i > this->num_bones )
		return NULL;

	return ( mstudiobone* ) ( ( uintptr_t ) this + this->bone_idx ) + i;
}

static inline void* studiohdr_find_mapping ( studiohdr* this ) {
	return ( ( void*(__fastcall*)( studiohdr*, void* ))cs_offsets.studiohdr_findmapping ) (this, NULL);
}

static inline void* studiohdr_seq_desc ( studiohdr* this, int seq ) {
	return ( ( void* ( __fastcall* )( studiohdr*, void*, int ) )cs_offsets.studiohdr_seq_desc ) ( this, NULL, seq );
}

typedef struct mdl mdl;
typedef struct imdlinfo imdlinfo;

VIRTUAL ( imdlinfo, mdl*, get_mdl, cs_idx_imdlinfo_get_mdl, ( idx ), uint32_t idx );
VIRTUAL ( imdlinfo, uint32_t, get_mdl_idx, cs_idx_imdlinfo_get_mdl_idx, ( name ), const char* name );
VIRTUAL ( imdlinfo, const char*, get_mdl_name, cs_idx_imdlinfo_get_mdl_name, ( pmdl ), mdl* pmdl );
VIRTUAL ( imdlinfo, studiohdr*, get_studio_mdl, cs_idx_imdlinfo_get_studio_mdl, ( pmdl ), mdl* pmdl );

#endif // !SDK_MDLINFO_H