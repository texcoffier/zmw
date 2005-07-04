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


void expand(const char *title)
{
  static int hexpand[] = { 0, 0, 0, 1 } ;
  static int vexpand[] = { 0, 0, 0, 1 } ;
  static int halign[] = { -1, 0, 1, 0 } ;
  static int valign[] = { -1, 0, 1, 0 } ;

  int i ;

  ZMW(zmw_window(title))
    {
      zmw_name("Row") ;
      ZMW(zmw_hbox())
	{
	  for(i=0; i<ZMW_TABLE_SIZE(hexpand); i++)
	    {
	      zmw_vertical_expand(vexpand[i]) ;
	      zmw_horizontal_expand(hexpand[i]) ;
	      zmw_vertical_alignment(valign[i]) ;
	      zmw_horizontal_alignment(halign[i]) ;
	      ZMW(zmw_vbox())
		{
		  zmw_horizontal_expand(0) ;
		  zmw_vertical_expand(0) ;
		  zmw_check_button_int_with_label(&vexpand[i], "Vertical Expand") ;
		  zmw_label("Vertical Alignment") ;
		  ZMW(zmw_hbox())
		    {
		      zmw_radio_button(&valign[i], -1) ;
		      zmw_radio_button(&valign[i], 0) ;
		      zmw_radio_button(&valign[i], 1) ;
		    }

		  zmw_check_button_int_with_label(&hexpand[i], "Horizontal Expand") ;
		  zmw_label("Horizontal Alignment") ;
		  ZMW(zmw_hbox())
		    {
		      zmw_radio_button(&halign[i], -1) ;
		      zmw_radio_button(&halign[i], 0) ;
		      zmw_radio_button(&halign[i], 1) ;
		    }

		}
	      zmw_border_relief_out_draw() ;
	    }
	}
    }
}

void test_expand(const char *title)
{
  static int x=50, y=50, wi=100, h=100 ;

  ZMW(zmw_window(title))
    {
      ZMW(zmw_fixed())
	{
	  ZMW(zmw_anchor_box(&x, &y, &wi, &h))
	    {
	      zmw_horizontal_expand(1) ;
	      zmw_vertical_expand(1) ;
	      zmw_vertical_alignment(0) ;
	      zmw_horizontal_alignment(0) ;
	      ZMW(zmw_hbox())
		{
		  zmw_vertical_expand(1) ;
		  zmw_horizontal_expand(0) ;
		  ZMW(zmw_vbox())
		    {
		  zmw_vertical_expand(0) ;
		      zmw_label("1") ;
		  zmw_vertical_expand(1) ;
		      zmw_label("2") ;
		  zmw_vertical_expand(0) ;
		      zmw_label("3") ;
		    }
		  zmw_border_embossed_in_draw() ;
		  zmw_vertical_expand(1) ;
		  zmw_horizontal_expand(1) ;
		  zmw_label("B") ;
		  zmw_border_embossed_in_draw() ;
		  zmw_vertical_expand(0) ;
		  zmw_horizontal_expand(0) ;
		  zmw_label("C") ;
		  zmw_border_embossed_in_draw() ;
		}
	    }
	}
    }
}
