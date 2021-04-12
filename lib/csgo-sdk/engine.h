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

VIRTUAL ( nci, const char*, name, cs_idx_nci_name, (  ) );
VIRTUAL ( nci, const char*, address, cs_idx_nci_address, (  ) );
VIRTUAL ( nci, float, time, cs_idx_nci_time, ( ) );
VIRTUAL ( nci, float, time_connected, cs_idx_nci_time_connected, ( ) );
VIRTUAL ( nci, int, buffer_size, cs_idx_nci_buffer_size, ( ) );
VIRTUAL ( nci, int, data_rate, cs_idx_nci_data_rate, (  ) );
VIRTUAL ( nci, bool, is_loopback, cs_idx_nci_is_loopback, (  ) );
VIRTUAL ( nci, bool, is_timing_out, cs_idx_nci_is_timing_out, (  ) );
VIRTUAL ( nci, bool, is_playback, cs_idx_nci_is_playback, (  ) );
VIRTUAL ( nci, float, latency, cs_idx_nci_latency, (  flow ), int flow );
VIRTUAL ( nci, float, avg_latency, cs_idx_nci_avg_latency, (  flow ), int flow );

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
	return *( nc** ) ( ( uintptr_t ) this + cs_offsets.iclientstate_nc );
}

static inline int* iclientstate_get_choked ( iclientstate* this ) {
	return ( int* ) ( ( uintptr_t ) this + cs_offsets.iclientstate_choked );
}

static inline int* iclientstate_get_last_cmd_ack ( iclientstate* this ) {
	return ( int* ) ( ( uintptr_t ) this + cs_offsets.iclientstate_last_cmd_ack );
}

static inline int* iclientstate_get_last_out_cmd ( iclientstate* this ) {
	return ( int* ) ( ( uintptr_t ) this + cs_offsets.iclientstate_last_out_cmd );
}

static inline int* iclientstate_get_server_tick ( iclientstate* this ) {
	return ( int* ) ( ( uintptr_t ) this + cs_offsets.iclientstate_server_tick );
}

static inline int* iclientstate_get_delta_tick ( iclientstate* this ) {
	return ( int* ) ( ( uintptr_t ) this + cs_offsets.iclientstate_delta_tick );
}

static inline float* iclientstate_get_next_cmd_time ( iclientstate* this ) {
	return ( float* ) ( ( uintptr_t ) this + cs_offsets.iclientstate_next_cmd_time );
}

static inline int* iclientstate_get_out_seq_num ( iclientstate* this ) {
	return ( int* ) ( ( uintptr_t ) this + cs_offsets.iclientstate_out_seq_num );
}

typedef struct iengine iengine;

VIRTUAL ( iengine, void, get_screen_size, cs_idx_iengine_get_screen_size, (  w, h ), int* w, int* h );
VIRTUAL ( iengine, void, server_cmd, cs_idx_iengine_server_cmd, (  cmd, reliable ), const char* cmd, bool reliable );
VIRTUAL ( iengine, void, client_cmd, cs_idx_iengine_client_cmd, (  cmd ), const char* cmd );
VIRTUAL ( iengine, bool, get_player_info, cs_idx_iengine_get_player_info, (  idx, pinfo ), int idx, player_info* pinfo );
VIRTUAL ( iengine, int, get_local_player, cs_idx_iengine_get_local_player, (  ) );
VIRTUAL ( iengine, void, get_angles, cs_idx_iengine_get_angles, (  ang ), vec3* ang );
VIRTUAL ( iengine, void, set_angles, cs_idx_iengine_set_angles, (  ang ), vec3* ang );
VIRTUAL ( iengine, bool, is_in_game, cs_idx_iengine_is_in_game, (  ) );
VIRTUAL ( iengine, bool, is_connected, cs_idx_iengine_is_connected, (  ) );
VIRTUAL ( iengine, nci*, get_nci, cs_idx_iengine_get_nci, (  ) );
VIRTUAL ( iengine, void, execute_cmd, cs_idx_iengine_execute_cmd, (  cmd ), const char* cmd );

#endif // !SDK_ENGINE_H