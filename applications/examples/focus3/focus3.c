/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

/* Complex focus test */
void focus3(void)
{
  static Zmw_Name focus_out = ZMW_NAME_UNREGISTERED("out") ;
  static Zmw_Name focus_in = ZMW_NAME_UNREGISTERED("in") ;
  static char *text[4] = { NULL } ;
  int i ;

  if ( text[0] == NULL )
    {
      for(i=0; i<4; i++)
	text[i] = strdup("?") ;
    }

  ZMW(zmw_window("Focus3"))
    {
      zmw_focus(&focus_out) ;
      ZMW(zmw_box_vertical())
	{
	  ZMW(zmw_box_vertical())
	    {
	      zmw_focus(&focus_in) ;
	      zmw_text_editable(&text[0]) ;
	      zmw_text_editable(&text[1]) ;
	    }
	  zmw_border_embossed_in_draw() ;
	  zmw_text_editable(&text[2]) ;
	  zmw_text_editable(&text[3]) ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_run(focus3) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 21 40   # First text
zmw_button_click           # focus on first text
zmw_move_cursor_to 21 63   # Second text
zmw_key $KEY_BACKSPACE     # FIRST=empty
zmw_move_cursor_to 21 118  # Fourth text
zmw_key $KEY_A             # FIRST=a
zmw_dump_screen 0

zmw_button_click           # Focus on fourth text
zmw_move_cursor_to 21 92   # Third text
zmw_key $KEY_BACKSPACE     # FOURTH=empty
zmw_move_cursor_to 21 63   # Second text
zmw_key $KEY_BACKSPACE     # FIRST=empty
zmw_dump_screen 1



REGRESSION TEST */
