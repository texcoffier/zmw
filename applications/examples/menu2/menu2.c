/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

void menu_file(Zmw_Boolean *choosing_a_filename)
{
  ZMW(zmw_window_popup_bottom_with_title("Menu"))
    {
      ZMW(zmw_vbox())
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
      ZMW(zmw_vbox())
	{
	  zmw_button("Menu") ;
	  ZMW( zmw_menu() )
	    menu_file(&choosing_a_filename) ;
	}


      zmw_file_selection(&choosing_a_filename
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
  zmw_main(menu2) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 26 36        # Menu
zmw_dump_screen 0


REGRESSION TEST */
