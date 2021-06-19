#ifndef GUI_MENU_H
#define GUI_MENU_H

#include <stdbool.h>

bool menu_is_open ( void );
void menu_set_opened ( bool open );
void menu_release ( void );
void menu_reset ( void );
void menu_free ( void );
void menu_init ( void );
void menu_draw ( void );

#endif // !GUI_MENU_H
