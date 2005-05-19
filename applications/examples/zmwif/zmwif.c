/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

void zmwif(void)
{
  static int visible1 = 1, visible2 = 1, visible3 = 1 ;
  int v ;

  v = 0 ;
  ZMW(zmw_window("If"))
    {
      ZMW(zmw_vbox())
	{
	  zmw_check_button_int_with_label(&visible1, "Next text visible") ;
	  ZMW( zmw_if(visible1) )
	    zmw_label("I am the text") ;
	  zmw_check_button_int_with_label(&visible2, "Window visible") ;
	  ZMW( zmw_if(visible2) )
	    ZMW(zmw_window("Title"))
	    {
	      v = 1 ;
	      zmw_label("Content") ;
	    }
	  zmw_int(v) ;
	  zmw_check_button_int_with_label(&visible3, "Next void is visible") ;
	  ZMW( zmw_if(visible3) )
	    ZMW(zmw_void())
	        zmw_label("text in void") ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(zmwif) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 83 41 # first toggle
zmw_dump_screen 0

zmw_button_click
zmw_dump_screen 1

zmw_move_cursor_to 83 57 # second toggle
zmw_button_click
zmw_dump_screen 2

zmw_move_cursor_to 83 41 # first toggle
zmw_button_click
zmw_dump_screen 3

zmw_move_cursor_to 83 111 # third toggle
zmw_button_click
zmw_dump_screen 4

REGRESSION TEST */
