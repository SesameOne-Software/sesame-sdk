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

#include "lib/ThemidaSDK/Include/C/ThemidaSDK.h"
#include "lib/csgo-sdk/offsets.h"

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

static inline vec3* lerpvec3 ( float t, vec3* a, vec3* b, vec3* out ) {
	*out = *b;
	return vec3_add ( vec3_mulf ( vec3_sub ( out, a ), t ), a );
}

static inline vec3a* lerpvec3a ( float t, vec3a* a, vec3a* b, vec3a* out ) {
	*out = *b;
	return vec3a_add ( vec3a_mulf ( vec3a_sub ( out, a ), t ), a );
}

static inline float lerpf ( float t, float a, float b ) {
	return a + ( b - a ) * t;
}

static inline float clampf( float x, float a, float b ) {
        return max( a, min( x, b ) );
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

/* debug console */
static void utils_print_console ( uint8_t* clr, sds msg ) {
    static void ( __cdecl* console_color_msg_fn )( uint8_t*, const char*, ... ) = NULL;

    if (!console_color_msg_fn )
        console_color_msg_fn = ( void ( __cdecl* )( uint8_t*, const char*, ... ) )GetProcAddress ( GetModuleHandleA ( "tier0.dll" ), "?ConColorMsg@@YAXABVColor@@PBDZZ" );

    console_color_msg_fn ( clr, "[Sesame-SDK] %s", msg );
    sdsfree ( msg );
}

typedef enum {
	keybind_mode_hold,
	keybind_mode_toggle,
	keybind_mode_always,
	keybind_mode_disabled
} keybind_mode;

extern bool mouse_down [ 5 ];
extern bool key_down [ 512 ];
extern bool key_toggled [ 512 ];
extern bool last_key_toggled [ 512 ];

static inline bool utils_key_state ( int vkey ) {
	switch ( vkey ) {
	case VK_LBUTTON: return mouse_down [ 0 ];
	case VK_RBUTTON: return mouse_down [ 1 ];
	case VK_MBUTTON: return mouse_down [ 2 ];
	case VK_XBUTTON1: return mouse_down [ 3 ];
	case VK_XBUTTON2: return mouse_down [ 4 ];
	}

	return key_down [ vkey ];
}

static inline bool utils_last_key_state ( int vkey ) {
	return last_key_toggled [ vkey ];
}

static inline void utils_update_key_states ( ) {
	for ( auto i = 0; i < 512; i++ ) {
		if ( !utils_key_state ( i ) && utils_last_key_state ( i ) )
			key_toggled [ i ] = !key_toggled [ i ];

		last_key_toggled [ i ] = utils_key_state ( i );
	}
}

static inline bool utils_keybind_active ( int vkey, keybind_mode mode ) {
	switch ( mode ) {
	case keybind_mode_hold: return vkey && utils_key_state ( vkey ); break;
	case keybind_mode_toggle: return vkey && key_toggled [ vkey ]; break;
	case keybind_mode_always: return true; break;
	}

	return false;
}

#endif // !UTILS_H