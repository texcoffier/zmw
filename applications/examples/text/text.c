/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void text(void)
{
  int i ;
  char tmp[99] ;
  static char *text1 = NULL, *text2 = NULL ;
  static int cursor_pos = 5 ; // Must be static
  static int text1_length ;

  if ( text1 == NULL )
    {
      // The initial value must be malloced
      text1 = strdup("text 1") ;
      text1_length = strlen(text1) ;
      text2 = strdup("text 2") ;
    }

  ZMW(zmw_window("Text"))
    {
      ZMW(zmw_vbox())
	{
	  for(i=0; i<2; i++)
	    {
	      // Texts can be computed
	      sprintf(tmp, "Label number %d", i) ;
	      zmw_label(tmp) ;
	    }
	  // "cursor_pos" is only modified on user event.
	  sprintf(tmp, "cursor_pos = %d", cursor_pos) ;
	  zmw_label(tmp) ;

	  zmw_horizontal_alignment(-1) ; // Left
	  zmw_entry_with_cursor(&text1, &cursor_pos) ;
	  if ( zmw_changed() )
	    text1_length = strlen(text1) ;

	  sprintf(tmp, "text1_length = %d", text1_length) ;
	  zmw_label(tmp) ;	  

	  zmw_horizontal_expand(Zmw_False) ;
	  zmw_border_width(6) ;
	  zmw_entry(&text2) ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(text) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 23 78          # Begin of text1
zmw_button_press
zmw_dump_screen 0

zmw_move_cursor_to 40 78          # Select a zone
zmw_button_release
zmw_dump_screen 1

zmw_key $KEY_A
zmw_dump_screen 2

zmw_key $KEY_LEFT
zmw_dump_screen 3

REGRESSION TEST */
