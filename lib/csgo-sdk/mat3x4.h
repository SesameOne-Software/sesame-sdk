#ifndef SDK_MAT3X4_H
#define SDK_MAT3X4_H

#include "vec3.h"

#include "include/utils.h"

typedef struct {
	float values [ 3 ][ 4 ];
} mat3x4;

typedef struct __declspec( align( 16 ) ) {
	float values [ 3 ][ 4 ];
} mat3x4a;

#define MAT3X4_INIT(mat3x4)\
static inline mat3x4* mat3x4##_init ( mat3x4* this ) {\
	memset ( this, 0, sizeof ( *this ) );\
	return this;\
}

#define MAT3X4_SET_ORIGIN(mat3x4)\
static inline mat3x4* mat3x4_set_origin ( mat3x4* this, vec3* src ) {\
	this->values [ 0 ][ 3 ] = src->x;\
	this->values [ 1 ][ 3 ] = src->y;\
	this->values [ 2 ][ 3 ] = src->z;\
	return this;\
}

#define MAT3X4_GET_ORIGIN(mat3x4)\
static inline mat3x4* mat3x4_get_origin ( mat3x4* this, vec3* src ) {\
	src->x = this->values [ 0 ][ 3 ];\
	src->y = this->values [ 1 ][ 3 ];\
	src->z = this->values [ 2 ][ 3 ];\
	return this;\
}

#define MAT3X4_EQUALS(mat3x4)\
static inline bool mat3x4_equals ( mat3x4* this, mat3x4* src ) {\
	for (int i = 0; i < sizeof(this->values) / sizeof(float); i++)\
		if ( this->values [ i ] != src->values [ i ] )\
			return false;\
	return true;\
}

MAT3X4_INIT ( mat3x4 )
MAT3X4_SET_ORIGIN ( mat3x4 )
MAT3X4_GET_ORIGIN ( mat3x4 )
MAT3X4_EQUALS ( mat3x4 )

MAT3X4_INIT ( mat3x4a )
MAT3X4_SET_ORIGIN ( mat3x4a )
MAT3X4_GET_ORIGIN ( mat3x4a )
MAT3X4_EQUALS ( mat3x4a )

#endif // !SDK_MAT3X4_H