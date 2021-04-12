#ifndef GUI_MENU_H
#define GUI_MENU_H

#include <stdbool.h>

bool menu_is_open ( );
void menu_set_opened ( bool open );
void menu_release ( );
void menu_reset ( );
void menu_free ( );
void menu_init ( );
void menu_draw ( );

#endif // !GUI_MENU_H