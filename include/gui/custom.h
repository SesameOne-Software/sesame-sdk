#ifndef GUI_CUSTOM_H
#define GUI_CUSTOM_H

#define GUI_MENU_PADDING 12.0f
#define GUI_LINE_HEIGHT 26.0f
#define GUI_MAX_COMBO_HEIGHT 200.0f
#define GUI_TABS_HEIGHT 94.0f

static const char* gui_title = NULL;
static nk_flags gui_flags = 0;
static struct nk_rect gui_total_rect;
static struct nk_rect gui_rect;
static int gui_tab_count = 0;
static int gui_cur_tab_idx = 0;
static bool gui_configs_open = false;

static inline void gui_layout ( int cols ) {
	nk_row_layout ( ses_ctx.nk_ctx, NK_DYNAMIC, GUI_LINE_HEIGHT, cols, 0 );
}

static inline bool gui_button ( const char* name ) {
	gui_layout ( 1 );
	return nk_button_label ( ses_ctx.nk_ctx, name );
}

static inline void gui_checkbox ( const char* name, bool* val ) {
	gui_layout ( 1 );
	nk_checkbox_label ( ses_ctx.nk_ctx, name, val );
}

static inline void gui_sliderf ( const char* name, float min, float* val, float max, float step, const char* fmt ) {
	gui_layout ( 2 ); {
		nk_label ( ses_ctx.nk_ctx, name, NK_TEXT_LEFT );
		sds best_fmt = sdsempty ( );
		if ( fmt )
			best_fmt = sdscat ( sdscat ( best_fmt, fmt ), "##%s" );
		else
			best_fmt = sdscat ( best_fmt, "%.1f##%s" );
		sds formatted = sdscatprintf (sdsempty(), best_fmt, *val );
		nk_label ( ses_ctx.nk_ctx, formatted, NK_TEXT_RIGHT );
		sdsfree ( best_fmt );
		sdsfree ( formatted );
	}
	gui_layout ( 1 );
	nk_slider_float ( ses_ctx.nk_ctx, min, val, max, step );
}

static inline void gui_slider ( const char* name, int min, int* val, int max, int step, const char* fmt ) {
	gui_layout ( 2 ); {
		nk_label ( ses_ctx.nk_ctx, name, NK_TEXT_LEFT );
		sds best_fmt = sdsempty ( );
		if ( fmt )
			best_fmt = sdscat ( sdscat ( best_fmt, fmt ), "##%s" );
		else
			best_fmt = sdscat ( best_fmt, "%d##%s" );
		sds formatted = sdscatprintf ( sdsempty ( ), best_fmt, *val );
		nk_label ( ses_ctx.nk_ctx, formatted, NK_TEXT_RIGHT );
		sdsfree ( best_fmt );
		sdsfree ( formatted );
	}
	gui_layout ( 1 );

	nk_slider_int ( ses_ctx.nk_ctx, min, val, max, step );
}

static inline void gui_combobox ( const char* name, const char** items, int* selected ) {
	gui_layout ( 1 );
	nk_label ( ses_ctx.nk_ctx, name, NK_TEXT_LEFT );

	int len = 0;
	while ( items [ len ] != NULL )
		len++;

	sds* items_new = malloc(sizeof(sds) * len);
	for ( int i = 0; i < len; i++ )
		items_new [ i ] = sdscatprintf ( sdsempty ( ), "%s##%%s", items [ i ], name, "DropdownItem" );

	nk_combobox ( ses_ctx.nk_ctx, name, items_new, len, selected, GUI_LINE_HEIGHT, nk_vec2 ( nk_widget_width ( ses_ctx.nk_ctx ), GUI_MAX_COMBO_HEIGHT ) );

	for ( int i = 0; i < len; i++ )
		sdsfree ( items_new[i] );
	free ( items_new );
}

static bool gui_tabs_begin ( int count ) {
	assert ( !gui_tab_count && "Did you forget to call gui_tabs_end?" );
	assert ( !gui_cur_tab_idx && "Did you forget to call gui_tabs_end?" );

	gui_tab_count = count;

	return true;
}

static void gui_tab ( const char* name, int* selected ) {
	assert ( gui_tab_count && "Did you forget to call gui_tabs_end?" );

	const float tab_start_x = gui_total_rect.x + gui_total_rect.w * 0.5f - ( ses_ctx.fonts.menu_icons_font->handle.height * 2.5f * ( gui_tab_count - 1 ) ) * 0.5f;
	const float tab_x = tab_start_x + ( ses_ctx.fonts.menu_icons_font->handle.height * 2.5f ) * gui_cur_tab_idx;

	const size_t name_len = strlen ( name );
	const float text_w = ses_ctx.fonts.menu_icons_font->handle.width ( ses_ctx.fonts.menu_icons_font->handle.userdata, ses_ctx.fonts.menu_icons_font->handle.height, name, name_len );
	const struct nk_rect icon_rect = nk_rect ( tab_x - text_w * 0.5f, gui_total_rect.y + GUI_TABS_HEIGHT * 0.597f - ses_ctx.fonts.menu_icons_font->handle.height * 0.5f, text_w, ses_ctx.fonts.menu_icons_font->handle.height );
	
	struct nk_anim_data* anim_data = nk_get_anim ( name );

	const struct nk_color dynamic_color = nk_do_anim_color ( ses_ctx.nk_ctx, &anim_data->main_fraction, *selected == gui_cur_tab_idx ? 1.0f : -1.0f, &( struct nk_color ){226, 178, 235, 255}, & ( struct nk_color ){255, 255, 255, 255} );
	const struct nk_color shadow_color = nk_rgba ( 0, 0, 0, 15 );

	nk_draw_text ( &ses_ctx.nk_ctx->current->buffer, nk_rect( icon_rect.x, icon_rect.y + 1.0f, icon_rect.w, icon_rect.h ), name, name_len, ses_ctx.fonts.menu_icons_font, shadow_color );
	nk_draw_text ( &ses_ctx.nk_ctx->current->buffer, nk_rect ( icon_rect.x, icon_rect.y + 2.0f, icon_rect.w, icon_rect.h ), name, name_len, ses_ctx.fonts.menu_icons_font, shadow_color );
	nk_draw_text ( &ses_ctx.nk_ctx->current->buffer, icon_rect, name, name_len, ses_ctx.fonts.menu_icons_font, dynamic_color );

	nk_flags fl;
	if ( !gui_configs_open && nk_button_behavior ( &fl, icon_rect, &ses_ctx.nk_ctx->input, NK_BUTTON_DEFAULT ) )
		*selected = gui_cur_tab_idx;

	gui_cur_tab_idx++;
}

static void gui_tabs_end ( ) {
	gui_tab_count = 0;
	gui_cur_tab_idx = 0;
}

static inline bool gui_begin ( const char* title, struct nk_rect* bounds, nk_flags flags ) {
	gui_title = title;
	gui_flags = flags;

	nk_style_push_font ( ses_ctx.nk_ctx, &ses_ctx.fonts.menu_medium_font->handle );

	ses_ctx.nk_ctx->style.window.padding = nk_vec2 ( -5.0f, -5.0f );

	nk_style_push_color ( ses_ctx.nk_ctx, &ses_ctx.nk_ctx->style.window.background, nk_rgba ( 0, 0, 0, 0 ) );
	nk_style_push_style_item ( ses_ctx.nk_ctx, &ses_ctx.nk_ctx->style.window.fixed_background, nk_style_item_color ( nk_rgba ( 0, 0, 0, 0 ) ) );

	nk_begin ( ses_ctx.nk_ctx, gui_title, *bounds, gui_flags );
	
	nk_style_pop_style_item ( ses_ctx.nk_ctx );
	nk_style_pop_color ( ses_ctx.nk_ctx );

	/* window bg */
	nk_fill_rect ( &ses_ctx.nk_ctx->current->buffer, *bounds, ses_ctx.nk_ctx->style.window.rounding, ses_ctx.nk_ctx->style.window.background );

	/* title bar */
	struct nk_color shadow_start_color = nk_rgba( 0, 0, 0, 75 );
	struct nk_color shadow_end_color = nk_rgba( 0, 0, 0, 0 );

	struct nk_color left_color = nk_rgba(204, 82, 224, 255 );
	struct nk_color right_color = nk_rgba( 218, 158, 228, 255 );

	nk_fill_rect_multi_color ( &ses_ctx.nk_ctx->current->buffer, nk_rect ( bounds->x, bounds->y + GUI_TABS_HEIGHT - GUI_TABS_HEIGHT * 0.108f, bounds->w, GUI_TABS_HEIGHT * 0.108f * 2.0f ), shadow_start_color, shadow_start_color, shadow_end_color, shadow_end_color );

	nk_fill_rect ( &ses_ctx.nk_ctx->current->buffer, nk_rect ( bounds->x, bounds->y, ses_ctx.nk_ctx->style.window.rounding * 2.0f, GUI_TABS_HEIGHT ), ses_ctx.nk_ctx->style.window.rounding, left_color );
	nk_fill_rect ( &ses_ctx.nk_ctx->current->buffer, nk_rect ( bounds->x + bounds->w - ses_ctx.nk_ctx->style.window.rounding * 2.0f, bounds->y, ses_ctx.nk_ctx->style.window.rounding * 2.0f, GUI_TABS_HEIGHT ), ses_ctx.nk_ctx->style.window.rounding, right_color );
	nk_fill_rect_multi_color ( &ses_ctx.nk_ctx->current->buffer, nk_rect ( bounds->x + ses_ctx.nk_ctx->style.window.rounding, bounds->y, bounds->w - ses_ctx.nk_ctx->style.window.rounding * 2.0f, GUI_TABS_HEIGHT ), left_color, right_color, right_color, left_color );
	nk_stroke_rect ( &ses_ctx.nk_ctx->current->buffer, nk_rect ( bounds->x, bounds->y, bounds->w, GUI_TABS_HEIGHT ), ses_ctx.nk_ctx->style.window.rounding, ses_ctx.nk_ctx->style.tab.border, ses_ctx.nk_ctx->style.tab.border_color );

	const size_t title_len = strlen ( gui_title );
	const float text_w = ses_ctx.fonts.menu_xsmall_font->handle.width ( ses_ctx.fonts.menu_xsmall_font->handle.userdata, ses_ctx.fonts.menu_xsmall_font->handle.height, gui_title, title_len );

	/* title text */
	nk_draw_text ( &ses_ctx.nk_ctx->current->buffer, nk_rect ( bounds->x + bounds->w * 0.5f - text_w * 0.5f, bounds->y + GUI_TABS_HEIGHT * 0.3f - ses_ctx.fonts.menu_xsmall_font->handle.height * 0.5f, text_w, ses_ctx.fonts.menu_xsmall_font->handle.height ), title, title_len, ses_ctx.fonts.menu_xsmall_font, nk_rgba( 255, 255, 255, 255 ) );

	/* side menu icon */
	struct nk_color side_menu_icon_color = nk_rgba ( 255, 255, 255, 133 );

	const float menu_icon_h = GUI_TABS_HEIGHT * 0.050f;
	const float menu_icon_w = bounds->w * 0.075f;
	const float menu_icon_w1 = menu_icon_w * 0.7f;

	nk_fill_rect ( &ses_ctx.nk_ctx->current->buffer, nk_rect ( bounds->x + GUI_TABS_HEIGHT * 0.45f + ( menu_icon_w - menu_icon_w1 ) * 0.5f, bounds->y + GUI_TABS_HEIGHT * 0.5f - menu_icon_h * 0.5f - GUI_MENU_PADDING, menu_icon_w1, menu_icon_h ), menu_icon_h * 0.5f, side_menu_icon_color );
	nk_fill_rect ( &ses_ctx.nk_ctx->current->buffer, nk_rect ( bounds->x + GUI_TABS_HEIGHT * 0.45f, bounds->y + GUI_TABS_HEIGHT * 0.5f - menu_icon_h * 0.5f, menu_icon_w, menu_icon_h ), menu_icon_h * 0.5f, side_menu_icon_color );
	nk_fill_rect ( &ses_ctx.nk_ctx->current->buffer, nk_rect ( bounds->x + GUI_TABS_HEIGHT * 0.45f + ( menu_icon_w - menu_icon_w1 ) * 0.5f, bounds->y + GUI_TABS_HEIGHT * 0.5f - menu_icon_h * 0.5f + GUI_MENU_PADDING, menu_icon_w1, menu_icon_h ), menu_icon_h * 0.5f, side_menu_icon_color );
	
	/* menu options button (cog) */
	const struct nk_rect icon_rect = nk_rect ( bounds->x + bounds->w - GUI_MENU_PADDING * 5.0f - GUI_MENU_PADDING * 2.0f, bounds->y + GUI_TABS_HEIGHT - GUI_MENU_PADDING * 2.0f, GUI_MENU_PADDING * 2.0f * 2.0f, GUI_MENU_PADDING * 2.0f * 2.0f );
	const struct nk_color icon_border_color = nk_rgba ( 0, 0, 0, 22 );
	const struct nk_color shadow_color = nk_rgba ( 0, 0, 0, 15 );

	nk_fill_circle ( &ses_ctx.nk_ctx->current->buffer, nk_rect ( icon_rect.x, icon_rect.y + 2.0f, icon_rect.w, icon_rect.h ), shadow_color );
	nk_fill_circle ( &ses_ctx.nk_ctx->current->buffer, nk_rect ( icon_rect.x, icon_rect.y + 4.0f, icon_rect.w, icon_rect.h ), shadow_color );
	nk_fill_circle ( &ses_ctx.nk_ctx->current->buffer, icon_rect, nk_rgba ( 255, 255, 255, 255 ) );
	nk_stroke_circle ( &ses_ctx.nk_ctx->current->buffer, icon_rect, 2.0f, icon_border_color );

	const size_t options_text_len = strlen ( "" );
	const float options_text_w = ses_ctx.fonts.menu_icons_large_font->handle.width ( ses_ctx.fonts.menu_icons_large_font->handle.userdata, ses_ctx.fonts.menu_icons_large_font->handle.height, "", options_text_len );
	nk_draw_text ( &ses_ctx.nk_ctx->current->buffer, nk_rect ( icon_rect.x + icon_rect.w * 0.5f - options_text_w * 0.5f, icon_rect.y + icon_rect.h * 0.5f - ses_ctx.fonts.menu_icons_large_font->handle.height * 0.5f, options_text_w, ses_ctx.fonts.menu_icons_large_font->handle.height ), "", options_text_len, ses_ctx.fonts.menu_icons_large_font, nk_rgba ( 140, 140, 140, 255 ) );
	
	nk_flags fl;
	if ( nk_button_behavior ( &fl, icon_rect, &ses_ctx.nk_ctx->input, NK_BUTTON_DEFAULT ) )
		gui_configs_open = true;

	struct nk_anim_data* anim_data = nk_get_anim ( "" );
	const float dynamic_color = nk_do_anim ( ses_ctx.nk_ctx, &anim_data->main_fraction, gui_configs_open ? 1.0f : -1.0f, 0, 255 );

	nk_style_push_color ( ses_ctx.nk_ctx, &ses_ctx.nk_ctx->style.window.background, nk_rgba ( ses_ctx.nk_ctx->style.window.background.r, ses_ctx.nk_ctx->style.window.background.g, ses_ctx.nk_ctx->style.window.background.b, dynamic_color ) );
	nk_style_push_style_item ( ses_ctx.nk_ctx, &ses_ctx.nk_ctx->style.window.fixed_background, nk_style_item_color ( ses_ctx.nk_ctx->style.window.background ) );

	if ( dynamic_color > 0.0f && nk_popup_begin ( ses_ctx.nk_ctx, NK_POPUP_STATIC, "config menu", NK_WINDOW_NO_SCROLLBAR, nk_rect ( icon_rect.x + icon_rect.w * 0.25f, icon_rect.y - icon_rect.h, GUI_TABS_HEIGHT * 2.0f, GUI_TABS_HEIGHT * 2.0f ) ) ) {
		nk_row_layout ( ses_ctx.nk_ctx, NK_DYNAMIC, GUI_TABS_HEIGHT, 1, 0 );
		if ( nk_group_begin ( ses_ctx.nk_ctx, "List", 0 ) ) {
			static int selected = -1;
			gui_layout ( 1 );
			for ( int i = 0; i < 16; ++i ) {
				if ( nk_select_label ( ses_ctx.nk_ctx, ( selected == i ) ? "Selected" : "Unselected", NK_TEXT_CENTERED, selected == i ) )
					selected = ( selected == i ) ? -1 : i;
			}
		} nk_group_end ( ses_ctx.nk_ctx );
		
		gui_layout ( 1 );

		if ( gui_button ( "Save Config" ) )
			ses_cfg_save ( &ses_cfg, sdscat( sdsdup ( ses_ctx.ses_dir ), "\\configs\\test.json" ) );

		if ( gui_button ( "Load Config" ) )
			ses_cfg_load ( &ses_cfg, sdscat ( sdsdup ( ses_ctx.ses_dir ), "\\configs\\test.json" ) );
	} nk_popup_end ( ses_ctx.nk_ctx );

	nk_style_pop_style_item ( ses_ctx.nk_ctx );
	nk_style_pop_color ( ses_ctx.nk_ctx );

	/* window outline */
	nk_stroke_rect ( &ses_ctx.nk_ctx->current->buffer, *bounds, ses_ctx.nk_ctx->style.window.rounding, ses_ctx.nk_ctx->style.window.border, ses_ctx.nk_ctx->style.window.border_color );

	/* bottom watermark */
	struct nk_color watermark_color = nk_rgba( 255, 255, 255, 66 );

	sds watermark_str = sdscat( sdscat ( sdscat ( sdsempty ( ), "Copyright (c) 2021 " ), gui_title ), ".");

	const size_t watermark_len = strlen ( watermark_str );
	const float watermark_w = ses_ctx.fonts.menu_xsmall_font->handle.width ( ses_ctx.fonts.menu_xsmall_font->handle.userdata, ses_ctx.fonts.menu_xsmall_font->handle.height, watermark_str, title_len );

	nk_draw_text ( &ses_ctx.nk_ctx->current->buffer, nk_rect ( bounds->x + GUI_MENU_PADDING, bounds->y + bounds->h - ses_ctx.fonts.menu_xsmall_font->handle.height - GUI_MENU_PADDING, watermark_w, ses_ctx.fonts.menu_xsmall_font->handle.height ), watermark_str, watermark_len, ses_ctx.fonts.menu_xsmall_font, watermark_color );
	sdsfree ( watermark_str );

	struct nk_vec2 window_pos = nk_window_get_position ( ses_ctx.nk_ctx );
	gui_rect.x = bounds->x = window_pos.x;
	gui_rect.y = bounds->y = window_pos.y;
	gui_total_rect = *bounds;

	const float extra_padding = GUI_LINE_HEIGHT * 1.269f;
	ses_ctx.nk_ctx->current->layout->at_y = bounds->y + GUI_TABS_HEIGHT + extra_padding;
	ses_ctx.nk_ctx->current->layout->at_x = bounds->x + extra_padding * 2.3f;
	ses_ctx.nk_ctx->current->layout->clip = ses_ctx.nk_ctx->current->layout->bounds = ses_ctx.nk_ctx->current->bounds = nk_rect ( ses_ctx.nk_ctx->current->layout->at_x, ses_ctx.nk_ctx->current->layout->at_y, bounds->w - extra_padding * 2.3f * 2.0f, bounds->h - GUI_LINE_HEIGHT - extra_padding * 4.0f );

	return true;
}

static inline void gui_end ( ) {
	nk_end ( ses_ctx.nk_ctx );
	nk_style_pop_font ( ses_ctx.nk_ctx );
}

#endif