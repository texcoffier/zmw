/* DO NOT DISPLAY */
#include "zmw/zmw.h"
void top()
{
  zmw_horizontal_expand(Zmw_False) ;
  ZMW(zmw_window("State"))
/* DO NOT DISPLAY */
ZMW(zmw_box_vertical())
  {
    zmw_horizontal_alignment(ZMW_CENTER) ;
    zmw_foreground(0,0,1) ; // Blue
    zmw_text("Blue") ;
    ZMW(zmw_box_vertical())
      {
	zmw_horizontal_alignment(ZMW_LEFT) ;
        zmw_text("Blue") ;
	zmw_foreground(1,0,0) ; // Red
        zmw_text("Red") ;
	zmw_foreground(0,1,0) ; // Green
        zmw_font("-adobe-helvetica-bold-*-*-*-*-240-*-*-*-*-*-1") ;
        zmw_text("Green") ;
      }
    zmw_horizontal_alignment(ZMW_RIGHT) ;
    zmw_text("Blue") ;
  }
/* DO NOT DISPLAY */
}

int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ; // Take ZMW parameters
  zmw_run(top) ; // Launch the infinite loop
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 26 35
zmw_dump_screen 0

REGRESSION TEST */
