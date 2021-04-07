#ifndef SDK_EVENTS_H
#define SDK_EVENTS_H

#include "include/utils.h"

typedef struct event event;

VIRTUAL ( event, const char*, get_name, 1, ( this ) );
VIRTUAL ( event, bool, is_empty, 4, ( this, key ), const char* key );
VIRTUAL ( event, bool, get_bool, 5, ( this, key, val ), const char* key, bool val );
VIRTUAL ( event, int32_t, get_int, 6, ( this, key, val ), const char* key, int32_t val );
VIRTUAL ( event, int64_t, get_uint64, 7, ( this, key, val ), const char* key, int64_t val );
VIRTUAL ( event, float, get_float, 8, ( this, key, val ), const char* key, float val );
VIRTUAL ( event, const char*, get_string, 9, ( this, key, val ), const char* key, const char* val );
VIRTUAL ( event, const wchar_t*, get_wstring, 10, ( this, key, val ), const char* key, const wchar_t* val );
VIRTUAL ( event, void, set_bool, 11, ( this, key, val ), const char* key, bool val );
VIRTUAL ( event, void, set_int, 12, ( this, key, val ), const char* key, int32_t val );
VIRTUAL ( event, void, set_uint64, 13, ( this, key, val ), const char* key, int64_t val );
VIRTUAL ( event, void, set_float, 14, ( this, key, val ), const char* key, float val );
VIRTUAL ( event, void, set_string, 15, ( this, key, val ), const char* key, const char* val );
VIRTUAL ( event, void, set_wstring, 16, ( this, key, val ), const char* key, const wchar_t* val );

typedef struct event_listener event_listener;

typedef struct {
	void ( __thiscall* destructor )( event_listener* );
	void ( __thiscall* fire_game_event )( event_listener*, event* );
	int ( __thiscall* get_event_debug_id )( event_listener* );
	int debug_id;
} event_listener;

event_listener* event_listener_init ( event_listener* this, void ( __thiscall* callback )( event_listener*, event* ) );

typedef struct ievents ievents;

VIRTUAL ( ievents, bool, add_listener, 3, ( this, listener, name, server_side ), event_listener* listener, const char* name, bool server_side );
VIRTUAL ( ievents, bool, find_listener, 4, ( this, listener, name ), event_listener* listener, const char* name );
VIRTUAL ( ievents, int, remove_listener, 5, ( this, listener ), event_listener* listener );

#endif // !SDK_EVENTS_H