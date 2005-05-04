/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

/* Only one accelerator for each key */

void accel_menu2(void)
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
	  zmw_button_with_hidden_accelerator("--", 0, 'A') ;
	  if ( zmw_activated() )
	    i-- ;
	}
    }
}

/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(accel_menu2) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 40 30
zmw_dump_screen 0

zmw_key $KEY_A
zmw_dump_screen 1


REGRESSION TEST */
