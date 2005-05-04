/* DO NOT DISPLAY */
#include "zmw/zmw.h"
#include <math.h>
/* DO NOT DISPLAY */

unsigned int rnd1(int i)
{
  i = i + 7 ;
  return i*i*i ;
}
unsigned int rnd2(int i)
{
  i = i + 7 ;
  return i*i*i*i ;
}

void table2(void)
{
  ZMW(zmw_window("Table"))
    {
      //   zmw_horizontal_expand(Zmw_False) ;
      //   zmw_vertical_expand(Zmw_False) ;
      ZMW(zmw_vbox())
	{
	  ZMW(zmw_table(1))
	    {
	      zmw_label("III") ;
	      ZMW(zmw_decorator(Zmw_Decorator_Border_Relief))
		zmw_label("X") ;
	    }
	  ZMW(zmw_table(2))
	    {
	      zmw_label("III") ;
	      ZMW(zmw_decorator(Zmw_Decorator_Border_Relief))
		zmw_label("X") ;
	    }
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(table2) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 45 34
zmw_dump_screen 0

REGRESSION TEST */
