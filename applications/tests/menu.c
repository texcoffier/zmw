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


static void popup_leaf(int j, int *radio)
{
  int i ;
  char name[99] ;
  
  ZMW(zmw_window_popup_right())
    {
      ZMW(zmw_vbox())
	{
	  zmw_tearoff() ;
	  for(i=0; i<3; i++)
	    {
	      sprintf(name, "Button-%d-%d", j,i) ;
	      zmw_name(name) ;
	      zmw_button(name) ;
	    }
	  zmw_name("RadioBox") ;
	  for(i=0; i<3; i++)
	      zmw_radio_button_with_label(&radio[j], i, "Radio toggle") ;
	}
    }
}

static void popup_node()
{
  int i ;
  char name[99] ;
  static int v ;
  static char *text = NULL ;
  static int radio[3*3] ;

  if ( text == NULL )
    text = strdup("Edit me!") ;

  zmw_horizontal_alignment(-1) ;
  ZMW(zmw_window_popup_right())
    {
      ZMW(zmw_vbox())
	{
	  zmw_tearoff() ;
	  for(i=0; i<3; i++)
	    {
	      sprintf(name, "Button-%d", i) ;
	      zmw_name(name) ;
	      zmw_button(name) ;
	      ZMW( zmw_menu() )
		{
		  popup_leaf(i, &radio[i*3]) ;
		}
	    }
	  zmw_horizontal_expand(0) ;
	  zmw_name("ToggleBox") ;
	  zmw_check_button_int_with_label(&v, "Toggle in menu") ;
	  zmw_entry(&text) ;
	  
	  zmw_button("Print a message") ;
	  if ( zmw_activated() )
	  	zmw_printf("button in menu activated\n") ;
	}
    }
}

void test_menu(const char *title)
{
  ZMW(zmw_window(title))
    {
      zmw_name("Stack") ;
      ZMW(zmw_vbox())
	{
	  zmw_button("Menu") ;
	  ZMW( zmw_menu() )
	    popup_node() ;
	}
    }
}

void test_minimal_menu(const char *title)
{
  ZMW(zmw_window(title))
    {
      zmw_name("But") ;
      zmw_button("Menu") ;
      ZMW( zmw_menu() )
	{
	  ZMW(zmw_window_popup_right())
	    {
	      zmw_name("Action") ;
	      zmw_button("Action") ;
	      if ( zmw_activated() )
		printf("Activated\n") ;
	    }
	}
    }
}

void test_basic_menu(const char *title)
{
  static int detached0 = 0, detached1 = 0 ;

  ZMW(zmw_window(title))
    {
      zmw_name("Stack") ;
      ZMW(zmw_vbox())
	{
	  zmw_name("But") ;
	  zmw_button("Menu") ;
	  ZMW( zmw_menu_with_detached(&detached0) )
	    {
	      ZMW(zmw_window_popup_right())
		{
		  zmw_name("But1") ;
		  zmw_button("But1") ;
		  ZMW( zmw_menu_with_detached(&detached1) )
		    {
		      ZMW(zmw_window_popup_right())
			{
			  zmw_name("But2") ;
			  zmw_button("But2") ;
			}
		    }
		}
	    }
	}
    }
}


void test_basic_menu2(const char *title)
{
  ZMW(zmw_window(title))
    {
      zmw_name("Stack") ;
      ZMW(zmw_vbox())
	{
	  zmw_name("But") ;
	  zmw_button("Menu") ;
	  ZMW( zmw_menu() )
	    {
	      ZMW(zmw_window_popup_right())
		{
		  zmw_name("But1") ;
		  zmw_button("But1") ;
	/*	  ZMW ( zmw_tip() )
		  	{
		  	  ZMW(zmw_window_popup_right(0) )
		  	  	{
		  	  		zmw_label("foo") ;
		  	  	}
		  	}
		  	*/
		  if ( zmw_activated() )
		  	zmw_printf("button activated\n") ;
		  ZMW ( zmw_tip() )
		  	{
		  	  ZMW(zmw_window_popup_bottom() )
		  	  	{
		  	  		zmw_label("bar") ;
		  	  	}
		  	}
		}
	    }
	}
    }
}

void test_menu_tearoff_simple(const char *title)
{
  ZMW(zmw_window(title))
    {
      zmw_button("Menu") ;
      ZMW( zmw_menu() )
	{
	  ZMW(zmw_window_popup_right_with_title("pop"))
	    {
	      ZMW(zmw_vbox())
		{
		  zmw_tearoff() ;
		  zmw_button("button") ;
		}
	    }
	}
    }
}

void test_menu_tearoff_simple_orig(const char *title)
{
  static GdkWindow *w = NULL ;
  static int detached = 0 ;

  ZMW(zmw_window(title))
    {
      zmw_button("Menu") ;
      ZMW( zmw_menu_with_detached(&detached) )
	{
	  ZMW(zmw_window_popup_right_with_id_and_title(&w, "pop"))
	    {
	      ZMW(zmw_vbox())
		{
		  zmw_tearoff() ;
		  zmw_button("button") ;
		}
	    }
	}
    }
}
