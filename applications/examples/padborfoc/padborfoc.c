/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

void rectangle(/* any parameter you may want */)
{
  switch( zmw_subaction_get() )
    {
    case Zmw_Compute_Required_Size:
      zmw_min_width_set(1) ;
      zmw_min_height_set(1) ;
    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:
      zmw_draw_rectangle(Zmw_Color_Foreground, Zmw_True
			 ,zmw_allocated_x_get(), zmw_allocated_y_get()
			 ,zmw_allocated_width_get(), zmw_allocated_height_get()
			 ) ;
      break ;
    default:
      break ;
    }
}

void rect(int with_border)
{
  int b[] = { 0
	      , Zmw_Decorator_Border_Relief
	      , Zmw_Decorator_Focusable| Zmw_Decorator_Border_Focus
	      , Zmw_Decorator_Border_Relief | Zmw_Decorator_Focusable | Zmw_Decorator_Border_Focus } ;
  
  if ( with_border )
    {
      
      ZMW( zmw_decorator( b[with_border] ) )
	{
	  ZMW(rectangle())
	    { /* no children */ }
	}
    }
  else
    ZMW(rectangle())
      { /* no children */ }
}

void item(int with_border, int border, int padding, int focus)
{
  ZMW(zmw_decorator( Zmw_Decorator_Interior | Zmw_Decorator_Border_In
		     | Zmw_Decorator_Border_Embossed) )
    {
      zmw_border_width(padding) ;
      zmw_focus_width(focus) ;
      zmw_padding_width(padding) ;
      ZMW(zmw_vbox())
	{
	  ZMW(zmw_hbox())
	    {
	      zmw_foreground(1,0,0) ;
	      rect(with_border) ;
	      zmw_foreground(1,0.3,0) ;
	      rect(with_border) ;
	      zmw_foreground(1,0.6,0) ;
	      rect(with_border) ;
	    }
	  ZMW(zmw_hbox())
	    {
	      zmw_foreground(1,0,1) ;
	      rect(with_border) ;
	      zmw_foreground(1,0.5,1) ;
	      rect(with_border) ;
	    }
	  ZMW(zmw_hbox())
	    {
	      ZMW(zmw_vbox())
		{
		  /*
		  zmw_foreground(0,0,1) ;
		  rect(with_border) ;
		  zmw_foreground(0,0.3,1) ;
		  rect(with_border) ;
		  */
		  zmw_foreground(0,0.6,1) ;
		  rect(with_border) ;
		}
	      ZMW(zmw_vbox())
		{
		  zmw_foreground(0.4,0.4,0.4) ;
		  rect(with_border) ;
		  zmw_foreground(1,1,1) ;
		  rect(with_border) ;
		}
	    }
	}
    }
}

void test(void)
{
  int p, b, with_border ;

  ZMW(zmw_window("Pixel"))
    {
      zmw_horizontal_expand(Zmw_False) ;
      zmw_vertical_expand(Zmw_False) ;
      zmw_padding_width(0) ;
      zmw_border_width(2) ;
      ZMW(zmw_hbox())
	{
	  for(with_border=0; with_border<4; with_border++)
	    {
	      ZMW(zmw_vbox())
		for(b=0; b<4; b++)
		  {
		    ZMW(zmw_hbox())
		      {
			for(p=0; p<3; p++)
			  {
			    item(with_border, b/2, p, b%2) ;
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
  zmw_main(test) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 23 76
zmw_dump_screen 0


REGRESSION TEST */
