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



void test_viewport(const char *title)
{
  static GdkWindow *w = NULL ;
  static float x=0, y=0, x2=0, y2=0 ;
  static int toggle[100] ;
  int i ;
  char buf[99] ;

  ZMW(zmw_window(&w, title))
    {
      ZMW(zmw_viewport_with_scrollbar(&x, &y))
	{
	  ZMW(zmw_box_vertical())
	    {
	      for(i=0; i<10; i++)
		{
		  sprintf(buf, "Hello %d terrible world!", i) ;
		  zmw_toggle_with_label(&toggle[i], buf) ;
		}
	      zmw_width(200) ;
	      zmw_height(100) ;
	      ZMW(zmw_viewport_with_scrollbar(&x2, &y2))
		{
		  ZMW(zmw_box_vertical())
		    {
		      zmw_text("A long long long long text to scroll") ;
		      zmw_text("A long text to scroll") ;
		      zmw_text("A long text to scroll") ;
		    }
		}
	    }
	}
    }
}
