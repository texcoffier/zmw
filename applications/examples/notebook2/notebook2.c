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
	  zmw_text("NB") ;
	  ZMW(zmw_notebook(&current_page_2))
	    {
	      zmw_text("p1") ;
	      zmw_text("c1") ;
	      zmw_text("p2") ;
	      zmw_text("c2") ;
	    }

	  zmw_text("T1") ;
	  zmw_toggle(&t1) ;

	  zmw_text("T2") ;
	  zmw_toggle(&t2) ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_run(notebook2) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 34 48 # p2
zmw_dump_screen 0

zmw_button_click
zmw_dump_screen 1

zmw_move_cursor_to 34 32 # T1
zmw_button_click
zmw_dump_screen 2

zmw_move_cursor_to 17 47 # toggle 1
zmw_button_click
zmw_dump_screen 3

zmw_move_cursor_to 51 33 # T2
zmw_button_click
zmw_dump_screen 4

zmw_move_cursor_to 17 47 # toggle 2
zmw_button_click
zmw_dump_screen 5

zmw_move_cursor_to 15 31 # NB
zmw_button_click
zmw_dump_screen 6




REGRESSION TEST */
