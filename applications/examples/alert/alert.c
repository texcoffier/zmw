/* DO NOT DISPLAY */
#include "zmw/zmw.h"
#include <errno.h>
/* DO NOT DISPLAY */

void alert(void)
{
  static char *message = NULL ;

  ZMW(zmw_window("Boxes"))
    {
      ZMW(zmw_vbox())
	{
	  zmw_button("Do not click on this button") ;
	  if ( zmw_activated() )
	    message = "I said to not click on this button" ;
	  zmw_button("This button do nothing") ;
	  if ( zmw_activated() )
	    message = "It really do nothing!" ;

	  zmw_alert(&message) ;		  
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(alert) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 77 41 # First boutton
zmw_dump_screen 0

zmw_button_click
zmw_dump_screen 1

zmw_move_cursor_to 89 65 # Second boutton
zmw_button_click
zmw_dump_screen 2

zmw_move_cursor_to 302 77 # OK Button
zmw_button_click
zmw_dump_screen 3




REGRESSION TEST */
