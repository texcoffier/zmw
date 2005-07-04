#include "zmw/zmw.h"
void top()
{
  zmw_horizontal_alignment(0) ;
  zmw_vertical_alignment(0) ;
  zmw_horizontal_expand(0) ;
  zmw_vertical_expand(1) ;
  ZMW(zmw_window("cs_bug"))
    {
      zmw_width(300);
      zmw_height(50);
      ZMW(zmw_hbox())
	{
	  zmw_button("1") ;
	  zmw_vertical_alignment(-1) ;
	  zmw_button("2") ;
	  zmw_vertical_alignment(0) ;
	  zmw_horizontal_expand(1) ;
	  zmw_button("3") ;
	  zmw_horizontal_expand(0) ;
	  zmw_button("4") ;
	  zmw_vertical_alignment(1) ;
	  zmw_button("5") ;
	  zmw_vertical_alignment(0) ;
	  zmw_button("6") ;
	  zmw_vertical_expand(0) ;
	  zmw_button("7") ;
	  zmw_vertical_alignment(-1) ;
	  zmw_button("8") ;
	}
    }
}

int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ; // Take ZMW parameters
  zmw_main(top) ; // Launch the infinite loop
  return 0 ;
}

/* REGRESSION TEST

zmw_move_cursor_to 26 35
zmw_dump_screen 0

REGRESSION TEST */
