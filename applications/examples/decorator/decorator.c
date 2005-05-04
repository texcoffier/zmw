/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
#define C(X,Y) X ## Y
#define T(O) decorated_text(#O, C(Zmw_Decorator_,O)) ; \
             decorated_text(#O, C(Zmw_Decorator_,O) \
                                | Zmw_Decorator_Border_In)

void decorated_text(const char *text, int option)
{
  ZMW(zmw_decorator(option))
    zmw_label(text) ;
  if ( zmw_activated() )
    printf("%s activated\n", text) ;
}


void decorator(void)
{
  ZMW(zmw_window("Decorator"))
    {
      ZMW(zmw_table(2))
	{
	  zmw_label("Option") ;
	  zmw_label("Option | Border_In") ;

	  T(Focusable) ;
	  T(Activable) ;
	  T(Border_Solid) ;
	  T(Border_Embossed) ;
	  T(Border_Relief) ;
	  T(Interior) ;
	  T(Pushable) ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(decorator) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 150 57 # Right Focusable
zmw_dump_screen 0

zmw_button_click
zmw_move_cursor_to 150 81 # Right Activable
zmw_button_click
zmw_move_cursor_to 150 190 # Right Pushable
zmw_button_click

zmw_dump_screen 1

REGRESSION TEST */
