#ifndef SDK_EVENTS_H
#define SDK_EVENTS_H

#include "utils.h"

typedef struct event event;

VIRTUAL( event, const char*, get_name, cs_idx_event_get_name, ( this ) );
VIRTUAL( event, bool, is_empty, cs_idx_event_is_empty, ( this, key ), const char* key );
VIRTUAL( event, bool, get_bool, cs_idx_event_get_bool, ( this, key, val ), const char* key, bool val );
VIRTUAL( event, int32_t, get_int, cs_idx_event_get_int, ( this, key, val ), const char* key, int32_t val );
VIRTUAL( event, int64_t, get_int64, cs_idx_event_get_int64, ( this, key, val ), const char* key, int64_t val );
VIRTUAL( event, float, get_float, cs_idx_event_get_float, ( this, key, val ), const char* key, float val );
VIRTUAL( event, const char*, get_string, cs_idx_event_get_string, ( this, key, val ), const char* key, const char* val );
VIRTUAL( event, const wchar_t*, get_wstring, cs_idx_event_get_wstring, ( this, key, val ), const char* key, const wchar_t* val );
VIRTUAL( event, void, set_bool, cs_idx_event_set_bool, ( this, key, val ), const char* key, bool val );
VIRTUAL( event, void, set_int, cs_idx_event_set_int, ( this, key, val ), const char* key, int32_t val );
VIRTUAL( event, void, set_int64, cs_idx_event_set_int64, ( this, key, val ), const char* key, int64_t val );
VIRTUAL( event, void, set_float, cs_idx_event_set_float, ( this, key, val ), const char* key, float val );
VIRTUAL( event, void, set_string, cs_idx_event_set_string, ( this, key, val ), const char* key, const char* val );
VIRTUAL( event, void, set_wstring, cs_idx_event_set_wstring, ( this, key, val ), const char* key, const wchar_t* val );

typedef struct event_listener event_listener;

struct event_listener {
	__attribute__( ( thiscall ) ) void( *destructor )( event_listener* );
	__attribute__( ( thiscall ) ) void( *fire_game_event )( event_listener*, event* );
	__attribute__( ( thiscall ) ) int( *get_event_debug_id )( event_listener* );
	int debug_id;
};

event_listener* event_listener_init( event_listener* this, __attribute__( ( thiscall ) ) void( *callback )( event_listener*, event* ) );

typedef struct ievents ievents;

VIRTUAL( ievents, bool, add_listener, cs_idx_ievents_add_listener, ( this, listener, name, server_side ), event_listener* listener, const char* name, bool server_side );
VIRTUAL( ievents, bool, find_listener, cs_idx_ievents_find_listener, ( this, listener, name ), event_listener* listener, const char* name );
VIRTUAL( ievents, int, remove_listener, cs_idx_ievents_remove_listener, ( this, listener ), event_listener* listener );

#endif // !SDK_EVENTS_H