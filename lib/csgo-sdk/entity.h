#ifndef SDK_ENTITY_H
#define SDK_ENTITY_H

#include "netvars.h"
#include "vec3.h"
#include "client.h"

#include "include/utils.h"

typedef struct studiohdr studiohdr;
typedef uint32_t ehandle;

typedef struct renderable renderable;
typedef struct networkable networkable;

typedef struct entity entity;
typedef struct smoke smoke;
typedef struct fog fog;
typedef struct planted_c4 planted_c4;
typedef struct tonemap tonemap;
typedef struct player player;

OFFSET ( entity, uint32_t, idx, 0x64 );
OFFSET ( entity, bool, dormant, 0xED );
OFFSET ( entity, renderable, renderable, 0x4 );
OFFSET ( entity, networkable, networkable, 0x8 );
OFFSET ( entity, uint32_t, eflags, 0xE8 );
OFFSET ( entity, uint32_t, effects, 0xF0 );
OFFSET ( entity, vec3, abs_vel, 0x94 );
OFFSET ( entity, studiohdr*, studiohdr, 0x294C );
NETVAR ( entity, vec3, origin, "DT_BaseEntity->m_vecOrigin" );
NETVAR ( entity, uint32_t, team, "DT_BaseEntity->m_iTeamNum" );

static inline client_class* entity_get_client_class ( entity* this ) {
	return ( ( client_class * ( __fastcall* )( networkable*, void* ) )utils_vfunc ( entity_get_networkable ( this ), 2 ) )( entity_get_networkable ( this ), NULL );
}

static inline bool entity_is_player(entity* this) {
	const client_class* cc = entity_get_client_class(this);
	return cc && cc->class_id == 40;
}

NETVAR ( smoke, bool, did_smoke_effect, "DT_SmokeGrenadeProjectile->m_bDidSmokeEffect" );

NETVAR ( fog, bool, enabled, "DT_FogController->m_fog.enable" );
NETVAR ( fog, bool, blendable, "DT_FogController->m_fog.blend" );
NETVAR ( fog, uint32_t, color_primary, "DT_FogController->m_fog.colorPrimary" );
NETVAR ( fog, uint32_t, color_secondary, "DT_FogController->m_fog.colorSecondary" );
NETVAR ( fog, float, start, "DT_FogController->m_fog.start" );
NETVAR ( fog, float, end, "DT_FogController->m_fog.end" );
NETVAR ( fog, float, far_z, "DT_FogController->m_fog.farz" );
NETVAR ( fog, float, max_density, "DT_FogController->m_fog.maxdensity" );
NETVAR ( fog, uint32_t, color_primary_lerp_to, "DT_FogController->m_fog.colorPrimaryLerpTo" );
NETVAR ( fog, uint32_t, color_secondary_lerp_to, "DT_FogController->m_fog.colorSecondaryLerpTo" );
NETVAR ( fog, float, start_lerp_to, "DT_FogController->m_fog.startLerpTo" );
NETVAR ( fog, float, end_lerp_to, "DT_FogController->m_fog.endLerpTo" );

NETVAR ( planted_c4, bool, ticking, "DT_PlantedC4->m_bBombTicking" );
NETVAR ( planted_c4, int, site, "DT_PlantedC4->m_nBombSite" );
NETVAR ( planted_c4, float, blow_time, "DT_PlantedC4->m_flC4Blow" );
NETVAR ( planted_c4, float, timer_length, "DT_PlantedC4->m_flTimerLength" );
NETVAR ( planted_c4, float, defuse_length, "DT_PlantedC4->m_flDefuseLength" );
NETVAR ( planted_c4, float, defuse_countdown, "DT_PlantedC4->m_flDefuseCountDown" );
NETVAR ( planted_c4, bool, defused, "DT_PlantedC4->m_bBombDefused" );
NETVAR ( planted_c4, ehandle, defuser_handle, "DT_PlantedC4->m_hBombDefuser" );

// FILL IN LATER
static inline player* planted_c4_defuser ( planted_c4* this ) {
	
}

NETVAR ( tonemap, bool, using_auto_exposure_min, "DT_EnvTonemapController->m_bUseCustomAutoExposureMin" );
NETVAR ( tonemap, bool, using_auto_exposure_max, "DT_EnvTonemapController->m_bUseCustomAutoExposureMax" );
NETVAR ( tonemap, bool, using_bloom_scale, "DT_EnvTonemapController->m_bUseCustomBloomScale" );
NETVAR ( tonemap, float, auto_exposure_min, "DT_EnvTonemapController->m_flCustomAutoExposureMin" );
NETVAR ( tonemap, float, auto_exposure_max, "DT_EnvTonemapController->m_flCustomAutoExposureMax" );
NETVAR ( tonemap, float, bloom_scale, "DT_EnvTonemapController->m_flCustomBloomScale" );
NETVAR ( tonemap, float, bloom_scale_min, "DT_EnvTonemapController->m_flCustomBloomScaleMinimum" );
NETVAR ( tonemap, float, bloom_exponent, "DT_EnvTonemapController->m_flBloomExponent" );
NETVAR ( tonemap, float, bloom_saturation, "DT_EnvTonemapController->m_flBloomSaturation" );
NETVAR ( tonemap, float, percent_target, "DT_EnvTonemapController->m_flTonemapPercentTarget" );
NETVAR ( tonemap, float, percent_bright_pixels, "DT_EnvTonemapController->m_flTonemapPercentBrightPixels" );
NETVAR ( tonemap, float, min_avg_lum, "DT_EnvTonemapController->m_flTonemapMinAvgLum" );
NETVAR ( tonemap, float, rate, "DT_EnvTonemapController->m_flTonemapRate" );

#endif // !SDK_ENGINE_H