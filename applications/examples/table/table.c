/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void table(void)
{
  int i ;
  static int width[] = { 20, 20, 20 } ;
  static int width2[] = { 50, 100 } ;
  static Zmw_Boolean selection[6] ;

  ZMW(zmw_window("Table"))
    {
      zmw_horizontal_expand(Zmw_False) ;
      ZMW(zmw_vbox())
	{
	  zmw_label("zmw_table") ;
	  ZMW(zmw_table(3))
	    {
	      for(i=0; i<12; i++)
		zmw_int(i) ;
	    }


	  zmw_label("+ widths") ;
	  ZMW(zmw_table_with_widths(3, width))
	    {
	      for(i=0; i<12; i++)
		zmw_int( width[i%3] ) ;
	    }


	  zmw_label("+ selection") ;
	  ZMW(zmw_table_with_widths_and_selection
	      (2, width2, selection, Zmw_False))
	    {
	      zmw_label("i") ;
	      zmw_label("i * i") ;
	      for(i=0; i<5; i++)
		{
		  zmw_int(1000+i) ;
		  zmw_int((1000+i)*(1000+i)) ;
		}
	    }
	  if ( zmw_activated() )
	    for(i=1; i<5; i++)
	      fprintf(stderr, "selection[%d] = %d\n", i, selection[i]) ;
	  /* do not allow header selection */
	  selection[0] = Zmw_False ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(table) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 45 34
zmw_dump_screen 0

zmw_move_cursor_to 116 136 # Last | of first table
zmw_button_press
zmw_move_cursor_to 126 136
zmw_button_release

zmw_move_cursor_to 90 136 # Last-1 | of first table
zmw_button_press
zmw_move_cursor_to 101 136
zmw_button_release

zmw_move_cursor_to 70 136 # Last-2 | of first table
zmw_button_press
zmw_move_cursor_to 82 136
zmw_button_release

zmw_move_cursor_to 39 136 # first | of first table (undraggable)
zmw_button_press
zmw_move_cursor_to 49 136
zmw_button_release

zmw_move_cursor_to 39 200 # header line of second table (unselectable)
zmw_button_click
zmw_move_cursor_to 39 214 # first line of second table
zmw_button_click
zmw_move_cursor_to 39 242 # third line of second table
zmw_button_click
zmw_move_cursor_to 39 267 # fifth line of second table
zmw_button_click

zmw_dump_screen 1





REGRESSION TEST */
