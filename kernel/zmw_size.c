/*
    ZMW: A Zero Memory Widget Library
    Copyright (C) 2002-2004 Thierry EXCOFFIER, Université Claude Bernard, LIRIS

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

#include "zmw/zmw.h"

Zmw_Rectangle zmw_rectangle_max(Zmw_Rectangle *a, Zmw_Rectangle *b)
{
  Zmw_Rectangle c ;

  if ( a->x < b->x )
    c.x = a->x ;
  else
    c.x = b->x ;

  if ( a->y < b->y )
    c.y = a->y ;
  else
    c.y = b->y ;

  if ( a->x + a->width > b->x + b->width )
    c.width = a->x + a->width - c.x ;
  else
    c.width = b->x + b->width - c.x ;

  if ( a->y + a->height > b->y + b->height )
    c.height = a->y + a->height - c.y ;
  else
    c.height = b->y + b->height - c.y ;

  return(c) ;
}

void zmw_rectangle_void(Zmw_Rectangle *a)
{
  memset(a, 0, sizeof(*a)) ;
}

/*
 *
 */

void zmw_padding_add(Zmw_Rectangle *r, int padding)
{
 r->width  += 2*padding ;
 r->height += 2*padding ;
 if ( r->x != ZMW_VALUE_UNDEFINED )
   r->x -= padding ;
 if ( r->y != ZMW_VALUE_UNDEFINED )
   r->y -= padding ;
}

void zmw_padding_remove(Zmw_Rectangle *r, int padding)
{
 r->width  -= 2*padding ;
 r->height -= 2*padding ;
 if ( r->x != ZMW_VALUE_UNDEFINED )
   r->x += padding ;
 if ( r->y != ZMW_VALUE_UNDEFINED )
   r->y += padding ;
}


