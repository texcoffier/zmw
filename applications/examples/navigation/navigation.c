/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

void navigation(void)
{
  static char *title[] = {"H V", "V H"} ;
  static void (*box1[])() = { zmw_hbox, zmw_vbox } ;
  static void (*box2[])() = { zmw_vbox, zmw_hbox } ;

  int i, j, w ;
  char buf[99] ;

  for(w=0; w<2; w++)
    {
      ZMW(zmw_window(title[w]))
	{
	  ZMW(box1[w]())
	    {
	      for(i=0; i<4; i++)
		ZMW(box2[w]())
		  {
		    for(j=0; j<4; j++)
		      {
			sprintf(buf, "%c", 'A' + i + 4*(j + 4*w)) ;
			zmw_name(buf) ;
			zmw_button(buf) ;
			if ( zmw_activated() )
			  {
			    printf("(%s)", buf) ;
			    fflush(stdout) ;
			  }
		      }
		  }
	    }
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(navigation) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 26 36
zmw_button_click
zmw_dump_screen 0

zmw_key_press $KEY_CONTROL

zmw_key $KEY_LEFT
zmw_dump_screen 1

zmw_key $KEY_UP
zmw_dump_screen 2

zmw_key $KEY_RIGHT
zmw_dump_screen 3

zmw_key $KEY_DOWN
zmw_dump_screen 4

zmw_key $KEY_LEFT
zmw_dump_screen 5

zmw_key $KEY_UP
zmw_dump_screen 6

zmw_key $KEY_RIGHT
zmw_sleep $SMALL2
zmw_key $KEY_RIGHT
zmw_sleep $SMALL2
zmw_key $KEY_RIGHT
zmw_sleep $SMALL2
zmw_key $KEY_RIGHT
zmw_dump_screen 7

zmw_key $KEY_DOWN
zmw_dump_screen 8

zmw_key $KEY_LEFT
zmw_dump_screen 9

zmw_key $KEY_DOWN
zmw_sleep $SMALL2
zmw_key $KEY_DOWN
zmw_dump_screen 10

zmw_key $KEY_RIGHT
zmw_dump_screen 11

zmw_key_release $KEY_CONTROL


zmw_move_cursor_to 99 116 # P
zmw_button_click
zmw_key_press $KEY_CONTROL
zmw_key $KEY_RIGHT
zmw_key_release $KEY_CONTROL



REGRESSION TEST */
