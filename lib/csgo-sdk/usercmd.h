#ifndef SDK_USERCMD_H
#define SDK_USERCMD_H

#include "vec3.h"
#include "crc32.h"

#include "include/utils.h"

#define MULTIPLAYER_BACKUP 150

typedef enum {
	in_none = 0,
	in_attack = ( 1 << 0 ),
	in_jump = ( 1 << 1 ),
	in_duck = ( 1 << 2 ),
	in_forward = ( 1 << 3 ),
	in_back = ( 1 << 4 ),
	in_use = ( 1 << 5 ),
	in_cancel = ( 1 << 6 ),
	in_left = ( 1 << 7 ),
	in_right = ( 1 << 8 ),
	in_moveleft = ( 1 << 9 ),
	in_moveright = ( 1 << 10 ),
	in_attack2 = ( 1 << 11 ),
	in_run = ( 1 << 12 ),
	in_reload = ( 1 << 13 ),
	in_alt1 = ( 1 << 14 ),
	in_alt2 = ( 1 << 15 ),
	in_score = ( 1 << 16 ),
	in_speed = ( 1 << 17 ),
	in_walk = ( 1 << 18 ),
	in_zoom = ( 1 << 19 ),
	in_weapon1 = ( 1 << 20 ),
	in_weapon2 = ( 1 << 21 ),
	in_bullrush = ( 1 << 22 ),
	in_grenade1 = ( 1 << 23 ),
	in_grenade2 = ( 1 << 24 ),
	in_lookspin = ( 1 << 25 )
} in_buttons;

typedef struct {
	PAD( 4 );
	int	cmd_num;
	int tick_count;
	vec3 angles;
	vec3 aim_dir;
	float forward_move;
	float side_move;
	float up_move;
	in_buttons buttons;
	uint8_t impulse;
	int weapon_select;
	int weapon_subtype;
	uint32_t rand_seed;
	short mouse_dx;
	short mouse_dy;
	bool has_been_predicted;
	PAD ( 24 );
} usercmd;

crc32 usercmd_get_checksum ( usercmd* this ) {
	crc32 crc;
	crc32_init ( crc );
	
	crc32_process ( crc, &this->cmd_num );
	crc32_process ( crc, &this->tick_count );
	crc32_process ( crc, &this->angles );
	crc32_process ( crc, &this->aim_dir );
	crc32_process ( crc, &this->forward_move );
	crc32_process ( crc, &this->side_move );
	crc32_process ( crc, &this->up_move );
	crc32_process ( crc, &this->buttons );
	crc32_process ( crc, &this->impulse );
	crc32_process ( crc, &this->weapon_select );
	crc32_process ( crc, &this->weapon_subtype );
	crc32_process ( crc, &this->rand_seed );
	crc32_process ( crc, &this->mouse_dx );
	crc32_process ( crc, &this->mouse_dy );

	crc32_final ( crc );

	return crc;
}

typedef struct {
	usercmd cmd;
	crc32 crc;
} verified_usercmd;

#endif // !SDK_USERCMD_H