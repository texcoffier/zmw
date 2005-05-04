/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

void swappable_text(char **i, int **current)
{
  zmw_label(*i) ;
  zmw_drag_swap((int*)i, current) ;
  ZMW( zmw_if( zmw_drag_from_running() ) )
    {
      ZMW(zmw_window_drag() )
	if ( *current )
	  zmw_label((char*)**current) ;
    }
}

void dragswap(void)
{
  static int table1[] = { 1, 2, 3, 4 } ;
  static char *table2[] = { "A","B","C","D","E","F","G","H" } ;
  static int *current1 = NULL, *current2 = NULL ;
  int i ;

  ZMW(zmw_window("DragSwap"))
    {
      ZMW(zmw_vbox())
	{
	  // Swap the children of a container
	  ZMW(zmw_hbox())
	    for(i=0; i<ZMW_TABLE_SIZE(table1); i++)
	      {
		zmw_int(table1[i]) ;
		zmw_drag_swap(&table1[i], &current1) ;
		ZMW( zmw_if( zmw_drag_from_running() ) )
		  ZMW(zmw_window_drag() )
		  {
		    if ( current1 )
		      zmw_int(*current1) ;
		  }
	      }
	  // Swap the children in the TWO next containers
	  ZMW(zmw_hbox())
	    for(i=0; i<ZMW_TABLE_SIZE(table2)/2; i++)
	      swappable_text(&table2[i], &current2) ;
	  ZMW(zmw_hbox())
	    for(i=ZMW_TABLE_SIZE(table2)/2; i<ZMW_TABLE_SIZE(table2); i++)
	      swappable_text(&table2[i], &current2) ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(dragswap) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 16 34 # 1
zmw_dump_screen 0

zmw_button_press
zmw_move_cursor_to 16 52 # A
zmw_dump_screen 1

zmw_move_cursor_to 35 51 # C
zmw_move_cursor_to 35 35 # 3
zmw_dump_screen 2

zmw_move_cursor_to 27 35 # 2
zmw_dump_screen 3

zmw_button_release
zmw_move_cursor_to 16 52 # A
zmw_button_press
zmw_dump_screen 4

zmw_move_cursor_to 25 52 # B
zmw_move_cursor_to 26 70 # F
zmw_button_release
zmw_dump_screen 5



REGRESSION TEST */
