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


void test_trace(const char *title)
{
  static GdkWindow *w = NULL, *w2 = NULL ;

  zmw_name("Window") ;
  ZMW(zmw_window(&w, title))
    {
      zmw_name("BoxH") ;
      ZMW(zmw_box_horizontal())
	{
	  zmw_name("BoxV") ;
	  ZMW(zmw_box_vertical())
	    {
	      zmw_name("TextA") ;
	      zmw_text("A") ;
	      zmw_name("TextB") ;
	      zmw_text("B") ;
	      zmw_name("OtherWindow") ;
	      ZMW(zmw_window(&w2, "Other Window"))
		{
		  zmw_name("text") ;
		  zmw_text("text") ;
		}
	    }
	  ZMW(zmw_box_vertical())
	    {
	      zmw_name("TextC") ;
	      zmw_text("C") ;
	      zmw_name("TextD") ;
	      zmw_text("D") ;
	    }
	}
    }
}

void test_trace_2(const char *title)
{
  static GdkWindow *w = NULL ;

  zmw_name("Window") ;
  ZMW(zmw_window(&w, title))
    {
      zmw_name("Box") ;
      ZMW(zmw_box_horizontal())
	{
	      zmw_name("Text A") ;
	      zmw_text("A") ;
	      zmw_name("Text B") ;
	      zmw_text("B") ;
	}
    }
}
