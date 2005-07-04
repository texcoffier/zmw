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

/*
 * This file is used to create "zmw_vbox.c"
 * It is a Python script that edit the source : zmw-swap-coordinates.py
 */

#include "zmw/zmw.h"
#include "zmw/zmw_private.h" /* This include is only here for speed up */

/*
 *
 */
void zmw_hbox_required_size(int focusable)
{
  int width, height, i ;
  Zmw_Rectangle r ;

  width = 0 ;
  height = 0 ;
  for(i=0; i<zmw_nb_of_children_get(); i++)
    {
      if ( !zmw_child__used_by_parent_get(i) )
	continue ;

      r = *zmw_child__required_get(i) ;
      zmw_padding_add(&r, zmw_child__padding_width_get(i)) ;

      if ( r.x == ZMW_VALUE_UNDEFINED )
	{
	  width += r.width ;
	}
      else
	{
	  if ( r.x + r.width > width )
	    width = r.x + r.width ;
	}
      if ( r.height > height )
	height = r.height ;
      if ( r.y != ZMW_VALUE_UNDEFINED )
	if ( r.y + r.height > height )
	  height = r.y + r.height ;
    }
  if ( focusable )
    {
      width += 2 * zmw_focus_width_get() ;
      height += 2 * zmw_focus_width_get() ;
    }
  zmw_min_width_set(width) ;
  zmw_min_height_set(height) ;
}

void zmw_hbox_children_allocated_size(int focusable)
{
  int i, last, first, nb_expandable ;

  if (  zmw_nb_of_children_get() == 0 )
    return ;

  if ( 0 )
    {
      fprintf(stderr,"\n%s\n", __FUNCTION__) ;
      fprintf(stderr," min=%s", zmw_rectangle_string(zmw_min_get()));
      fprintf(stderr," req=%s", zmw_rectangle_string(zmw_required_get()));
      fprintf(stderr," all=%s", zmw_rectangle_string(zmw_allocated_get()));
      fprintf(stderr,"\n") ;
      for(i=0; i<zmw_nb_of_children_get(); i++)
	{
	  fprintf(stderr, "%d : ha=%d va=%d he=%d/%d ve=%d/%d"
		  , i
		  , zmw_child__horizontal_alignment_get(i)
		  , zmw_child__vertical_alignment_get(i)
		  , zmw_child__horizontal_expand_get(i)
		  , zmw_child__horizontaly_expanded_get(i)
		  , zmw_child__vertical_expand_get(i)
		  , zmw_child__verticaly_expanded_get(i)
		  ) ;
	  fprintf(stderr, " min=%s", zmw_rectangle_string(zmw_child__min_get(i))) ;
	  fprintf(stderr, " req=%s", zmw_rectangle_string(zmw_child__required_get(i))) ;
	  fprintf(stderr, "\n") ;
	}
    }

  nb_expandable = 0 ;
  for(i=0; i<zmw_nb_of_children_get(); i++)
    {
      if ( zmw_child__used_by_parent_get(i)
	   && zmw_child__horizontaly_expanded_get(i)
	   )
	nb_expandable++ ;
    }
  
  last = 0 ;
  first = 1 ;
  for(i=0; i<zmw_nb_of_children_get(); i++)
    {
      if ( !zmw_child__used_by_parent_get(i) )
	{
	  zmw_child__allocated_set(i, zmw_child__required_get(i)) ;
	  continue ;
	}
      /*
       * X compute
       */
      if ( zmw_child__required_x_get(i) != ZMW_VALUE_UNDEFINED )
	{
	  zmw_child__allocated_x_set(i,
				     zmw_allocated_x_get()
				     + zmw_child__required_x_get(i)
				     + zmw_child__padding_width_get(i)
				     ) ;
	}
      else
	{
	  if ( first )
	    {
	      zmw_child__allocated_x_set(i,
					 zmw_allocated_x_get()
					 + zmw_child__padding_width_get(i)
					 ) ;
	      if ( focusable )
		{
		  *zmw_child__allocated_x_get_ptr(i) += zmw_focus_width_get() ;
		  *zmw_child__allocated_y_get_ptr(i) += zmw_focus_width_get() ;
		}
	      if ( nb_expandable == 0 )
		{
		  if ( zmw_cs_horizontal_alignment_get() >= 0 )
		    {
		      if ( zmw_cs_horizontal_alignment_get() > 0 )
			zmw_child__allocated_x_set
			  (i,
			   zmw_allocated_x_get()
			   + zmw_child__padding_width_get(i)
			   + ( zmw_allocated_width_get()
			       - zmw_required_width_get() )
			   ) ;
		      else
			zmw_child__allocated_x_set
			  (i,
			   zmw_allocated_x_get()
			   + zmw_child__padding_width_get(i)
			   + ( zmw_allocated_width_get()
			       - zmw_required_width_get() ) / 2
			   ) ;
		    }
		}
	    }
	  else
	    {
	      zmw_child__allocated_x_set(i,
					 zmw_child__allocated_x_get(last)
					 + zmw_child__allocated_width_get(last)
					 + zmw_child__padding_width_get(last)
					 + zmw_child__padding_width_get(i)
					 ) ;
	    }
	}
      first = 0 ;
	    
      /*
       * Y and height compute
       */
      if ( zmw_child__required_y_get(i) != ZMW_VALUE_UNDEFINED )
	{
	  zmw_child__allocated_y_set(i, zmw_allocated_y_get()
				     + zmw_child__padding_width_get(i)
				     + zmw_child__required_y_get(i)
				     ) ;
	  zmw_child__allocated_height_set(i, zmw_allocated_y_get()
					  + zmw_allocated_height_get()
					  - zmw_child__allocated_y_get(i)
					  - zmw_child__padding_width_get(i)
					  ) ;

	}
      else
	{
	  zmw_alignment_vertical_make(i, focusable ? zmw_focus_width_get() : 0);
	}
	  
      /*
       * Width compute
       */
      if ( zmw_child__horizontaly_expanded_get(i) )
	{
	  zmw_child__allocated_width_set
	    (i,
	     zmw_child__required_width_get(i)
	     + (zmw_allocated_width_get()
		- zmw_min_width_get())/nb_expandable /* Can't be a 0 divide */
	     ) ;
	}
      else
	{
	  /* Should test if ``reductable'' */
	  zmw_child__allocated_width_set(i, zmw_child__required_width_get(i)) ;
	}

      
      last = i ;
    }
}

void zmw_hbox_with_activable(Zmw_Boolean activable)
{
  if ( activable )
    {
      zmw_focusable() ;
    }

  switch( zmw_subaction_get() )
    {
    case Zmw_Compute_Required_Size:
      zmw_hbox_required_size(activable) ;
      break ;
    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:
      if ( activable )
	{
	  zmw_border_draw( (activable     ? Zmw_Border_Focusable : 0) |
			   (zmw_focused() ? Zmw_Border_Draw_Focus : 0)
			   ) ;
	}

    case Zmw_Compute_Children_Allocated_Size:
      zmw_hbox_children_allocated_size(activable) ;
      break ;
    case Zmw_Input_Event:
      if ( activable )
	{
	  zmw_activable() ;
	  if ( zmw_key_string_unsensitive() )
	    {
	      zmw_activated_set(Zmw_True) ;
	    }
	}
    default:
      break ;
    }
}

void zmw_hbox()
{
  zmw_hbox_with_activable(Zmw_False) ;
}

void zmw_hbox_activable()
{
  zmw_hbox_with_activable(Zmw_True) ;
}





void zmw_alignment_horizontal_make(int i, int border_width)
{
  int free_space ;

  zmw_child__allocated_x_set(i
			     , zmw_allocated_x_get()
			     + border_width
			     + zmw_child__padding_width_get(i)
			     ) ;
  zmw_child__allocated_width_set(i
				 , zmw_allocated_width_get()
				 - 2 * border_width
				 - 2 * zmw_child__padding_width_get(i)
				 ) ;

  if ( ! zmw_child__horizontaly_expanded_get(i) )
    {
      free_space = zmw_allocated_width_get()
	- 2 * border_width
	- zmw_child__required_width_get(i)
	- 2 * zmw_child__padding_width_get(i) ;
      *zmw_child__allocated_width_get_ptr(i) -= free_space ;
      switch(zmw_child__horizontal_alignment_get(i))
	    {
	    case -1:
	      break ;
	    case 0:
	      *zmw_child__allocated_x_get_ptr(i) += free_space / 2 ;
	      break ;
	    case 1:
	      *zmw_child__allocated_x_get_ptr(i) += free_space ;
	      break ;
	    }
    }
}

