/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void viewport(void)
{
  static Zmw_Float_0_1 x=0, y=0 ;
  int i ;
  static int toggle[20] = { 0 } ;
  char tmp[99] ;

  ZMW(zmw_window("Viewport"))
    {
      ZMW(zmw_vbox())
	{
	  // If the size is not set, the viewport take
	  // the size of its content.
	  zmw_height(100) ;
	  zmw_width(90) ;
	  ZMW(zmw_viewport_with_scrollbar(&x, &y))
	    {
	      ZMW(zmw_vbox())
		{
		  for(i=0; i<sizeof(toggle)/sizeof(toggle[0]); i++)
		    {
		      sprintf(tmp, "i is equal to %d", i) ;
		      zmw_check_button_int_with_label(&toggle[i], tmp) ;
		    }
		}
	    }
	  zmw_height(ZMW_VALUE_UNDEFINED) ;
	  zmw_width(ZMW_VALUE_UNDEFINED) ;
	  sprintf(tmp, "x = %g", x) ;
	  zmw_label(tmp) ;
	  sprintf(tmp, "y = %g", y) ;
	  zmw_label(tmp) ;	  
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(viewport) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 94 67 # middle vertical scrollbar
zmw_dump_screen 0

zmw_button_click
zmw_move_cursor_to 38 121 # middle horizontal scrollbar
zmw_button_click
zmw_dump_screen 1

zmw_move_cursor_to 17 69 # toggle 10
zmw_button_click
zmw_dump_screen 2

REGRESSION TEST */
