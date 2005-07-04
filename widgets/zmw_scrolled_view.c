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
#include "zmw/zmw_private.h" /* This include is only here for speed up */

Zmw_Boolean zmw_child_visible(int child)
{
  if ( !zmw_child__used_by_parent_get(child) )
    return Zmw_False ;
  return zmw_child__allocated_y_get(child) + zmw_child__allocated_height_get(child)
    < zmw_parent__allocated_y_get() + zmw_parent__allocated_height_get() ;
}

/* Assume that the height of the next child is the same */
Zmw_Boolean zmw_child_next_visible(int child)
{
  return zmw_child__allocated_y_get(child)
    + 2*zmw_child__allocated_height_get(child)
    < zmw_parent__allocated_y_get() + zmw_parent__allocated_height_get() ;
}

void zmw_scrolled_view_clamp(int *start, int *nb, int max)
{
  if ( *start + *nb >= max )
    {
      *start = max - *nb ;
      if ( *start < 0 )
	{
	  *start = 0 ;
	  *nb = max ;
	}
    }
}

void zmw_scrolled_view_with_columns(int *start, int *nb, int max, int nb_cols)
{
  Zmw_Float_0_1 y, y_size, delta ;
  int i, j ;

  ZMW_EXTERNAL_RESTART ;

  zmw_scrolled_view_clamp(start, nb, max) ;
  ZMW(zmw_hbox())
    {
      zmw_horizontal_expand(Zmw_True) ;
      zmw_vertical_expand(Zmw_True) ;
      zmw_vertical_alignment(-1) ;
      
      ZMW_EXTERNAL ;

      if ( zmw_action_get() == zmw_action_dispatch_accelerator )
	continue ;


      if ( max>*nb && zmw_drawing() )
	{
	  if (  *nb * nb_cols > zmw_nb_of_children_get() )
	    {
	      ZMW_HERE ;
	      zmw_debug_trace() ;
	      zmw_printf("zmw_scrolled_view does not contains "
			 "the good number of children (%d) *nb=%d\n"
			 , zmw_nb_of_children_get(), *nb);
	      continue ;
	    }

	  /* Trim number of item displayed */
	  for(i=zmw_nb_of_children_get()-1; i>=0; i--)
	    if ( zmw_child_visible(i) )
	      {
		*nb = 0 ;
		for(j=0; j<i+1; j++)
		  if ( zmw_child__used_by_parent_get(j) )
		    (*nb)++ ;
		*nb /= nb_cols ;
		break ;
	      }
	  /* Increase the number of items displayed */
	  if ( zmw_child_next_visible(zmw_nb_of_children_get()-1) )
	    *nb = *nb * 1.5 + 10 ; // Replace this by a better one
	  zmw_scrolled_view_clamp(start, nb, max) ;
	}
      
      zmw_horizontal_expand(Zmw_False) ;

      if ( max )
	{
	  y = *start / (float)max ;
	  y_size = *nb / (float)max ;
	  if ( y_size > 1 )
	    y_size = 1 ;
	  if ( y > 1 - y_size )
	    {
	      y = 1 - y_size ;
	      *start = max - *nb ;
	    }
	  delta = 1. / *nb ;
	}
      else
	{
	  y = 0 ;
	  y_size = 1 ;
	  delta = 0 ;
	}
      zmw_vertical_expand(Zmw_True) ;
      zmw_vscrollbar_with_delta(&y, y_size, delta) ;
      if ( zmw_changed() )
	{
	  *start = y * max + 0.499 ;
	  zmw_scrolled_view_clamp(start, nb, max) ;
	}

    }
  ZMW_EXTERNAL_STOP ;
}

void zmw_scrolled_view(int *start, int *nb, int max)
{
   zmw_scrolled_view_with_columns(start, nb, max, 1) ;
}

