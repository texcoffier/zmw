/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

void navigation2(void)
{
  static float x=0, y=0 ;
  char buf[99] ;
  int i, j ;

  ZMW(zmw_window("Navigation2"))
    {	
      ZMW(zmw_vbox())
	{
	  zmw_button("above") ;
	  if ( zmw_activated() )
	    {
	    }
	  ZMW(zmw_viewport_with_scrollbar(&x, &y))
	    {
	      ZMW(zmw_vbox())
		{
		  for(i=0; i<4; i++)
		    {
		      ZMW(zmw_hbox())
			{
			  for(j=0; j<4; j++)
			    {
			      sprintf(buf, "[%d,%d]", i, j) ;
			      zmw_button(buf) ;
			      if ( zmw_activated() )
				{
				}
			    }
			}
		    }
		}
	    }
	  zmw_button("under") ;
	  if ( zmw_activated() )
	    {
	    }
	}
    }
}

/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(navigation2) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 26 36
zmw_button_click
zmw_dump_screen 0

zmw_key $KEY_RIGHT
zmw_dump_screen 1

zmw_key $KEY_DOWN
zmw_dump_screen 2

zmw_key $KEY_DOWN
zmw_dump_screen 3

zmw_key $KEY_DOWN
zmw_key $KEY_RIGHT
zmw_dump_screen 4

zmw_key $KEY_DOWN
zmw_dump_screen 5

zmw_key $KEY_UP
zmw_dump_screen 6

zmw_key $KEY_UP
zmw_dump_screen 7

zmw_key $KEY_RIGHT
zmw_dump_screen 8

zmw_key $KEY_RIGHT
zmw_key $KEY_DOWN
zmw_key $KEY_DOWN
zmw_key $KEY_DOWN
zmw_dump_screen 9

zmw_key $KEY_LEFT
zmw_dump_screen 10

zmw_key $KEY_UP
zmw_dump_screen 11

zmw_key $KEY_UP
zmw_dump_screen 12


REGRESSION TEST */
