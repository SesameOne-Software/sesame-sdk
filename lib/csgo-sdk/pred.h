#ifndef SDK_PRED_H
#define SDK_PRED_H

#include "vec3.h"
#include "usercmd.h"
#include "player.h"

#include "include/utils.h"

typedef struct imovehelper imovehelper;

VIRTUAL(imovehelper, void, set_host, 1, (host ), void* host);

typedef struct {
	PAD( 8 );
	bool in_prediction; //0x8
	PAD( 1 );
	bool engine_paused; //0xA
	PAD( 1 );
	int previous_start_frame; //0xC
	PAD( 8 );
	bool is_first_time_predicted; //0x18
	PAD ( 3 );
	int cmds_predicted; //0x1C
} ipred;

typedef struct imove imove;

VIRTUAL(ipred, void, get_local_viewangles, 12, ( angle ), vec3* angle);
VIRTUAL(ipred, void, set_local_viewangles, 13, ( angle ), vec3* angle);
VIRTUAL(ipred, void, update, 3, ( start_frame, valid_frame, inc_ack, out_cmd ), int start_frame, bool valid_frame, int inc_ack, int out_cmd);
VIRTUAL(ipred, void, check_moving_ground, 18, ( ent, frametime ), player* ent, double frametime);
VIRTUAL(ipred, void, setup_move, 20, ( ent, cmd, helper, movedata ), player* ent, usercmd* cmd, imove* helper, void* movedata);
VIRTUAL(ipred, void, finish_move, 21, ( ent, cmd, movedata ), player* ent, usercmd* cmd, void* movedata);

VIRTUAL(imove, void, process_movement, 1, ( ent, movedata ), player* ent, void* movedata);
VIRTUAL(imove, void, reset, 2, ( ));
VIRTUAL(imove, void, start_track_pred_errors, 3, ( ent ), player* ent);
VIRTUAL(imove, void, finish_track_pred_errors, 4, ( ent ), player* ent);

#endif // !SDK_PRED_H