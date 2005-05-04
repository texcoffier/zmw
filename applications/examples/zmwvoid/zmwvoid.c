/* DO NOT DISPLAY */
#include "zmw/zmw.h"

void zmwvoid(void)
{
  ZMW(zmw_window("Void"))
    {
      ZMW(zmw_vbox())
	{
	  zmw_button("Grey") ;
	  ZMW(zmw_void())
	    {
	      zmw_rgb(0.8,.5,.5) ;
	      zmw_button("Red") ;
	    }
	  ZMW(zmw_void())
	    {
	      zmw_rgb(0.5,.8,.5) ;
	      ZMW(zmw_window("Green"))
		{
		  zmw_rgb(0.5,0.5,0.8) ;
		  zmw_button("Blue button") ;
		}
	    }
	  zmw_button("Same Grey") ;
	  ZMW(zmw_void())
	    ZMW(zmw_void())
	      zmw_label("Void in Void") ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(zmwvoid) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 30 30
zmw_dump_screen 0


REGRESSION TEST */
