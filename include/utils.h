#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <windows.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>

#include "lib/vector/vector.h"
#include "lib/hashmap/hashmap.h"
#include "lib/sds/sds.h"
#include "lib/yyjson/yyjson.h"

/* utility macros */
//#define _CLAMP_TYPE(type, extension)\
//static inline type clamp##extension(type x, type a, type b) {\
//	return max(a, min(x, b));\
//}
//_CLAMP_TYPE(int, i)
//_CLAMP_TYPE(long, l)
//_CLAMP_TYPE(float, f)
//_CLAMP_TYPE(double, d)
//
//#define clamp(x, a, b) _Generic((x), \
//	int: clampi, \
//	long: clampl, \
//	float: clampf, \
//	double: clampd, \
//	default: clampf \
//)(x, a, b)

static inline float clampf( float x, float a, float b ) {    
\
        return max( a, min( x, b ) );\
}

/* from google's chromium project */
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

#define REG void* ecx, void* edx
#define REG_OUT ecx, edx

/* helpers for creation of variables and functions */
#define COMBINE2( x, y ) x##y
#define COMBINE( x, y ) COMBINE2( x, y )

#define PAD( sz ) \
	uint8_t COMBINE( pad_, __COUNTER__ )[ sz ] \

/* emulate the functionality of a function with __thiscall calling convention (located within c++ classes) */
#define VIRTUAL( _struct, type, name, idx, _args, ... ) \
static inline type COMBINE( COMBINE(_struct, _), name ) ( _struct* this, __VA_ARGS__ ) { \
    type(__stdcall* volatile fn)(__VA_ARGS__) = NULL;\
    __asm {push idx}\
    __asm {push this}\
    __asm {call utils_vfunc}\
    __asm {add esp, 8}\
    __asm {mov fn, eax}\
    __asm {mov ecx, this}\
    return fn##_args##; \
}

/* generic utility functions */
static inline void utils_sleep( unsigned long ms ) {
    Sleep( ms );
}

static inline void* utils_vfunc( void* this, int idx ) {
    return ( *( void*** )this )[ idx ];
}

/* pattern scanner */
typedef uintptr_t pattern;

pattern pattern_search( const char* mod, const char* pat );

static inline pattern pattern_rip( pattern addr ) {
    return addr + *( int* )( addr + 1 ) + 5;
}

#endif // !UTILS_H