#ifndef SDK_ENGINE_H
#define SDK_ENGINE_H

#include "include/utils.h"
#include "mat3x4.h"

typedef struct {
	PAD ( 8 );

	union {
		uint64_t steam_id_64;

		struct {
			uint32_t xuid_low;
			uint32_t xuid_high;
		};
	};

	char name [ 128 ];
	uint32_t user_id;
	char guid [ 33 ];
	uint32_t steam_id;
	char friends_name [ 128 ];
	bool fake_player;
	bool is_hltv;
	uint32_t custom_files [ 4 ];
	uint8_t files_downloaded;
} player_info;

typedef struct nci nci;

VIRTUAL ( nci, const char*, name, 0, (  ) );
VIRTUAL ( nci, const char*, address, 1, (  ) );
VIRTUAL ( nci, float, time, 2, ( ) );
VIRTUAL ( nci, float, time_connected, 3, ( ) );
VIRTUAL ( nci, int, buffer_size, 4, ( ) );
VIRTUAL ( nci, int, data_rate, 5, (  ) );
VIRTUAL ( nci, bool, is_loopback, 6, (  ) );
VIRTUAL ( nci, bool, is_timing_out, 7, (  ) );
VIRTUAL ( nci, bool, is_playback, 8, (  ) );
VIRTUAL ( nci, float, latency, 9, (  flow ), int flow );
VIRTUAL ( nci, float, avg_latency, 10, (  flow ), int flow );

typedef struct {
	PAD ( 20 );
	bool processing_messages;
	bool should_delete;
	PAD ( 2 );
	int out_seq_nr;
	int in_seq_nr;
	int out_seq_nr_ack;
	int out_rel_state_count;
	int in_rel_state_count;
	int choked;
	PAD ( 1044 );
} nc;

typedef struct iclientstate iclientstate;

static inline nc* iclientstate_get_nc ( iclientstate* this ) {
	return *( nc** ) ( ( uintptr_t ) this + 0x9C );
}

static inline int* iclientstate_get_choked ( iclientstate* this ) {
	return ( int* ) ( ( uintptr_t ) this + 0x4D30 );
}

static inline int* iclientstate_get_last_cmd_ack ( iclientstate* this ) {
	return ( int* ) ( ( uintptr_t ) this + 0x4D34 );
}

static inline int* iclientstate_get_last_out_cmd ( iclientstate* this ) {
	return ( int* ) ( ( uintptr_t ) this + 0x4D2C );
}

static inline int* iclientstate_get_server_tick ( iclientstate* this ) {
	return ( int* ) ( ( uintptr_t ) this + 0x164 );
}

static inline int* iclientstate_get_delta_tick ( iclientstate* this ) {
	return ( int* ) ( ( uintptr_t ) this + 0x174 );
}

static inline float* iclientstate_get_next_cmd_time ( iclientstate* this ) {
	return ( float* ) ( ( uintptr_t ) this + 0x114 );
}

static inline int* iclientstate_get_out_seq_num ( iclientstate* this ) {
	return ( int* ) ( ( uintptr_t ) this + 0x4D24 );
}

typedef struct iengine iengine;

VIRTUAL ( iengine, void, get_screen_size, 5, (  w, h ), int* w, int* h );
VIRTUAL ( iengine, void, server_cmd, 6, (  cmd, reliable ), const char* cmd, bool reliable );
VIRTUAL ( iengine, void, client_cmd, 7, (  cmd ), const char* cmd );
VIRTUAL ( iengine, bool, get_player_info, 8, (  idx, pinfo ), int idx, player_info* pinfo );
VIRTUAL ( iengine, int, get_local_player, 12, (  ) );
VIRTUAL ( iengine, void, get_angles, 18, (  ang ), vec3* ang );
VIRTUAL ( iengine, void, set_angles, 19, (  ang ), vec3* ang );
VIRTUAL ( iengine, bool, is_in_game, 26, (  ) );
VIRTUAL ( iengine, bool, is_connected, 27, (  ) );
VIRTUAL ( iengine, nci*, get_nci, 78, (  ) );
VIRTUAL ( iengine, void, execute_cmd, 108, (  cmd ), const char* cmd );

#endif // !SDK_ENGINE_H