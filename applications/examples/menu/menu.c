/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

void menu()
{
  ZMW(zmw_window_popup_bottom_with_title("Menu"))
    {
      ZMW(zmw_vbox())
	{
	  zmw_tearoff() ;
	  zmw_button("An action") ;
	  if ( zmw_activated() )
	    printf("Action!\n") ;
	}
    }
}

void window(void)
{
  static int detached = 0 ;

  ZMW(zmw_window("Window"))
    {
      ZMW(zmw_vbox())
	{
	  zmw_button("Menu") ;
	  ZMW(zmw_menu_with_detached(&detached))
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

zmw_button_click
zmw_dump_screen 1

zmw_move_cursor_to 26 66        # Detach
zmw_button_click
zmw_dump_screen 2



REGRESSION TEST */
