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

void test_state(const char *title)
{
  ZMW(zmw_window(title))
    {
      ZMW(zmw_vbox())
	{
	  zmw_horizontal_alignment(0) ;
	  zmw_color(Zmw_Color_Foreground, 0x000000ff) ;
	  zmw_label("Blue") ;
	  ZMW(zmw_vbox())
	    {
	      zmw_horizontal_alignment(-1) ;
	      zmw_label("Blue") ;
	      zmw_color(Zmw_Color_Foreground, 0x00ff0000) ;
	      zmw_label("Red") ;
	      zmw_color(Zmw_Color_Foreground, 0x0000ff00) ;
	      zmw_font_family("swiss") ;
	      zmw_font_style(Zmw_Font_Style_Italic) ;
	      zmw_font_size(32) ;
	      zmw_label("Green") ;
	    }
	  zmw_horizontal_alignment(1) ;
	  zmw_label("Blue") ;
	}
    }
}
