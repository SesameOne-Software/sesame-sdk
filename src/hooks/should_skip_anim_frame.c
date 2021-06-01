#include "hooks/hooks.h"

__attribute__( ( thiscall ) ) bool hooks_should_skip_anim_frame( renderable* this ) {
	return false;
}