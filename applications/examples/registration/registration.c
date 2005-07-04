/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

void registration(void)
{
  static Zmw_Name *clicked = NULL ;

  zmw_name_initialize(&clicked, "Clicked button") ;
  zmw_padding_width(1) ;
  zmw_border_width(1) ;
  ZMW(zmw_window("Registration"))
    {
      ZMW(zmw_vbox())
	{
	  ZMW(zmw_hbox())
	    {
	      zmw_label("Clicked button =") ;
	      if ( zmw_name_registered(clicked) )
		zmw_label(zmw_name_registered(clicked)) ;
	      else
		zmw_label("No button clicked         ") ;
	    }
	  

	  ZMW(zmw_decorator(Zmw_Decorator_Border_Solid))
	    {
	      ZMW(zmw_hbox())
		{
		  zmw_label("Box1") ;
		  zmw_button("Button1") ;
		  if ( zmw_activated() )
		    zmw_name_register(clicked) ;
		}
	    }
	  
	  ZMW(zmw_decorator(Zmw_Decorator_Border_Solid))
	    {
	      ZMW(zmw_vbox())
		{
		  ZMW(zmw_hbox())
		    {
		      zmw_label("Box2") ;
		      zmw_button("Button2") ;
		      if ( zmw_activated() )
			zmw_name_register(clicked) ;
		    }
		  ZMW(zmw_hbox())
		    {
		      zmw_button("Button3") ;
		      if ( zmw_activated() )
			zmw_name_register(clicked) ;
		      if ( zmw_name_is(clicked) )
			zmw_label("This button == last clicked") ;
		      else
			zmw_label("This button != last clicked") ;
		    }
		}
	    }
	  if ( zmw_name_contains(clicked) )
	    zmw_label("Box2 contains last clicked") ;
	  else
	    zmw_label("Last clicked is not in Box2") ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(registration) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 195 49 # button 1
zmw_dump_screen 0

zmw_button_click
zmw_dump_screen 1

zmw_move_cursor_to 189 78 # button 2
zmw_button_click
zmw_dump_screen 2

zmw_move_cursor_to 43 100 # button 3
zmw_button_click
zmw_dump_screen 3



REGRESSION TEST */
