/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

void boxed_text(const char *text, int i)
{
  ZMW(zmw_decorator(Zmw_Decorator_Border_Embossed))
    {
      if ( i == 2 )
	{
	  zmw_horizontal_expand(Zmw_False) ;
	  zmw_vertical_expand(Zmw_False) ;
	}
      zmw_text(text) ;
    }
}

void box(void)
{
  int i ;
  char tmp[99] ;

  ZMW(zmw_window("Boxes"))
    {
      ZMW(zmw_box_vertical())
	{
	  for(i=0; i<3; i++) // Only i=1 is False
	    {
	      zmw_horizontal_expand(Zmw_True) ;
	      ZMW(zmw_decorator(Zmw_Decorator_Interior
				|Zmw_Decorator_Border_In))
		{
		  zmw_horizontal_expand(Zmw_False) ;
		  zmw_font("12x24") ;
		  sprintf(tmp, "i = %d", i ) ;
		  zmw_text(tmp) ;
		}

	      zmw_horizontal_expand(i) ;
	      zmw_vertical_expand(i) ;

	      ZMW(zmw_box_vertical())
		{
		  zmw_horizontal_alignment(-1) ;
		  boxed_text("Left", i) ;
		  zmw_horizontal_alignment(0) ;
		  boxed_text("Horizontal Center", i) ;
		  zmw_horizontal_alignment(1) ;
		  boxed_text("Right", i) ;
		  zmw_height(40) ;
		  ZMW(zmw_box_horizontal())
		    {
		      zmw_vertical_alignment(-1) ;
		      boxed_text("Top", i) ;
		      zmw_vertical_alignment(0) ;
		      boxed_text("Middle", i) ;
		      zmw_vertical_alignment(1) ;
		      boxed_text("Bottom", i) ;
		    }
		}
	    }
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_run(box) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 26 36
zmw_dump_screen 0




REGRESSION TEST */
