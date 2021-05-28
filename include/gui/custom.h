#ifndef GUI_CUSTOM_H
#define GUI_CUSTOM_H

#include "lib/dirent/dirent.h"

#define GUI_MENU_PADDING 12.0f
#define GUI_LINE_HEIGHT 26.0f
#define GUI_MAX_COMBO_HEIGHT 200.0f
#define GUI_TABS_HEIGHT 94.0f

static const char* gui_title = NULL;
static nk_flags gui_flags = 0;
static struct nk_rect gui_total_rect;
static struct nk_rect gui_rect;
static struct nk_rect gui_last_rect;
static int gui_tab_count = 0;
static int gui_cur_tab_idx = 0;
static bool gui_configs_open = false;

typedef sds* vec_sds;
static vec_sds gui_configs_list = NULL;
static vec_sds gui_subtabs_list = NULL;
static vec_sds gui_subtabs_subtitles_list = NULL;
static int* gui_subtabs_cur_val_ptr = NULL;

static bool gui_subtabs_open = false;
static struct nk_rect gui_subtabs_rect;
static int gui_subtab_count = 0;
static bool gui_just_refreshed_subtabs = false;

static int gui_first_group_y = 0;

static void gui_reload_config_list ( ) {
	/* refresh config list */
			/* delete current configs list */
	if ( gui_configs_list ) {
		for ( int i = 0; i < vector_size ( gui_configs_list ); i++ )
			sdsfree ( gui_configs_list [ i ] );

		vector_free ( gui_configs_list );
	}

	/* create new list */
	gui_configs_list = vector_create ( );

	/* add all elements to the list */
	dirent* dp = NULL;
	DIR* dfd = NULL;

	sds dir_name = sdscat ( sdsnew ( ses_ctx.ses_dir ), "\\configs\\" );

	if ( ( dfd = opendir ( dir_name ) ) != NULL ) {
		while ( ( dp = readdir ( dfd ) ) != NULL ) {
			struct stat stbuf;
			sds file_name = sdscat ( sdsnew ( dir_name ), dp->d_name );

			if ( stat ( file_name, &stbuf ) == -1 ) {
				sdsfree ( file_name );
				continue;
			}

			if ( ( stbuf.st_mode & S_IFMT ) == S_IFDIR )
				continue;
			else if ( strstr ( file_name, ".json" ) ) {
				sds sds_config_name = sdsnew ( dp->d_name );
				sdsrange ( sds_config_name, 0, sdslen ( sds_config_name ) - 5 - 1 );

				vector_add ( &gui_configs_list, sds, sds_config_name );
			}

			sdsfree ( file_name );
		}

		closedir ( dfd );
	}

	sdsfree ( dir_name );
}

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

static inline void gui_header(const char* name, const char* section) {
	nk_row_layout(ses_ctx.nk_ctx, NK_DYNAMIC, GUI_LINE_HEIGHT * 1.0f, 1, 0);
	nk_style_push_font(ses_ctx.nk_ctx, &ses_ctx.fonts.menu_large_font->handle);

	sds new_header = sdscatfmt(sdsempty(), "%s • %s", name, section);
	nk_label(ses_ctx.nk_ctx, new_header, NK_TEXT_LEFT);
	sdsfree(new_header);

	nk_style_pop_font(ses_ctx.nk_ctx);
}

static bool gui_group_begin() {
	gui_first_group_y = ses_ctx.nk_ctx->current->layout->at_y + GUI_LINE_HEIGHT;
	ses_ctx.nk_ctx->current->layout->at_x += ses_ctx.nk_ctx->style.tab.indent + ses_ctx.nk_ctx->style.tab.border;
	ses_ctx.nk_ctx->current->layout->bounds.w -= ses_ctx.nk_ctx->style.tab.indent + ses_ctx.nk_ctx->style.tab.border;
}

static void gui_group_end() {
	ses_ctx.nk_ctx->current->layout->at_x -= ses_ctx.nk_ctx->style.tab.indent + ses_ctx.nk_ctx->style.tab.border;
	ses_ctx.nk_ctx->current->layout->bounds.w += ses_ctx.nk_ctx->style.tab.indent + ses_ctx.nk_ctx->style.tab.border;

	nk_stroke_line(&ses_ctx.nk_ctx->current->buffer, ses_ctx.nk_ctx->current->layout->at_x, gui_first_group_y, ses_ctx.nk_ctx->current->layout->at_x, ses_ctx.nk_ctx->current->layout->at_y + GUI_LINE_HEIGHT, 2.0f, ses_ctx.nk_ctx->style.tab.border_color);
}

// TODO
static bool gui_subtabs_begin ( int count ) {
	//assert ( !gui_subtab_count && "Did you forget to call gui_tabs_end?" );

	gui_subtab_count = count;

	if (!gui_subtabs_list) {
		gui_just_refreshed_subtabs = true;
		gui_subtabs_list = vector_create();
	}

	if ( !gui_subtabs_subtitles_list ) {
		gui_just_refreshed_subtabs = true;
		gui_subtabs_subtitles_list = vector_create ( );
	}
}

static void gui_subtab ( const char* name, const char* subtitle, int* selected ) {
	//assert (gui_subtab_count && "Did you forget to call gui_subtabs_begin?" );

	if ( gui_just_refreshed_subtabs ) {
		vector_add ( &gui_subtabs_list, sds, sdscat ( sdsnew ( name ), "##Subtab" ) );
		vector_add ( &gui_subtabs_subtitles_list, sds, sdscat( sdscat ( sdsnew ( subtitle ), "##SubtabSubtitle" ), name ) );
	}

	gui_subtabs_cur_val_ptr = selected;
}

static void gui_subtabs_end ( ) {
	gui_just_refreshed_subtabs = false;
}

static bool gui_tabs_begin ( int count ) {
	assert ( !gui_tab_count && "Did you forget to call gui_tabs_end?" );
	assert ( !gui_cur_tab_idx && "Did you forget to call gui_tabs_end?" );

	gui_tab_count = count;

	return true;
}

static void gui_tab ( const char* name, int* selected ) {
	assert ( gui_tab_count && "Did you forget to call gui_tabs_begin?" );

	const float tab_start_x = gui_total_rect.x + gui_total_rect.w * 0.5f - ( ses_ctx.fonts.menu_icons_font->handle.height * 2.5f * ( gui_tab_count - 1 ) ) * 0.5f;
	const float tab_x = tab_start_x + ( ses_ctx.fonts.menu_icons_font->handle.height * 2.5f ) * gui_cur_tab_idx;

	const size_t name_len = nk_strlen ( name );
	const float text_w = ses_ctx.fonts.menu_icons_font->handle.width ( ses_ctx.fonts.menu_icons_font->handle.userdata, ses_ctx.fonts.menu_icons_font->handle.height, name, name_len );
	const struct nk_rect icon_rect = nk_rect ( tab_x - text_w * 0.5f, gui_total_rect.y + GUI_TABS_HEIGHT * 0.597f - ses_ctx.fonts.menu_icons_font->handle.height * 0.5f, text_w, ses_ctx.fonts.menu_icons_font->handle.height );
	
	struct nk_anim_data* anim_data = nk_get_anim ( name );

	const struct nk_color dynamic_color = nk_do_anim_color ( ses_ctx.nk_ctx, &anim_data->main_fraction, *selected == gui_cur_tab_idx ? 1.0f : -1.0f, &( struct nk_color ){226, 178, 235, 255}, & ( struct nk_color ){255, 255, 255, 255} );
	const struct nk_color shadow_color = nk_rgba ( 0, 0, 0, 15 );

	nk_draw_text ( &ses_ctx.nk_ctx->current->buffer, nk_rect( icon_rect.x, icon_rect.y + 1.0f, icon_rect.w, icon_rect.h ), name, name_len, ses_ctx.fonts.menu_icons_font, shadow_color );
	nk_draw_text ( &ses_ctx.nk_ctx->current->buffer, nk_rect ( icon_rect.x, icon_rect.y + 2.0f, icon_rect.w, icon_rect.h ), name, name_len, ses_ctx.fonts.menu_icons_font, shadow_color );
	nk_draw_text ( &ses_ctx.nk_ctx->current->buffer, icon_rect, name, name_len, ses_ctx.fonts.menu_icons_font, dynamic_color );

	nk_flags fl;
	if ( nk_button_behavior ( &fl, icon_rect, &ses_ctx.nk_ctx->input, NK_BUTTON_DEFAULT ) ) {
		/* if already selected, open subtabs */
		if (*selected == gui_cur_tab_idx) {
			gui_configs_open = false;
			gui_subtabs_open = true;
		}
		else {/* reset subtabs list so we dont draw the ones from last selected tab for a frame*/
			if (gui_subtabs_list) {
				for (int i = 0; i < vector_size(gui_subtabs_list); i++)
					sdsfree(gui_subtabs_list[i]);

				vector_free(gui_subtabs_list);
				gui_subtabs_list = NULL;
			}

			if ( gui_subtabs_subtitles_list ) {
				for ( int i = 0; i < vector_size ( gui_subtabs_subtitles_list ); i++ )
					sdsfree ( gui_subtabs_subtitles_list [ i ] );

				vector_free ( gui_subtabs_subtitles_list );
				gui_subtabs_subtitles_list = NULL;
			}

			gui_subtab_count = 0;
		}

		if ( !gui_configs_open )
			*selected = gui_cur_tab_idx;
	}
	
	/* set subtabs popup rect according to currently selected tab */
	if ( *selected == gui_cur_tab_idx ) {
		const struct nk_vec2 subtabs_rect_dim = nk_vec2 ( gui_subtab_count * (GUI_TABS_HEIGHT * 0.5f), ses_ctx.fonts.menu_icons_font->handle.height + ses_ctx.fonts.menu_xsmall_font->handle.height + ses_ctx.nk_ctx->style.window.popup_padding.y * 2.0f );
		gui_subtabs_rect = nk_rect ( icon_rect.x - gui_total_rect.x + icon_rect.w * 0.5f - subtabs_rect_dim.x * 0.5f, icon_rect.y - gui_total_rect.y + icon_rect.h + GUI_LINE_HEIGHT * 0.5f, subtabs_rect_dim.x, subtabs_rect_dim.y );
	}

	gui_cur_tab_idx++;
}

static void gui_tabs_end ( ) {
	gui_tab_count = 0;
	gui_cur_tab_idx = 0;
}

static inline bool gui_begin ( const char* title, struct nk_rect* bounds, nk_flags flags ) {
	gui_title = title;
	gui_flags = flags;

	gui_rect.x = bounds->x = gui_last_rect.x;
	gui_rect.y = bounds->y = gui_last_rect.y;
	gui_total_rect = *bounds;

	nk_style_push_font ( ses_ctx.nk_ctx, &ses_ctx.fonts.menu_medium_font->handle );

	ses_ctx.nk_ctx->style.window.padding = nk_vec2 ( -5.0f, -5.0f );

	nk_style_push_color ( ses_ctx.nk_ctx, &ses_ctx.nk_ctx->style.window.background, nk_rgba ( 0, 0, 0, 0 ) );
	nk_style_push_style_item ( ses_ctx.nk_ctx, &ses_ctx.nk_ctx->style.window.fixed_background, nk_style_item_color ( nk_rgba ( 0, 0, 0, 0 ) ) );

	nk_begin ( ses_ctx.nk_ctx, gui_title, *bounds, gui_flags );

	/* dragging logic from inside nuklear */ {
		/* calculate draggable window space */
		struct nk_rect header = nk_rect ( bounds->x, bounds->y, bounds->w, GUI_TABS_HEIGHT );

		/* window movement by dragging */
		int left_mouse_down = ses_ctx.nk_ctx->input.mouse.buttons [ NK_BUTTON_LEFT ].down;
		int left_mouse_clicked = ( int ) ses_ctx.nk_ctx->input.mouse.buttons [ NK_BUTTON_LEFT ].clicked;
		int left_mouse_click_in_cursor = nk_input_has_mouse_click_down_in_rect ( &ses_ctx.nk_ctx->input, NK_BUTTON_LEFT, header, nk_true );

		if ( left_mouse_down && left_mouse_click_in_cursor && !left_mouse_clicked ) {
			ses_ctx.nk_ctx->current->bounds.x = ses_ctx.nk_ctx->current->bounds.x + ses_ctx.nk_ctx->input.mouse.delta.x;
			ses_ctx.nk_ctx->current->bounds.y = ses_ctx.nk_ctx->current->bounds.y + ses_ctx.nk_ctx->input.mouse.delta.y;
			ses_ctx.nk_ctx->input.mouse.buttons [ NK_BUTTON_LEFT ].clicked_pos.x += ses_ctx.nk_ctx->input.mouse.delta.x;
			ses_ctx.nk_ctx->input.mouse.buttons [ NK_BUTTON_LEFT ].clicked_pos.y += ses_ctx.nk_ctx->input.mouse.delta.y;
			ses_ctx.nk_ctx->style.cursor_active = ses_ctx.nk_ctx->style.cursors [ NK_CURSOR_MOVE ];
		}
	}

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

	const size_t title_len = nk_strlen ( gui_title );
	const float text_w = ses_ctx.fonts.menu_xsmall_font->handle.width ( ses_ctx.fonts.menu_xsmall_font->handle.userdata, ses_ctx.fonts.menu_xsmall_font->handle.height, gui_title, title_len );

	/* title text */
	nk_draw_text ( &ses_ctx.nk_ctx->current->buffer, nk_rect ( bounds->x + bounds->w * 0.5f - text_w * 0.5f, bounds->y + GUI_TABS_HEIGHT * 0.3f - ses_ctx.fonts.menu_xsmall_font->handle.height * 0.5f, text_w, ses_ctx.fonts.menu_xsmall_font->handle.height ), title, title_len, ses_ctx.fonts.menu_xsmall_font, nk_rgba( 255, 255, 255, 150 ) );

	/* side menu icon */
	struct nk_color side_menu_icon_color = nk_rgba ( 255, 255, 255, 133 );

	const float menu_icon_h = GUI_TABS_HEIGHT * 0.055f;
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

	const size_t options_text_len = nk_strlen ( "" );
	const float options_text_w = ses_ctx.fonts.menu_icons_large_font->handle.width ( ses_ctx.fonts.menu_icons_large_font->handle.userdata, ses_ctx.fonts.menu_icons_large_font->handle.height, "", options_text_len );
	nk_draw_text ( &ses_ctx.nk_ctx->current->buffer, nk_rect ( icon_rect.x + icon_rect.w * 0.5f - options_text_w * 0.5f, icon_rect.y + icon_rect.h * 0.5f - ses_ctx.fonts.menu_icons_large_font->handle.height * 0.5f, options_text_w, ses_ctx.fonts.menu_icons_large_font->handle.height ), "", options_text_len, ses_ctx.fonts.menu_icons_large_font, nk_rgba ( 140, 140, 140, 255 ) );
	
	nk_flags fl;
	bool opened_popup_this_frame = false;
	if ( nk_button_behavior ( &fl, icon_rect, &ses_ctx.nk_ctx->input, NK_BUTTON_DEFAULT ) ) {
		gui_configs_open = !gui_configs_open;

		if ( gui_configs_open ) {
			gui_reload_config_list ( );
			opened_popup_this_frame = true;
			gui_subtabs_open = false;
		}
	}

	/* popups */ {
		struct nk_anim_data* anim_data = nk_get_anim ( "" );
		const float dynamic_color = nk_do_anim ( ses_ctx.nk_ctx, &anim_data->main_fraction, gui_configs_open ? 1.0f : -1.0f, 0, 255 );

		nk_style_push_color ( ses_ctx.nk_ctx, &ses_ctx.nk_ctx->style.window.background, nk_rgba ( ses_ctx.nk_ctx->style.window.background.r, ses_ctx.nk_ctx->style.window.background.g, ses_ctx.nk_ctx->style.window.background.b, dynamic_color ) );
		nk_style_push_style_item ( ses_ctx.nk_ctx, &ses_ctx.nk_ctx->style.window.fixed_background, nk_style_item_color ( ses_ctx.nk_ctx->style.window.background ) );

		/* config menu popup */
		if ( dynamic_color > 0.0f && nk_popup_begin ( ses_ctx.nk_ctx, NK_POPUP_STATIC, "config menu", NK_WINDOW_NO_SCROLLBAR, nk_rect ( icon_rect.x - bounds->x + icon_rect.w + GUI_LINE_HEIGHT * 0.5f, icon_rect.y - bounds->y, GUI_TABS_HEIGHT * 3.0f, GUI_TABS_HEIGHT * 1.8333f ) ) ) {
			bool popup_closed = false;

			nk_row_layout ( ses_ctx.nk_ctx, NK_DYNAMIC, GUI_TABS_HEIGHT * 1.333f, 1, 0 );
			if ( nk_group_begin ( ses_ctx.nk_ctx, "List", 0 ) ) {
				bool reload_after_done = false;

				for ( int i = 0; i < vector_size ( gui_configs_list ); i++ ) {
					const float ratios [ ] = { 0.58f, 0.14f, 0.14f, 0.14f };
					nk_layout_row ( ses_ctx.nk_ctx, NK_DYNAMIC, GUI_LINE_HEIGHT, COUNT_OF ( ratios ), ratios );

					sds config_name = sdscat ( sdsnew ( gui_configs_list [ i ] ), "##config" );
					nk_select_label ( ses_ctx.nk_ctx, config_name, NK_TEXT_CENTERED, false );
					sdsfree ( config_name );

					nk_style_push_font ( ses_ctx.nk_ctx, &ses_ctx.fonts.menu_icons_font->handle );

					sds iter_num_str = sdsfromlonglong ( i );
					sds load_icon = sdscat ( sdsnew ( "##" ), iter_num_str );
					sds save_icon = sdscat ( sdsnew ( "##" ), iter_num_str );
					sds delete_icon = sdscat ( sdsnew ( "##" ), iter_num_str );

					if ( nk_button_label ( ses_ctx.nk_ctx, load_icon ) )
						ses_cfg_load ( &ses_cfg, sdscatfmt ( sdsempty ( ), "%s\\configs\\%s.json", ses_ctx.ses_dir, gui_configs_list [ i ] ) );
					if ( nk_button_label ( ses_ctx.nk_ctx, save_icon ) )
						ses_cfg_save ( &ses_cfg, sdscatfmt ( sdsempty ( ), "%s\\configs\\%s.json", ses_ctx.ses_dir, gui_configs_list [ i ] ) );
					if ( nk_button_label ( ses_ctx.nk_ctx, delete_icon ) ) {
						sds file_dir = sdscatfmt ( sdsempty ( ), "%s\\configs\\%s.json", ses_ctx.ses_dir, gui_configs_list [ i ] );
						remove ( file_dir );
						sdsfree ( file_dir );
						reload_after_done = true;
					}

					nk_style_pop_font ( ses_ctx.nk_ctx );

					sdsfree ( iter_num_str );
					sdsfree ( save_icon );
					sdsfree ( load_icon );
					sdsfree ( delete_icon );
				}

				/* reload configs list */
				if ( reload_after_done )
					gui_reload_config_list ( );
			} nk_group_end ( ses_ctx.nk_ctx );

			const float ratios [ ] = { 0.86f, 0.14f };
			nk_layout_row ( ses_ctx.nk_ctx, NK_DYNAMIC, GUI_LINE_HEIGHT, COUNT_OF ( ratios ), ratios );

			static struct nk_text_edit text_edit = { 0 };
			if ( !text_edit.initialized )
				nk_textedit_init_default ( &text_edit );

			/* we should probably exlude the "." so people know not to manually add the file extensions */
			nk_edit_buffer ( ses_ctx.nk_ctx, NK_EDIT_FIELD, &text_edit, nk_filter_default );

			nk_style_push_font ( ses_ctx.nk_ctx, &ses_ctx.fonts.menu_icons_font->handle );
			if ( nk_button_label ( ses_ctx.nk_ctx, "" ) && text_edit.string.len ) {
				char* config_name_textbox = nk_str_get ( &text_edit.string );
				config_name_textbox [ nk_str_len ( &text_edit.string ) ] = '\0';
				ses_cfg_save ( &ses_cfg, sdscatfmt ( sdsempty ( ), "%s\\configs\\%s.json" ), ses_ctx.ses_dir, config_name_textbox );

				/* reload configs list */
				gui_reload_config_list ( );
			}
			nk_style_pop_font ( ses_ctx.nk_ctx );

			/* close popup if we click outside of it */
			if ( nk_input_is_mouse_pressed ( &ses_ctx.nk_ctx->input, NK_BUTTON_LEFT )
				&& !nk_input_is_mouse_hovering_rect ( &ses_ctx.nk_ctx->input, ses_ctx.nk_ctx->current->bounds )
				&& !opened_popup_this_frame ) {
				nk_popup_close ( ses_ctx.nk_ctx );
				gui_configs_open = false;
				popup_closed = true;
			}
		} nk_popup_end ( ses_ctx.nk_ctx );

		nk_style_pop_style_item ( ses_ctx.nk_ctx );
		nk_style_pop_color ( ses_ctx.nk_ctx );

		struct nk_anim_data* anim_data_subtabs = nk_get_anim ( "subtabs" );
		const struct nk_color dynamic_color_subtabs = nk_do_anim_color( ses_ctx.nk_ctx, &anim_data_subtabs->main_fraction, gui_subtabs_open ? 1.0f : -1.0f, &(struct nk_color){255, 255, 255, 0}, & (struct nk_color){255, 255, 255, 100});

		nk_style_push_color ( ses_ctx.nk_ctx, &ses_ctx.nk_ctx->style.window.background, dynamic_color_subtabs);
		nk_style_push_style_item ( ses_ctx.nk_ctx, &ses_ctx.nk_ctx->style.window.fixed_background, nk_style_item_color ( ses_ctx.nk_ctx->style.window.background ) );
		const float backup_popup_border = ses_ctx.nk_ctx->style.window.popup_border;
		const float backup_button_border = ses_ctx.nk_ctx->style.button.border;
		ses_ctx.nk_ctx->style.window.popup_border = 0.0f;
		ses_ctx.nk_ctx->style.button.border = 0.0f;
		nk_style_push_color(ses_ctx.nk_ctx, &ses_ctx.nk_ctx->style.button.normal.data.color, nk_rgba(0, 0, 0, 0));
		nk_style_push_color(ses_ctx.nk_ctx, &ses_ctx.nk_ctx->style.button.hover.data.color, nk_rgba(0, 0, 0, 0));
		nk_style_push_color(ses_ctx.nk_ctx, &ses_ctx.nk_ctx->style.button.active.data.color, nk_rgba(0, 0, 0, 0));

		/* subtabs popup */
		if (gui_subtabs_list && gui_subtabs_subtitles_list && gui_subtab_count > 0
			&& dynamic_color_subtabs.a > 0 && nk_popup_begin ( ses_ctx.nk_ctx, NK_POPUP_STATIC, "subtabs", NK_WINDOW_NO_SCROLLBAR, gui_subtabs_rect ) ) {
			bool popup_closed = false;
			bool clicked_tab = false;

			/* subtitles */
			nk_row_layout ( ses_ctx.nk_ctx, NK_DYNAMIC, 2.0f, gui_subtab_count, 0 );
			
			nk_style_push_font ( ses_ctx.nk_ctx, &ses_ctx.fonts.menu_xsmall_font->handle );
			for (int i = 0; i < vector_size( gui_subtabs_subtitles_list ); i++) {
				struct nk_anim_data* anim_data = nk_get_anim(gui_subtabs_list[i]);
				/* TODO: FIX LATER -- i cant think of a better color to show unselected items without losing visibility... */
				const struct nk_color text_color = nk_lerp_color ( &( struct nk_color ) { 255, 255, 255, 255 }, & ( struct nk_color ){255, 255, 255, 255}, anim_data->main_fraction );
				nk_style_push_color(ses_ctx.nk_ctx, &ses_ctx.nk_ctx->style.text.color, text_color);

				nk_label ( ses_ctx.nk_ctx, gui_subtabs_subtitles_list[i], NK_TEXT_CENTERED );

				nk_style_pop_color(ses_ctx.nk_ctx);
			}
			nk_style_pop_font ( ses_ctx.nk_ctx );

			/* icons */
			gui_layout ( gui_subtab_count );

			nk_style_push_font ( ses_ctx.nk_ctx, &ses_ctx.fonts.menu_icons_font->handle );
			for ( int i = 0; i < vector_size ( gui_subtabs_list ); i++ ) {
				struct nk_anim_data* anim_data = nk_get_anim ( gui_subtabs_list [ i ] );
				const struct nk_color text_color = nk_do_anim_color ( ses_ctx.nk_ctx, &anim_data->main_fraction, *gui_subtabs_cur_val_ptr == i ? 1.0f : -1.0f, &( struct nk_color ){255, 255, 255, 100}, & ( struct nk_color ){255, 255, 255, 255} );
				nk_style_push_color ( ses_ctx.nk_ctx, &ses_ctx.nk_ctx->style.button.text_active, text_color );
				nk_style_push_color ( ses_ctx.nk_ctx, &ses_ctx.nk_ctx->style.button.text_hover, text_color );
				nk_style_push_color ( ses_ctx.nk_ctx, &ses_ctx.nk_ctx->style.button.text_normal, text_color );

				if ( nk_button_label ( ses_ctx.nk_ctx, gui_subtabs_list [ i ] ) ) {
					*gui_subtabs_cur_val_ptr = i;
					clicked_tab = true;
				}

				nk_style_pop_color ( ses_ctx.nk_ctx );
				nk_style_pop_color ( ses_ctx.nk_ctx );
				nk_style_pop_color ( ses_ctx.nk_ctx );
			}
			nk_style_pop_font(ses_ctx.nk_ctx);

			/* close popup if we click outside of it */
			if ( clicked_tab ||(nk_input_is_mouse_pressed ( &ses_ctx.nk_ctx->input, NK_BUTTON_LEFT )
				&& !nk_input_is_mouse_hovering_rect ( &ses_ctx.nk_ctx->input, ses_ctx.nk_ctx->current->bounds ) ) ) {
				nk_popup_close ( ses_ctx.nk_ctx );
				gui_subtabs_open = false;
				popup_closed = true;
			}
		} nk_popup_end ( ses_ctx.nk_ctx );

		nk_style_pop_color(ses_ctx.nk_ctx);
		nk_style_pop_color(ses_ctx.nk_ctx);
		nk_style_pop_color(ses_ctx.nk_ctx);
		nk_style_pop_style_item ( ses_ctx.nk_ctx );
		nk_style_pop_color ( ses_ctx.nk_ctx );
		ses_ctx.nk_ctx->style.window.popup_border = backup_popup_border;
		ses_ctx.nk_ctx->style.button.border = backup_button_border;
	}

	/* window outline */
	nk_stroke_rect ( &ses_ctx.nk_ctx->current->buffer, *bounds, ses_ctx.nk_ctx->style.window.rounding, ses_ctx.nk_ctx->style.window.border, ses_ctx.nk_ctx->style.window.border_color );

	/* bottom watermark */
	struct nk_color watermark_color = nk_rgba( 255, 255, 255, 66 );

	sds watermark_str = sdscat( sdscat ( sdscat ( sdsempty ( ), "Copyright (c) 2021 " ), gui_title ), ".");

	const size_t watermark_len = nk_strlen ( watermark_str );
	const float watermark_w = ses_ctx.fonts.menu_xsmall_font->handle.width ( ses_ctx.fonts.menu_xsmall_font->handle.userdata, ses_ctx.fonts.menu_xsmall_font->handle.height, watermark_str, title_len );

	nk_draw_text ( &ses_ctx.nk_ctx->current->buffer, nk_rect ( bounds->x + GUI_MENU_PADDING, bounds->y + bounds->h - ses_ctx.fonts.menu_xsmall_font->handle.height - GUI_MENU_PADDING, watermark_w, ses_ctx.fonts.menu_xsmall_font->handle.height ), watermark_str, watermark_len, ses_ctx.fonts.menu_xsmall_font, watermark_color );
	sdsfree ( watermark_str );

	struct nk_vec2 window_pos = nk_window_get_position ( ses_ctx.nk_ctx );
	
	gui_last_rect = *bounds;
	gui_last_rect.x = window_pos.x;
	gui_last_rect.y = window_pos.y;

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