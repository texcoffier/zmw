/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void filechooser2(void)
{
  static char *filename_current = NULL ;
  static Zmw_Boolean choosing_a_filename = Zmw_True ;

  if ( filename_current == NULL )
    filename_current = strdup("tst") ;
  
  zmw_file_selection(&choosing_a_filename
		  , &filename_current
		  , "Choose a filename"
		  , "Load file"
		  ) ;
  if ( zmw_activated() )
    printf("Load file : '%s'\n", filename_current) ;

}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(filechooser2) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 23 37 # cursor on Load file
zmw_dump_screen 0


REGRESSION TEST */
