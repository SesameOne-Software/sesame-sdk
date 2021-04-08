#include "sdk.h"

static void __fastcall event_listener_destructor ( event_listener* this, void* edx ) {
	ievents_remove_listener ( cs_ievents, this );
}

static int __fastcall event_listener_get_event_debug_id ( event_listener* this, void* edx ) {
	return this->debug_id;
}

event_listener* event_listener_init ( event_listener* this, void ( __fastcall* callback )( event_listener*, void*, event* ) ) {
	this->destructor = event_listener_destructor;
	this->fire_game_event = callback;
	this->get_event_debug_id = event_listener_get_event_debug_id;
	this->debug_id = 1337;
}