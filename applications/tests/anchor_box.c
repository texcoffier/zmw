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


void test_anchor_box(const char *title)
{
  static int x=5, y=5, width=60, height=40 ;
  static int x2=5, y2=100, width2=160, height2=140 ;

  ZMW(zmw_window(title))
    {
      zmw_height(300) ;
      zmw_width(300) ;
      zmw_horizontal_expand(1) ;
      zmw_vertical_expand(1) ;
      zmw_horizontal_alignment(0) ;
      zmw_vertical_alignment(0) ;
      ZMW(zmw_fixed())
	{
	  ZMW(zmw_anchor_box(&x, &y, &width, &height))
	    {
	      zmw_label("toto") ;
	    }
	  ZMW(zmw_anchor_box(&x2, &y2, &width2, &height2))
	    {
	      ZMW(zmw_vbox())
		{
		  zmw_label("blablablabla") ;
		  zmw_button("top") ;
		  ZMW(zmw_hbox())
		    {
		      zmw_button("foo") ;
		      zmw_label("blabla") ;
		      zmw_button("bar") ;
		    }
		  zmw_label("blablablabla") ;
		}
	    }
	}
    }
}
