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

/*
 * Used to change attributes for a widget without the attributes going
 * to the siblings.
 * This widget remove a level of padding, so its size
 * is the same than its content.
 */
void zmw_void()
{
  int i, nb ;

  switch(ZMW_SUBACTION)
    {
    case Zmw_Compute_Required_Size:
      if ( ZMW_NB_OF_CHILDREN == 0 )
	ZMW_ABORT ;
      for(i=0, nb=0; i<ZMW_NB_OF_CHILDREN; i++)
	if ( ZMW_CHILDREN[i].used_to_compute_parent_size )
	  nb++ ;
      if ( nb > 1 )
	ZMW_ABORT ;

      if ( ZMW_CHILDREN[0].used_to_compute_parent_size )
	{
	  /* Remove the padding */
	  ZMW_SIZE_MIN = ZMW_CHILDREN[0].required ;
	  ZMW_SIZE_MIN.width -= 2 * ZMW_PADDING_WIDTH ;
	  ZMW_SIZE_MIN.height -= 2 * ZMW_PADDING_WIDTH ;
	}

      ZMW_USED_TO_COMPUTE_PARENT_SIZE
	= ZMW_CHILDREN[0].used_to_compute_parent_size ;
      break ;

    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:
    case Zmw_Compute_Children_Allocated_Size:
      if ( ZMW_CHILDREN[0].used_to_compute_parent_size )
	{
	  ZMW_CHILDREN[0].allocated.x = ZMW_SIZE_ALLOCATED.x - ZMW_PADDING_WIDTH ;
	  ZMW_CHILDREN[0].allocated.y = ZMW_SIZE_ALLOCATED.y - ZMW_PADDING_WIDTH ;
	  ZMW_CHILDREN[0].allocated.width = ZMW_SIZE_ALLOCATED.width + 2*ZMW_PADDING_WIDTH ;
	  ZMW_CHILDREN[0].allocated.height= ZMW_SIZE_ALLOCATED.height + 2*ZMW_PADDING_WIDTH ;
	}
      break ;

    default:
    }
}

