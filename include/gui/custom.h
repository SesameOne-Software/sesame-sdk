#ifndef GUI_CUSTOM_H
#define GUI_CUSTOM_H

#define GUI_LINE_HEIGHT 26.0f
#define GUI_MAX_COMBO_HEIGHT 200.0f

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

static inline bool gui_begin ( const char* title, struct nk_rect bounds, nk_flags flags ) {
	nk_style_push_font ( ses_ctx.nk_ctx, &ses_ctx.fonts.menu_font->handle );

	if ( !nk_begin ( ses_ctx.nk_ctx, title, bounds,
		flags ) )
		return false;



	return true;
}

static inline void gui_end ( ) {
	nk_end ( ses_ctx.nk_ctx );
	nk_style_pop_font ( ses_ctx.nk_ctx );
}

#endif