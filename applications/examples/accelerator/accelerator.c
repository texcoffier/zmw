/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void accelerator(void)
{
  int c, a, m, s ;
  static int code = -1 ;
  char buf[99] ;

  ZMW(zmw_window("Accelerator"))
    {
      ZMW(zmw_vbox())
	{
	  zmw_horizontal_alignment(-1) ;
	  for(c=0; c<=GDK_CONTROL_MASK; c += GDK_CONTROL_MASK)
	    for(a=0; a<=GDK_MOD1_MASK; a += GDK_MOD1_MASK)
	      for(m=0; m<=GDK_MOD2_MASK; m += GDK_MOD2_MASK)
		for(s=0; s<=GDK_SHIFT_MASK; s += GDK_SHIFT_MASK)
		  {
		    sprintf(buf, "Button %2d", s|c|a|m) ;
		    zmw_button_with_accelerator(buf, s|c|a|m, 'A') ;
		    if ( zmw_activated() )
		      code = s|c|a|m ;
		  }
	  zmw_int(code) ;
	}
    }
}

/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(accelerator) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_small_scale 2

zmw_move_cursor_to 232 222 # Right middle of the window
zmw_dump_screen 0

zmw_key $KEY_A
zmw_dump_screen 1

zmw_key_press $KEY_SHIFT
zmw_dump_screen 2

zmw_key $KEY_A
zmw_dump_screen 3

zmw_key_press $KEY_CONTROL
zmw_dump_screen 4

zmw_key $KEY_A
zmw_dump_screen 5

zmw_key_press $KEY_ALT
zmw_dump_screen 6

zmw_key $KEY_A
zmw_dump_screen 7

zmw_key_release $KEY_ALT
zmw_key_press $KEY_META
zmw_dump_screen 8

zmw_key $KEY_A
zmw_dump_screen 9

zmw_key_press $KEY_ALT
zmw_dump_screen 10

zmw_key $KEY_A
zmw_dump_screen 11

zmw_key_release $KEY_ALT
zmw_key_release $KEY_META
zmw_key_release $KEY_CONTROL
zmw_key_release $KEY_SHIFT

zmw_button_click
zmw_dump_screen 12

zmw_key_press $KEY_CONTROL
zmw_key_press $KEY_META
zmw_key $KEY_A
zmw_dump_screen 13

zmw_key_release $KEY_META
zmw_key_release $KEY_CONTROL

zmw_small_restore

REGRESSION TEST */
