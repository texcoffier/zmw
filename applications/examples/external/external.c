/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

void between_braces(void)
{
  ZMW_EXTERNAL_RESTART ;
  ZMW(zmw_box_horizontal())
    {
      zmw_text("{") ;
      ZMW_EXTERNAL ;
      zmw_text("}") ;
    }
  ZMW_EXTERNAL_STOP ;
}

void external(void)
{
  ZMW(zmw_window("Composition"))
    {
      ZMW(between_braces())
	{
	  zmw_text("HELLO") ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_run(external) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 26 36
zmw_dump_screen 0




REGRESSION TEST */
