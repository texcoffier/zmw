/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void toggle(void)
{
  static int t1 = 0, t1_change = 0, bits = 56789932 ;
  static int radio = (int)"None" ;
  int i ;

  ZMW(zmw_window("Toggle"))
    {
      zmw_horizontal_alignment(-1) ;
      ZMW(zmw_box_vertical())
	{
	  /* Some boolean toggles */
	  t1 = zmw_toggle_with_label(t1, "Toggle 1") ;
	  if ( zmw_activated() )
	    t1_change = 1 ;

	  zmw_toggle_int_with_label(&t1, "Toggle 2 : same as 1") ;

	  zmw_toggle_int_with_label(&t1_change, "Toggle 1 was activated") ;

	  /* Editing the bits of an integer */
	  ZMW(zmw_box_horizontal())
	    {
	      zmw_width(5) ;
	      zmw_padding_width(0) ;
	      zmw_focus_width(1) ;
	      zmw_border_width(1) ;
	      for(i=0; i<32; i++)
		zmw_toggle_bits_int(&bits, 1<<(31-i)) ;
	    }
	  zmw_int_editable(&bits) ;

	  /* Radio button */
	  zmw_radio_with_label(&radio, (int)"Bonjour", "Hello") ;
	  zmw_radio_with_label(&radio, (int)"Au revoir", "Goodbye") ;
	  zmw_radio_with_label(&radio, (int)"Oui", "Yes") ;
	  zmw_text("French Translation:") ;
	  zmw_text((char*)radio) ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_run(toggle) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 56 38 # On toggle 1
zmw_dump_screen 0

zmw_button_press
zmw_dump_screen 1

zmw_button_release
zmw_dump_screen 2

zmw_move_cursor_to 19 85 # On toggle 1 was activated
zmw_button_click
zmw_move_cursor_to 19 36 # On toggle 1 square
zmw_button_press
zmw_dump_screen 3

zmw_button_release
zmw_move_cursor_to 17 174 # On "yes"
zmw_button_click
zmw_move_cursor_to 74 99 # On a bit
zmw_button_click
zmw_dump_screen 4


zmw_move_cursor_to 16 118 # Start of the integer
zmw_button_click
zmw_key $KEY_A
zmw_dump_screen 5

zmw_move_cursor_to 19 35 # On toggle 1
zmw_button_click          # Activate toggle
zmw_key $KEY_A            # Desactivate toggle
zmw_dump_screen 6



REGRESSION TEST */
