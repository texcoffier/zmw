/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

void menu_file(Zmw_Boolean *choosing_a_filename)
{
  ZMW(zmw_window_popup_bottom_with_title("Menu"))
    {
      ZMW(zmw_box_vertical())
	{
	  zmw_button("Load") ;
	  if ( zmw_activated() )
	    *choosing_a_filename = Zmw_True ;
	}
    }
}

void menu2(void)
{
  static Zmw_Boolean choosing_a_filename = Zmw_False ;
  static char *filename_current = NULL ;


  if ( filename_current == NULL )
    filename_current = strdup("/") ;


  ZMW(zmw_window("Window"))
    {
      ZMW(zmw_box_vertical())
	{
	  zmw_button("Menu") ;
	  if ( zmw_window_is_popped() )
	    menu_file(&choosing_a_filename) ;
	}


      zmw_filechooser(&choosing_a_filename
		      , &filename_current
		      , "Choose a filename"
		      , "Load file"
		      ) ;
      if ( zmw_activated() )
	printf("Loaded!\n") ;
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_run(menu2) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 26 36        # Menu
zmw_dump_screen 0


REGRESSION TEST */
