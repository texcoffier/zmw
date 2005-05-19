/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

void submenu()
{
  ZMW(zmw_window_popup_right_with_title("Submenu"))
    {
      ZMW(zmw_vbox())
	{
	  zmw_tearoff() ;
	  zmw_button("Action 2") ;
	  zmw_button("Action 3") ;
	}
    }
}

void menu()
{
  static int boolean ;
  static char *text = NULL ;

  if ( text == NULL )
    text = strdup("Editable text") ;

  ZMW(zmw_window_popup_bottom_with_title("Menu"))		
    {
      ZMW(zmw_vbox())
	{
	  zmw_tearoff() ;
	  zmw_button("Submenu") ;
	  ZMW ( zmw_menu() )
	    submenu() ;
	  zmw_button("An action") ;
	  zmw_check_button_int_with_label(&boolean, "Toggle") ;
	  zmw_entry(&text) ;
	}
    }
}

void window(void)
{
  ZMW(zmw_window("Window"))
    {
      ZMW(zmw_vbox())
	{
	  zmw_button("Menu") ;
	  ZMW ( zmw_menu() )
	    menu() ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(window) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 26 36        # Menu
zmw_dump_screen 0

zmw_button_press
zmw_dump_screen 1

zmw_move_cursor_to 59 97        # Submenu
zmw_dump_screen 2

zmw_button_release
zmw_dump_screen 3

zmw_move_cursor_to 50 68        # Detach
zmw_button_click
zmw_dump_screen 4

zmw_move_cursor_to 142 141 # Text
zmw_button_click
zmw_key $KEY_A
zmw_move_cursor_to 93 106 # Toggle
zmw_button_click                      # Push the Toggle
zmw_move_cursor_to 126 55 # Submenu
zmw_button_click
zmw_dump_screen 5

zmw_move_cursor_to 87 33 # Attach
zmw_button_click
zmw_move_cursor_to 29 35 # Menu
zmw_button_press
zmw_move_cursor_to 44 130 # Toggle
zmw_button_release                    # Pop the Toggle
zmw_dump_screen 6

zmw_move_cursor_to 29 35 # Menu
zmw_button_click
zmw_move_cursor_to 54 35 # DO NOT GO IN DIAGONAL !
zmw_move_cursor_to 54 171 # Text
zmw_button_press
zmw_move_cursor_to 80 171 # Text
zmw_dump_screen 7

zmw_button_release
zmw_key $KEY_A
zmw_dump_screen 8

zmw_move_cursor_to 67 88 # Submenu
zmw_button_click
zmw_move_cursor_to 176 92 # Detach of submenu
zmw_button_click
zmw_dump_screen 9


REGRESSION TEST */
