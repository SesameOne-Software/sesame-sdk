#include "hooks/hooks.h"

/* make sure player has correct angles when getting out of dormancy */
__attribute__( ( thiscall ) ) bool hooks_is_renderable_in_pvs( void* this, renderable* renderable ) {
	return true;
}
