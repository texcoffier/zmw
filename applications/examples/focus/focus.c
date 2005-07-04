/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

/* Minimal focus test where the user redefine focus */
void focus(void)
{
  static Zmw_Name *focus = NULL ;
  static char *text = NULL ;

  if ( text == NULL )
    {
      text = strdup("?") ;
      zmw_name_initialize(&focus, "var") ;
    }

  ZMW(zmw_window("Focus"))
    {
      zmw_focus(focus) ;
      zmw_entry(&text) ;
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(focus) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 30 15
zmw_dump_screen 0

zmw_button_click
zmw_dump_screen 1

zmw_key $KEY_A
zmw_dump_screen 2

REGRESSION TEST */
