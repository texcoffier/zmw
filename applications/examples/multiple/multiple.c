/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void multiple(void)
{
  int i ;

  ZMW(zmw_window("Multiple"))
    {
      ZMW(zmw_vbox())
	{
	  zmw_label("Very Bad code:") ;
	  i = 0 ;
	  ZMW(zmw_decorator(Zmw_Decorator_Border_Solid))
	    {
	      ZMW(zmw_vbox())
		{
		  zmw_int(i) ;
		  i++ ;
		  zmw_int(i) ;
		  i++ ;
		}
	    }		  
	  zmw_label("Bad code:") ;
	  ZMW(zmw_decorator(Zmw_Decorator_Border_Solid))
	    {
	      i = 0 ;
	      ZMW(zmw_vbox())
		{
		  zmw_int(i) ;
		  i++ ;
		  zmw_int(i) ;
		  i++ ;
		}
	    }		  
	  zmw_label("Good code:") ;
	  ZMW(zmw_decorator(Zmw_Decorator_Border_Solid))
	    {
	      ZMW(zmw_vbox())
		{
		  i = 0 ;
		  zmw_int(i) ;
		  i++ ;
		  zmw_int(i) ;
		  i++ ;
		}
	    }		  
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(multiple) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 20 20
zmw_dump_screen 0

REGRESSION TEST */
