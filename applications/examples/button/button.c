/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void button(void)
{
  static int on_1 = 0, on_2 = 0, on_3 = 0 ;
  char tmp[99] ;

  ZMW(zmw_window("Button"))
    {
      zmw_horizontal_expand(Zmw_False) ;
      ZMW(zmw_vbox())
	{
	  zmw_button("Button 1") ;
	  if ( zmw_activated() )
	    on_1 = 1 ;

	  zmw_button_with_accelerator
	    ("Button 2", GDK_CONTROL_MASK, 'a') ;
	  if ( zmw_activated() )
	    on_2 = 1 ;

	  zmw_button_with_hidden_accelerator
	    ("Button 3", GDK_CONTROL_MASK, 'b') ;
	  if ( zmw_activated() )
	    on_3 = 1 ;

	  zmw_button("Tested if on_2==True") ;
	  if ( on_2 && zmw_activated() )
	    printf("I have been clicked !\n") ;

	  sprintf(tmp, "State = %d %d %d", on_1, on_2, on_3) ;
	  zmw_label(tmp) ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(button) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST


zmw_move_cursor_to 83 39         # On button 1
zmw_dump_screen 0

zmw_button_press
zmw_dump_screen 1

zmw_button_release
zmw_dump_screen 2

zmw_move_cursor_to 145 140         # Bottom right

zmw_key_press $KEY_CONTROL
zmw_key $KEY_A
zmw_dump_screen 3
zmw_key_release $KEY_CONTROL

zmw_key_press $KEY_CONTROL
zmw_key $KEY_B
zmw_dump_screen 4
zmw_key_release $KEY_CONTROL


zmw_key_press $KEY_CONTROL
zmw_sleep 1
zmw_dump_screen 5

zmw_key_release $KEY_CONTROL

REGRESSION TEST */
