/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

void registration(void)
{
  static Zmw_Name clicked = ZMW_NAME_UNREGISTERED("Clicked button") ;

  zmw_padding_width(1) ;
  zmw_border_width(1) ;
  ZMW(zmw_window("Registration"))
    {
      ZMW(zmw_box_vertical())
	{
	  ZMW(zmw_box_horizontal())
	    {
	      zmw_text("Clicked button =") ;
	      if ( zmw_name_registered(&clicked) )
		zmw_text(zmw_name_registered(&clicked)) ;
	      else
		zmw_text("No button clicked         ") ;
	    }
	  

	  ZMW(zmw_decorator(Zmw_Decorator_Border_Solid))
	    {
	      ZMW(zmw_box_horizontal())
		{
		  zmw_text("Box1") ;
		  zmw_button("Button1") ;
		  if ( zmw_activated() )
		    zmw_name_register(&clicked) ;
		}
	    }
	  
	  ZMW(zmw_decorator(Zmw_Decorator_Border_Solid))
	    {
	      ZMW(zmw_box_vertical())
		{
		  ZMW(zmw_box_horizontal())
		    {
		      zmw_text("Box2") ;
		      zmw_button("Button2") ;
		      if ( zmw_activated() )
			zmw_name_register(&clicked) ;
		    }
		  ZMW(zmw_box_horizontal())
		    {
		      zmw_button("Button3") ;
		      if ( zmw_activated() )
			zmw_name_register(&clicked) ;
		      if ( zmw_name_is(&clicked) )
			zmw_text("This button == last clicked") ;
		      else
			zmw_text("This button != last clicked") ;
		    }
		}
	    }
	  if ( zmw_name_contains(&clicked) )
	    zmw_text("Box2 contains last clicked") ;
	  else
	    zmw_text("Last clicked is not in Box2") ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_run(registration) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 195 49 # button 1
zmw_dump_screen 0

zmw_button_click
zmw_dump_screen 1

zmw_move_cursor_to 186 75 # button 2
zmw_button_click
zmw_dump_screen 2

zmw_move_cursor_to 37 97 # button 3
zmw_button_click
zmw_dump_screen 3



REGRESSION TEST */
