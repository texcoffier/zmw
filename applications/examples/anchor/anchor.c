/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void anchor(void)
{
  static int anchor_x = 100 ;
  static Zmw_Float_0_1 sb2_x = 0.5, sb2_y = 0.1 ;
  char tmp[99] ;

  ZMW(zmw_window("Anchor"))
    {
      ZMW(zmw_box_vertical())
	{
	  // Add a frame around the box
	  ZMW(zmw_decorator(Zmw_Decorator_Border_Embossed))
	    {
	      // Box without automatic widget placement
	      ZMW(zmw_box())
		{
		  zmw_height(20) ;
		  zmw_y(0) ;
		  zmw_x(0) ;
		  zmw_text("Anchor-->") ;
		  zmw_x(anchor_x) ;
		  zmw_anchor_vertical(&anchor_x) ;
		  zmw_x(anchor_x+10) ;
		  zmw_int_editable(&anchor_x) ;
		}
	    }

	  sprintf(tmp, "sb2_x=%g sb2_y=%g", sb2_x, sb2_y) ;
	  zmw_text(tmp) ;
	  // Set the size of the scrollbar area
	  zmw_width(150) ;
	  zmw_height(80) ;
	  zmw_scrollbar2(&sb2_x, anchor_x/200., &sb2_y, 0.1) ;
	  // Synchronized with the previous. Default height
	  zmw_height(ZMW_VALUE_UNDEFINED) ;
	  zmw_scrollbar_horizontal(&sb2_x, anchor_x/200.) ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_run(anchor) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 59 87 # On scrollbar 2
zmw_dump_screen 0

zmw_button_press
zmw_move_cursor_to 115 120
zmw_button_release
zmw_dump_screen 1

zmw_move_cursor_to 115 40 # On anchor
zmw_button_press
zmw_move_cursor_to 78 41
zmw_button_release
zmw_dump_screen 2

zmw_move_cursor_to 115 161
zmw_button_press
zmw_move_cursor_to 20 40
zmw_button_release
zmw_dump_screen 3


REGRESSION TEST */
