/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void filechooser(void)
{
  static char *filename_current = NULL ;
  static Zmw_Boolean choosing_a_filename = Zmw_False ;

  if ( filename_current == NULL )
    filename_current = strdup("tst") ;
  
  ZMW(zmw_window("FC"))
    {
      ZMW(zmw_vbox())
	{
	  zmw_button("Load File") ;
	  if ( zmw_activated() )
	    choosing_a_filename = Zmw_True ;

	  
	  zmw_file_selection(&choosing_a_filename
			  , &filename_current
			  , "Choose a filename"
			  , "Load file"
			  ) ;
	  if ( zmw_activated() )
	    printf("Load file : '%s'\n", filename_current) ;


	  zmw_label("File name current:") ;
	  zmw_label(filename_current) ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(filechooser) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 23 37 # cursor on Load file
zmw_dump_screen 0

zmw_button_click
zmw_dump_screen 1

zmw_key $KEY_SLASH
zmw_dump_screen 2

zmw_key $KEY_A
zmw_dump_screen 3

zmw_key $KEY_BACKSPACE
zmw_move_cursor_to 181 92 # b file
zmw_button_click
zmw_dump_screen 4


REGRESSION TEST */
