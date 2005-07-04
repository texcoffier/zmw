/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */


void f1(void)
{
  *zmw_debug_get_ptr() |= Zmw_Debug_Window_Auto_Resize ;
  ZMW(zmw_window("F1"))
    {
      zmw_label("My application") ;
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(f1) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

# DO_NOT_MAKE_REGRESSION_TEST

zmw_move_cursor_to 26 36
zmw_key_press $KEY_CONTROL
zmw_key_press $KEY_F1
zmw_key_release $KEY_F1
zmw_key_release $KEY_CONTROL

zmw_move_cursor_to 124 137
zmw_button_click
zmw_move_cursor_to 124 109
zmw_button_click
zmw_move_cursor_to 124 84
zmw_button_click
zmw_move_cursor_to 124 58
zmw_button_click

zmw_dump_screen 0




REGRESSION TEST */
