#ifndef SDK_BEAMS_H
#define SDK_BEAMS_H

#include "vec3.h"
#include "entity.h"
#include "trace.h"
#include "offsets.h"

#include "utils.h"

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

static inline void beam_info_init( beam_info* this ) {
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

VIRTUAL( ibeams, void, init, cs_idx_ibeams_init, ( this ) );
VIRTUAL( ibeams, void, shutdown, cs_idx_ibeams_shutdown, ( this ) );
VIRTUAL( ibeams, void, clear, cs_idx_ibeams_clear, ( this ) );
VIRTUAL( ibeams, void, draw, cs_idx_ibeams_draw, ( this, pbeam ), beam* pbeam );
VIRTUAL( ibeams, beam*, create_points, cs_idx_ibeams_create_points, ( this, pbeam_info ), beam_info* pbeam_info );

#endif // !SDK_BEAMS_H