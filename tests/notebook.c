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


static void toggle_page()
{
  static int toggle_a=0, toggle_b=0 ;

  zmw_text("Toggle") ;

  ZMW(zmw_box_vertical())
    {
      zmw_text("Toggle A") ;
      zmw_toggle(&toggle_a) ;
      zmw_text("Toggle B") ;
      zmw_toggle(&toggle_b) ;
    }
}
static void text_page()
{
  zmw_text("Text") ;
  zmw_text("Simple Text") ;
}

static void editable_text_page()
{
  static char *editable_text = NULL ;

  if ( editable_text == NULL )
    editable_text = strdup("Some text") ;

  ZMW(zmw_box_vertical())
    {
      zmw_text("Editable") ;
      zmw_text("Text") ;
    }

  ZMW(zmw_box_horizontal())
    {
      zmw_text("Édit: ") ;
      zmw_text_editable(&editable_text) ;
    }
}

void radio_page()
{
  int i ;
  static int radio = 2 ;

  zmw_text("Radio") ;

  ZMW(zmw_box_vertical())
    {
      for(i=0 ;i<4; i++)
	ZMW(zmw_box_horizontal())
	{
	  zmw_text("Radio") ;
	  zmw_radio(&radio, i) ;
	}
    }
}

void notebook_page()
{
  int i ;
  static int page = 0 ;
  char tmp[999] ;

  zmw_name("notebook label") ;
  zmw_text("Notebook") ;

  ZMW(zmw_box_vertical())
    {
      ZMW(zmw_notebook(&page))
	{	  
	  for(i=0 ;i<4; i++)
	    {
	      sprintf(tmp, "Page %d", i) ;
	      zmw_text(tmp) ;
	      
	      sprintf(tmp, "Page content : %*d", i*2+1,i) ;
	      zmw_text(tmp) ;
	    }
	}
    }
}

void window_page()
{
  static GdkWindow *w = NULL, *w2 = NULL ;
  static int detached = 0 ;

  zmw_name("window page label") ;
  zmw_text("Window") ;
  zmw_vertical_alignment(1) ;
  zmw_name("wp") ;
  ZMW(zmw_box_vertical())
    {
      zmw_text("When this page is visible") ;
      zmw_text("A window must appear") ;
      zmw_text("or more if the menu is detached") ;
      ZMW(zmw_window(&w, "In the notebook page"))
	{
	  zmw_text("This window appear only for page 'Window'") ;
	}
      zmw_name("menu button") ;
      zmw_button("Menu") ;
      if ( zmw_window_is_popped(detached) )
	{
	  zmw_name("menu window") ;
	  ZMW(zmw_window_popup(&w2,detached))
	    {
	      zmw_name("menu") ;
	      ZMW(zmw_box_vertical())
		{
		  zmw_tearoff(&detached, &w2) ;
		  zmw_button("An action") ;
		}
	    }
	}
    }
}


void test_notebook(const char *title)
{
  static GdkWindow *w = NULL ;
  static int p = 0 ;

  zmw_horizontal_expand(0) ;

  ZMW(zmw_window(&w, title))
    {
      zmw_horizontal_alignment(0) ;
      ZMW(zmw_box_vertical())
	{
	  zmw_text("Text above the notebook") ;
	  ZMW(zmw_notebook(&p))
	    {
	      text_page() ;
	      toggle_page() ;
	      editable_text_page() ;
	      radio_page() ;
	      notebook_page() ;
	      window_page() ;
	    }
	  zmw_text("Text under the notebook") ;
	}
    }
}
