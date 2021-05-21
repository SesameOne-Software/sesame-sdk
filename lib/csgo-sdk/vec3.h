#ifndef SDK_VEC3_H
#define SDK_VEC3_H

#include "include/utils.h"

static inline float cs_deg2rad( float deg ) {
	return deg * M_PI / 180.0f;
}

static inline float cs_rad2deg( float rad ) {
	return rad * 180.0f / M_PI;
}

static inline void cs_sincos( float rad, float* sin_out, float* cos_out ) {
	*sin_out = sinf( rad );
	*cos_out = cosf( rad );
}

static inline float cs_norm_rotation( float ang ) {
	if ( isnan ( ang ) || isinf ( ang ) )
		ang = 0.0f;

	ang = fmodf ( ang, 360.0f );

	if ( ang > 180.0f )
		ang -= 360.0f;
	
	if ( ang < -180.0f )
		ang += 360.0f;

	return ang;
}

static inline float cs_approachf ( float target, float value, float speed ) {
	float delta = target - value;

	if ( delta > speed )
		value += speed;
	else if ( delta < -speed )
		value -= speed;
	else
		value = target;

	return value;
}

static inline float cs_remap_val_clamped ( float val, float a, float b, float c, float d ) {
	if ( a == b )
		return val >= b ? d : c;
	
	return d + ( d - c ) * clampf ( ( val - a ) / ( b - a ), 0.0f, 1.0f );
}

static inline float cs_anglemod ( float a ) {
	return ( 360.0f / 65536 ) * ( ( int ) ( a * ( 65536.0f / 360.0f ) ) & 65535 );
}

static inline float cs_clamp_cycle(float x) {
	x -= (int)x;

	if (x < 0.0f)
		x += 1.0f;
	else if (x > 1.0f)
		x -= 1.0f;

	return x;
}

static inline float cs_approach_rotation ( float target, float value, float speed ) {
	target = cs_anglemod ( target );
	value = cs_anglemod ( value );

	float delta = target - value;

	if ( speed < 0.0f )
		speed = -speed;

	if ( delta < -180.0f )
		delta += 360.0f;
	else if ( delta > 180.0f )
		delta -= 360.0f;

	if ( delta > speed )
		value += speed;
	else if ( delta < -speed )
		value -= speed;
	else
		value = target;

	return value;
}

static inline float cs_angle_diff ( float dst, float src ) {
	float delta = fmodf ( dst - src, 360.0f );

	if ( dst > src ) {
		if ( delta >= 180.0f )
			delta -= 360.0f;
	}
	else {
		if ( delta <= -180.0f )
			delta += 360.0f;
	}

	return delta;
}

static inline float cs_bias ( float x, float amount ) {
	static float last_exponent = 0.0f;

	if ( amount != -1.0f )
		last_exponent = logf ( amount ) * -1.4427f;

	return powf ( x, last_exponent );
}

static inline float cs_gain ( float x, float amount ) {
	if ( x < 0.5f )
		return 0.5f * cs_bias ( 2.0f * x, 1.0f - amount );
	
	return 1.0f - 0.5f * cs_bias ( 2.0f - 2.0f * x, 1.0f - amount );
}

typedef struct {
	float x, y, z;
} vec3;

typedef struct __declspec( align( 16 ) ) {
	float x, y, z;
} vec3a;

#define VEC3_INIT(vec3)\
static inline vec3* vec3##_init ( vec3* this ) {\
	this->x = this->y = this->z = 0.0f;\
	return this;\
}

#define VEC3_ZERO(vec3)\
static inline vec3* vec3##_zero ( vec3* this ) {\
	return vec3##_init ( this );\
}

#define VEC3_IS_VALID(vec3)\
static inline bool vec3##_is_valid ( vec3* this ) {\
	return isfinite ( this->x ) && isfinite ( this->y ) && isfinite ( this->z );\
}

#define VEC3_IS_ZERO(vec3)\
static inline bool vec3##_is_zero ( vec3* this ) {\
	return !this->x && !this->y && !this->z;\
}

#define VEC3_EQUALS(vec3)\
static inline bool vec3##_equals ( vec3* this, vec3* src ) {\
	return this->x == src->x && this->y == src->y && this->z == src->z;\
}

#define VEC3_ADD(vec3)\
static inline vec3* vec3##_add ( vec3* this, vec3* src ) {\
	this->x += src->x;\
	this->y += src->y;\
	this->z += src->z;\
	return this;\
}

#define VEC3_ADDF(vec3)\
static inline vec3* vec3##_addf ( vec3* this, float src ) {\
	this->x += src;\
	this->y += src;\
	this->z += src;\
	return this;\
}

#define VEC3_SUB(vec3)\
static inline vec3* vec3##_sub ( vec3* this, vec3* src ) {\
	this->x -= src->x;\
	this->y -= src->y;\
	this->z -= src->z;\
	return this;\
}

#define VEC3_SUBF(vec3)\
static inline vec3* vec3##_subf ( vec3* this, float src ) {\
	this->x -= src;\
	this->y -= src;\
	this->z -= src;\
	return this;\
}

#define VEC3_MUL(vec3)\
static inline vec3* vec3##_mul ( vec3* this, vec3* src ) {\
	this->x *= src->x;\
	this->y *= src->y;\
	this->z *= src->z;\
	return this;\
}

#define VEC3_MULF(vec3)\
static inline vec3* vec3##_mulf ( vec3* this, float src ) {\
	this->x *= src;\
	this->y *= src;\
	this->z *= src;\
	return this;\
}

#define VEC3_DIV(vec3)\
static inline vec3* vec3##_div ( vec3* this, vec3* src ) {\
	this->x /= src->x;\
	this->y /= src->y;\
	this->z /= src->z;\
	return this;\
}

#define VEC3_DIVF(vec3)\
static inline vec3* vec3##_divf ( vec3* this, float src ) {\
	this->x /= src;\
	this->y /= src;\
	this->z /= src;\
	return this;\
}

#define VEC3_LEN(vec3)\
static inline float vec3##_len ( vec3* this ) {\
	return sqrtf( this->x * this->x + this->y * this->y + this->z * this->z );\
}

#define VEC3_LEN_SQR(vec3)\
static inline float vec3##_len_sqr ( vec3* this ) {\
	return this->x * this->x + this->y * this->y + this->z * this->z;\
}

#define VEC3_LEN2D(vec3)\
static inline float vec3##_len2d ( vec3* this ) {\
	return sqrtf ( this->x * this->x + this->y * this->y );\
}

#define VEC3_LEN2D_SQR(vec3)\
static inline float vec3##_len2d_sqr ( vec3* this ) {\
	return this->x * this->x + this->y * this->y;\
}

#define VEC3_NORM(vec3)\
static inline vec3* vec3##_norm ( vec3* this ) {\
	const float len = vec3##_len( this );\
\
	if ( len != 0.0f )\
		return vec3##_divf ( this, len );\
\
	return vec3##_zero ( this );\
}

#define VEC3_DIST(vec3)\
static inline float vec3##_dist ( vec3* this, vec3* to ) {\
	return vec3##_len( &( vec3 ) { this->x - to->x, this->y - to->y, this->z - to->z } );\
}

#define VEC3_DIST_SQR(vec3)\
static inline float vec3##_dist_sqr ( vec3* this, vec3* to ) {\
	return vec3##_len_sqr ( &( vec3 ) { this->x - to->x, this->y - to->y, this->z - to->z } );\
}

#define VEC3_DOT(vec3)\
static inline float vec3##_dot ( const vec3* this, const vec3* src ) {\
	return this->x * src->x + this->y * src->y + this->z * src->z;\
}

#define VEC3_CROSS(vec3)\
static inline vec3* vec3##_cross ( vec3* this, vec3* src ) {\
	vec3 backup_this = *this;\
	this->x = backup_this.y * src->z - backup_this.z * src->y;\
	this->y = backup_this.z * src->x - backup_this.x * src->z;\
	this->z = backup_this.x * src->y - backup_this.y * src->x;\
	return this;\
}

#define VEC3_CLAMP_ANGLE(vec3)\
static inline vec3* vec3##_clamp_angle ( vec3* this ) {\
	this->x = cs_norm_rotation( this->x );\
	this->y = cs_norm_rotation( this->y );\
	this->x = clampf( this->x, -89.0f, 89.0f );\
	this->y = clampf( this->y, -180.0f, 180.0f );\
	this->z = 0.0f;\
	return this;\
}

#define VEC3_TO_ANGLE(vec3)\
static inline vec3* vec3##_to_angle( vec3* this ) {\
	if ( !this->x && !this->y ) {\
		this->x = ( this->z > 0.0f ) ? 270.0f : 90.0f;\
		this->y = 0.0f;\
	}\
	else {\
		this->x = cs_rad2deg( atan2f( -this->z, vec3##_len2d(this) ) );\
		this->y = cs_rad2deg( atan2f( this->y, this->x ) );\
\
		if ( this->y < 0.0f )\
			this->y += 360.0f;\
		if ( this->x < 0.0f )\
			this->x += 360.0f;\
	}\
\
	this->z = 0.0f;\
	vec3##_clamp_angle( this );\
\
	return this;\
}

#define VEC3_TO_VEC(vec3)\
static inline vec3* vec3##_to_vec( vec3* this ) {\
	vec3##_clamp_angle( this );\
\
	float sp, sy, cp, cy;\
\
	cs_sincos( cs_deg2rad( this->y ), &sy, &cy );\
	cs_sincos( cs_deg2rad( this->x ), &sp, &cp );\
\
	this->x = cp * cy;\
	this->y = cp * sy;\
	this->z = -sp;\
\
	return this;\
}

#define VEC3_TO_VECS(vec3)\
static inline void vec3##_to_vecs ( const vec3* this, vec3* forward, vec3* right, vec3* up ) {\
	float sr, sp, sy, cr, cp, cy;\
\
	cs_sincos ( cs_deg2rad ( this->y ), &sy, &cy );\
	cs_sincos ( cs_deg2rad ( this->x ), &sp, &cp );\
	cs_sincos ( cs_deg2rad ( this->z ), &sr, &cr );\
\
	if ( forward ) {\
		forward->x = cp * cy;\
		forward->y = cp * sy;\
		forward->z = -sp;\
	}\
\
	if ( right ) {\
		right->x = -1.0f * sr * sp * cy + -1.0f * cr * -sy;\
		right->y = -1.0f * sr * sp * sy + -1.0f * cr * cy;\
		right->z = -1.0f * sr * cp;\
	}\
\
	if ( up ) {\
		up->x = cr * sp * cy + -sr * -sy;\
		up->y = cr * sp * sy + -sr * cy;\
		up->z = cr * cp;\
	}\
}

#define VEC3_APPROACH_VEC(vec3)\
void vec3##_approach_vec ( const vec3* a, const vec3* b, float rate, vec3* out ) {\
	vec3 delta = *a;\
	vec3##_sub ( &delta, b );\
\
	const float delta_len = vec3##_len(&delta );\
	const float rad = 1.0f / ( delta_len + FLT_EPSILON );\
\
	*out = *b;\
\
	if ( delta_len <= rate ) {\
		if ( -rate <= delta_len )\
			*out = *a;\
		else\
			vec3##_sub ( out, vec3##_mulf ( &delta, rad * rate ) );\
	}\
	else {\
		vec3##_add ( out, vec3##_mulf ( &delta, rad * rate ) );\
	}\
}

#define VEC3_ANGLE_TO(vec3)\
static inline vec3* vec3##_angle_to( vec3* this, vec3* towards ) {\
	vec3 delta = *towards;\
	*this = *vec3##_to_angle( vec3##_sub(&delta, this) );\
	vec3##_clamp_angle(this);\
	return this;\
}

#define VEC3_FOV_TO(vec3)\
static inline float vec3##_fov_to( vec3* this, vec3* angs_towards ) {\
	vec3 fwd_vec = *this, aim_vec = *angs_towards;\
	vec3##_to_vec(&fwd_vec);\
	vec3##_to_vec(&aim_vec);\
	return cs_rad2deg( acosf( vec3##_dot(&fwd_vec, &aim_vec) / vec3##_len_sqr(&fwd_vec) ) );\
}

VEC3_INIT ( vec3 )
VEC3_ZERO ( vec3 )
VEC3_IS_VALID ( vec3 )
VEC3_IS_ZERO ( vec3 )
VEC3_EQUALS ( vec3 )
VEC3_ADD ( vec3 )
VEC3_ADDF ( vec3 )
VEC3_SUB ( vec3 )
VEC3_SUBF ( vec3 )
VEC3_MUL ( vec3 )
VEC3_MULF ( vec3 )
VEC3_DIV ( vec3 )
VEC3_DIVF ( vec3 )
VEC3_LEN ( vec3 )
VEC3_LEN_SQR ( vec3 )
VEC3_LEN2D ( vec3 )
VEC3_LEN2D_SQR ( vec3 )
VEC3_NORM ( vec3 )
VEC3_DIST ( vec3 )
VEC3_DIST_SQR ( vec3 )
VEC3_DOT ( vec3 )
VEC3_CROSS ( vec3 )
VEC3_CLAMP_ANGLE ( vec3 )
VEC3_TO_ANGLE(vec3)
VEC3_TO_VEC ( vec3 )
VEC3_TO_VECS ( vec3 )
VEC3_APPROACH_VEC ( vec3 )
VEC3_ANGLE_TO(vec3)
VEC3_FOV_TO(vec3)

VEC3_INIT ( vec3a )
VEC3_ZERO ( vec3a )
VEC3_IS_VALID ( vec3a )
VEC3_IS_ZERO ( vec3a )
VEC3_EQUALS ( vec3a )
VEC3_ADD ( vec3a )
VEC3_ADDF ( vec3a )
VEC3_SUB ( vec3a )
VEC3_SUBF ( vec3a )
VEC3_MUL ( vec3a )
VEC3_MULF ( vec3a )
VEC3_DIV ( vec3a )
VEC3_DIVF ( vec3a )
VEC3_LEN ( vec3a )
VEC3_LEN_SQR ( vec3a )
VEC3_LEN2D ( vec3a )
VEC3_LEN2D_SQR ( vec3a )
VEC3_NORM ( vec3a )
VEC3_DIST ( vec3a )
VEC3_DIST_SQR ( vec3a )
VEC3_DOT ( vec3a )
VEC3_CROSS ( vec3a )
VEC3_CLAMP_ANGLE ( vec3a )
VEC3_TO_ANGLE(vec3a)
VEC3_TO_VEC ( vec3a )
VEC3_TO_VECS ( vec3a )
VEC3_APPROACH_VEC ( vec3a )
VEC3_ANGLE_TO(vec3a)
VEC3_FOV_TO(vec3a)

#endif // !SDK_VEC3_H