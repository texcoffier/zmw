/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

#define RGB(R,G,B) (((R)<<16) + ((G)<<8) + (B))

void hexa(int i, int nb_digits)
{
  char tmp[99] ;

  sprintf(tmp, "0x%0*X", nb_digits, i) ;
  zmw_label(tmp) ;
}

void appearance(void)
{
  int i, r, g, b ;
  static char *fonts[] = { "courier",
			   "fixed", "lucida", "helvetica", "times",
			   "new century schoolbook",  "monospace", "sans" } ;

  zmw_vertical_expand(Zmw_False) ;
  ZMW(zmw_window("Boxes"))
    {
      ZMW(zmw_table(5))
	{
	  zmw_label("Red") ;
	  zmw_label("Green") ;
	  zmw_label("Blue") ;
	  zmw_label("Value") ;
	  zmw_label("Sample") ;
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
		  zmw_padding_width(0) ;
		  ZMW(zmw_hbox())
		    {
		      zmw_color(Zmw_Color_Foreground, RGB(r, g, b)) ;
		      zmw_font_family(fonts[i]) ;
		      zmw_font_size(8) ;
		      zmw_label(fonts[i]) ;
		      zmw_font_size(10) ;
		      zmw_label(fonts[i]) ;
		      zmw_font_size(12) ;
		      zmw_label(fonts[i]) ;
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
  zmw_main(appearance) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 26 36
zmw_dump_screen 0




REGRESSION TEST */
