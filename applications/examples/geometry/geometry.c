/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

void text(const char *text)
{
  zmw_vertical_expand(Zmw_True) ;
  ZMW(zmw_decorator(Zmw_Decorator_Border_Embossed))
    {
      zmw_horizontal_expand(Zmw_False) ;
      zmw_vertical_expand(Zmw_False) ;
      zmw_label(text) ;
    }
}

void label(const char *text)
{
  zmw_vertical_expand(Zmw_False) ;
  zmw_label(text) ;
}

void geometry(void)
{
  static int window_height = ZMW_VALUE_UNDEFINED ;

  ZMW(zmw_window("Widget Geometry"))
    {
      // Height of the window
      zmw_height(500) ;
      ZMW(zmw_hbox())
	{
	  // The content does not expand to the full size
	  zmw_vertical_expand(Zmw_False) ;
	  zmw_height(window_height) ;
	  ZMW(zmw_vbox())
	    {
	      zmw_button("Enlarge box") ;
	      if ( zmw_activated() )
		window_height = 490 ;
	      
	      // This text seems flushed left
	      // because it take all the window width.
	      label("zmw_width(100)") ;
	      zmw_width(100) ;
	      text("This") ;
	      zmw_border_width(6) ;
	      label("zmw_border_width(6)") ;
	      // This box is not vertically expandable
	      // See the "label" function.
	      // The box width is the same than "This" text.
	      ZMW(zmw_hbox())
		{
		  // zmw_width is reset to ZMW_VALUE_UNDEFINED
		  // So the two next widgets width are
		  // computed by the horizontal box.
		  zmw_y(60) ;
		  text("i") ;
		  zmw_y(30) ;
		  text("s") ;
		}
	      zmw_width(ZMW_VALUE_UNDEFINED) ;
	      label("zmw_width(ZMW_VALUE_UNDEFINED)") ;
	      text("a") ;
	      zmw_horizontal_expand(Zmw_False) ;
	      label("zmw_horizontal_expand(Zmw_False)") ;
	      text("vertical") ;
	      // There is more space under because the padding
	      // is bigger for the next widget.
	      label("zmw_padding_width(6)") ;
	      // This padding is applied to the decorator
	      // and the text it contains
	      zmw_padding_width(6) ;
	      text("sentence") ;
	      label("zmw_horizontal_alignment(1)") ;
	      zmw_horizontal_alignment(1) ;
	      text(".") ;
	    }
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(geometry) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 99 98 # Enlarge button
zmw_dump_screen 0

zmw_button_click
zmw_dump_screen 1


REGRESSION TEST */
