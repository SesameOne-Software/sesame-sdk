#include "include/gui/menu.h"
#include "lib/csgo-sdk/sdk.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_BOOL
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_BUTTON_TRIGGER_ON_RELEASE
#define NK_IMPLEMENTATION
#include "lib/nuklear/nuklear.h"
#define NK_D3D9_IMPLEMENTATION
#include "lib/nuklear/nuklear_d3d9.h"
#include "lib/nuklear/nuklear_style.h"

#include "include/resources/resources.h"

static bool menu_open = false;
static struct nk_colorf bg;
struct nk_context* nk_ctx = NULL;

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
    if ( !nk_ctx ) {
        int w = 0, h = 0;
        iengine_get_screen_size ( cs_iengine, &w, &h );

        nk_ctx = nk_d3d9_init ( cs_id3ddev, w, h );

        struct nk_font_atlas* atlas = NULL;
        nk_d3d9_font_stash_begin ( &atlas );
        struct nk_font* droid = nk_font_atlas_add_from_memory ( atlas, resources_roboto, sizeof ( resources_roboto ), 16.0f, NULL );
        nk_d3d9_font_stash_end ( );
        nk_style_set_font ( nk_ctx, &droid->handle );

        set_style ( nk_ctx, THEME_DARK );
    }
}

void menu_free ( ) {
    nk_d3d9_shutdown ( );
}

void menu_draw ( ) {
    menu_init ( );

    menu_set_opened ( utils_keybind_active ( VK_INSERT, keybind_mode_toggle ) );

    if ( menu_is_open ( ) ) {
        if ( nk_begin ( nk_ctx, "Demo", nk_rect ( 50, 50, 230, 250 ),
            NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
            NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE ) )
        {
            enum { EASY, HARD };
            static int op = EASY;
            static int property = 20;

            nk_layout_row_static ( nk_ctx, 30, 80, 1 );
            if ( nk_button_label ( nk_ctx, "button" ) );
            nk_layout_row_dynamic ( nk_ctx, 30, 2 );
            if ( nk_option_label ( nk_ctx, "easy", op == EASY ) ) op = EASY;
            if ( nk_option_label ( nk_ctx, "hard", op == HARD ) ) op = HARD;
            nk_layout_row_dynamic ( nk_ctx, 22, 1 );
            nk_property_int ( nk_ctx, "Compression:", 0, &property, 100, 10, 1 );

            nk_layout_row_dynamic ( nk_ctx, 20, 1 );
            nk_label ( nk_ctx, "background:", NK_TEXT_LEFT );
            nk_layout_row_dynamic ( nk_ctx, 25, 1 );
            if ( nk_combo_begin_color ( nk_ctx, nk_rgb_cf ( bg ), nk_vec2 ( nk_widget_width ( nk_ctx ), 400 ) ) ) {
                nk_layout_row_dynamic ( nk_ctx, 120, 1 );
                bg = nk_color_picker ( nk_ctx, bg, NK_RGBA );
                nk_layout_row_dynamic ( nk_ctx, 25, 1 );
                bg.r = nk_propertyf ( nk_ctx, "#R:", 0, bg.r, 1.0f, 0.01f, 0.005f );
                bg.g = nk_propertyf ( nk_ctx, "#G:", 0, bg.g, 1.0f, 0.01f, 0.005f );
                bg.b = nk_propertyf ( nk_ctx, "#B:", 0, bg.b, 1.0f, 0.01f, 0.005f );
                bg.a = nk_propertyf ( nk_ctx, "#A:", 0, bg.a, 1.0f, 0.01f, 0.005f );
                nk_combo_end ( nk_ctx );
            }
        }

        nk_end ( nk_ctx );
    }

    nk_input_begin ( nk_ctx );

    nk_d3d9_render ( NK_ANTI_ALIASING_ON );
}