/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void box2(void)
{
  static Zmw_Float_0_1 x = 0.4, y = 0.33 ;

  ZMW(zmw_window("Box"))
    {
      zmw_width(150) ;
      zmw_height(150) ;

      ZMW(zmw_vbox())
	{
	  zmw_horizontal_expand(Zmw_True) ;
	  zmw_vertical_expand(Zmw_False) ;
	  zmw_hscrollbar(&x,0.1) ;
	  zmw_vertical_expand(Zmw_True) ;
	  ZMW(zmw_hbox())
	    {
	      zmw_horizontal_expand(Zmw_False) ;
	      zmw_vscrollbar(&y,0.1) ;
	      zmw_horizontal_expand(Zmw_True) ;
	      ZMW(zmw_fixed())
		{
		  zmw_x(x*100) ;
		  zmw_y(y*100) ;
		  zmw_label("Here") ;

		  zmw_x(x*100 - 20) ;
		  zmw_y(y*100 + 20) ;
		  zmw_label("UnderLeft of 'Here'") ;
		}
	    }
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(box2) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 45 34
zmw_dump_screen 0

zmw_button_press
zmw_dump_screen 1

zmw_move_cursor_to 20 34
zmw_dump_screen 2


REGRESSION TEST */
