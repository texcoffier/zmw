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


void test_quit(const char *title)
{
  static GdkWindow *w = NULL, *tip = NULL ;

  ZMW(zmw_window(&w, title))
    {
      zmw_button("Quit") ;
      if ( zmw_activated() )
	exit(0) ;
      if ( zmw_tip_visible() )
	{
	  ZMW(zmw_window_popup(&tip,0))
	    {
	      zmw_text("Quit the program") ;
	    }
	}
    }
}
