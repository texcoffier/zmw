/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

/* Minimal focus test with 2 texts */
void focus2(void)
{
  static Zmw_Name *focus = NULL ;
  static char *text = NULL, *text2 = NULL ;

  if ( text == NULL )
    {
      text = strdup("?") ;
      text2 = strdup("?") ;
      zmw_name_initialize(&focus, "var") ;
    }

  ZMW(zmw_window("Focus2"))
    {
      zmw_focus(focus) ;
      ZMW(zmw_vbox())
	{
	  zmw_entry(&text) ;
	  zmw_entry(&text2) ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(focus2) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 23 41   # First text
zmw_button_click           # Focus on first text
zmw_move_cursor_to 22 66   # Second text
zmw_key $KEY_BACKSPACE     # Remove char from first text
zmw_dump_screen 0

zmw_button_click           # Focus on second text
zmw_move_cursor_to 17 38   # First text
zmw_key $KEY_BACKSPACE     # Remove char from second text
zmw_dump_screen 1


REGRESSION TEST */
