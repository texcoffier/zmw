/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void anchor2(void)
{
  static Zmw_Float_0_1 sb1_x = 0.5, sb1_y = 0.1 ;
  static Zmw_Float_0_1 sb2_x = 0.5, sb2_y = 0.1 ;

  ZMW(zmw_window("Anchor2 1"))
    {
      zmw_width(80) ;
      zmw_height(80) ;
      zmw_scrollbar2(&sb1_x, 0.1, &sb1_y, 0.1) ;
    }
  ZMW(zmw_window("Anchor2 2"))
    {
      zmw_width(80) ;
      zmw_height(80) ;
      zmw_scrollbar2(&sb2_x, 0.1, &sb2_y, 0.1) ;
    }
  ZMW(zmw_window("Button"))
    {
      zmw_button("Center") ;
      if ( zmw_activated() )
	sb1_x = sb2_x = sb1_y = sb2_y = 0.5 ;
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(anchor2) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 37 61 # On first window
zmw_dump_screen 0

zmw_button_press
zmw_dump_screen 1

zmw_button_release
zmw_dump_screen 2


zmw_move_cursor_to 165 82 # On second window
zmw_dump_screen 3

zmw_button_press
zmw_dump_screen 4

zmw_button_release
zmw_dump_screen 5

zmw_move_cursor_to 228 37 # On button
zmw_button_press
zmw_dump_screen 6
zmw_button_release
zmw_dump_screen 7


REGRESSION TEST */
