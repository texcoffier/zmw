/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

void popup(void)
{
  ZMW(zmw_window("popup"))
    {
      ZMW(zmw_vbox())
	{
	  zmw_label("X") ;
	  zmw_label("Y") ;
	  ZMW( zmw_window_popup_right() )
	    zmw_label("r") ;
	  ZMW( zmw_window_popup_bottom() )
	    zmw_label("b") ;
	  zmw_label("Z") ;
	}
    }
}

/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(popup) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 20 20
zmw_dump_screen 0



REGRESSION TEST */
