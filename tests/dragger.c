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


void box_line(int action)
{
  static int x=80, width=80 ;
  int a ;

  ZMW(zmw_box_horizontal())
    {
      zmw_padding_width(3) ;
      zmw_horizontal_alignment(0) ;

      zmw_text("FIXEDSIZELEFT") ;
      zmw_border_embossed_in_draw() ;
      
      zmw_horizontal_expand(1) ;
      ZMW(zmw_box_horizontal())
	{
	  if ( action == 1 )
	    zmw_width(width) ;
	  ZMW(zmw_box_horizontal())
	    {
	      ZMW(zmw_box_vertical())
		{
		  zmw_text("A") ;
		}
	      zmw_border_embossed_in_draw() ;
	      ZMW(zmw_box_vertical())
		{
		  zmw_text("B") ;
		}
	      zmw_border_embossed_in_draw() ;
	      ZMW(zmw_box_vertical())
		{
		  zmw_text("C") ;
		}
	      zmw_border_embossed_in_draw() ;
	    }
	  zmw_border_embossed_in_draw() ;
	  if ( action == 1 )
	    zmw_width(ZMW_VALUE_UNDEFINED) ;

	  if ( action == 0 )
	    {
	      a = ZMW_SIZE_ALLOCATED.x ;
	      zmw_x(x) ;
	      zmw_anchor_vertical(&x) ;
	      zmw_x(ZMW_VALUE_UNDEFINED) ;
	    }


	  if ( action == 1 )
	    {
	      zmw_anchor_vertical(&width) ;
	      a = ZMW_SIZE_ALLOCATED.x ;
	    }

	  ZMW(zmw_box_horizontal())
	    {
	      zmw_text("A") ;
	      zmw_text("B") ;
	      zmw_text("C") ;
	    }
	  zmw_border_embossed_in_draw() ;
	}
      zmw_border_embossed_in_draw() ;
      zmw_horizontal_expand(0) ;
      
      zmw_text("FIXEDSIZERIGHT") ;
      zmw_border_embossed_in_draw() ;
    }
}

void test_dragger(const char *title)
{
  static GdkWindow *w = NULL ;

  ZMW(zmw_window(&w, title))
    {
      ZMW(zmw_box_vertical())
	{
	  zmw_text("The dragger change the X of the right widget") ;

	  box_line(0) ;

	  zmw_text("The dragger change the width of the left widget") ;
	  box_line(1) ;

	}
    }
}
