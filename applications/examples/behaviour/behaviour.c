/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

void behaviour(void)
{
  static Zmw_Name *focus[] = {NULL, NULL} ;
  static int sensible = Zmw_True ;
  static char *text = NULL ;
  int i ;

  if ( text == NULL )
    {
      text = strdup("Edite me!") ;
      zmw_name_initialize(&focus[0] , "Left" ) ;
      zmw_name_initialize(&focus[1] , "Right" ) ;
    }

  ZMW(zmw_window("Behaviours"))
    {
      ZMW(zmw_hbox())
	{
	  for(i=0; i<2; i++)
	    {
	      if ( i == 1 )
		zmw_sensitive(sensible) ;

	      ZMW(zmw_vbox())
		{
		  zmw_focus(focus[i]) ;

		  zmw_button("Make right column unsensible") ;
		  if ( zmw_activated() )
		    sensible = Zmw_False ;

		  zmw_name("secondbutton") ;
		  zmw_button("Make right column sensible") ;
		  if ( zmw_activated() )
		    sensible = Zmw_True ;

		  zmw_check_button_int_with_label(&sensible, "Sensible state") ;

		  zmw_entry(&text) ;
		}
	    }
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(behaviour) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 248 93 # right toggle
zmw_dump_screen 0

zmw_button_click
zmw_dump_screen 1

zmw_key $KEY_A  # try to change toggle state by key press
zmw_move_cursor_to 248 68 # button right : make right column sensible
zmw_button_click
zmw_dump_screen 2

zmw_move_cursor_to 68 66 # button left : make right column sensible
zmw_button_click
zmw_dump_screen 3

zmw_move_cursor_to 256 65 # button right : make right column sensible
zmw_key $KEY_A  # The right toggle make column unsensible
zmw_sleep 1             # I don't know why
zmw_dump_screen 4

zmw_move_cursor_to 86 40 # button left : make right column unsensible
zmw_key $KEY_A  # The left button with focus make columns sensible
zmw_dump_screen 5

REGRESSION TEST */
