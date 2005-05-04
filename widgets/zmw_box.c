/*
    ZMW: A Zero Memory Widget Library
    Copyright (C) 2002-2005 Thierry EXCOFFIER, Université Claude Bernard, LIRIS

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

/*
 * The following lines define the vertical and horizontal boxes
 */

#define C3(A,B,C) A ## B ## C

#define ZMW_HORIZONTAL(A,C) C3(A,horizontal,C)
#define ZMW_VERTICAL(A,C) C3(A,vertical,C)
#define ZMW_H(A,C) C3(A,h,C)
#define ZMW_V(A,C) C3(A,v,C)
#define X x
#define Y y
#define WIDTH width
#define HEIGHT height
#include "zmw_box_hv.h"


#undef ZMW_HORIZONTAL
#undef ZMW_VERTICAL
#undef ZMW_H
#undef ZMW_V
#undef X
#undef Y
#undef WIDTH
#undef HEIGHT

#define ZMW_HORIZONTAL(A,C) C3(A,vertical,C)
#define ZMW_VERTICAL(A,C) C3(A,horizontal,C)
#define ZMW_H(A,C) C3(A,v,C)
#define ZMW_V(A,C) C3(A,h,C)
#define X y
#define Y x
#define WIDTH height
#define HEIGHT width
#include "zmw_box_hv.h"



/*
 *
 */

void zmw_box_compute_required_size()
{
  Zmw_Rectangle c, d ;
  int i ;

  c = ZMW_CHILDREN[0].required ;
  zmw_padding_add(&c, ZMW_CHILDREN[0].current_state.padding_width) ;
  for(i=1; i<ZMW_NB_OF_CHILDREN; i++)
      if ( ZMW_CHILDREN[i].used_to_compute_parent_size )
	{
	  d = ZMW_CHILDREN[i].required ;
	  zmw_padding_add(&d, ZMW_CHILDREN[i].current_state.padding_width) ;
	  c = zmw_rectangle_max(&d, &c) ;
	}

  ZMW_SIZE_MIN.width = c.width ;
  ZMW_SIZE_MIN.height = c.height ;
}


void zmw_fixed()
{
  int i ;

  switch( ZMW_SUBACTION )
    {
    case Zmw_Compute_Required_Size:
      if ( ZMW_NB_OF_CHILDREN )
	{
	  zmw_box_compute_required_size() ;
	}
      else
	{
	  ZMW_SIZE_MIN.width = 10 ;
	  ZMW_SIZE_MIN.height = 10 ;
	}
      break ;

    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:
    case Zmw_Compute_Children_Allocated_Size:
      for(i=0; i<ZMW_NB_OF_CHILDREN; i++)
	{
	  if ( !ZMW_CHILDREN[i].used_to_compute_parent_size )
	    continue ;

	  ZMW_CHILDREN[i].allocated = ZMW_CHILDREN[i].required ;
	  ZMW_CHILDREN[i].allocated.x += ZMW_SIZE_ALLOCATED.x + ZMW_CHILDREN[i].current_state.padding_width ;
	  ZMW_CHILDREN[i].allocated.y += ZMW_SIZE_ALLOCATED.y + ZMW_CHILDREN[i].current_state.padding_width ;
	}
      break ;

    case Zmw_Input_Event:
 //     zmw_focusable() ;
      zmw_activable() ;
      break ;

    default:
      break ;
    }
}



