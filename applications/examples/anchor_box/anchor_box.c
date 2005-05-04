/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void anchor_box(void)
{
  static int x=100, y=100, width=60, height=40 ;


  ZMW(zmw_window("Anchor Box"))
    {
      zmw_height(300) ;
      zmw_width(300) ;

      ZMW(zmw_fixed())
        {
          ZMW(zmw_anchor_box(&x, &y, &width, &height))
            {
              zmw_label("MoveMe") ;
            }
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(anchor_box) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 120 130 # In the anchor box
zmw_dump_screen 0

zmw_button_click
zmw_dump_screen 1

zmw_move_cursor_to 134 144 # In the center anchor
zmw_button_press
zmw_dump_screen 2

zmw_move_cursor_to 144 164
zmw_dump_screen 3

zmw_button_release
zmw_dump_screen 4

zmw_move_cursor_to 122 151 # upper left anchor
zmw_button_press
zmw_move_cursor_to 80 90
zmw_button_release
zmw_dump_screen 5

zmw_move_cursor_to 134 96 # upper border
zmw_button_press
zmw_move_cursor_to 10  106
zmw_button_release
zmw_move_cursor_to 132 186 # lower border
zmw_button_press
zmw_move_cursor_to 400 176
zmw_button_release
zmw_move_cursor_to 88 145 # left border
zmw_button_press
zmw_move_cursor_to 98 30
zmw_button_release
zmw_move_cursor_to 176 146 # right border
zmw_button_press
zmw_move_cursor_to 150 100
zmw_button_release
zmw_dump_screen 6

zmw_move_cursor_to 108 179 # bottom left
zmw_button_press
zmw_move_cursor_to 50 278
zmw_button_release
zmw_move_cursor_to 151 110 # top right
zmw_button_press
zmw_move_cursor_to 247 49
zmw_button_release
zmw_dump_screen 7

zmw_move_cursor_to 248 278 # lower right
zmw_button_press
zmw_move_cursor_to 68 66
zmw_button_release
zmw_dump_screen 8




REGRESSION TEST */
