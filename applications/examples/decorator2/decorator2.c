/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

void clip(void)
{
  ZMW_EXTERNAL_RESTART ;
  ZMW(zmw_decorator(Zmw_Border_Solid|Zmw_Decorator_Clip))
    {
      ZMW_EXTERNAL ;
    }
  ZMW_EXTERNAL_STOP ;
}

void decorator2(void)
{
  ZMW(zmw_window("Decorator2"))
    {
      ZMW(zmw_hbox())
	{
	  zmw_label("left") ;
	  zmw_width(100) ;
	  ZMW(zmw_decorator(Zmw_Decorator_Clip|Zmw_Decorator_Interior
			    |Zmw_Border_Solid))
	    {
	      zmw_horizontal_expand(Zmw_False) ;
	      zmw_horizontal_alignment(-1) ;
	      ZMW(zmw_hbox())
		{
		  zmw_label("L") ;
		  ZMW(zmw_decorator(Zmw_Decorator_Clip|Zmw_Decorator_Interior))
		    zmw_label("A clipped text because it is long") ;
		  zmw_label("R") ;
		}
	    }
	  zmw_width(ZMW_VALUE_UNDEFINED) ;
	  zmw_label("middle") ;
	  zmw_width(100) ;
	  ZMW(zmw_decorator(Zmw_Decorator_Clip|Zmw_Decorator_Interior
			    |Zmw_Border_Solid))
	    zmw_label("A clipped text because it is long") ;
	  zmw_width(ZMW_VALUE_UNDEFINED) ;
	  zmw_label("right") ;
	  zmw_width(50) ;
	  ZMW(clip())
	    zmw_label("ABCDEFGHIJKL") ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(decorator2) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 30 30
zmw_dump_screen 0

REGRESSION TEST */
