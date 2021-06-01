#include "gui/menu.h"
#include "gui/config.h"

#include "ses.h"

#define NK_IMPLEMENTATION
#include "nuklear/nuklear.h"
#define NK_D3D9_IMPLEMENTATION
#include "nuklear/nuklear_d3d9.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "gui/custom.h"

#include "resources/resources.h"

typedef enum {
    gui_tabs_rage = 0,
    gui_tabs_antiaim,
    gui_tabs_legit,
    gui_tabs_visuals,
    gui_tabs_misc,
    gui_tabs_code,
    gui_tabs_max,
} gui_tabs;

static gui_tabs menu_cur_tab = gui_tabs_rage;

typedef enum {
    gui_subtabs_rage_auto = 0,
    gui_subtabs_rage_awp,
    gui_subtabs_rage_scout,
    gui_subtabs_rage_rifle,
    gui_subtabs_rage_shotgun,
    gui_subtabs_rage_pistol,
    gui_subtabs_rage_revolver,
    gui_subtabs_rage_deagle,
    gui_subtabs_rage_exploits,
    gui_subtabs_rage_max,
} gui_subtabs_rage;

typedef enum {
    gui_subtabs_antiaim_stand = 0,
    gui_subtabs_antiaim_air,
    gui_subtabs_antiaim_run,
    gui_subtabs_antiaim_walk,
    gui_subtabs_antiaim_slowwalk,
    gui_subtabs_antiaim_crouch,
    gui_subtabs_antiaim_other,
    gui_subtabs_antiaim_max,
} gui_subtabs_antiaim;

typedef enum {
    gui_subtabs_legit_auto = 0,
    gui_subtabs_legit_awp,
    gui_subtabs_legit_scout,
    gui_subtabs_legit_rifle,
    gui_subtabs_legit_shotgun,
    gui_subtabs_legit_pistol,
    gui_subtabs_legit_revolver,
    gui_subtabs_legit_deagle,
    gui_subtabs_legit_max,
} gui_subtabs_legit;

typedef enum {
    gui_subtabs_visuals_players = 0,
    gui_subtabs_visuals_viewmodel,
    gui_subtabs_visuals_weapons,
    gui_subtabs_visuals_world,
    gui_subtabs_visuals_other,
    gui_subtabs_visuals_max,
} gui_subtabs_visuals;

typedef enum {
    gui_subtabs_misc_movement = 0,
    gui_subtabs_misc_skins,
    gui_subtabs_misc_other,
    gui_subtabs_misc_max,
} gui_subtabs_misc;

/* generic subtabs type */
static int menu_cur_subtab[ gui_tabs_max ] = { 0 };

static bool menu_open = false;
static struct nk_rect menu_pos = { 50.0f, 50.0f, 530.0f, 435.0f };
static struct nk_image menu_logo;

static inline struct nk_image image_load( uint8_t* data, size_t size ) {
    int x = 0, y = 0, n = 0;
    const uint8_t* image_raw = stbi_load_from_memory( data, size, &x, &y, &n, 4 );

    IDirect3DTexture9* tex = NULL;
    D3DLOCKED_RECT rect;

    IDirect3DDevice9_CreateTexture( cs_id3ddev, x, y, 0, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &tex, NULL );
    IDirect3DTexture9_LockRect( tex, 0, &rect, NULL, D3DLOCK_DISCARD );
    memcpy( rect.pBits, image_raw, x * y * 4 );
    IDirect3DTexture9_UnlockRect( tex, 0 );

    stbi_image_free( image_raw );

    return nk_image_id( ( int )tex );
}

static inline void menu_set_theme( ) {
    struct nk_color table[ NK_COLOR_COUNT ];

    table[ NK_COLOR_TEXT ] = nk_rgba( 255, 255, 255, 255 );
    table[ NK_COLOR_WINDOW ] = nk_rgba( 61, 71, 106, 255 );
    table[ NK_COLOR_HEADER ] = nk_rgba( 33, 42, 54, 255 );
    table[ NK_COLOR_BORDER ] = nk_rgba( 255, 255, 255, 33 );
    table[ NK_COLOR_BUTTON ] = table[ NK_COLOR_TOGGLE_CURSOR ] = table[ NK_COLOR_SELECT_ACTIVE ]
        = table[ NK_COLOR_SLIDER_CURSOR ] = table[ NK_COLOR_CHART_COLOR ] = table[ NK_COLOR_SCROLLBAR_CURSOR ]
        = table[ NK_COLOR_TAB_HEADER ] = nk_rgba( 204, 82, 224, 255 );
    table[ NK_COLOR_BUTTON_HOVER ] = table[ NK_COLOR_SLIDER_CURSOR_ACTIVE ] = table[ NK_COLOR_SCROLLBAR_CURSOR_ACTIVE ] = nk_rgba( 204, 82, 224, 255 );
    table[ NK_COLOR_BUTTON_ACTIVE ] = nk_rgba( 0, 123, 255, 255 );
    table[ NK_COLOR_TOGGLE ] = table[ NK_COLOR_PROPERTY ]
        = table[ NK_COLOR_EDIT ] = table[ NK_COLOR_COMBO ] = table[ NK_COLOR_CHART ]
        = table[ NK_COLOR_SCROLLBAR ] = nk_rgba( 48, 55, 81, 255 );
    table[ NK_COLOR_SLIDER ] = nk_rgba( 255, 255, 255, 255 );
    table[ NK_COLOR_TOGGLE_HOVER ] = nk_rgba( 0, 123, 255, 255 );
    table[ NK_COLOR_SELECT ] = nk_rgba( 0, 123, 255, 255 );
    table[ NK_COLOR_SLIDER_CURSOR_HOVER ] = nk_rgba( 0, 123, 255, 255 );
    table[ NK_COLOR_EDIT_CURSOR ] = nk_rgba( 210, 210, 210, 255 );
    table[ NK_COLOR_CHART_COLOR_HIGHLIGHT ] = nk_rgba( 204, 82, 224, 255 );
    table[ NK_COLOR_SCROLLBAR_CURSOR_HOVER ] = nk_rgba( 0, 123, 255, 255 );

    nk_style_from_table( ses_ctx.nk_ctx, table );
}

bool menu_is_open( ) {
    return menu_open;
}

void menu_set_opened( bool open ) {
    menu_open = open;
}

void menu_release( ) {
    nk_d3d9_release( );
}

void menu_reset( ) {
    int w = 0, h = 0;
    iengine_get_screen_size( cs_iengine, &w, &h );
    nk_d3d9_resize( w, h );
}

/* @emizu03 (my brother) */
/* NOTE: k and c can be tweaked if results are not ideal */
static void font_scale_brightness_func( unsigned char* map, int map_size ) {
    const float k = 10.0f;
    const float c = 0.4f;

    for ( int i = 0; i < map_size; i++ ) {
        const float norm = ( float )i / ( float )( map_size - 1 );
        const float func = 1.0f / ( 1.0f + expf( -k * ( norm - c ) ) );
        map[ i ] = ( unsigned char )( int )clampf( roundf( func * ( float )( map_size - 1 ) ), 0.0f, ( float )( map_size - 1 ) );
    }
}

void menu_init( ) {
    CLEAR_START;
    VM_SHARK_BLACK_START;
    STR_ENCRYPT_START;

    if ( !ses_ctx.nk_ctx ) {
        int w = 0, h = 0;
        iengine_get_screen_size( cs_iengine, &w, &h );

        ses_ctx.nk_ctx = nk_d3d9_init( cs_id3ddev, w, h );

        static const nk_rune all_font_range[ ] = {
            0x0020, 0xFFFF,
            0
        };

        static const nk_rune fontawesome_font_range[ ] = {
            0xF000, 0xF8FF,
            0
        };

        static struct nk_font_atlas* atlas = NULL;
        nk_d3d9_font_stash_begin( &atlas );

        struct nk_font_config config = { 0 };

        /* menu fonts */
        config = nk_font_config( 18.0f );
        config.range = all_font_range;
        config.oversample_v = 2;
        config.oversample_h = 2;
        config.scale_brightness_func = font_scale_brightness_func;
        config.pixel_snap = false;
        ses_ctx.fonts.menu_icons_font = nk_font_atlas_add_compressed( atlas, resources_fontawesome_compressed_data, resources_fontawesome_compressed_size, 18.0f, &config );

        config = nk_font_config( 26.0f );
        config.range = all_font_range;
        config.oversample_v = 2;
        config.oversample_h = 2;
        config.scale_brightness_func = font_scale_brightness_func;
        config.pixel_snap = false;
        ses_ctx.fonts.menu_icons_large_font = nk_font_atlas_add_compressed( atlas, resources_fontawesome_compressed_data, resources_fontawesome_compressed_size, 26.0f, &config );

        config = nk_font_config( 27.0f );
        config.range = all_font_range;
        config.oversample_v = 2;
        config.oversample_h = 2;
        config.scale_brightness_func = font_scale_brightness_func;
        config.pixel_snap = false;
        ses_ctx.fonts.menu_large_font = nk_font_atlas_add_compressed( atlas, resources_ubuntu_compressed_data, resources_ubuntu_compressed_size, 27.0f, &config );

        config = nk_font_config( 16.0f );
        config.range = nk_font_default_glyph_ranges( );
        config.oversample_v = 2;
        config.oversample_h = 2;
        config.scale_brightness_func = font_scale_brightness_func;
        config.pixel_snap = false;
        ses_ctx.fonts.menu_medium_font = nk_font_atlas_add_compressed( atlas, resources_ubuntu_compressed_data, resources_ubuntu_compressed_size, 16.0f, &config );

        config = nk_font_config( 13.0f );
        config.range = nk_font_default_glyph_ranges( );
        config.oversample_v = 2;
        config.oversample_h = 2;
        config.scale_brightness_func = font_scale_brightness_func;
        config.pixel_snap = false;
        ses_ctx.fonts.menu_small_font = nk_font_atlas_add_compressed( atlas, resources_ubuntu_compressed_data, resources_ubuntu_compressed_size, 13.0f, &config );

        config = nk_font_config( 12.0f );
        config.range = nk_font_default_glyph_ranges( );
        config.oversample_v = 2;
        config.oversample_h = 2;
        config.scale_brightness_func = font_scale_brightness_func;
        config.pixel_snap = false;
        ses_ctx.fonts.menu_xsmall_font = nk_font_atlas_add_compressed( atlas, resources_ubuntu_compressed_data, resources_ubuntu_compressed_size, 12.0f, &config );

        /* other fonts */
        config = nk_font_config( 16.0f );
        config.range = nk_font_default_glyph_ranges( );
        config.oversample_v = 2;
        config.oversample_h = 2;
        config.scale_brightness_func = font_scale_brightness_func;
        config.pixel_snap = false;
        ses_ctx.fonts.default_font = nk_font_atlas_add_compressed( atlas, resources_ubuntu_compressed_data, resources_ubuntu_compressed_size, 16.0f, &config );

        /* visuals fonts */
        config = nk_font_config( 16.0f );
        config.range = all_font_range;
        config.oversample_v = 2;
        config.oversample_h = 2;
        config.scale_brightness_func = font_scale_brightness_func;
        config.pixel_snap = false;
        ses_ctx.fonts.esp_font = nk_font_atlas_add_compressed( atlas, resources_ubuntu_compressed_data, resources_ubuntu_compressed_size, 16.0f, &config );

        config = nk_font_config( 26.0f );
        config.range = nk_font_default_glyph_ranges( );
        config.oversample_v = 2;
        config.oversample_h = 2;
        config.scale_brightness_func = font_scale_brightness_func;
        config.pixel_snap = false;
        ses_ctx.fonts.indicators_font = nk_font_atlas_add_compressed( atlas, resources_ubuntu_compressed_data, resources_ubuntu_compressed_size, 26.0f, &config );

        nk_d3d9_font_stash_end( );
        nk_style_set_font( ses_ctx.nk_ctx, &ses_ctx.fonts.default_font->handle );

        menu_logo = image_load( resources_logo_data, resources_logo_size );

        menu_set_theme( );
    }

    STR_ENCRYPT_END;
    VM_SHARK_BLACK_END;
    CLEAR_END;
}

void menu_free( ) {
    nk_d3d9_shutdown( );
}

void menu_draw( ) {
    VM_TIGER_WHITE_START;
    STR_ENCRYPT_START;

    menu_set_opened( utils_keybind_active( VK_INSERT, keybind_mode_toggle ) );

    if ( menu_is_open( ) ) {
        if ( gui_begin( "SESAME", &menu_pos, NK_WINDOW_NO_SCROLLBAR ) ) {
            if ( gui_tabs_begin( gui_tabs_max ) ) {
                gui_tab( "", &menu_cur_tab );
                gui_tab( "", &menu_cur_tab );
                gui_tab( "", &menu_cur_tab );
                gui_tab( "", &menu_cur_tab );
                gui_tab( "", &menu_cur_tab );
                gui_tab( "##", &menu_cur_tab );
            } gui_tabs_end( );

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

            switch ( menu_cur_tab ) {
            case gui_tabs_rage: {
                // TODO: EXPLOITS IS THE ONLY ICON IN RAGE TO USE THE BUG ICON: ""
                // TODO: YOU CAN ADD A WEAPON ICON TO TABS LIKE THIS:
                // "A##WeaponIcon" Where "A" is the character for the weapon icon in the font
            } break;
            case gui_tabs_antiaim: {
                if ( gui_subtabs_begin( gui_subtabs_antiaim_max ) ) {
                    gui_subtab( "", "stand", &menu_cur_subtab[ gui_tabs_antiaim ] );
                    gui_subtab( "", "air", &menu_cur_subtab[ gui_tabs_antiaim ] );
                    gui_subtab( "", "run", &menu_cur_subtab[ gui_tabs_antiaim ] );
                    gui_subtab( "", "walk", &menu_cur_subtab[ gui_tabs_antiaim ] );
                    gui_subtab( "", "swalk", &menu_cur_subtab[ gui_tabs_antiaim ] );
                    gui_subtab( "", "duck", &menu_cur_subtab[ gui_tabs_antiaim ] );
                    gui_subtab( "", "other", &menu_cur_subtab[ gui_tabs_antiaim ] );
                }
                gui_subtabs_end( );

                switch ( menu_cur_subtab[ gui_tabs_antiaim ] ) {
                case gui_subtabs_antiaim_stand: {

                } break;
                case gui_subtabs_antiaim_air: {

                } break;
                case gui_subtabs_antiaim_run: {

                } break;
                case gui_subtabs_antiaim_walk: {

                } break;
                case gui_subtabs_antiaim_slowwalk: {

                } break;
                case gui_subtabs_antiaim_crouch: {

                } break;
                case gui_subtabs_antiaim_other: {

                } break;
                }
            } break;
            case gui_tabs_legit: {
                // TODO: SIMILAR TO RAGE
            } break;
            case gui_tabs_visuals: {
                if ( gui_subtabs_begin( gui_subtabs_visuals_max ) ) {
                    gui_subtab( "", "player", &menu_cur_subtab[ gui_tabs_visuals ] );
                    gui_subtab( "", "vmodel", &menu_cur_subtab[ gui_tabs_visuals ] );
                    gui_subtab( "", "weapon", &menu_cur_subtab[ gui_tabs_visuals ] );
                    gui_subtab( "", "world", &menu_cur_subtab[ gui_tabs_visuals ] );
                    gui_subtab( "", "other", &menu_cur_subtab[ gui_tabs_visuals ] );
                }
                gui_subtabs_end( );

                switch ( menu_cur_subtab[ gui_tabs_visuals ] ) {
                case gui_subtabs_visuals_players: {

                } break;
                case gui_subtabs_visuals_viewmodel: {

                } break;
                case gui_subtabs_visuals_weapons: {

                } break;
                case gui_subtabs_visuals_world: {

                } break;
                case gui_subtabs_visuals_other: {

                } break;
                }
            } break;
            case gui_tabs_misc: {
                if ( gui_subtabs_begin( gui_subtabs_misc_max ) ) {
                    gui_subtab( "", "move", &menu_cur_subtab[ gui_tabs_misc ] );
                    gui_subtab( "", "skins", &menu_cur_subtab[ gui_tabs_misc ] );
                    gui_subtab( "", "other", &menu_cur_subtab[ gui_tabs_misc ] );
                }
                gui_subtabs_end( );

                switch ( menu_cur_subtab[ gui_tabs_misc ] ) {
                case gui_subtabs_misc_movement: {
                    gui_header( "Misc", "Movement" );

                    gui_checkbox( "Auto Jump", ses_cfg_get_item( &ses_cfg, misc, movement, auto_jump, bool ) );

                    if ( gui_group_begin( ) ) {
                        gui_combobox( "Auto Strafe", ( const char* [ ] ) { "None", "Legit", "Directional", "Rage", NULL }, ses_cfg_get_item( &ses_cfg, misc, movement, auto_strafer, int ) );
                    } gui_group_end( );

                    gui_checkbox( "Fast Stop", ses_cfg_get_item( &ses_cfg, misc, movement, fast_stop, bool ) );
                    gui_checkbox( "Checkbox", ses_cfg_get_item( &ses_cfg, gui, state, test_bool, bool ) );
                    gui_sliderf( "Slider Float", -180.0f, ses_cfg_get_item( &ses_cfg, gui, state, test_float, float ), 180.0f, 1.0f, NULL );
                    gui_slider( "Slider Int", 0, ses_cfg_get_item( &ses_cfg, gui, state, test_int, int ), 100, 5, NULL );
                } break;
                case gui_subtabs_misc_skins: {

                } break;
                case gui_subtabs_misc_other: {

                } break;
                }
            } break;
            case gui_tabs_code: {
                // TODO
            } break;
            default: break;
            }
        }

        gui_end( );
    }

    STR_ENCRYPT_END;
    VM_TIGER_WHITE_END;
}