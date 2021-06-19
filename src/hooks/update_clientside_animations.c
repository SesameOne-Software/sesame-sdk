#include "hooks/hooks.h"

/* prevent game from animating players (we will handle this ourselves) */
__attribute__( ( cdecl ) ) void hooks_update_clientside_animations( void ) {

}
