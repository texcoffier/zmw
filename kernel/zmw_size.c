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

void zmw_asked_size_set_required_size()
{
  int i ;

  for(i=0; i<ZMW_NB_OF_CHILDREN; i++)
    {
      if ( ZMW_CHILDREN[i].asked.x != ZMW_VALUE_UNDEFINED )
	ZMW_CHILDREN[i].min.x = ZMW_CHILDREN[i].asked.x ;
      if ( ZMW_CHILDREN[i].asked.y != ZMW_VALUE_UNDEFINED )
	ZMW_CHILDREN[i].min.y = ZMW_CHILDREN[i].asked.y ;
      if ( ZMW_CHILDREN[i].asked.width != ZMW_VALUE_UNDEFINED )
	ZMW_CHILDREN[i].min.width = ZMW_CHILDREN[i].asked.width ;
      if ( ZMW_CHILDREN[i].asked.height != ZMW_VALUE_UNDEFINED )
	ZMW_CHILDREN[i].min.height = ZMW_CHILDREN[i].asked.height ;
    }
}

/*
 * Zmw are in the box at the position they asked
 */
/*
void zmw_compute_box_size(int allocated)
{
  Zmw_Rectangle c ;
  int i ;

  some_precomputation(allocated) ;

  if ( allocated )
    {
      for(i=0; i<ZMW_NB_OF_CHILDREN; i++)
	{
	  ZMW_CHILDREN[i].allocated = ZMW_CHILDREN[i].required ;

	  if ( ZMW_CHILDREN[i].allocated.x == ZMW_VALUE_UNDEFINED )
	    ZMW_CHILDREN[i].allocated.x = 0 ;
	  if ( ZMW_CHILDREN[i].allocated.y == ZMW_VALUE_UNDEFINED )
	    ZMW_CHILDREN[i].allocated.y = 0 ;
	}
    }
  else
    {
      if ( ZMW_NB_OF_CHILDREN )
	{
	  c = ZMW_CHILDREN[0].allocated ;
	  for(i=1; i<ZMW_NB_OF_CHILDREN; i++)
	    c = rectangle_max(&ZMW_CHILDREN[i].allocated, &c) ;
	  ZMW_SIZE_REQUIRED = c ;
	}
      else
	{
	  ZMW_SIZE_REQUIRED = zMw[1].size.required ;
	}
    }
}
*/

/*
 *
 */

void zmw_compute_no_size()
{
  if ( ZMW_SUBACTION == Zmw_Compute_Required_Size )
    ZMW_SIZE = zMw[1].u.size ;
}

/*
 *
 */

#define SWAP(X,Y) { tmp = X ; X = Y ; Y = tmp ; }

static void size_swap_x_y(Zmw_Size *ws)
{
  int tmp ;

 SWAP(ws->allocated.x         ,ws->allocated.y       ) ;
 SWAP(ws->allocated.width     ,ws->allocated.height  ) ;
 SWAP(ws->required.x          ,ws->required.y        ) ;
 SWAP(ws->required.width      ,ws->required.height   ) ;
 SWAP(ws->min.x          ,ws->min.y        ) ;
 SWAP(ws->min.width      ,ws->min.height   ) ;
 SWAP(ws->asked.x             ,ws->asked.y           ) ;
 SWAP(ws->asked.width         ,ws->asked.height      ) ;
 SWAP(ws->horizontal_expand   ,ws->vertical_expand   ) ;
 SWAP(ws->horizontal_alignment,ws->vertical_alignment) ;
}

void zmw_swap_x_y()
{
  int i ;

  for(i=0; i<ZMW_NB_OF_CHILDREN; i++)
    size_swap_x_y(&ZMW_CHILDREN[i]) ;
  size_swap_x_y(&ZMW_SIZE) ;
  size_swap_x_y(&zMw[1].u.size) ;
}

