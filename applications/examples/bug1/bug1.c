/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

#define NB_COLS 1

void bug1(void)
{
  static int start = 0, nb = 1 ;
  int i, j ;
  static int widths[NB_COLS] = { 0 } ;

  if ( widths[0] == 0 )
    for(i=0; i<NB_COLS; i++)
      widths[i] = 20 ;

  ZMW(zmw_window("Bug1"))
    {
      ZMW(zmw_vbox())
	{
	  zmw_name("box") ;
	  ZMW(zmw_hbox())
	    {
	      zmw_name("button") ;
	      zmw_button("B") ;
	      zmw_name("popup") ;
	      ZMW( zmw_menu() )
		{
		  ZMW(zmw_window_popup_bottom())
		    {
		      zmw_label("x") ;
		    }
		}
	    }
	  zmw_name("scrolledview") ;
	  ZMW(zmw_scrolled_view_with_columns(&start, &nb, 100,NB_COLS))
	    {
	      zmw_name("table") ;
	      ZMW(zmw_table_with_widths(NB_COLS, widths))
		for(i=start;i<start+nb;i++)
		  {
		    for(j=0; j<NB_COLS; j++)
		      zmw_int(i + j) ;
		  }
	      
	    }
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(bug1) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 19 66 # Item 0 of the table
zmw_dump_screen 0

zmw_button_click
zmw_dump_screen 1

zmw_move_cursor_to 52 64 # Scroll bar
zmw_button_click
zmw_dump_screen 2

REGRESSION TEST */
