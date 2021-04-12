#ifndef SDK_RENDERVIEW_H
#define SDK_RENDERVIEW_H

#include "mat3x4.h"

#include "include/utils.h"

typedef struct {
	vec3 origin;
	vec3 angles;
	PAD( 4 );
	void* renderable;
	void* model;
	mat3x4* model_to_world;
	mat3x4* lighting_offset;
	vec3* lighting_origin;
	int flags;
	int entity_index;
	int skin;
	int body;
	int hitbox_set;
	uint16_t instance;
} mdlrender_info;

typedef struct irenderview irenderview;

VIRTUAL ( irenderview, void, set_alpha, cs_idx_irenderview_set_alpha, ( a ), float a );
VIRTUAL ( irenderview, void, set_color, cs_idx_irenderview_set_color, ( c ), float* c );

#endif // !SDK_RENDERVIEW_H