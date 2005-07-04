/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

/* Complex focus test */
void focus3(void)
{
  static Zmw_Name *focus_out = NULL, *focus_in = NULL ;
  static char *text[4] = { NULL } ;
  int i ;

  if ( text[0] == NULL )
    {
      for(i=0; i<4; i++)
	text[i] = strdup("?") ;
      zmw_name_initialize(&focus_out, "out") ;
      zmw_name_initialize(&focus_in, "in") ;
    }

  ZMW(zmw_window("Focus3"))
    {
      zmw_focus(focus_out) ;
      ZMW(zmw_vbox())
	{
	  ZMW(zmw_vbox())
	    {
	      zmw_focus(focus_in) ;
	      zmw_entry(&text[0]) ;
	      zmw_entry(&text[1]) ;
	    }
	  zmw_border_embossed_in_draw() ;
	  zmw_entry(&text[2]) ;
	  zmw_entry(&text[3]) ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(focus3) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 24 43   # First text
zmw_button_click           # focus on first text
zmw_move_cursor_to 21 71   # Second text
zmw_key $KEY_BACKSPACE     # FIRST=empty
zmw_move_cursor_to 23 126  # Fourth text
zmw_key $KEY_A             # FIRST=a
zmw_dump_screen 0

zmw_button_click           # Focus on fourth text
zmw_move_cursor_to 22 101  # Third text
zmw_key $KEY_BACKSPACE     # FOURTH=empty
zmw_move_cursor_to 25 69   # Second text
zmw_key $KEY_BACKSPACE     # FIRST=empty
zmw_dump_screen 1



REGRESSION TEST */
