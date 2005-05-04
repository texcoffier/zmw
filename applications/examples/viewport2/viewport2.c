/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void viewport2(void)
{
  static Zmw_Float_0_1 x=0, y=0 ;
  static Zmw_Float_0_1 x2=0, y2=0 ;
  static int width=100, height=100 ;
  int i ;

  ZMW(zmw_window("Viewport2"))
    {
      zmw_height(200) ;
      zmw_width(200) ;
      ZMW(zmw_vbox())
	{
	  zmw_height(height) ;
	  zmw_width(width) ;
	  ZMW(zmw_viewport_with_scrollbar(&x, &y))
	    {
	      zmw_height(100) ;
	      zmw_width(100) ;
	      zmw_color(Zmw_Color_Border_Light, 0x00FF0000) ;
	      zmw_color(Zmw_Color_Border_Dark, 0x00800000) ;

	      ZMW(zmw_viewport_with_scrollbar(&x2, &y2))
		{
		  ZMW(zmw_vbox())
		    {
		      for(i=0; i<20; i++)
			zmw_label("bla bla bla bla bla") ;
		    }
		}
	    }
	  zmw_height(ZMW_VALUE_UNDEFINED) ;
	  zmw_width(ZMW_VALUE_UNDEFINED) ;
	  zmw_button("Enlarge window") ;
	  if ( zmw_activated() )
	    {
	      height = 150 ;
	      width = 150 ;
	      x = 0 ;
	      y = 0 ;
	    }
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(viewport2) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 152 107 # bottom vertical scrollbar
zmw_dump_screen 0

zmw_button_click
zmw_move_cursor_to 139 120 # right horizontal scrollbar
zmw_button_click
zmw_dump_screen 1

zmw_move_cursor_to 110 97 # intern right horizontal scrollbar
zmw_button_click
zmw_move_cursor_to 128 74 # inter bottom vertical scrollbar
zmw_button_click
zmw_dump_screen 2

zmw_move_cursor_to 100 180 # Enlarge
zmw_button_click
zmw_dump_screen 3



REGRESSION TEST */
