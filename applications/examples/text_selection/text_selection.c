/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

void text_selection(void)
{
  int i ;
  static char *text[3] = {NULL} ;
  static int cursors[3] ;
  static Zmw_Name focus[3] =
    {
      ZMW_NAME_UNREGISTERED("var1"),
      ZMW_NAME_UNREGISTERED("var2"),
      ZMW_NAME_UNREGISTERED("var3"),
    } ;

  if ( text[0] == NULL )
    {
      text[0] = strdup("XXXXXXXXXXXXXXXX") ;
      text[1] = strdup("YYYYYYYYYYYYYYYY") ;
      text[2] = strdup("ZZZZZZZZZZZZZZZZ") ;
    }

  ZMW(zmw_window("Text Selection"))
    {
      ZMW(zmw_box_vertical())
	{
	  for(i=0; i<ZMW_TABLE_SIZE(text); i++)
	    {
	      ZMW(zmw_box_vertical())
		{
		  zmw_focus(&focus[i]) ;
		  zmw_text_editable(&text[i]) ;
		  zmw_text_editable_with_cursor(&text[i], &cursors[i]) ;
		}
	    }
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_run(text_selection) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 27 40         # Begin of text1
zmw_button_press
zmw_move_cursor_to 51 40         # Select some text
zmw_dump_screen 0
zmw_button_release
zmw_dump_screen 1

zmw_move_cursor_to 78 40         # Text1
zmw_button_press
zmw_move_cursor_to 107 66         # Select some text, but go down
zmw_dump_screen 2
zmw_button_release
zmw_dump_screen 3

zmw_move_cursor_to 53 119        # Text4
zmw_button_press
zmw_move_cursor_to 105 174       # Select some text, go down
zmw_dump_screen 4
zmw_button_release
zmw_dump_screen 5


REGRESSION TEST */
