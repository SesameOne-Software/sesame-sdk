#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <windows.h>
#include <stdint.h>
#include <stddef.h>

#include "lib/vector/vector.h"
#include "lib/hashmap/hashmap.h"
#include "lib/str/str.h"
#include "lib/yyjson/yyjson.h"

/* utility macros */
#define clamp(x, a, b) max(a, min(x, b))

/* from google's chromium project */
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

#define REG void* ecx, void* edx
#define REG_OUT ecx, edx

/* helpers for creation of variables and functions */
#define COMBINE2( x, y ) x##y
#define COMBINE( x, y ) COMBINE2( x, y )

#define PAD( sz ) \
	uint8_t COMBINE( pad_, __COUNTER__ )[ sz ]; \

#define VIRTUAL( _struct, type, name, idx, _args, ... ) \
static inline type COMBINE( COMBINE(_struct, _), name ) ( _struct* this, __VA_ARGS__ ) { \
	return ((##type##(__fastcall*)(_struct*, void*, __VA_ARGS__))utils_vfunc ( this, idx ))##_args##; \
}

/* generic utility functions */
static inline void utils_sleep ( unsigned long ms ) {
	Sleep ( ms );
}

static inline void* utils_vfunc ( void* _this, int idx ) {
	return ( *( void*** ) _this ) [ idx ];
}

/* pattern scanner */
typedef uintptr_t pattern;

pattern pattern_search ( const char* mod, const char* pat );

static inline pattern pattern_rip ( pattern addr ) {
	return addr + *( int* ) ( addr + 1 ) + 5;
}

#endif // !UTILS_H