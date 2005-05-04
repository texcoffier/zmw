/* DO NOT DISPLAY */
#include "zmw/zmw.h"

void event()
{
  static GdkWindow *window_tip_right =NULL , *window_tip_bottom = NULL;

/* DO NOT DISPLAY */
zmw_button("A button") ;

ZMW(zmw_if(zmw_tip_visible()))
  ZMW(zmw_window_popup_right_with_id(&window_tip_right))
     zmw_label("Right tip") ;

ZMW(zmw_tip()) // Equivalent to zmw_if(zmw_tip_visible())
  ZMW(zmw_window_popup_bottom_with_id(&window_tip_bottom))
     zmw_label("Bottom tip") ;
/* DO NOT DISPLAY */
}

void top()
{
  ZMW(zmw_window("Tip"))
    event() ;
}

int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ; // Take ZMW parameters
  zmw_main(top) ; // Launch the infinite loop
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 26 35
zmw_sleep 1
zmw_dump_screen 0

REGRESSION TEST */
