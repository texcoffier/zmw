/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void text_with_width(char *text, int *width)
{
  // Set width to a correct value if it is a NULL pointer
  // It is also possible to use "zmw_resource_pointer_get"
  zmw_resource_int_get(&width, "TextWidth", 100 /* Default */) ;

  ZMW(zmw_box_horizontal())
    {
      zmw_horizontal_expand(Zmw_False) ;

      zmw_width(*width) ;
      zmw_text(text) ;

      zmw_width(ZMW_VALUE_UNDEFINED) ;
      zmw_button("Enlarge text") ;
      if ( zmw_activated() )
	  *width *= 1.5 ;
    }
}

void resource(void)
{
  static int width = 100 ;

  zmw_auto_resize(Zmw_True) ;
  ZMW(zmw_window("Resource"))
    {
      ZMW(zmw_box_vertical())
	{
	  text_with_width("in resource", NULL) ;
	  text_with_width("in application", &width) ;
	}
    }

}

/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_run(resource) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 149 40          # On Enlarge text resource
zmw_dump_screen 0

zmw_button_click
zmw_dump_screen 1

zmw_move_cursor_to 149 70          # On Enlarge text application
zmw_button_click
zmw_dump_screen 2

REGRESSION TEST */
