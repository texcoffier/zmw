/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
#define HB(X) do { ZMW(zmw_hbox()) { X; } } while(0)

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
      ZMW(zmw_vbox())
	{
	  HB(zmw_label("Surname")  ; zmw_entry(&surname)) ;
	  HB(zmw_label("Firstname"); zmw_entry(&firstname)) ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(define) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 30 30  
zmw_dump_screen 0


REGRESSION TEST */
