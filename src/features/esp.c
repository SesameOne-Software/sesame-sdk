#include "features/esp.h"
#include "gui/config.h"

void features_esp_run() {
	if ( !iengine_is_in_game( cs_iengine ) || !iengine_is_connected( cs_iengine ) )
		return;

	player* local = cs_get_local( );

	if ( !local )
		return;

    if ( !*ses_cfg_get_item( &ses_cfg, visuals, players, esp, bool ) )
        return;

    /* esp code goes below */
}
