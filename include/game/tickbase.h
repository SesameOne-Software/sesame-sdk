#include "csgo-sdk/sdk.h"

typedef usercmd* vec_cmds;

/* for queuing and processing usercmds */
typedef struct {
    vec_cmds cmds;
    int num_cmds;
    int total_cmds;
    int dropped_packets;
    bool paused;
} cmd_ctx;

/* info about last 20 or so updates */
typedef struct {
    float time;
    int num_cmds;
    int dropped_packets;
} player_cmd_info;

static inline void player_cmd_info_init( player_cmd_info* this ) {
    this->time = 0.0f;
    this->num_cmds = 0;
    this->dropped_packets = 0;
}

typedef struct {
    float time;
    int num_cmds;
    int ticks_corrected;
    float final_simtime;
    float game_simtime;
    float server_frametime;
    vec3 abs_origin;
} player_sim_info;

static inline void player_sim_info_init( player_sim_info* this ) {
    this->time =0.0f;
    this->num_cmds = 0;
    this->ticks_corrected = 0;
    this->final_simtime = 0.0f;
    this->game_simtime = 0.0f;
    this->server_frametime = 0.0f;

    vec3_zero( &this->abs_origin );
}

typedef player_cmd_info* vec_player_cmd_info;
typedef player_sim_info* vec_player_sim_info;

int tickbase_get_dropped_cmds( void );
int tickbase_determine_sim_ticks( void );
void tickbase_adjust_player_time_base( player* player, int simulation_ticks );
