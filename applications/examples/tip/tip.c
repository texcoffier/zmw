/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void popup(Zmw_Boolean bottom)
{
  ZMW(bottom ? zmw_window_popup_bottom() : zmw_window_popup_right())
    {
      zmw_int(bottom) ;
    }
}

void tip(void)
{
  ZMW(zmw_window("Anchor"))
    {
      ZMW(zmw_box_vertical())
	{
	  zmw_button("Unsensible with tip") ;
	  if ( zmw_tip_visible() )
	    popup(Zmw_False) ;

	  zmw_button("tip + activate") ;
	  if ( zmw_tip_visible() )
	    popup(Zmw_False) ;
	  if ( zmw_activated() )
	    printf("Activated") ;

	  zmw_button("activate + tip") ;
	  if ( zmw_tip_visible() )
	    popup(Zmw_False) ;
	  if ( zmw_activated() )
	    printf("Activated") ;

	  zmw_button("activate + tip + tip") ;
	  if ( zmw_activated() )
	    printf("Activated") ;
	  if ( zmw_tip_visible() )
	    popup(Zmw_False) ;
	  if ( zmw_tip_visible() )
	    popup(Zmw_True) ;

	  zmw_button("tip + tip + activate") ;
	  if ( zmw_tip_visible() )
	    popup(Zmw_False) ;
	  if ( zmw_tip_visible() )
	    popup(Zmw_True) ;
	  if ( zmw_activated() )
	    printf("Activated") ;

	  zmw_button("tip + tip") ;
	  if ( zmw_tip_visible() )
	    popup(Zmw_False) ;
	  if ( zmw_tip_visible() )
	    popup(Zmw_True) ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_run(tip) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 70 16 # Window title
zmw_dump_screen 0

zmw_move_cursor_to 70 40 # 
zmw_sleep 1
zmw_dump_screen 1

zmw_move_cursor_to 70 65 # 
zmw_sleep 1
zmw_dump_screen 2

zmw_move_cursor_to 70 92 # 
zmw_sleep 1
zmw_dump_screen 3

zmw_move_cursor_to 70 118 # 
zmw_sleep 1
zmw_dump_screen 4

zmw_move_cursor_to 70 143 # 
zmw_sleep 1
zmw_dump_screen 5

zmw_move_cursor_to 70 168 # 
zmw_sleep 1
zmw_dump_screen 6

REGRESSION TEST */
