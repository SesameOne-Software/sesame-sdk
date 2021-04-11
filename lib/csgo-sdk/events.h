#ifndef SDK_EVENTS_H
#define SDK_EVENTS_H

#include "include/utils.h"

typedef struct event event;

VIRTUAL ( event, const char*, get_name, 1, ( ) );
VIRTUAL ( event, bool, is_empty, 4, ( key ), const char* key );
VIRTUAL ( event, bool, get_bool, 5, ( key, val ), const char* key, bool val );
VIRTUAL ( event, int32_t, get_int, 6, ( key, val ), const char* key, int32_t val );
VIRTUAL ( event, int64_t, get_uint64, 7, (  key, val ), const char* key, int64_t val );
VIRTUAL ( event, float, get_float, 8, (  key, val ), const char* key, float val );
VIRTUAL ( event, const char*, get_string, 9, (  key, val ), const char* key, const char* val );
VIRTUAL ( event, const wchar_t*, get_wstring, 10, (  key, val ), const char* key, const wchar_t* val );
VIRTUAL ( event, void, set_bool, 11, (  key, val ), const char* key, bool val );
VIRTUAL ( event, void, set_int, 12, (  key, val ), const char* key, int32_t val );
VIRTUAL ( event, void, set_uint64, 13, (  key, val ), const char* key, int64_t val );
VIRTUAL ( event, void, set_float, 14, (  key, val ), const char* key, float val );
VIRTUAL ( event, void, set_string, 15, (  key, val ), const char* key, const char* val );
VIRTUAL ( event, void, set_wstring, 16, (  key, val ), const char* key, const wchar_t* val );

typedef struct event_listener event_listener;

struct event_listener {
	void ( __fastcall* destructor )( event_listener*, void* );
	void( __fastcall* fire_game_event )(event_listener*, void*, event*);
	int( __fastcall* get_event_debug_id )(event_listener*, void*);
	int debug_id;
};

event_listener* event_listener_init ( event_listener* this, void ( __fastcall* callback )( event_listener*, void*, event* ) );

typedef struct ievents ievents;

VIRTUAL ( ievents, bool, add_listener, 3, (  listener, name, server_side ), event_listener* listener, const char* name, bool server_side );
VIRTUAL ( ievents, bool, find_listener, 4, (  listener, name ), event_listener* listener, const char* name );
VIRTUAL ( ievents, int, remove_listener, 5, (  listener ), event_listener* listener );

#endif // !SDK_EVENTS_H