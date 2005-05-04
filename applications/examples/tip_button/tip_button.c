/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void tip_button(void)
{
  static int b = -1 ;

  ZMW(zmw_window("Anchor"))
    {
      ZMW( zmw_vbox() )
	{
	  zmw_button("Menu 1") ;
	  ZMW( zmw_menu() )
	    ZMW(zmw_window_popup_bottom())
	    zmw_label("Item 1") ;
	  ZMW( zmw_tip() )
	    ZMW(zmw_window_popup_right())
	    zmw_label("tip 1") ;
	  if ( zmw_activated() )
	    b = 1 ;

	  zmw_button("Menu 2") ;
	  ZMW( zmw_tip() )
	    ZMW(zmw_window_popup_right())
	    zmw_label("tip 2") ;
	  ZMW( zmw_menu() )
	    ZMW(zmw_window_popup_bottom())
	    zmw_label("Item 2") ;
	  if ( zmw_activated() )
	    b = 2 ;

	  zmw_label("Activ.") ;
	  zmw_int(b) ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(tip_button) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 34 38 # Menu 1
zmw_dump_screen 0

zmw_sleep 1
zmw_dump_screen 1

zmw_button_click
zmw_dump_screen 2

zmw_move_cursor_to 35 38 # Menu 1
zmw_sleep 1
zmw_dump_screen 3

zmw_move_cursor_to 51 62 # Menu 2
zmw_sleep 1
zmw_dump_screen 4

zmw_button_click
zmw_dump_screen 5

zmw_move_cursor_to 52 62 # Menu 2
zmw_sleep 1
zmw_dump_screen 6


REGRESSION TEST */
