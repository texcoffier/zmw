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


static void toggle_page()
{
  static int toggle_a=0, toggle_b=0 ;

  zmw_button("Toggle") ;

  ZMW(zmw_vbox())
    {
      zmw_label("Toggle A") ;
      zmw_check_button_int(&toggle_a) ;
      zmw_label("Toggle B") ;
      zmw_check_button_int(&toggle_b) ;
    }
}
static void text_page()
{
  zmw_button("Text") ;
  zmw_label("Simple Text") ;
}

static void editable_text_page()
{
  static char *editable_text = NULL ;

  if ( editable_text == NULL )
    editable_text = strdup("Some text") ;

  ZMW(zmw_vbox_activable())
    {
      zmw_label("Editable") ;
      zmw_label("Text") ;
    }

  ZMW(zmw_hbox_activable())
    {
      zmw_label("Edit: ") ;
      zmw_entry(&editable_text) ;
    }
}

void radio_page()
{
  int i ;
  static int radio = 2 ;

  zmw_button("Radio") ;

  ZMW(zmw_vbox())
    {
      for(i=0 ;i<4; i++)
	ZMW(zmw_hbox())
	{
	  zmw_label("Radio") ;
	  zmw_radio_button(&radio, i) ;
	}
    }
}

void notebook_page()
{
  int i ;
  static int page = 0 ;
  char tmp[999] ;

  zmw_name("notebook label") ;
  zmw_button("Notebook") ;

  ZMW(zmw_vbox())
    {
      ZMW(zmw_notebook(&page))
	{	  
	  for(i=0 ;i<4; i++)
	    {
	      sprintf(tmp, "Page %d", i) ;
	      zmw_button(tmp) ;
	      
	      sprintf(tmp, "Page content : %*d", i*2+1,i) ;
	      zmw_label(tmp) ;
	    }
	}
    }
}

void window_page()
{
  zmw_name("window page label") ;
  zmw_button("Window") ;
  zmw_vertical_alignment(1) ;
  zmw_name("wp") ;
  ZMW(zmw_vbox())
    {
      zmw_label("When this page is visible") ;
      zmw_label("A window must appear") ;
      zmw_label("or more if the menu is detached") ;
      ZMW(zmw_window("In the notebook page"))
	{
	  zmw_label("This window appear only for page 'Window'") ;
	}
      zmw_name("menu button") ;
      zmw_button("Menu") ;
      ZMW( zmw_menu() )
	{
	  ZMW(zmw_window_popup_right())
	    {
	      zmw_name("menu") ;
	      ZMW(zmw_vbox())
		{
		  zmw_tearoff() ;
		  zmw_button("An action") ;
		}
	    }
	}
    }
}


void test_notebook(const char *title)
{
  static int p = 0 ; // Visible page

  zmw_horizontal_expand(0) ;

  ZMW(zmw_window(title))
    {
      zmw_horizontal_alignment(0) ;
      ZMW(zmw_vbox())
	{
	  zmw_label("Text above the notebook") ;
	  ZMW(zmw_notebook(&p))
	    {
	      text_page() ;
	      toggle_page() ;
	      editable_text_page() ;
	      radio_page() ;
	      notebook_page() ;
	      window_page() ;
	    }
	  zmw_label("Text under the notebook") ;
	}
    }
}
