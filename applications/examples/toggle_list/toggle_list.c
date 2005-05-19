/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void toggle_list(void)
{
  static int toggle_state[5] = {0, 0, 1, 0, 0} ;
  char toggle_name[100] ;
  int i ;

  ZMW(zmw_vbox())
    {
      for(i=0; i<5; i++)
	{
	  sprintf(toggle_name, "Toggle %d", i) ;
	  zmw_check_button_int_with_label(&toggle_state[i], toggle_name) ;
	}
    }
}
/* DO NOT DISPLAY */

void top()
{
  ZMW(zmw_window("Toggle"))
    toggle_list() ;
}

int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ; // Take ZMW parameters
  zmw_main(top) ; // Launch the infinite loop
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 26 15
zmw_dump_screen 0

REGRESSION TEST */
