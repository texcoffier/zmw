/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void copypaste(void)
{
  static char *text1 = NULL, *text2 = NULL ;

  if ( text1 == NULL )
    {
      text1 = strdup("text 1") ;
      text2 = strdup("text 2") ;
    }

  zmw_auto_resize(Zmw_True) ;
  ZMW(zmw_window("Text"))
    {
      ZMW(zmw_vbox())
	{
	  zmw_entry(&text1) ;
	  zmw_entry(&text2) ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(copypaste) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 53 41          # end of text 1
zmw_dump_screen 0

zmw_button_press
zmw_move_cursor_to 25 38          # in text1
zmw_dump_screen 1

zmw_button_release
zmw_dump_screen 2

zmw_move_cursor_to 34 65          # in text2
zmw_button2_click
zmw_dump_screen 3

REGRESSION TEST */
