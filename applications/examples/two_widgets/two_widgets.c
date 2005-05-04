/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void two_widgets(void)
{
  ZMW(zmw_vbox())
    {
      zmw_label("Hello") ;
      zmw_label("World!") ;
    }
}
/* DO NOT DISPLAY */

void top()
{
  ZMW(zmw_window("Box"))
    two_widgets() ;
}

int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ; // Take ZMW parameters
  zmw_main(top) ; // Launch the infinite loop
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 26 15
zmw_dump_screen 0

REGRESSION TEST */
