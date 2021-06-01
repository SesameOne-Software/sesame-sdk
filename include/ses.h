#ifndef SES_H
#define SES_H

#include "csgo-sdk/sdk.h"

typedef enum {
	ses_round_starting,
	ses_round_in_progress,
	ses_round_ending,
} ses_round;

typedef struct {
	/* sesame file dir */
	sds ses_dir;
	/* set to true to unload the cheat */
	bool shutdown;
	/* determines whether or not the current will be choked at the end of create_move */
	bool choke;
	/* whether or not create_move is currently being called */
	bool in_move;
	/* number of ticks sent since last choke */
	uint32_t num_sent;
	/* number of ticks sent since last send */
	uint32_t num_choked;
	/* indicates the type of state the game round is in */
	ses_round round;
	/* prediction variables */
	vec3 unpredicted_vel;
	/* for fixing movement in create_move */
	vec3 input_angles;
	float input_forward_move, input_side_move;
	/* nuklear context */
	struct nk_context* nk_ctx;
	struct {
		struct nk_font* menu_icons_font;
		struct nk_font* menu_icons_large_font;
		struct nk_font* menu_large_font;
		struct nk_font* menu_medium_font;
		struct nk_font* menu_small_font;
		struct nk_font* menu_xsmall_font;

		struct nk_font* default_font;
		struct nk_font* esp_font;
		struct nk_font* indicators_font;
	} fonts;
} ses_ctx_s;

extern ses_ctx_s ses_ctx;

static inline void ses_shutdown( ) {
	ses_ctx.shutdown = true;
}

#endif // !SES_H