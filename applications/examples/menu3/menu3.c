/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

#define SPAN 2

void recursive_menu(int id)
{
  char buf[99] ;
  int i ;

  if ( id > 31 )
    return ;

  sprintf(buf, "#%d", id) ;
  zmw_button(buf) ;
  ZMW( zmw_popup() )
    {
      ZMW(zmw_window_popup_right_with_title(buf))
	{
	  ZMW(zmw_box_vertical())
	    {
	      zmw_tearoff() ;
	      for(i=0; i<SPAN; i++)
		recursive_menu(SPAN*id + i) ;
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
