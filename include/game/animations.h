#include "csgo-sdk/sdk.h"

typedef struct {
    server_animstate animstate;
} game_animation_data;

extern game_animation_data game_animation_list[ MAX_PLAYERS + 1 ];

bool game_animation_init( void );
bool game_animation_free( void );
bool game_animation_level_init( void );
bool game_animation_level_shutdown( void );
