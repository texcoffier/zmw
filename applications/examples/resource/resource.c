/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void text_with_width(char *text, int *width)
{
  // Set width to a correct value if it is a NULL pointer

  // If the call to "zmw_resource_int_get" is made before ZMW.
  // The resource is taken from the previous widget if there is one
  // or from  "parent_name/." if it is the first.
  // The code (more readable, but not intuitive) is :
  //
  // zmw_resource_int_get(&width, "TextWidth", 100 /* Default */) ;
  // ZMW(zmw_hbox())
  //    {
  //
  ZMW((zmw_resource_int_get(&width, "TextWidth", 100 /* Default */),
       zmw_hbox()))
    {
      zmw_horizontal_expand(Zmw_False) ;

      zmw_width(*width) ;
      zmw_label(text) ;

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
      ZMW(zmw_vbox())
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
  zmw_main(resource) ;
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
