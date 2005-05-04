/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void accel_menu(void)
{
  static int i = 0 ;

  ZMW(zmw_window("Accelerator"))
    {
      ZMW(zmw_vbox())
	{
	  zmw_button("Menu") ;
	  ZMW( zmw_menu() )
	    {
	      ZMW( zmw_window_popup_right() )
		{
		  zmw_button_with_accelerator("++", 0, 'A') ;
		  if ( zmw_activated() )
		    i++ ;
		}
	    }
	  zmw_int(i) ;
	}
    }
}

/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(accel_menu) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 40 30 # On "Menu"
zmw_dump_screen 0

zmw_button_click
zmw_dump_screen 1

zmw_move_cursor_to 87 35 # On "Go"
zmw_button_click
zmw_dump_screen 2

zmw_move_cursor_to 30 59 # On the integer
zmw_key $KEY_A
zmw_dump_screen 3

REGRESSION TEST */
