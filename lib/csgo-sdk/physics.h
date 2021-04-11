#ifndef SDK_PHYSICS_H
#define SDK_PHYSICS_H

#include "include/utils.h"

typedef struct {
	PAD( 20 );
} surface_physics_params;

typedef struct {
	PAD( 36 );
} surface_audio_params;

typedef struct {
	PAD( 24 );
} surface_sound_names;

typedef struct {
	PAD( 8 );
	float penetration_modifier;
	float damage_modifier;
	uint16_t material;
	uint8_t climbable;
	PAD( 4 );
} surface_game_props;

typedef struct {
	surface_physics_params physics;
	surface_audio_params audio;
	surface_sound_names sounds;
	surface_game_props game;
} surfacedata;

typedef struct iphys iphys;

VIRTUAL ( iphys, surfacedata*, get_surface, 5, ( idx ), uint32_t idx );

#endif // !SDK_PHYSICS_H