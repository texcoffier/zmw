/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void message(GdkWindow **w, Zmw_Boolean *visible,
		 const char *title, const char *button_name)
{
  /* Indicates that a child will be inserted */
  ZMW_EXTERNAL_RESTART ;
  if ( *visible )
    ZMW(zmw_window_with_id(w, title))
    {
      ZMW(zmw_vbox())
	{
	  ZMW_EXTERNAL ; /* Insert the child here */
	  zmw_horizontal_alignment(Zmw_False) ;
	  zmw_horizontal_expand(Zmw_False) ;
	  /* The button to close the message window */
	  zmw_button(button_name) ;
	  if ( zmw_activated() )
	    *visible = Zmw_False ;
	}
    }
  /* Indicates that a child has been inserted */
  ZMW_EXTERNAL_STOP ;
}

void use_the_message_widget(void)
{
  static GdkWindow *message_window=NULL ;
  static Zmw_Boolean visible = Zmw_True ;

  ZMW(message(&message_window, &visible, "My Message", "Close window"))
    {
      zmw_label("Any widget you want") ;
    }
}
/* DO NOT DISPLAY */

int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ; // Take ZMW parameters
  zmw_main(use_the_message_widget) ; // Launch the infinite loop
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 30 35
zmw_dump_screen 0

REGRESSION TEST */
