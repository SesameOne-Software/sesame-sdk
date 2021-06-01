#include "sdk.h"

__attribute__( ( thiscall ) ) static void event_listener_destructor( event_listener* this ) {
	ievents_remove_listener( cs_ievents, this );
}

__attribute__( ( thiscall ) ) static int event_listener_get_event_debug_id( event_listener* this ) {
	return this->debug_id;
}

event_listener* event_listener_init( event_listener* this, __attribute__( ( thiscall ) ) void( *callback )( event_listener*, event* ) ) {
	this->destructor = event_listener_destructor;
	this->fire_game_event = callback;
	this->get_event_debug_id = event_listener_get_event_debug_id;
	this->debug_id = 1337;
}