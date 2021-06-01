#ifndef SDK_MAT3X4_H
#define SDK_MAT3X4_H

#include "vec3.h"

#include "utils.h"

typedef struct {
	float values[ 3 ][ 4 ];
} mat3x4;

typedef struct __attribute__( ( aligned( 16 ) ) ) {
	float values[ 3 ][ 4 ];
} mat3x4a;

#define MAT3X4_GET(mat3x4)\
static inline float* mat3x4##_at ( mat3x4* this, int row, int col ) {\
	return &(this->values [ row ][ col ]);\
}

#define MAT3X4_INIT(mat3x4)\
static inline mat3x4* mat3x4##_init ( mat3x4* this ) {\
	memset ( this, 0, sizeof ( *this ) );\
	return this;\
}

#define MAT3X4_SET_ORIGIN(mat3x4)\
static inline mat3x4* mat3x4##_set_origin ( mat3x4* this, vec3* src ) {\
	*mat3x4##_at(this, 0, 3) = src->x;\
	*mat3x4##_at(this, 1, 3) = src->y;\
	*mat3x4##_at(this, 2, 3) = src->z;\
	return this;\
}

#define MAT3X4_GET_ORIGIN(mat3x4)\
static inline vec3* mat3x4##_get_origin ( mat3x4* this, vec3* src ) {\
	src->x = *mat3x4##_at(this, 0, 3);\
	src->y = *mat3x4##_at(this, 1, 3);\
	src->z = *mat3x4##_at(this, 2, 3);\
	return src;\
}

#define MAT3X4_EQUALS(mat3x4)\
static inline bool mat3x4##_equals ( mat3x4* this, mat3x4* src ) {\
	for (int i = 0; i < sizeof(this->values) / sizeof(float); i++)\
		if ( this->values [ i ] != src->values [ i ] )\
			return false;\
	return true;\
}

MAT3X4_INIT( mat3x4 )
MAT3X4_GET( mat3x4 )
MAT3X4_SET_ORIGIN( mat3x4 )
MAT3X4_GET_ORIGIN( mat3x4 )
MAT3X4_EQUALS( mat3x4 )

MAT3X4_INIT( mat3x4a )
MAT3X4_GET( mat3x4a )
MAT3X4_SET_ORIGIN( mat3x4a )
MAT3X4_GET_ORIGIN( mat3x4a )
MAT3X4_EQUALS( mat3x4a )

#endif // !SDK_MAT3X4_H