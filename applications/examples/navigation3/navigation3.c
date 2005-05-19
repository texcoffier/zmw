/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

void navigation3(void)
{
  static int value = 2005 ;

  ZMW(zmw_window("Navigation 3"))
    {
      ZMW(zmw_vbox())
	{
	  zmw_button("Menu") ;
	  ZMW(zmw_menu())
	    ZMW(zmw_window_popup_bottom())
	    {
	      ZMW(zmw_vbox())
		{
		  zmw_button("Incremente value") ;
		  if ( zmw_activated() )
		    value++ ;
		  zmw_button("Sub menu 1") ;
		  ZMW(zmw_menu())
		    ZMW(zmw_window_popup_right())
		    {
		      zmw_button("value += 10") ;
		      if ( zmw_activated() )
			value += 10 ;
		    }
		  zmw_button("Sub menu 2") ;
		  ZMW(zmw_menu())
		    ZMW(zmw_window_popup_right())
		    {
		      zmw_button("value -= 10") ;
		      if ( zmw_activated() )
			value -= 10 ;
		    }
		}
	    }
	  zmw_label("The value") ;
	  zmw_int_editable(&value) ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(navigation3) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 21 85
zmw_button_click
zmw_dump_screen 0

zmw_key_press $KEY_CONTROL
zmw_key $KEY_UP
zmw_key_release $KEY_CONTROL
zmw_dump_screen 1

zmw_key $KEY_A
zmw_dump_screen 2

zmw_key $KEY_DOWN
zmw_dump_screen 3

zmw_key $KEY_A
zmw_dump_screen 4

zmw_key $KEY_UP    # Should not be necessary : bug
zmw_sleep $SMALL2
zmw_key $KEY_A
zmw_sleep $SMALL2
zmw_key $KEY_DOWN
zmw_sleep $SMALL2
zmw_key $KEY_DOWN
zmw_sleep $SMALL2
zmw_key $KEY_A
zmw_dump_screen 5

zmw_key $KEY_DOWN
zmw_sleep $SMALL2
zmw_key $KEY_A
zmw_dump_screen 6

zmw_key $KEY_RIGHT
zmw_sleep $SMALL2
zmw_key $KEY_A
zmw_dump_screen 7

zmw_move_cursor_to 34 37
zmw_dump_screen 8

zmw_button_click
zmw_key $KEY_A
zmw_dump_screen 9

zmw_key $KEY_DOWN
zmw_dump_screen 10

zmw_key $KEY_ESCAPE
zmw_dump_screen 11

zmw_button_click
zmw_dump_screen 12

zmw_key $KEY_ESCAPE
zmw_dump_screen 13


REGRESSION TEST */
