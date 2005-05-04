/* DO NOT DISPLAY */
#include "zmw/zmw.h"
#include <errno.h>
/* DO NOT DISPLAY */

void message(void)
{
  static int message_visible = Zmw_False ;

  ZMW(zmw_window("Message"))
    {
      zmw_button("Popup") ;
      if ( zmw_activated() )
	message_visible = Zmw_True ;

      ZMW( message_visible = zmw_message(message_visible,
					 "Message window",
					 "Close window") )
	{
	  zmw_vertical_alignment(0) ;
	  zmw_vertical_expand(Zmw_False) ;
	  ZMW( zmw_hbox() )
	    {
	      zmw_image_from_file("warning.png") ;
	      zmw_label("Why did you click?") ;
	    }
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(message) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 28 37 # The boutton
zmw_dump_screen 0

zmw_button_click
zmw_dump_screen 1

zmw_move_cursor_to 160 111 # The quit boutton
zmw_button_click
zmw_dump_screen 2


REGRESSION TEST */
