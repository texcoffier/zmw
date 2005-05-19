/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

#define SPAN 2
#define NB 32

int detached_state[NB] = { 0 } ;

void recursive_menu(int id)
{
  char buf[99] ;
  int i ;

  if ( id >= NB )
    return ;

  sprintf(buf, "#%d", id) ;
  zmw_button(buf) ;
  ZMW( zmw_menu_with_detached(&detached_state[id]) )
    {
      ZMW(zmw_window_popup_right_with_title(buf))
	{
	  ZMW(zmw_vbox())
	    {
	      zmw_tearoff() ;
	      for(i=0; i<SPAN; i++)
		recursive_menu(SPAN*id + i) ;
	    }
	}
    }
}

void menu3(void)
{
  static int initialised = 0 ;

  if ( ! initialised )
    {
      initialised = 1 ;
      detached_state[1] = Zmw_Menu_Contains_A_Detached ;
      detached_state[2] = Zmw_Menu_Contains_A_Detached ;
      detached_state[3] = Zmw_Menu_Is_Detached ;
      detached_state[4] = Zmw_Menu_Is_Detached ;
      detached_state[7] = Zmw_Menu_Contains_A_Detached ;
      detached_state[15] = Zmw_Menu_Contains_A_Detached ;
      detached_state[31] = Zmw_Menu_Is_Detached ;      
    }


  ZMW(zmw_window("Window1"))
    {
      recursive_menu(1) ;
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(menu3) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 17 34        # L1

zmw_dump_screen 0


REGRESSION TEST */
