/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

#define RGB(R,G,B) (((R)<<16) + ((G)<<8) + (B))

void hexa(int i, int nb_digits)
{
  char tmp[99] ;

  sprintf(tmp, "0x%0*X", nb_digits, i) ;
  zmw_text(tmp) ;
}

void appearance(void)
{
  int i, r, g, b ;
  static char *fonts[] = { "-adobe-courier-medium-o-*-*-8-*-*-*-*-*-iso8859-1",
			   "10x20", "12x24", "5x7", "8x13",
			   "9x15",  "7x13", "7x14" } ;

  zmw_vertical_expand(Zmw_False) ;
  ZMW(zmw_window("Boxes"))
    {
      ZMW(zmw_table(5))
	{
	  zmw_text("Red") ;
	  zmw_text("Green") ;
	  zmw_text("Blue") ;
	  zmw_text("Value") ;
	  zmw_text("Sample") ;
	  i = 0 ;
	  for(r = 0; r < 256; r += 255)
	    for(g = 0; g < 256; g += 255)
	      for(b = 0; b < 256; b += 255)
		{
		  hexa(r, 2) ;
		  hexa(g, 2) ;
		  hexa(b, 2) ;
		  hexa(RGB(r, g, b), 6) ;
		  // The color/font change is trapped by the box
		  ZMW(zmw_box_vertical())
		    {
		      zmw_color(ZMW_FOREGROUND, RGB(r, g, b)) ;
		      zmw_font(fonts[i]) ;
		      zmw_text(fonts[i]) ;
		    }
		  i++ ;
		}
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_run(appearance) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 26 36
zmw_dump_screen 0




REGRESSION TEST */
