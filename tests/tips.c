/*
    ZMW: A Zero Memory Widget Library
    Copyright (C) 2002-2003  Thierry EXCOFFIER, LIRIS

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

static void tip(const char *t)
{
  static GdkWindow *w ;

  if ( zmw_tip_visible() )
    {
      ZMW(tip_in_place
	  ? zmw_box_vertical()
	     : zmw_window_popup(&w,0) )
	{  
	  ZMW(zmw_box_vertical())
	    {
	      zmw_text("Tip:") ;
	      zmw_text(t) ;
	    }
	  zmw_border_embossed_in_draw() ;
	}
    }
}

void test_tips(const char *title)
{
  static GdkWindow *w=NULL, *w2=NULL ;
  static int p = 0 ;
  
  ZMW(zmw_window(&w, title))
    {
      zmw_name("Stack") ;
      ZMW(zmw_box_vertical())
	{
	  zmw_name("?") ;
	  zmw_text("Help will appear if the cursor") ;
	  zmw_text("stay a moment in the same place") ;
	  zmw_button("Supercalifragilix....") ;
	  tip("This button does really nothing") ;
	  zmw_button("Button to pop a menu with help") ;
	  tip("Try it ! (YES THERE IS A BUG, I KNOW IT)") ;
	  if ( zmw_window_is_popped(p) )
	    ZMW(zmw_window_popup(&w2,p))
	    {
	      ZMW(zmw_box_vertical())
		{
		  zmw_tearoff(&p, &w2) ;
		  tip("Click here to detach the menu") ;
		  zmw_button("Action 1") ;
		  tip("Some action") ;
		  if ( zmw_activated() )
		    printf("Action 1 activated\n") ;
		  zmw_button("Action 2") ;
		  tip("Some other action") ;
		}
	    }
	  zmw_toggle_with_label(&tip_in_place, "Tip opened in place") ;
	  tip("This toogle allows to not use a new window for the tip");
	  zmw_text("Next to Last text in this window") ;
	  zmw_text("Last text in this window") ;
	}
      tip("Generic window tip for items without tips") ;
    }
}
