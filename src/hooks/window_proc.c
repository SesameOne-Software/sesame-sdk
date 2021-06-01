#include "hooks/hooks.h"

#include "gui/menu.h"
#include "nuklear/nuklear.h"

WNDPROC hooks_backup_window_proc = NULL;

extern int nk_d3d9_handle_event( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );

__attribute__( ( stdcall ) ) LRESULT window_proc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
	bool skip_mouse_input_processing = false;

	switch ( uMsg ) {
	case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
	case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK: {
		int button = 0;
		if ( uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK ) { button = 0; }
		if ( uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONDBLCLK ) { button = 1; }
		if ( uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONDBLCLK ) { button = 2; }
		if ( uMsg == WM_XBUTTONDOWN || uMsg == WM_XBUTTONDBLCLK ) { button = ( GET_XBUTTON_WPARAM( wParam ) == XBUTTON1 ) ? 3 : 4; }
		mouse_down[ button ] = true;
		skip_mouse_input_processing = true;
		break;
	}
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_XBUTTONUP: {
		int button = 0;
		if ( uMsg == WM_LBUTTONUP ) { button = 0; }
		if ( uMsg == WM_RBUTTONUP ) { button = 1; }
		if ( uMsg == WM_MBUTTONUP ) { button = 2; }
		if ( uMsg == WM_XBUTTONUP ) { button = ( GET_XBUTTON_WPARAM( wParam ) == XBUTTON1 ) ? 3 : 4; }
		mouse_down[ button ] = false;
		skip_mouse_input_processing = true;
		break;
	}
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if ( wParam < 256 )
			key_down[ wParam ] = true;
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		if ( wParam < 256 )
			key_down[ wParam ] = false;
		break;
	case WM_MOUSEWHEEL:
		break;
	}

	nk_d3d9_handle_event( hWnd, uMsg, wParam, lParam );

	if ( menu_is_open( ) && ( ( skip_mouse_input_processing || wParam <= VK_XBUTTON2 ) || ( uMsg == WM_MOUSEWHEEL ) ) )
		return true;

	return CallWindowProcA( hooks_backup_window_proc, hWnd, uMsg, wParam, lParam );
}