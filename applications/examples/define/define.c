/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
#define HB(X) do { ZMW(zmw_box_horizontal()) { X; } } while(0)

void define(void)
{
  static char *surname = NULL, *firstname = NULL ;

  if ( surname == NULL )
    {
      surname = strdup("Colombus") ;
      firstname = strdup("Christopher") ;
    }

  ZMW(zmw_window("Boxes"))
    {
      ZMW(zmw_box_vertical())
	{
	  HB(zmw_text("Surname")  ; zmw_text_editable(&surname)) ;
	  HB(zmw_text("Firstname"); zmw_text_editable(&firstname)) ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_run(define) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 30 30  
zmw_dump_screen 0


REGRESSION TEST */
