/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

/* Complex focus test */
void focus4(void)
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

  ZMW(zmw_window("Focus4"))
    {
      zmw_focus(&focus_out) ;
      ZMW(zmw_box_vertical())
	{
	  zmw_text_editable(&text[0]) ;
	  ZMW(zmw_box_vertical())
	    {
	      zmw_focus(&focus_in) ;
	      zmw_text_editable(&text[1]) ;
	      zmw_text_editable(&text[2]) ;
	    }
	  zmw_border_embossed_in_draw() ;
	  zmw_text_editable(&text[3]) ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_run(focus4) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 22 38   # First text
zmw_button_click           # focus on first text
zmw_move_cursor_to 24 93   # Third text
zmw_button_click           # focus on third text
zmw_move_cursor_to 22 66   # Second text
zmw_key $KEY_BACKSPACE     # THIRD TEXT = empty
zmw_dump_screen 0

zmw_move_cursor_to 22 111  # Fourth text
zmw_key $KEY_BACKSPACE     # FIRST TEXT = empty
zmw_dump_screen 1



REGRESSION TEST */
