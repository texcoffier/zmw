/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

/*
 * This widget container class change its OWN alignment when clicked
 */
void alignment(int *horizontal_alignment)
{
  zmw_resource_int_get(&horizontal_alignment
		       , "HorizontalAlignment"
		       , 0) ;

  if ( zmw_button_pressed() )
    {
      *horizontal_alignment = ((*horizontal_alignment + 3) % 3) - 1 ;
    }
  zmw_horizontal_alignment_set(*horizontal_alignment) ;
  zmw_decorator(Zmw_Decorator_Border_Relief) ;
}
void resource2(void)
{
  static int horizontal_alignment = 0 ;
  ZMW(zmw_window("Resource"))
    {
      zmw_horizontal_expand(Zmw_True) ;
      ZMW(zmw_vbox())
	{
	  zmw_horizontal_expand(Zmw_False) ;
	  zmw_label("Click to change the alignment") ;
	  ZMW(alignment(&horizontal_alignment)) zmw_label("Application") ;
	  ZMW(alignment(NULL))                  zmw_label("Resource") ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(resource2) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 67 51 # Application (on the left)
zmw_dump_screen 0

zmw_button_click
zmw_dump_screen 1

zmw_button_click
zmw_dump_screen 2

zmw_move_cursor_to 104 79 # Resource
zmw_button_click
zmw_dump_screen 3


REGRESSION TEST */
