#include "include/gui/menu.h"
#include "include/gui/config.h"

#include "include/ses.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_BOOL
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
//#define NK_BUTTON_TRIGGER_ON_RELEASE
#define NK_IMPLEMENTATION
#include "lib/nuklear/nuklear.h"
#define NK_D3D9_IMPLEMENTATION
#include "lib/nuklear/nuklear_d3d9.h"

#include "include/resources/resources.h"

static bool menu_open = false;

static inline void menu_set_theme ( ) {
    struct nk_color table [ NK_COLOR_COUNT ];

    table [ NK_COLOR_TEXT ] = nk_rgba ( 255, 255, 255, 255 );
    table [ NK_COLOR_WINDOW ] = nk_rgba ( 32, 38, 45, 255 );
    table [ NK_COLOR_HEADER ] = nk_rgba ( 33, 42, 54, 255 );
    table [ NK_COLOR_BORDER ] = nk_rgba ( 0, 123, 255, 200 );
    table [ NK_COLOR_BUTTON ] = table [ NK_COLOR_TOGGLE_CURSOR ] = table [ NK_COLOR_SELECT_ACTIVE ]
        = table [ NK_COLOR_SLIDER_CURSOR ] = table [ NK_COLOR_CHART_COLOR ] = table [ NK_COLOR_SCROLLBAR_CURSOR ]
        = table [ NK_COLOR_TAB_HEADER ] = nk_rgba ( 204, 82, 224, 255 );
    table [ NK_COLOR_BUTTON_HOVER ] = table [ NK_COLOR_SLIDER_CURSOR_ACTIVE ] = table [ NK_COLOR_SCROLLBAR_CURSOR_ACTIVE ] = nk_rgba ( 204, 82, 224, 255 );
    table [ NK_COLOR_BUTTON_ACTIVE ] = nk_rgba ( 0, 123, 255, 255 );
    table [ NK_COLOR_TOGGLE ] = table [ NK_COLOR_SLIDER ] = table [ NK_COLOR_PROPERTY ]
        = table [ NK_COLOR_EDIT ] = table [ NK_COLOR_COMBO ] = table [ NK_COLOR_CHART ]
        = table [ NK_COLOR_SCROLLBAR ] = nk_rgba ( 33, 42, 54, 255 );
    table [ NK_COLOR_TOGGLE_HOVER ] = nk_rgba ( 0, 123, 255, 255 );
    table [ NK_COLOR_SELECT ] = nk_rgba ( 0, 123, 255, 255 );
    table [ NK_COLOR_SLIDER_CURSOR_HOVER ] = nk_rgba ( 0, 123, 255, 255 );
    table [ NK_COLOR_EDIT_CURSOR ] = nk_rgba ( 210, 210, 210, 255 );
    table [ NK_COLOR_CHART_COLOR_HIGHLIGHT ] = nk_rgba ( 204, 82, 224, 255 );
    table [ NK_COLOR_SCROLLBAR_CURSOR_HOVER ] = nk_rgba ( 0, 123, 255, 255 );

    nk_style_from_table ( ses_ctx.nk_ctx, table );
}

bool menu_is_open ( ) {
    return menu_open;
}

void menu_set_opened ( bool open ) {
    menu_open = open;
}

void menu_release ( ) {
    nk_d3d9_release ( );
}

void menu_reset ( ) {
    int w = 0, h = 0;
    iengine_get_screen_size ( cs_iengine, &w, &h );
    nk_d3d9_resize ( w, h );
}

void menu_init ( ) {
    CLEAR_START;
    VM_SHARK_BLACK_START;
    STR_ENCRYPT_START;

    if ( !ses_ctx.nk_ctx ) {
        int w = 0, h = 0;
        iengine_get_screen_size ( cs_iengine, &w, &h );

        ses_ctx.nk_ctx = nk_d3d9_init ( cs_id3ddev, w, h );

        static const nk_rune custom_font_range[] = {
            //0x0020, 0x00FF,
            //0x0102, 0x0103,
            //0x0110, 0x0111,
            //0x0128, 0x0129,
            //0x0168, 0x0169,
            //0x01A0, 0x01A1,
            //0x01AF, 0x01B0,
            //0x0400, 0x052F,
            //0x1EA0, 0x1EF9,
            //0x2DE0, 0x2DFF,
            //0x3000, 0x30FF,
            //0x31F0, 0x31FF,
            //0x4e00, 0x9FAF,
            //0xA640, 0xA69F,
            //0xFF00, 0xFFEF,
            0x0020, 0xFFFF,
            0
        };

        static struct nk_font_atlas* atlas = NULL;
        nk_d3d9_font_stash_begin ( &atlas );
       
        struct nk_font_config config = nk_font_config ( 18.0f );
        config.pixel_snap = true;
        config.range = custom_font_range;
        ses_ctx.fonts.menu_font = nk_font_atlas_add_compressed ( atlas, resources_noto_compressed_data, resources_noto_compressed_size, 18.0f, &config );
       
        config = nk_font_config ( 16.0f );
        config.pixel_snap = true;
        config.range = nk_font_default_glyph_ranges ( );
        ses_ctx.fonts.default_font = nk_font_atlas_add_compressed ( atlas, resources_noto_compressed_data, resources_noto_compressed_size, 16.0f, &config );

        config = nk_font_config ( 16.0f );
        config.pixel_snap = true;
        config.range = custom_font_range;
        ses_ctx.fonts.esp_font = nk_font_atlas_add_compressed ( atlas, resources_noto_compressed_data, resources_noto_compressed_size, 16.0f, &config );

        config = nk_font_config ( 26.0f );
        config.pixel_snap = true;
        config.range = nk_font_default_glyph_ranges();
        ses_ctx.fonts.indicators_font = nk_font_atlas_add_compressed ( atlas, resources_noto_compressed_data, resources_noto_compressed_size, 26.0f, &config );
        
        nk_d3d9_font_stash_end ( );
        nk_style_set_font ( ses_ctx.nk_ctx, &ses_ctx.fonts.default_font->handle );
        
        menu_set_theme( );
    }

    STR_ENCRYPT_END;
    VM_SHARK_BLACK_END;
    CLEAR_END;
}

void menu_free ( ) {
    nk_d3d9_shutdown ( );
}

void menu_draw ( ) {
    VM_TIGER_WHITE_START;
    STR_ENCRYPT_START;

    menu_set_opened ( utils_keybind_active ( VK_INSERT, keybind_mode_toggle ) );

    if ( menu_is_open ( ) ) {
        nk_style_push_font ( ses_ctx.nk_ctx, &ses_ctx.fonts.menu_font->handle );
        if ( nk_begin ( ses_ctx.nk_ctx, "Sesame", nk_rect ( 300, 300, 420, 420 ),
            NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_TITLE | NK_WINDOW_SCROLL_AUTO_HIDE ) )
        {
            const auto line_height = 26.0f;

            nk_layout_row_dynamic ( ses_ctx.nk_ctx, line_height, 1 );

            if ( nk_button_label ( ses_ctx.nk_ctx, "Reload Theme" ) )
                menu_set_theme ( );

            if ( nk_button_label ( ses_ctx.nk_ctx, "Save Config" ) )
                ses_cfg_save ( &ses_cfg, sdsnew ("D:\\Documents\\test.json") );

            if ( nk_button_label ( ses_ctx.nk_ctx, "Load Config" ) )
                ses_cfg_load ( &ses_cfg, sdsnew("D:\\Documents\\test.json") );

            nk_checkbox_label ( ses_ctx.nk_ctx, "Fast stop", ses_cfg_get_item ( &ses_cfg, misc, movement, fast_stop, bool ) );
            nk_checkbox_label ( ses_ctx.nk_ctx, "Auto jump", ses_cfg_get_item ( &ses_cfg, misc, movement, auto_jump, bool ) );
            nk_label ( ses_ctx.nk_ctx, "Auto strafe", NK_TEXT_LEFT );
            nk_combobox( ses_ctx.nk_ctx, (const char*[] ) { "None", "Legit", "Directional", "Rage" }, 4, ses_cfg_get_item ( &ses_cfg, misc, movement, auto_strafer, int ), line_height, nk_vec2( nk_widget_width ( ses_ctx.nk_ctx ), 200.0F) );

            nk_checkbox_label ( ses_ctx.nk_ctx, "Checkbox", ses_cfg_get_item ( &ses_cfg, gui, state, test_bool, bool ) );
            nk_label ( ses_ctx.nk_ctx, "Slider float", NK_TEXT_LEFT );
            nk_slider_float ( ses_ctx.nk_ctx, -180.0f, ses_cfg_get_item ( &ses_cfg, gui, state, test_float, float ), 180.0f, 2.0f );
            nk_label ( ses_ctx.nk_ctx, "Slider int", NK_TEXT_LEFT );
            nk_slider_int ( ses_ctx.nk_ctx, 0, ses_cfg_get_item ( &ses_cfg, gui, state, test_int, int ), 100, 1 );
        }

        nk_end ( ses_ctx.nk_ctx );
        nk_style_pop_font ( ses_ctx.nk_ctx );
    }

    STR_ENCRYPT_END;
    VM_TIGER_WHITE_END;
}