/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void hidden(void)
{
  static char *filename_current = NULL ;
  static Zmw_Boolean choosing_a_filename = Zmw_False ;

  if ( filename_current == NULL )
    filename_current = strdup("tst") ;
  
  ZMW(zmw_window("FC"))
    {
      zmw_button("File") ;
      ZMW( zmw_menu() )
	ZMW(zmw_window_popup_bottom())
	{
	  zmw_button("Load") ;
	  if ( zmw_activated() )
	    choosing_a_filename = Zmw_True ;
	}
      /* The file chooser must be here and not inside the former block.
       * If it is before it will disappear when the menu popdown
       */
      zmw_height(100) ;
      zmw_file_selection(&choosing_a_filename
		      , &filename_current
		      , "Choose a filename"
		      , "Load file"
			  ) ;
      if ( zmw_activated() )
	printf("Load file : '%s'\n", filename_current) ;
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(hidden) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 25 36 # File button
zmw_dump_screen 0

zmw_button_click
zmw_dump_screen 1

zmw_move_cursor_to 28 60 # Load button
zmw_button_click
zmw_dump_screen 2


REGRESSION TEST */
