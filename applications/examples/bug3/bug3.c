/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

void bug3(void)
{
  ZMW(zmw_window("Bug3"))
    {
      ZMW(zmw_box_vertical())
	{
	  zmw_width(100) ;
	  zmw_text("text") ;
	  ZMW(zmw_box_horizontal())
	    {
	      zmw_text("i") ;
	      zmw_text("j") ;
	    }
	}
    }
}

/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_run(bug3) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 20 36
zmw_dump_screen 0


REGRESSION TEST */
