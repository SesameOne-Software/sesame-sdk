#include "hooks/hooks.h"

/* prevent the game from skipping bone setups between frames */
__attribute__( ( thiscall ) ) bool hooks_should_skip_anim_frame( renderable* this ) {
	return false;
}
