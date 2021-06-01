#ifndef SDK_CVAR_H
#define SDK_CVAR_H

#include "utils.h"

typedef enum {
	fcvar_none = 0,
	fcvar_unregistered = ( 1 << 0 ),
	fcvar_developmentonly = ( 1 << 1 ),
	fcvar_gamedll = ( 1 << 2 ),
	fcvar_clientdll = ( 1 << 3 ),
	fcvar_hidden = ( 1 << 4 ),
	fcvar_protected = ( 1 << 5 ),
	fcvar_sponly = ( 1 << 6 ),
	fcvar_archive = ( 1 << 7 ),
	fcvar_notify = ( 1 << 8 ),
	fcvar_userinfo = ( 1 << 9 ),
	fcvar_cheat = ( 1 << 14 ),
	fcvar_printableonly = ( 1 << 10 ),
	fcvar_unlogged = ( 1 << 11 ),
	fcvar_never_as_string = ( 1 << 12 ),
	fcvar_replicated = ( 1 << 13 ),
	fcvar_demo = ( 1 << 16 ),
	fcvar_dontrecord = ( 1 << 17 ),
	fcvar_not_connected = ( 1 << 22 ),
	fcvar_archive_xbox = ( 1 << 24 ),
	fcvar_server_can_execute = ( 1 << 28 ),
	fcvar_server_cannot_query = ( 1 << 29 ),
	fcvar_clientcmd_can_execute = ( 1 << 30 )
} fcvar;

typedef struct {
	const char* str;
	int str_len;
	float f;
	int i;
} cvar_val;

typedef struct cvar cvar;

struct cvar {
	PAD( 4 );
	cvar* next;
	int registered;
	const char* name;
	const char* help_string;
	int flags;
	void* callback;
	cvar* parent;
	const char* default_value;
	cvar_val value;
	int has_min;
	float min;
	int has_max;
	float max;
	void* callbacks;
};

static inline void cvar_get_string( cvar* this ) {
	if ( this->flags & fcvar_never_as_string )
		return "FCVAR_NEVER_AS_STRING";

	return this->value.str ? this->value.str : "";
}

static inline float cvar_get_float( cvar* this ) {
	const uint32_t xored = *( uint32_t* )&this->value.f ^ ( uint32_t )this;
	return *( float* )&xored;
}

static inline int cvar_get_int( cvar* this ) {
	return ( int )( this->value.i ^ ( uint32_t )this );
}

static inline bool cvar_get_bool( cvar* this ) {
	return cvar_get_int( this );
}

static inline void cvar_null_cb( cvar* this ) {
	*( int* )( ( uintptr_t )&this->callbacks + 0xC ) = 0;
}

VIRTUAL( cvar, void, set_valuesz, cs_idx_cvar_set_valuesz, ( this, value ), const char* value );
VIRTUAL( cvar, void, set_valuef, cs_idx_cvar_set_valuef, ( this, value ), float value );
VIRTUAL( cvar, void, set_valuei, cs_idx_cvar_set_valuei, ( this, value ), int value );

typedef struct icvar icvar;

VIRTUAL( icvar, cvar*, find, cs_idx_icvar_find, ( this, name ), const char* name );

#endif // !SDK_CVAR_H