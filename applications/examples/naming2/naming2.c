/* DO NOT DISPLAY */
#include "zmw/zmw.h"

void toggle_and_its_name(int *value)
{
  char buf[999] ;

  ZMW(zmw_hbox())
    {
      zmw_check_button_int(value) ;
      strcpy(buf, zmw_name_full) ;
      zmw_label("toggle named") ;
      zmw_label(buf) ;
    }
}

/*
 * "hide_label" variable is used because
 * if "label_visible" is modified, the widget hierarchy change.
 * So the state change must be done outside widget hierarchy.
 */

/* DO NOT DISPLAY */
void naming2(void)
{
  static int t1=0, t2=0 ;
  static int label_visible=1 ;
  int hide_label ;

  hide_label = 0 ;
  ZMW(zmw_window("Named toggle"))
    {
      ZMW(zmw_vbox())
	{
	  if ( label_visible )
	    zmw_label("Click on first toggle and go outside") ;
	  /*STARTEMPHASIS*/zmw_name("Toggle") ;/*STOPEMPHASIS*/
	  toggle_and_its_name(&t1) ;
	  if ( zmw_cursor_leave() )
	    hide_label = 1 ;
	  toggle_and_its_name(&t2) ;
	}
    }
  if ( hide_label )
    label_visible = 0 ;
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(naming2) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 22 52 # toggle 1
zmw_dump_screen 0

zmw_button_click
zmw_dump_screen 1

zmw_move_cursor_to 42 52 # toggle 1
zmw_move_cursor_to 22 73 # toggle 2
zmw_sleep 1
zmw_dump_screen 2


REGRESSION TEST */
