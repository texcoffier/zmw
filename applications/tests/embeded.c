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

/*
 * This widget put a border around its content
 */

void zmw_border_widget()
{
  ZMW_EXTERNAL_RESTART ;

  ZMW(zmw_vbox())
    {
      ZMW_EXTERNAL ;
    }
  zmw_border_embossed_in_draw() ;

  ZMW_EXTERNAL_STOP ;
}


void test_embeded(const char *title)
{
  ZMW(zmw_window(title))
    {
      ZMW(zmw_vbox())
	{
	  zmw_label("text not embeded") ;

	  ZMW(zmw_border_widget())
	    {
	      zmw_label("2 lines of text") ;
	      zmw_label("embeded once") ;
	    }

	  ZMW(zmw_border_widget())
	    {
	      ZMW(zmw_border_widget())
		{
		  zmw_label("2 lines of text") ;
		  zmw_label("embeded twice") ;
		}
	    }
	}
    }
}

