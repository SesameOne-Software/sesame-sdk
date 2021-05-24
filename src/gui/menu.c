#include "include/gui/menu.h"
#include "include/gui/config.h"

#include "include/ses.h"

#define NK_IMPLEMENTATION
#include "lib/nuklear/nuklear.h"
#define NK_D3D9_IMPLEMENTATION
#include "lib/nuklear/nuklear_d3d9.h"

#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb/stb_image.h"

#include "include/gui/custom.h"

#include "include/resources/resources.h"

static int cur_tab = 0;
static bool menu_open = false;
static struct nk_rect menu_pos = { 50.0f, 50.0f, 530.0f, 435.0f };
static struct nk_image menu_logo;

static inline struct nk_image image_load(uint8_t* data, size_t size) {
    int x = 0, y = 0, n = 0;
    const uint8_t* image_raw = stbi_load_from_memory(data, size, &x, &y, &n, 4);

    IDirect3DTexture9* tex = NULL;
    D3DLOCKED_RECT rect;

    IDirect3DDevice9_CreateTexture(cs_id3ddev, x, y, 0, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &tex, NULL);
    IDirect3DTexture9_LockRect(tex, 0, &rect, NULL, D3DLOCK_DISCARD);
    memcpy(rect.pBits, image_raw, x * y * 4);
    IDirect3DTexture9_UnlockRect(tex, 0);

    stbi_image_free(image_raw);

    return nk_image_id((int)tex);
}

static inline void menu_set_theme ( ) {
    struct nk_color table [ NK_COLOR_COUNT ];

    table [ NK_COLOR_TEXT ] = nk_rgba ( 255, 255, 255, 255 );
    table [ NK_COLOR_WINDOW ] = nk_rgba ( 61, 71, 106, 255 );
    table [ NK_COLOR_HEADER ] = nk_rgba ( 33, 42, 54, 255 );
    table [ NK_COLOR_BORDER ] = nk_rgba ( 255, 255, 255, 33 );
    table [ NK_COLOR_BUTTON ] = table [ NK_COLOR_TOGGLE_CURSOR ] = table [ NK_COLOR_SELECT_ACTIVE ]
        = table [ NK_COLOR_SLIDER_CURSOR ] = table [ NK_COLOR_CHART_COLOR ] = table [ NK_COLOR_SCROLLBAR_CURSOR ]
        = table [ NK_COLOR_TAB_HEADER ] = nk_rgba ( 204, 82, 224, 255 );
    table [ NK_COLOR_BUTTON_HOVER ] = table [ NK_COLOR_SLIDER_CURSOR_ACTIVE ] = table [ NK_COLOR_SCROLLBAR_CURSOR_ACTIVE ] = nk_rgba ( 204, 82, 224, 255 );
    table [ NK_COLOR_BUTTON_ACTIVE ] = nk_rgba ( 0, 123, 255, 255 );
    table [ NK_COLOR_TOGGLE ] = table [ NK_COLOR_PROPERTY ]
        = table [ NK_COLOR_EDIT ] = table [ NK_COLOR_COMBO ] = table [ NK_COLOR_CHART ]
        = table [ NK_COLOR_SCROLLBAR ] = nk_rgba (48, 55, 81, 255 );
    table[NK_COLOR_SLIDER] = nk_rgba(255, 255, 255, 255);
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
       
        /*
        struct nk_font* menu_icons_font;
		struct nk_font* menu_large_font;
		struct nk_font* menu_medium_font;
		struct nk_font* menu_small_font;
        */

        struct nk_font_config config = nk_font_config ( 18.0f );
        config.oversample_v = config.oversample_h = 1;
        config.pixel_snap = true;
        
        /* menu fonts */
        config = nk_font_config ( 18.0f );
        config.range = custom_font_range;
        ses_ctx.fonts.menu_icons_font = nk_font_atlas_add_compressed ( atlas, resources_fontawesome_compressed_data, resources_fontawesome_compressed_size, 18.0f, &config );
        
        config = nk_font_config ( 26.0f );
        config.range = custom_font_range;
        ses_ctx.fonts.menu_icons_large_font = nk_font_atlas_add_compressed ( atlas, resources_fontawesome_compressed_data, resources_fontawesome_compressed_size, 26.0f, &config );

        config = nk_font_config ( 27.0f );
        config.range = nk_font_default_glyph_ranges ( );
        ses_ctx.fonts.menu_large_font = nk_font_atlas_add_compressed ( atlas, resources_ubuntu_compressed_data, resources_ubuntu_compressed_size, 27.0f, &config );
        
        config = nk_font_config ( 16.5f );
        config.range = nk_font_default_glyph_ranges ( );
        ses_ctx.fonts.menu_medium_font = nk_font_atlas_add_compressed ( atlas, resources_ubuntu_compressed_data, resources_ubuntu_compressed_size, 16.5f, &config );

        config = nk_font_config ( 13.5f );
        config.range = nk_font_default_glyph_ranges ( );
        ses_ctx.fonts.menu_small_font = nk_font_atlas_add_compressed ( atlas, resources_ubuntu_compressed_data, resources_ubuntu_compressed_size, 13.5f, &config );
        
        config = nk_font_config ( 12.0f );
        config.range = nk_font_default_glyph_ranges ( );
        ses_ctx.fonts.menu_xsmall_font = nk_font_atlas_add_compressed ( atlas, resources_ubuntu_compressed_data, resources_ubuntu_compressed_size, 12.0f, &config );

        /* other fonts */
        config = nk_font_config ( 16.0f );
        config.range = nk_font_default_glyph_ranges ( );
        ses_ctx.fonts.default_font = nk_font_atlas_add_compressed ( atlas, resources_ubuntu_compressed_data, resources_ubuntu_compressed_size, 16.0f, &config );

        /* visuals fonts */
        config = nk_font_config ( 16.0f );
        config.range = custom_font_range;
        ses_ctx.fonts.esp_font = nk_font_atlas_add_compressed ( atlas, resources_ubuntu_compressed_data, resources_ubuntu_compressed_size, 16.0f, &config );

        config = nk_font_config ( 26.0f );
        config.range = nk_font_default_glyph_ranges();
        ses_ctx.fonts.indicators_font = nk_font_atlas_add_compressed ( atlas, resources_ubuntu_compressed_data, resources_ubuntu_compressed_size, 26.0f, &config );
        
        nk_d3d9_font_stash_end ( );
        nk_style_set_font ( ses_ctx.nk_ctx, &ses_ctx.fonts.default_font->handle );

        menu_logo = image_load(resources_logo_data, resources_logo_size);

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
        if ( gui_begin ( "SESAME", &menu_pos, NK_WINDOW_NO_SCROLLBAR ) ) {
           if ( gui_tabs_begin ( 5 ) ) {
               gui_tab ( "", &cur_tab );
               gui_tab ( "", &cur_tab );
               gui_tab ( "", &cur_tab );
               gui_tab ( "", &cur_tab );
               gui_tab ( "", &cur_tab );
           }
           gui_tabs_end ( );

            if ( gui_button ( "Reload Theme" ) )
                menu_set_theme ( );

            //const float backup_row_height = ses_ctx.nk_ctx->current->layout->row.height;
            //ses_ctx.nk_ctx->current->layout->row.height = 100.0f;
            //nk_image(ses_ctx.nk_ctx, menu_logo);
            //ses_ctx.nk_ctx->current->layout->row.height = backup_row_height;

            //const player* local = cs_get_local();
            //if (local) {
            //    const sds desync_str = sdscatprintf(sdsempty(), "Desync: %.2f", player_get_max_desync(local));
            //    nk_label(ses_ctx.nk_ctx, desync_str, NK_LEFT);
            //    sdsfree(desync_str);
            //}
            
            gui_checkbox ( "Fast stop", ses_cfg_get_item ( &ses_cfg, misc, movement, fast_stop, bool ) );
            gui_checkbox ( "Auto jump", ses_cfg_get_item ( &ses_cfg, misc, movement, auto_jump, bool ) );
            gui_combobox ("Auto strafe", (const char*[] ) { "None", "Legit", "Directional", "Rage", NULL }, ses_cfg_get_item ( &ses_cfg, misc, movement, auto_strafer, int ) );
            gui_checkbox ( "Checkbox", ses_cfg_get_item ( &ses_cfg, gui, state, test_bool, bool ) );
            gui_sliderf ("Slider float", -180.0f, ses_cfg_get_item ( &ses_cfg, gui, state, test_float, float ), 180.0f, 1.0f, NULL );
            gui_slider ( "Slider int", 0, ses_cfg_get_item ( &ses_cfg, gui, state, test_int, int ), 100, 5, NULL );
        }

        gui_end ( );
    }

    STR_ENCRYPT_END;
    VM_TIGER_WHITE_END;
}