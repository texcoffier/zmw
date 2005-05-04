#include "zmw/zmw.h"

void hello_world(void)
{
  ZMW(zmw_window("My Window"))
    {
      ZMW(zmw_vbox())
	{
	  zmw_label("Hello World!") ;
	  zmw_button("Quit") ;
	  if ( zmw_activated() )
	    zmw_main_quit(0) ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(hello_world) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 34 43          # On quit button
zmw_dump_screen 0                 # Dump screen

zmw_button_press
zmw_dump_screen 1                 # Dump screen : button pressed

zmw_button_release
zmw_dump_screen 2                 # Dump screen : no more application

REGRESSION TEST */
