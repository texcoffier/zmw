/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

void popdown(void)
{
  static char *text = NULL ;
  static int toggle = 0, radio = 0 ;
  static float x ;

  if ( text == NULL )
    text = strdup("editable text") ;

  ZMW(zmw_window("Popdown"))
    {
      ZMW(zmw_vbox())
	{
	  zmw_button("Menu") ;
	  ZMW( zmw_menu() )
	    ZMW(zmw_window_popup_right())
	    {
	      ZMW(zmw_vbox())
		{
		  zmw_button("An action") ;
		}
	    }
	  zmw_entry(&text) ;
	  zmw_check_button_int_with_label(&toggle, "toggle with label") ;
	  zmw_check_button_int(&toggle) ;
	  zmw_hscrollbar(&x, 0.1) ;
	  zmw_button("button unsensible") ;
	  zmw_button("button sensible") ;
	  if ( zmw_activated() )
	    fprintf(stderr, "Hello\n") ;
	  zmw_radio_button(&radio, 0) ;
	  zmw_radio_button(&radio, 1) ;
	  zmw_radio_button_with_label(&radio, 0, "lab1") ;
	  zmw_radio_button_with_label(&radio, 1, "lab2") ;
	}
    }
}

/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(popdown) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

popupmenu() { zmw_move_cursor_to 69 37 ; zmw_button_click ; }

zmw_move_cursor_to 69 37        # Menu
zmw_dump_screen 0

popupmenu
zmw_move_cursor_to 123 63 # text
zmw_button_click
zmw_dump_screen 1

popupmenu
zmw_move_cursor_to 20 87 # Toggle with label : on toggle
zmw_button_click
zmw_dump_screen 2

popupmenu
zmw_move_cursor_to 71 87 # Toggle with label : on button
zmw_button_click
zmw_dump_screen 3

popupmenu
zmw_move_cursor_to 46 108 # Big Toggle
zmw_button_click
zmw_dump_screen 4

popupmenu
zmw_move_cursor_to 58 125 # Scrollbar
zmw_button_click
zmw_dump_screen 5

popupmenu
zmw_move_cursor_to 70 149 # Unsensible button
zmw_button_click
zmw_dump_screen 6

popupmenu
zmw_move_cursor_to 71 174 # Sensible button
zmw_button_click
zmw_dump_screen 7

popupmenu
zmw_move_cursor_to 70 214 # Radio 2
zmw_button_click
zmw_dump_screen 8

popupmenu
zmw_move_cursor_to 60 232 # Radio with label 1
zmw_button_click
zmw_dump_screen 9


REGRESSION TEST */
