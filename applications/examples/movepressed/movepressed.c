/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

void movepressed(void)
{
  static float x = 0.2 ;

  ZMW(zmw_window("MovePressed"))
    {
      ZMW(zmw_vbox())
	{
	  zmw_hscrollbar(&x, 0.1) ;
	  zmw_button("Menu 1") ;
	  ZMW( zmw_menu() )
	    ZMW(zmw_window_popup_bottom())
	      zmw_button("Action 1") ;
	}
    }
  ZMW(zmw_window("MovePressed"))
    {
      zmw_button("Menu 2") ;
      ZMW( zmw_menu() )
	ZMW(zmw_window_popup_bottom())
	   zmw_button("Action 2") ;
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(movepressed) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 26 36        # Scrollbar
zmw_dump_screen 0

zmw_button_press
zmw_move_cursor_to 103 35  # Menu 2
zmw_dump_screen 1

zmw_move_cursor_to 33 59  # Menu 1
zmw_dump_screen 2

zmw_button_release

REGRESSION TEST */
