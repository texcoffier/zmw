/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

void bug3(void)
{
  ZMW(zmw_window("Bug3"))
    {
      ZMW(zmw_vbox())
	{
	  zmw_width(100) ;
	  zmw_label("text") ;
	  ZMW(zmw_hbox())
	    {
	      zmw_label("i") ;
	      zmw_label("j") ;
	    }
	}
    }
}

/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(bug3) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 20 36
zmw_dump_screen 0


REGRESSION TEST */
