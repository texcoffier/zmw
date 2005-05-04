/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void hello_world_2(void)
{
  ZMW(zmw_window("Title"))
    {
      zmw_label("Hello World! I am a label.") ;
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ; // Take ZMW parameters
  zmw_main(hello_world_2) ; // Launch the infinite loop
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 26 15
zmw_dump_screen 0

REGRESSION TEST */
