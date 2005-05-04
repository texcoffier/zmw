/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

void bug2(void)
{
  static int start = 0, nb = 0 ;
  static int widths[] = { 20 } ;

  ZMW(zmw_window("Bug1"))
    {
      ZMW(zmw_vbox())
	{
	  zmw_name("int") ;
	  zmw_int_editable(&widths[0]) ;
	  zmw_name("scrolledview") ;
	  ZMW(zmw_scrolled_view_with_columns(&start, &nb, 0,1))
	    {
	      zmw_name("table") ;
	      ZMW(zmw_table_with_widths(1, widths))
		{
		}
	    }
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(bug2) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 22 36 # The editable int
zmw_dump_screen 0

zmw_button_click
zmw_move_cursor_to 24 62 # In the table
zmw_key $KEY_A           # The int should become "2"
zmw_dump_screen 1

zmw_move_cursor_to 47 63 # In the scrollbar
zmw_key $KEY_BACKSPACE   # The int should become "0"
zmw_dump_screen 2


REGRESSION TEST */
