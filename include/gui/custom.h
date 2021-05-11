#ifndef GUI_CUSTOM_H
#define GUI_CUSTOM_H

#define GUI_LINE_HEIGHT 26.0f
#define GUI_MAX_COMBO_HEIGHT 200.0f
#define GUI_TABS_WIDTH 100.0f

static struct nk_rect gui_rect;
static int gui_tab_count = 0;
static int gui_cur_tab_idx = 0;

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

	const float tab_w = gui_rect.w / ( float ) gui_tab_count;

	nk_flags fl;
	if ( nk_button_behavior ( &fl, nk_rect ( gui_rect.x + tab_w * (float) gui_cur_tab_idx, gui_rect.y + gui_rect.h - GUI_TABS_WIDTH, tab_w, GUI_TABS_WIDTH), &ses_ctx.nk_ctx->input, NK_BUTTON_DEFAULT ) ) {
		*selected = gui_cur_tab_idx;
	}

	gui_cur_tab_idx++;
	//ses_ctx.nk_ctx->style.window.rounding
}

static void gui_tabs_end ( ) {
	gui_tab_count = 0;
	gui_cur_tab_idx = 0;
}

static inline bool gui_begin ( const char* title, struct nk_rect* bounds, nk_flags flags ) {
	nk_style_push_font ( ses_ctx.nk_ctx, &ses_ctx.fonts.menu_font->handle );

	nk_style_push_color(ses_ctx.nk_ctx, &ses_ctx.nk_ctx->style.window.background, nk_rgba(0, 0, 0, 0));
	nk_style_push_style_item(ses_ctx.nk_ctx, &ses_ctx.nk_ctx->style.window.fixed_background, nk_style_item_color(nk_rgba(0, 0, 0, 0)));

	ses_ctx.nk_ctx->style.window.padding = nk_vec2(0, 0);
	nk_begin(ses_ctx.nk_ctx, "menu_sidebar", nk_rect(bounds->x, bounds->y, GUI_TABS_WIDTH, bounds->h), NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_MOVABLE );

	struct nk_vec2 window_pos = nk_window_get_position(ses_ctx.nk_ctx);
	gui_rect.x = bounds->x = window_pos.x;
	gui_rect.y = bounds->y = window_pos.y;

	struct nk_color top_color = (struct nk_color){ 213, 145, 224, 255 };
	struct nk_color bottom_color = (struct nk_color){ 102, 151, 204, 255 };
	nk_fill_rect(&ses_ctx.nk_ctx->current->buffer, nk_rect(bounds->x, bounds->y, GUI_TABS_WIDTH, ses_ctx.nk_ctx->style.window.rounding * 2.0f), ses_ctx.nk_ctx->style.window.rounding, top_color);
	nk_fill_rect(&ses_ctx.nk_ctx->current->buffer, nk_rect(bounds->x, bounds->y + bounds->h - ses_ctx.nk_ctx->style.window.rounding * 2.0f, GUI_TABS_WIDTH, ses_ctx.nk_ctx->style.window.rounding * 2.0f), ses_ctx.nk_ctx->style.window.rounding, bottom_color);
	nk_fill_rect_multi_color(&ses_ctx.nk_ctx->current->buffer, nk_rect(bounds->x, bounds->y + ses_ctx.nk_ctx->style.window.rounding * 1.0f, GUI_TABS_WIDTH, bounds->h - ses_ctx.nk_ctx->style.window.rounding * 2.0f), top_color, top_color, bottom_color, bottom_color );

	nk_end(ses_ctx.nk_ctx);

	nk_style_pop_style_item(ses_ctx.nk_ctx);
	nk_style_pop_color(ses_ctx.nk_ctx);

	ses_ctx.nk_ctx->style.window.padding = nk_vec2(26, 26);
	if ( !nk_begin ( ses_ctx.nk_ctx, title, nk_rect(bounds->x + GUI_TABS_WIDTH + GUI_LINE_HEIGHT, bounds->y, bounds->w - GUI_TABS_WIDTH - GUI_LINE_HEIGHT, bounds->h), flags ) )
		return false;

	return true;
}

static inline void gui_end ( ) {
	nk_end ( ses_ctx.nk_ctx );
	nk_style_pop_font ( ses_ctx.nk_ctx );
}

#endif