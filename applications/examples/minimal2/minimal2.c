/* DO NOT DISPLAY */
#include "zmw/zmw.h"
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
			 , ZMW_SIZE_ALLOCATED.x    
			 , ZMW_SIZE_ALLOCATED.y
			 , ZMW_SIZE_ALLOCATED.width
			 , ZMW_SIZE_ALLOCATED.height) ;
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
    case Zmw_Subaction_Last:
      /* Only here to remove a compiler warning, this case is never called */
      /* You can replace this case and the other unused by "default:" */
      break ;
    }
}
void rect() { ZMW(rectangle()) { /* no children */ } }
/* DO NOT DISPLAY */
void rectangle_activable(void)
{
  if ( ZMW_SUBACTION == Zmw_Input_Event )
    {
      if ( zmw_button_pressed() ) // Button pressed inside the rectangle
	ZMW_SIZE_ACTIVATED = Zmw_True ; // Widget is activated
    }
  else
    rectangle() ;
}

void minimal2(void)
{
  static Zmw_Boolean has_been_activated = Zmw_False ;

  ZMW(zmw_window("Rect + Activable"))
    {
      ZMW(zmw_hbox())
	{
	  ZMW( rectangle_activable() ) { }
	  if ( zmw_activated() )
	    has_been_activated = Zmw_True ;
	  zmw_label("Has been activated:") ;
	  zmw_int(has_been_activated) ;
	}
    }
}
/* DO NOT DISPLAY */

int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(minimal2) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 13 34 # On the rectangle
zmw_dump_screen 0

zmw_button_click
zmw_dump_screen 1

REGRESSION TEST */
