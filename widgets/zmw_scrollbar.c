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

#include "zmw.h"

void zmw_scrollbar_horizontal(float *x)
{
  ZMW(zmw_box_horizontal())
    {
      /*      if ( ZMW_CALL_NUMBER ) */
	zmw_x( (int)(*x * zMw[-1].u.size.allocated.width) ) ;
      zmw_height(10) ;
      zmw_anchor_vertical_float(x) ;
      if ( *x < 0 )
	*x = 0 ;
      else if ( *x > 1 )
	*x = 1 ;
    }
  zmw_border_embossed_in_draw() ;
}
