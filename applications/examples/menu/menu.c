/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

void menu(int *detached)
{
  ZMW(zmw_window_popup_bottom_with_detached_and_title(detached, "Menu"))
    {
      ZMW(zmw_box_vertical())
	{
	  zmw_tearoff_with_detached(detached) ;
	  zmw_button("An action") ;
	}
    }
}

void window(void)
{
  static int detached = 0 ;

  ZMW(zmw_window("Window"))
    {
      ZMW(zmw_box_vertical())
	{
	  zmw_button("Menu") ;
	  if ( zmw_window_is_popped_with_detached(&detached) )
	    menu(&detached) ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_run(window) ;
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
