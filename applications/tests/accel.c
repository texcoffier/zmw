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


void test_accel(const char *title)
{
  static int tv = 0 ;
  char buf[99] ;

  ZMW(zmw_window(title))
    {
      ZMW(zmw_vbox())
	{
	  zmw_button("Menu To Change The Value") ;
	  ZMW( zmw_menu() )
	    {
	      ZMW( zmw_window_popup_right() )
		{
		  ZMW( zmw_vbox() )
		    {
		      zmw_button_with_accelerator("Increment value"
						  ,GDK_CONTROL_MASK, 'I') ;
		      if ( zmw_activated() )
			tv++ ;
		      zmw_button_with_accelerator("Decrement value"
						  ,GDK_CONTROL_MASK, 'D') ;
		      if ( zmw_activated() )
			tv-- ;
		    }
		}
	    }

	  ZMW( zmw_hbox() )
	    {
	      zmw_label("Value : ") ;
	      sprintf(buf, "%d", tv) ;
	      zmw_label(buf) ;
	    }
	}
    }
}
