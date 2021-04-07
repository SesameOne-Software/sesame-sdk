#ifndef SDK_INPUT_H
#define SDK_INPUT_H

#include "vec3.h"
#include "usercmd.h"

#include "include/utils.h"

typedef struct {
	PAD ( 12 );
	bool track_ir_available;
	bool mouse_initialized;
	bool mouse_active;
	bool joystick_advanced_init;
	PAD ( 44 );
	void* keys;
	PAD ( 108 );
	bool camera_intercepting_mouse;
	bool camera_in_thirdperson;
	bool camera_moving_with_mouse;
	vec3 camera_offset;
	bool camera_distance_move;
	int camera_old_x;
	int camera_old_y;
	int camera_x;
	int camera_y;
	bool camera_is_orthographic;
	vec3 last_viewangles;
	vec3 last_viewangles_tilt;
	float last_forward_move;
	int clear_input_state;
	usercmd* cmds;
	verified_usercmd* verified_cmds;
} iinput;

usercmd* iinput_get_cmd ( iinput* this, int sequence_number ) {
	return &this->cmds [ sequence_number % MULTIPLAYER_BACKUP ];
}

verified_usercmd* iinput_get_verified_cmd ( iinput* this, int sequence_number ) {
	return &this->verified_cmds [ sequence_number % MULTIPLAYER_BACKUP ];
}

#endif // !SDK_INPUT_H