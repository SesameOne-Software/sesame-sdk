#ifndef SDK_VEC3_H
#define SDK_VEC3_H

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

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
	return vec3_init ( this );\
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
	return vec3##_len( &( vec3 ) { .x = this->x - to->x, .y = this->y - to->y, .z = this->z - to->z } );\
}

#define VEC3_DIST_SQR(vec3)\
static inline float vec3##_dist_sqr ( vec3* this, vec3* to ) {\
	return vec3##_len_sqr ( &( vec3 ) { .x = this->x - to->x, .y = this->y - to->y, .z = this->z - to->z } );\
}

#define VEC3_DOT(vec3)\
static inline float vec3##_dot ( vec3* this, vec3* src ) {\
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

#endif // !SDK_VEC3_H