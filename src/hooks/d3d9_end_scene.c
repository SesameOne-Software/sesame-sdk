#include "include/hooks/hooks.h"
#include "include/ses.h"

#include "include/gui/menu.h"
#include "lib/nuklear/nuklear.h"
#include "lib/nuklear/nuklear_d3d9.h"

#include <intrin.h>

HRESULT __stdcall hooks_d3d9_end_scene ( IDirect3DDevice9* device ) {
	typedef HRESULT ( __stdcall* hooks_d3d9_end_scene_fn )( IDirect3DDevice9* device );
	static void* ret = NULL;

	if ( !ret )
		ret = _ReturnAddress ( );

	if ( ret != _ReturnAddress ( ) )
		return ( ( hooks_d3d9_end_scene_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_d3d9_end_scene ] ) )( device );

	utils_update_key_states ( );
	
	ses_ctx.nk_ctx->delta_time_seconds = cs_iglobals->frametime;

	/* all other visual-related items */ {
		nk_style_push_color ( ses_ctx.nk_ctx, &ses_ctx.nk_ctx->style.window.background, nk_rgba ( 0, 0, 0, 0 ) );
		nk_style_push_style_item ( ses_ctx.nk_ctx, &ses_ctx.nk_ctx->style.window.fixed_background, nk_style_item_color ( nk_rgba ( 0, 0, 0, 0 ) ) );
	
		int w = 0, h = 0;
		iengine_get_screen_size ( cs_iengine, &w, &h );
		nk_begin ( ses_ctx.nk_ctx, "Overlay", nk_rect ( 0.0f, 0.0f, w, h ), NK_WINDOW_BACKGROUND | NK_WINDOW_NO_INPUT );
	
		//nk_fill_rect ( &ses_ctx.nk_ctx->current->buffer, nk_rect ( 200,200,500,500), 10.0f, ( struct nk_color ) { 255, 255, 255, 255 } );

		///* remove later */
		//sds watermark_str = sdsnew ("@ses#1997" );
		//nk_draw_text ( &ses_ctx.nk_ctx->current->buffer, nk_rect ( 20.0f, 20.0f, 0.0f, 0.0f ), watermark_str, sdslen( watermark_str ), ses_ctx.fonts.esp_font, ( struct nk_color ) { 255, 255, 255, 255 } );
		//sdsfree ( watermark_str );
	
		nk_end ( ses_ctx.nk_ctx );
	
		nk_style_pop_style_item ( ses_ctx.nk_ctx );
		nk_style_pop_color ( ses_ctx.nk_ctx );
	}

	/* menu */
	menu_draw ( );

	/* render nuklear vertex buffer */
	nk_d3d9_render ( NK_ANTI_ALIASING_ON );

	nk_input_begin ( ses_ctx.nk_ctx );
	nk_input_end ( ses_ctx.nk_ctx );

	return ( ( hooks_d3d9_end_scene_fn ) subhook_get_trampoline ( hooks_subhooks [ subhook_d3d9_end_scene ] ) )( device );
}