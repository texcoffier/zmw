/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

void scrolledview(void)
{
  int i ;
  static int start=0, visible=10, max=1000 ;

  ZMW(zmw_window("Scrolled View"))
    {
      zmw_width(60) ;
      zmw_vertical_expand(Zmw_True) ;
      zmw_horizontal_expand(Zmw_True) ;
      ZMW(zmw_box_vertical())
	{
	  zmw_vertical_expand(Zmw_False) ;
	  zmw_text("visible = ") ;
	  zmw_int(visible) ;

	  zmw_vertical_expand(Zmw_True) ;
	  zmw_horizontal_expand(Zmw_True) ;
	  ZMW(zmw_scrolled_view(&start, &visible, max))
	    {
	      ZMW(zmw_box_vertical())
		{
		  zmw_vertical_expand(Zmw_False) ;
		  for(i=start; i<start+visible; i++)
		    zmw_int(i) ;
		}
	    }
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_run(scrolledview) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 57 183
sleep 1
zmw_dump_screen 0

zmw_button_click
zmw_dump_screen 1




REGRESSION TEST */
