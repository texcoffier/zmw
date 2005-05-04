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


void the_boxed_label(int i)
{
  if ( i != 0 )
    ZMW(zmw_vbox())
    {
      zmw_padding_width(5) ;
      if ( i == 2 )
	{
	  zmw_label("Top") ;
	  zmw_label("Center") ;
	  zmw_label("Bottom") ;
	}
      else
	{
	  zmw_label("Label") ;
	}
    }
  else
    zmw_label("Label") ;

  zmw_border_embossed_in_draw() ;
}

void test_asked(const char *title)
{
  static int x=100, y=60, width=100, height=80 ;
  int i ;

  zmw_auto_resize(1) ;
  ZMW(zmw_window(title))
    {
      zmw_horizontal_alignment(0) ;
      zmw_vertical_alignment(0) ;

      ZMW(zmw_vbox())
	{
	  zmw_label("The boxed widget size and position is") ;
	  zmw_label("modified by the four values") ;
	  
	  ZMW(zmw_hbox())
	    {
	      zmw_label("X=") ;
	      zmw_int_editable(&x) ;
	      zmw_label("Y=") ;
	      zmw_int_editable(&y) ;
	      zmw_label("Width=") ;
	      zmw_int_editable(&width) ;
	      zmw_label("Height=") ;
	      zmw_int_editable(&height) ;
	    }
	  for(i=0;i<3;i++)
	    {
	      zmw_label("---------------------") ;
	      ZMW(zmw_hbox())
		{
		  zmw_label("BoxA") ;
		  zmw_x(x) ;
		  zmw_y(y) ;
		  zmw_width(width) ;
		  zmw_height(height) ;
		  the_boxed_label(i) ;
		  zmw_x(ZMW_VALUE_UNDEFINED) ;
		  zmw_y(ZMW_VALUE_UNDEFINED) ;
		  zmw_width(ZMW_VALUE_UNDEFINED) ;
		  zmw_height(ZMW_VALUE_UNDEFINED) ;
		  zmw_label("BoxC") ;
		}
	    }
	}
    }
}
