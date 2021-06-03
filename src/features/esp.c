#include "features/esp.h"
#include "gui/config.h"

typedef enum {
    esp_flags_health = (1 << 0),
    esp_flags_armor = (1 << 1),
    esp_flags_desync = (1 << 2),
    esp_flags_name = (1 << 3),
    esp_flags_weapon = (1 << 4),
    esp_flags_box = (1 << 5),
    esp_flags_fakeduck = (1 << 6),
    esp_flags_reload = (1 << 7),
    esp_flags_flashed = (1 << 8),
    esp_flags_scoped = (1 << 9),
    esp_flags_planting = (1 << 10),
    esp_flags_doubletap = (1 << 11),
    esp_flags_hideshots = (1 << 12),
    esp_flags_fatal = (1 << 13),
    esp_flags_max = (1 << 14),
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
    bool flags[esp_flags];
    sds player_name;
    sds weapon_name;
    esp_lerp_valf health, armor, box_alpha, weapon_alpha; 
    esp_box box;
    bool is_dormant;
} esp_record;

typedef esp_record* vec_esp_record;
static vec_esp_record esp_records = NULL;

bool features_esp_init() {
}

bool features_esp_free() {
    
    if (esp_records) {
        for (size_t i = 0; i < vector_size(esp_records); i++) {
            esp_record* record = &esp_records[i];

            sdsfree(record->player_name);
            sdsfree(record->weapon_name);
        }

        vector_free(esp_records);
        esp_records = NULL;
    }
}

bool features_esp_level_init() {
    if (esp_records)
        features_esp_free();

    esp_records = vector_create();

    for_each_player() {
        esp_record* temp = vector_add_asg(&esp_records);
        
        memset(temp, 0, sizeof(*temp));
        temp->idx = -1;
        temp = NULL;
    }

    /* append an extra slot so we can access the record without modifying any index */
    esp_record* temp = vector_add_asg(&esp_records);
    
    memset(temp, 0, sizeof(*temp));
    temp->idx = -1;
    temp = NULL;
}

bool features_esp_level_shutdown() {
    if (esp_records)
        features_esp_free();
}

void features_esp_run() {
	if ( !iengine_is_in_game( cs_iengine ) || !iengine_is_connected( cs_iengine ) )
		return;

	player* local = cs_get_local( );

	if ( !local )
		return;

    if ( !*ses_cfg_get_item( &ses_cfg, visuals, players, esp, bool ) )
        return;

    /* esp code goes below */
    cs_for_each_player() {
        esp_record* record = &esp_records[iter.idx];
        
        if (!iter.idx || !iter.ent || !entity_is_player((entity*)iter.ent) || !player_is_alive(iter.ent)) {
            /* reset esp variables */
            continue;
        }

        bool updated = false;

        /* set esp variables, set up bounding box */
        vec3 origin = *player_abs_origin(iter.ent);

        /* do dynamic updates (using sound and radar) */

        /* fade in and out of dormancy if no information is recieved */

        /* lerp esp boxes in between positions when dormant */

        /* handle dormancy */
        if (*entity_dormant((entity*)iter.ent)) {

        }
        else {

        }

        /* set updated */
        if (updated)
            record->idx = iter.idx;

        /* draw actual esp */ 
        if (record->idx != -1) {

        }
    }
}
