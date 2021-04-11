#ifndef SDK_BEAMS_H
#define SDK_BEAMS_H

#include "vec3.h"
#include "entity.h"
#include "trace.h"

#include "include/utils.h"

typedef struct beam beam;

typedef struct {
	int type;
	entity* start_ent;
	int start_attachment;
	entity* end_ent;
	int end_attachment;
	vec3 start;
	vec3 end;
	int model_idx;
	const char* model_name;
	int halo_idx;
	const char* halo_name;
	float halo_scale;
	float life;
	float width;
	float end_width;
	float fade_len;
	float amplitude;
	float brightness;
	float speed;
	int start_frame;
	float frame_rate;
	float red;
	float green;
	float blue;
	bool renderable;
	int segments;
	int flags;
	vec3 center;
	float start_radius;
	float end_radius;
} beam_info;

static inline void beam_info_init ( beam_info* this ) {
	this->type = 0;
	this->segments = -1;
	this->model_name = NULL;
	this->halo_name = NULL;
	this->model_idx = -1;
	this->halo_idx = -1;
	this->renderable = true;
	this->flags = 0;
}

typedef struct ibeams ibeams;

VIRTUAL ( ibeams, void, init, 1, ( ) );
VIRTUAL ( ibeams, void, shutdown, 2, ( ) );
VIRTUAL ( ibeams, void, clear, 3, ( ) );
VIRTUAL ( ibeams, void, draw, 5, ( pbeam ), beam* pbeam );
VIRTUAL ( ibeams, beam*, create_points, 13, ( pbeam_info ), beam_info* pbeam_info );

#endif // !SDK_BEAMS_H