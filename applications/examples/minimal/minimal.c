/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void rectangle(/* any parameter you may want */)
{
    switch( ZMW_SUBACTION )
    {
    case Zmw_Compute_Required_Size:
      ZMW_SIZE_MIN.width = 5 ;
      ZMW_SIZE_MIN.height = 5 ;
      break ;
    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:
      zmw_draw_rectangle(Zmw_Color_Foreground, Zmw_True
			 , ZMW_SIZE_ALLOCATED.x
			 , ZMW_SIZE_ALLOCATED.y
			 , ZMW_SIZE_ALLOCATED.width
			 , ZMW_SIZE_ALLOCATED.height
			 ) ;
      /* fall thru */
    case Zmw_Compute_Children_Allocated_Size: break ;
    case Zmw_Init:          break ;
    case Zmw_Post_Drawing:  break ;
    case Zmw_Input_Event:   break ;
    case Zmw_Nothing:       break ;
    case Zmw_Clean:         break ;
    case Zmw_Debug_Message: break ;
    }
}

void rect()
{
  ZMW(rectangle()) { /* no children */ }
}

void minimal(void)
{
  ZMW(zmw_window("Minimal"))
    {
      ZMW(zmw_box_horizontal())
	{
	  ZMW(zmw_box_vertical())
	    {
	      zmw_text("Above") ;
	      rect() ;
	      zmw_text("Under") ;
	    }
	  rect() ;
	  zmw_text("Right") ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_run(minimal) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 23 76          # Begin of text1
zmw_dump_screen 0

REGRESSION TEST */
