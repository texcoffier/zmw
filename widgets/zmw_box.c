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

#include <zmw/zmw.h>
#include "zmw/zmw_private.h" /* This include is only here for speed up */


void zmw_box_compute_required_size()
{
  Zmw_Rectangle c, d ;
  int i ;

  c = *zmw_child__required_get(0) ;
  zmw_padding_add(&c, zmw_child__padding_width_get(0)) ;
  for(i=1; i<zmw_nb_of_children_get(); i++)
      if ( zmw_child__used_by_parent_get(i) )
	{
	  d = *zmw_child__required_get(i) ;
	  zmw_padding_add(&d, zmw_child__padding_width_get(i)) ;
	  c = zmw_rectangle_max(&d, &c) ;
	}

  zmw_min_width_set(c.width) ;
  zmw_min_height_set(c.height) ;
}

void zmw_fixed()
{
  int i ;

  switch( zmw_subaction_get() )
    {
    case Zmw_Compute_Required_Size:
      if ( zmw_nb_of_children_get() )
	{
	  zmw_box_compute_required_size() ;
	}
      else
	{
	  zmw_min_width_set(10) ;
	  zmw_min_height_set(10) ;
	}
      break ;

    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:
    case Zmw_Compute_Children_Allocated_Size:
      for(i=0; i<zmw_nb_of_children_get(); i++)
	{
	  if ( !zmw_child__used_by_parent_get(i) )
	    continue ;

	  zmw_child__allocated_set(i, zmw_child__required_get(i)) ;
	  *zmw_child__allocated_x_get_ptr(i) += zmw_allocated_x_get() + zmw_child__padding_width_get(i) ;
	  *zmw_child__allocated_y_get_ptr(i) += zmw_allocated_y_get() + zmw_child__padding_width_get(i) ;
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



