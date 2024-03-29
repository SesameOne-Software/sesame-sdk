#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <windows.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>

#include "vector/vector.h"
#include "hashmap/hashmap.h"
#include "sds/sds.h"
#include "yyjson/yyjson.h"

#include "ThemidaSDK/Include/C/ThemidaSDK.h"
#include "csgo-sdk/offsets.h"

/* utility macros */
#define _CLAMP_TYPE(type, extension)\
static inline type clamp##extension(type x, type a, type b) {\
	return max(a, min(x, b));\
}

_CLAMP_TYPE(int, i)
_CLAMP_TYPE(long, l)
_CLAMP_TYPE(float, f)
_CLAMP_TYPE(double, d)

#define clamp(x, a, b) _Generic((x), \
	int: clampi, \
	long: clampl, \
	float: clampf, \
    double: clampd, \
	default: clampf \
)(x, a, b)

#define _LERP_TYPE(type, extension)\
static inline type lerp##extension(type a, type b, float t) {\
	return a + (type)((float)( b - a ) * t);\
}

_LERP_TYPE(int, i)
_LERP_TYPE(long, l)
_LERP_TYPE(float, f)
_LERP_TYPE(double, d)

#define lerp(x, a, b) _Generic((x), \
	int: lerpi, \
	long: lerpl, \
	float: lerpf, \
    double: lerpd, \
	default: lerpf \
)(x, a, b)

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
static inline type COMBINE( COMBINE(_struct, _), name ) ( _struct* this __VA_OPT__(, __VA_ARGS__) ) { \
    __attribute__( ( thiscall ) ) type(* fn)(_struct* this __VA_OPT__(, __VA_ARGS__)) = utils_vfunc(this, idx);\
    return fn _args; \
}

#define for_each(iter, list) \
for ( typeof(*list)* iter = list; iter < list + COUNT_OF(list); iter++ )

#define MAKE_STATIC(var, val) \
 NULL; \
static bool init_##var = false; \
if ( !init_##var ) { \
    var = val; \
    init_##var = true; \
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
static void utils_print_console( uint8_t* clr, sds msg ) {
	static __attribute__( ( cdecl ) ) void( *console_color_msg_fn )( uint8_t*, const char*, ... ) = NULL;

	if ( !console_color_msg_fn )
		console_color_msg_fn = ( __attribute__( ( cdecl ) ) void( * )( uint8_t*, const char*, ... ) )GetProcAddress( GetModuleHandleA( "tier0.dll" ), "?ConColorMsg@@YAXABVColor@@PBDZZ" );

	console_color_msg_fn( clr, "[Sesame-SDK] %s", msg );
	sdsfree( msg );
}

typedef enum {
	keybind_mode_hold,
	keybind_mode_toggle,
	keybind_mode_always,
	keybind_mode_disabled
} keybind_mode;

extern bool mouse_down[ 5 ];
extern bool key_down[ 512 ];
extern bool key_toggled[ 512 ];
extern bool last_key_toggled[ 512 ];

static inline bool utils_key_state( int vkey ) {
	switch ( vkey ) {
	case VK_LBUTTON: return mouse_down[ 0 ];
	case VK_RBUTTON: return mouse_down[ 1 ];
	case VK_MBUTTON: return mouse_down[ 2 ];
	case VK_XBUTTON1: return mouse_down[ 3 ];
	case VK_XBUTTON2: return mouse_down[ 4 ];
	}

	return key_down[ vkey ];
}

static inline bool utils_last_key_state( int vkey ) {
	return last_key_toggled[ vkey ];
}

static inline void utils_update_key_states( ) {
	for ( int i = 0; i < 512; i++ ) {
		if ( !utils_key_state( i ) && utils_last_key_state( i ) )
			key_toggled[ i ] = !key_toggled[ i ];

		last_key_toggled[ i ] = utils_key_state( i );
	}
}

static inline bool utils_keybind_active( int vkey, keybind_mode mode ) {
	switch ( mode ) {
	case keybind_mode_hold: return vkey && utils_key_state( vkey ); break;
	case keybind_mode_toggle: return vkey && key_toggled[ vkey ]; break;
	case keybind_mode_always: return true; break;
	}

	return false;
}

#endif // !UTILS_H
