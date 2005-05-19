/*
    ZMW: A Zero Memory Widget Library
    Copyright (C) 2002-2003 Thierry EXCOFFIER, Université Claude Bernard, LIRIS

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Contact: Thierry.EXCOFFIER@liris.univ-lyon1.fr
*/

static int tip_in_place = 0 ;
static int new_tip_in_place = 0 ;

static void tip(const char *t, int right)
{
  ZMW ( zmw_tip() )
    {
      ZMW(tip_in_place
	  ? zmw_vbox()
	     : ( right ? zmw_window_popup_right() : zmw_window_popup_bottom() )
	     )
	{  
	  ZMW(zmw_vbox())
	    {
	      zmw_label("Tip:") ;
	      zmw_label(t) ;
	    }
	  zmw_border_embossed_in_draw() ;
	}
    }
}

void test_tips(const char *title)
{
  ZMW(zmw_window(title))
    {
      zmw_name("Stack") ;
      ZMW(zmw_vbox())
	{
	  zmw_name("?") ;
	  zmw_label("Help will appear if the cursor") ;
	  zmw_label("stay a moment in the same place") ;
	  zmw_button("Supercalifragilix....") ;
	  tip("This button does really nothing",1) ;
	  zmw_button("Button to pop a menu with help") ;
	  tip("Try it!",1) ;
	  ZMW( zmw_menu() )
	    ZMW(zmw_window_popup_right())
	    {
	      ZMW(zmw_vbox())
		{
		  zmw_tearoff() ;
		  tip("Click here to detach the menu",1) ;
		  zmw_button("Action 1") ;
		  tip("Some action",1) ;
		  if ( zmw_activated() )
		    printf("Action 1 activated\n") ;
		  zmw_button("Action 1b : no tip") ;
		  zmw_button("Action 2") ;
		  tip("Some other action",1) ;
		}
	    }
	  zmw_check_button_int_with_label(&new_tip_in_place, "Tip opened in place") ;
	  tip("This toogle allows to not use a new window for the tip",1);
	  zmw_button("Two tips on this button") ;
	  tip("Right tip", 1) ;
	  tip("Bottom tip", 0) ;
	  zmw_label("Next to Last text in this window") ;
	  zmw_label("Last text in this window") ;
	}
      tip("Generic window tip for items without tips",1) ;
    }

  if ( zmw_state_change_allowed() )
    tip_in_place = new_tip_in_place ;
}
