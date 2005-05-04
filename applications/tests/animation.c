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


void test_animation(const char *title)
{
  struct timeval tv ;

  gettimeofday(&tv, NULL);

  ZMW(zmw_window(title))
    {
      ZMW(zmw_vbox())
	{

	  zmw_height(100) ;
	  zmw_label(ctime(&tv.tv_sec)) ;

	  if ( zmw_drawing() )
	    zmw_draw_line(Zmw_Color_Border_Dark
			  , 50, 50
			  , 50 + 50*cos((tv.tv_sec+tv.tv_usec/1000000.))
			  , 50 + 50*sin((tv.tv_sec+tv.tv_usec/1000000.))
			  ) ;
	}
      zmw_need_repaint() ;
    }
}
