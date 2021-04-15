#ifndef SES_H
#define SES_H

#include "lib/csgo-sdk/sdk.h"

typedef enum {
	ses_round_starting,
	ses_round_in_progress,
	ses_round_ending,
} ses_round;

typedef struct {
	/* set to true to unload the cheat */
	bool shutdown;
	/* determines whether or not the current will be choked at the end of create_moved */
	bool choke;
	/* whether or not create_move is currently being called */
	bool in_move;
	/* number of ticks sent since last choke */
	uint32_t num_sent;
	/* number of ticks sent since last send */
	uint32_t num_choked;
	/* indicates the type of state the game round is in */
	ses_round round;
} ses_ctx_s;

extern ses_ctx_s ses_ctx;

void ses_shutdown( );

#endif // !SES_H