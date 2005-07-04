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
#include "zmw/zmw_private.h" /* This include is only here for speed up */

/*
 * Used to change attributes for a widget without the attributes going
 * to the siblings.
 * This widget remove a level of padding, so its size
 * is the same than its content.
 */
void zmw_void()
{
  int i, nb ;

  switch(zmw_subaction_get())
    {
    case Zmw_Compute_Required_Size:
      if ( zmw_nb_of_children_get() == 0 )
	ZMW_ABORT ;
      for(i=0, nb=0; i<zmw_nb_of_children_get(); i++)
	if ( zmw_child__used_by_parent_get(i) )
	  nb++ ;
      if ( nb > 1 )
	ZMW_ABORT ;

      /* Retrieve size even if not used (to please cache check) */
      if ( zmw_child__used_by_parent_get(0) )
	{
	  zmw_min_set(zmw_child__required_get(0)) ;
	}
      else
	{
	  /* To make cache checking happy */
	  zmw_min_width_set(0) ;
	  zmw_min_height_set(0) ;
	}

      zmw_used_by_parent_set(zmw_child__used_by_parent_get(0)) ;
      break ;

    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:
    case Zmw_Compute_Children_Allocated_Size:
      if ( zmw_child__used_by_parent_get(0) )
	{
	  zmw_child__allocated_set(0,zmw_allocated_get()) ;
	}
      else
	{
	  /* Make valgrind happy ? */
	  zmw_child__allocated_set(0,zmw_allocated_get()) ;
	}
      break ;

    default:
      break ;
    }
}

/*
 * As zmw_void but do not display the children if not visible
 */

void zmw_if_(Zmw_Boolean visible, Zmw_Boolean recurse_on_init)
{
  if ( ! visible )
    {
      if ( !recurse_on_init || zmw_call_number_get() > 0)	
	zmw_call_number_set(100) ;

      zmw_used_by_parent_set(Zmw_False) ;
      /* To make valgrind happy */
      zmw_rectangle_void(zmw_min_get()) ;
      zmw_rectangle_void(zmw_allocated_get()) ;
      return ;
    }
  zmw_void() ;
}

void zmw_if(Zmw_Boolean visible)
{
  zmw_if_(visible, Zmw_False) ;
}

void zmw_if_with_accelerators(Zmw_Boolean visible)
{
  zmw_if_(visible, Zmw_True) ;
}

/*
 * To make coding easy.
 * Should be used inside ZMW()
 */

void zmw_tip()
{
  zmw_if(zmw_tip_visible()) ;
}

void zmw_menu()
{
  zmw_if_with_accelerators(zmw_window_is_popped()) ;
}

void zmw_menu_without_accelerators()
{
  zmw_if(zmw_window_is_popped()) ;
}

void zmw_menu_with_detached(int *detached)
{
  zmw_if(zmw_window_is_popped_with_detached(detached)) ;
}

