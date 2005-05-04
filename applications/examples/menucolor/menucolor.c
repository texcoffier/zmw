/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */


void menucolor(void)
{
  ZMW(zmw_window("Menu"))
    {
      ZMW(zmw_vbox())
	{
	  zmw_button("Menu") ;
	  ZMW ( zmw_menu() )
	    {
	      ZMW(zmw_window_popup_right())
		{
		  zmw_rgb(1,0,0) ;
		  zmw_button("red button") ;
		}
	    }
	  zmw_button("same color has previous") ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(menucolor) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 26 36
zmw_button_click
zmw_dump_screen 0




REGRESSION TEST */
