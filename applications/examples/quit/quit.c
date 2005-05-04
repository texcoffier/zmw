#include "zmw/zmw.h"

void hello_world(void)
{
  static GdkWindow *id_window = NULL, *id_tip = NULL ;
  
  ZMW(zmw_window_with_id(&id_window, "My Window"))
    {
      ZMW(zmw_vbox())
	{
	  zmw_label("Hello World!") ;
	  zmw_button("Quit") ;
	  if ( zmw_activated() )
	    zmw_main_quit(0) ;
	  ZMW( zmw_tip() )
	    {
	      zmw_border_width(2) ;
	      ZMW(zmw_window_popup_right_with_id(&id_tip))
		{
		  ZMW(zmw_decorator(Zmw_Decorator_Border_Solid))
		    {
		      zmw_label("Click here to close the application") ;
		    }
		}
	    }
	}
    }
}

int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(hello_world) ;
  return 0 ;
}
/* REGRESSION TEST

zmw_move_cursor_to 34 43          # On quit button
zmw_sleep 1                       # Some time for the tip to appear
zmw_dump_screen 0                 # Dump screen

zmw_button_press
zmw_dump_screen 1                 # Dump screen : button pressed

zmw_button_release
zmw_dump_screen 2                 # Dump screen : no more application

REGRESSION TEST */
