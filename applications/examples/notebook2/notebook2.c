/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void notebook2(void)
{
  static int current_page = 0, current_page_2 = 0 ;
  static int t1 = 0, t2 = 0 ;

  ZMW(zmw_window("Notebook"))
    {
      ZMW(zmw_notebook(&current_page))
	{
	  zmw_button("NB") ;
	  ZMW(zmw_notebook(&current_page_2))
	    {
	      zmw_button("p1") ;
	      zmw_label("c1") ;
	      zmw_button("p2") ;
	      zmw_label("c2") ;
	    }

	  zmw_button("T1") ;
	  zmw_check_button_int(&t1) ;

	  zmw_button("T2") ;
	  zmw_check_button_int(&t2) ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(notebook2) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 54 65 # p2
zmw_dump_screen 0

zmw_button_click
zmw_dump_screen 1

zmw_move_cursor_to 46 36 # T1
zmw_button_click
zmw_dump_screen 2

zmw_move_cursor_to 49 71   # toggle 1
zmw_button_click
zmw_dump_screen 3

zmw_move_cursor_to 74 34 # T2
zmw_button_click
zmw_dump_screen 4

zmw_move_cursor_to 49 71       # toggle 2
zmw_button_click
zmw_dump_screen 5

zmw_move_cursor_to 21 35 # NB
zmw_button_click
zmw_dump_screen 6




REGRESSION TEST */
