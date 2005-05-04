/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */


void fontlist(void)
{
  static char *families[] = {
    "clearlyu alternate glyphs",
    "courier",
    "mincho",
    "lucida",
    "clearlyu",
    "nil",
    "terminal",
    "monospace",
    "clearlyu ligature",
    "song ti",
    "clean",
    "clearlyu arabic extra",
    "serif",
    "symbol",
    "fixed",
    "lucidabright",
    "new century schoolbook",
    "helvetica",
    "open look cursor",
    "gothic",
    "charter",
    "lucidatypewriter",
    "sans",
    "ledfixed",
    "clearlyu pua",
    "fangsong ti",
    "utopia",
    "times",
    "open look glyph",
    "newspaper",
    NULL
  } ;
  int i ;

  ZMW(zmw_window("Boxes"))
    {
      ZMW(zmw_vbox())
	{
	  for(i=0 ; families[i]; i++)
	    {
	      zmw_font_family(families[i]) ;
	      ZMW(zmw_hbox())
		{
		  zmw_label(families[i]) ;
		  zmw_label("0123456789 abcdefg ABCDEFG llllll") ;
		}
	    }
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(fontlist) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

# DO_NOT_MAKE_REGRESSION_TEST

zmw_move_cursor_to 26 36

zmw_sleep 1

zmw_dump_screen 0




REGRESSION TEST */
