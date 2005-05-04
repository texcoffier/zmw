/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void rectangle(/* any parameter you may want */)
{
  switch( ZMW_SUBACTION )
    {
    case Zmw_Compute_Required_Size:
      /* Compute its minimum size from its children sizes */
      ZMW_SIZE_MIN.width = ZMW_SIZE_MIN.height = 5 ; break ;
    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:
      /* Compute its children sizes and placement and draw the background */
      zmw_draw_rectangle(Zmw_Color_Foreground, Zmw_True
	 , ZMW_SIZE_ALLOCATED.x    , ZMW_SIZE_ALLOCATED.y
	 , ZMW_SIZE_ALLOCATED.width, ZMW_SIZE_ALLOCATED.height
	 ) ;
      break ;
    case Zmw_Compute_Children_Allocated_Size:
      /* Compute the children size and placement */ break ;
    case Zmw_Init:
      /* Do some initializations before the children */ break ;
    case Zmw_Post_Drawing:
      /* Do some drawing after the children drawing */ break ;
    case Zmw_Input_Event:
      /* Use the event if it is for the widget */ break ;
    case Zmw_Nothing:
      /* Nothing to be done */ break ;
    case Zmw_Clean:
      /* Free ressources used by the widget class */ break ;
    case Zmw_Debug_Message:
      /* Displays some debugging messages in the web browser */  break ;
    }
}
void rect() { ZMW(rectangle()) { /* no children */ } }
void minimal(void)
{
  ZMW(zmw_window("Minimal"))
    {
      ZMW(zmw_hbox())
	{
	  ZMW(zmw_vbox())
	    {
	      zmw_label("Above") ;
	      rect() ;
	      zmw_label("Under") ;
	    }
	  rect() ;
	  zmw_label("Right") ;
	}
    }
}
/* DO NOT DISPLAY */

int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(minimal) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 23 76          # Begin of text1
zmw_dump_screen 0

REGRESSION TEST */
