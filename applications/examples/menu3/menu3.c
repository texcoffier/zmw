/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

void recursive_menu(int level)
{
  char buf[99] ;

  if ( level > 1000 )
    return ;

  sprintf(buf, "L%d", level) ;
  zmw_button(buf) ;
  if ( zmw_window_is_popped() )
    {
      ZMW(zmw_window_popup_right_with_title(buf))
	{
	  ZMW(zmw_box_vertical())
	    {
	      zmw_tearoff() ;
	      recursive_menu(2*level) ;
	      recursive_menu(2*level+1) ;
	    }
	}
    }
}

void menu3(void)
{
  ZMW(zmw_window("Window1"))
    {
      recursive_menu(1) ;
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_run(menu3) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 17 34        # L1

zmw_dump_screen 0


REGRESSION TEST */
