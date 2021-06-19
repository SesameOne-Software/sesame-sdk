#include "features/esp.h"
#include "gui/config.h"
#include "nuklear/nuklear.h"

typedef enum {
    esp_flags_health    = ( 1 << 0 ),
    esp_flags_armor     = ( 1 << 1 ),
    esp_flags_desync    = ( 1 << 2 ),
    esp_flags_name      = ( 1 << 3 ),
    esp_flags_weapon    = ( 1 << 4 ),
    esp_flags_box       = ( 1 << 5 ),
    esp_flags_fakeduck  = ( 1 << 6 ),
    esp_flags_reload    = ( 1 << 7 ),
    esp_flags_flashed   = ( 1 << 8 ),
    esp_flags_scoped    = ( 1 << 9 ),
    esp_flags_planting  = ( 1 << 10 ),
    esp_flags_doubletap = ( 1 << 11 ),
    esp_flags_hideshots = ( 1 << 12 ),
    esp_flags_fatal     = ( 1 << 13 ),
    esp_flags_max       = ( 1 << 14 ),
} esp_flags;

typedef struct {
    float cur;
    float target;
} esp_lerp_valf;

typedef struct {
    vec3 cur;
    vec3 target;
} esp_lerp_valvec3;

typedef struct {
    vec3 mins;
    vec3 maxs;    
    esp_lerp_valvec3 pos;
    struct nk_rect screen;
} esp_box;

typedef struct {
    int idx;
    bool flags[ esp_flags_max ];
    sds player_name;
    sds weapon_name;
    esp_lerp_valf health, armor, box_alpha, weapon_alpha; 
    esp_box box;
    bool is_dormant;
} esp_record;

static esp_record esp_records[ MAX_PLAYERS + 1 ] = { 0 };

bool features_esp_init( void ) {

}

bool features_esp_free( void ) {
    for_each ( record, esp_records ) {
        if ( record->player_name ) sdsfree( record->player_name );
        if ( record->weapon_name ) sdsfree( record->weapon_name );

        record->player_name = NULL;
        record->weapon_name = NULL;
    }
}

bool features_esp_level_init( void ) {
    esp_record default_rec = { 0 };
    default_rec.idx = -1;
    
    for_each ( record, esp_records )
        *record = default_rec;
}

bool features_esp_level_shutdown( void ) {

}

void features_esp_run( void ) {
	if ( !iengine_is_in_game( cs_iengine ) || !iengine_is_connected( cs_iengine ) )
		return;

	player* local = cs_get_local( );

	if ( !local )
		return;

    if ( !*ses_cfg_get_item( &ses_cfg, visuals, players, esp, bool ) )
        return;

    /* esp code goes below */
    cs_for_each_player( ) {
        esp_record* record = &esp_records[ iter.idx ];
        
        if ( !iter.idx || !iter.player || !entity_is_player( ( entity* ) iter.player ) || !player_is_alive( iter.player ) ) {
            /* reset esp variables */
            continue;
        }

        bool updated = false;

        /* set esp variables, set up bounding box */
        vec3 origin = *player_abs_origin( iter.player );

        /* do dynamic updates (using sound and radar) */

        /* fade in and out of dormancy if no information is recieved */

        /* lerp esp boxes in between positions when dormant */

        /* handle dormancy */
        if ( *entity_dormant( ( entity* ) iter.player ) ) {

        }
        else {

        }

        /* set updated */
        if ( updated )
            record->idx = iter.idx;

        /* draw actual esp */ 
        if ( record->idx != -1 ) {

        }
    }
}
