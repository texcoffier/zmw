/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

void menu4()
{
  ZMW(zmw_window("Menu"))
    {
      zmw_text("Hello") ;
    }
}

void window(void)
{
  static int detached = 0 ;

  ZMW(zmw_window("Window"))
    {
      ZMW(zmw_box_vertical())
	{
	  zmw_button("Menu") ;
	  if ( zmw_activated() )
	    detached = 1 - detached ;

	  ZMW(zmw_if(detached))
	    menu4() ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_run(window) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_dump_screen 0



REGRESSION TEST */
