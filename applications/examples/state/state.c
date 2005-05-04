/* DO NOT DISPLAY */
#include "zmw/zmw.h"
void top()
{
  zmw_horizontal_expand(Zmw_False) ;
  ZMW(zmw_window("State"))
/* DO NOT DISPLAY */
ZMW(zmw_vbox())
  {
    zmw_horizontal_alignment(ZMW_CENTER) ;
    zmw_foreground(0,0,1) ; // Blue
    zmw_label("Blue") ;
    ZMW(zmw_vbox())
      {
	zmw_horizontal_alignment(ZMW_LEFT) ;
        zmw_label("Blue") ;
	zmw_foreground(1,0,0) ; // Red
        zmw_label("Red") ;
	zmw_foreground(0,1,0) ; // Green
        zmw_font_family("sans") ;
        zmw_font_size(24) ;
        zmw_font_style(Zmw_Font_Style_Italic) ;
        zmw_label("Green") ;
      }
    zmw_horizontal_alignment(ZMW_RIGHT) ;
    zmw_label("Blue") ;
  }
/* DO NOT DISPLAY */
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
zmw_dump_screen 0

REGRESSION TEST */
