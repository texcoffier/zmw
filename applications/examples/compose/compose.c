/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void toggle_with_label(int *value, const char *label)
{
  ZMW(zmw_hbox_activable())
    {
      zmw_check_button_int(value) ;
      zmw_label(label) ;
    }
  if ( zmw_activated() ) 
    *value = 1 - *value ;
}
/* DO NOT DISPLAY */

void top()
{
  static int i = 0 ;
  ZMW(zmw_window("Compose"))
    toggle_with_label(&i, "The toggle label") ;
}

int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ; // Take ZMW parameters
  zmw_main(top) ; // Launch the infinite loop
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 69 35
zmw_dump_screen 0

zmw_button_press
zmw_dump_screen 1

zmw_button_release
zmw_dump_screen 2

zmw_move_cursor_to 16 32
zmw_button_press
zmw_dump_screen 3

zmw_button_release
zmw_dump_screen 4

zmw_button_press
zmw_dump_screen 5

zmw_button_release
zmw_dump_screen 6


REGRESSION TEST */
