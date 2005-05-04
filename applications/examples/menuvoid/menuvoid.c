/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

/*
 * This program shows a library bug.
 * As this program is not clean (bugged?) I do not correct the bug.
 *
 * The menu stay popped if it contains a detached.
 *
 * zmw_menu() indicates DO_NOT_MAP_WINDOW to its child
 * but its child is zmw_void and not a zmw_window
 * So the window is mapped....
 */

void menuvoid(void)
{
  static int i = 0 ;

  ZMW(zmw_window("Menu void"))
    {
      ZMW(zmw_vbox())
	{
	  zmw_button("Menu") ;
	  zmw_name("popup") ;
	  ZMW(zmw_menu())
	    {
	      ZMW( zmw_void() ) // Only to trigger the bug
		{
		  ZMW(zmw_window_popup_right_with_title("Menu"))
		    {
		      zmw_button("Item1") ;
		      ZMW(zmw_menu())
			{
			  ZMW(zmw_window_popup_right_with_title("Menu"))
			    {
			      zmw_tearoff() ;
			    }
			}
		    }
		}
	    }
	  zmw_int_editable(&i) ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(menuvoid) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_small_scale 2

zmw_move_cursor_to 31 38        # Menu
zmw_button_click
zmw_move_cursor_to 67 36        # Item 1
zmw_button_click
zmw_move_cursor_to 140 41       # Detach
zmw_button_click
zmw_dump_screen 0

REGRESSION TEST */
