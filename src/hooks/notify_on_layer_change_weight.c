#include "include/hooks/hooks.h"

void __fastcall hooks_notify_on_layer_change_weight ( REG, animlayer* layer, float new_weight ) {
	//typedef void ( __fastcall* notify_on_layer_change_weight_fn )( REG, animlayer* layer, float new_weight );
	//( ( notify_on_layer_change_weight_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_notify_on_layer_change_weight ] ) )( REG_OUT, layer, new_weight );
}