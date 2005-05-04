/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void boxactivable(void)
{
  static int a=0 ;

  zmw_padding_width(0) ;
  ZMW(zmw_window("BoxActivable"))
    {
      zmw_horizontal_alignment(0) ;
      zmw_vertical_expand(0) ;

      ZMW(zmw_decorator(Zmw_Decorator_Border_Relief))
	{
	  ZMW(zmw_hbox_activable())
	    {
	      a = zmw_check_button(a) ;
	      zmw_label("XXXX") ;
	    }
	  if ( zmw_activated() )
	    a = 1 ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(boxactivable) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 18 34
zmw_dump_screen 0

zmw_button_click
zmw_dump_screen 1

zmw_move_cursor_to 30 34
zmw_button_click
zmw_dump_screen 2

REGRESSION TEST */
