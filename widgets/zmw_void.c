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

      /* Retrieve size even if not used (to please cache check) */
      if ( ZMW_CHILDREN[0].used_to_compute_parent_size )
	{
	  ZMW_SIZE_MIN = ZMW_CHILDREN[0].required ;
	}
      else
	{
	  /* To make cache checking happy */
	  ZMW_SIZE_MIN.width = 0 ;
	  ZMW_SIZE_MIN.height = 0 ;
	}

      ZMW_USED_TO_COMPUTE_PARENT_SIZE
	= ZMW_CHILDREN[0].used_to_compute_parent_size ;
      break ;

    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:
    case Zmw_Compute_Children_Allocated_Size:
      if ( ZMW_CHILDREN[0].used_to_compute_parent_size )
	{
	  ZMW_CHILDREN[0].allocated = ZMW_SIZE_ALLOCATED ;
	}
      break ;

    default:
    }
}

/*
 * As zmw_void but do not display the children if not visible
 */


void zmw_if(Zmw_Boolean visible)
{
  if ( ! visible )
    {
      if ( ZMW_CALL_NUMBER > 0 )
	ZMW_CALL_NUMBER = 100 ;
      if (  ZMW_SUBACTION == Zmw_Compute_Required_Size )
	{
	  ZMW_USED_TO_COMPUTE_PARENT_SIZE = Zmw_False ;
	  /* To make cache checking happy */
	  /* Must be the same values than in the former function */
	  ZMW_SIZE_MIN.width = 0 ;
	  ZMW_SIZE_MIN.height = 0 ;
	}
      return ;
    }

  zmw_void() ;
}

/*
 * To make coding easy.
 * Should be used inside ZMW()
 */

void zmw_tip()
{
  zmw_if(zmw_tip_visible()) ;
}

void zmw_popup()
{
  zmw_if(zmw_window_is_popped()) ;
}

void zmw_popup_with_detached(int *detached)
{
  zmw_if(zmw_window_is_popped_with_detached(detached)) ;
}

