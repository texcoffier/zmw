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
 *
 */
void zmw_box_horizontal_required_size()
{
  int width, height, i ;

  width = 0 ;
  height = 0 ;
  for(i=0; i<ZMW_NB_OF_CHILDREN; i++)
    {
      if ( !ZMW_CHILDREN[i].used_to_compute_parent_size )
	continue ;

      if ( ZMW_CHILDREN[i].required.x == ZMW_VALUE_UNDEFINED )
	{
	  width += ZMW_CHILDREN[i].required.width ;
	}
      else
	{
	  if ( ZMW_CHILDREN[i].required.x > width )
	    width = ZMW_CHILDREN[i].required.x
	      + ZMW_CHILDREN[i].required.width;
	  else
	    {
	      /* Same as above.
	       * A max should be computed
	       */
	      width = ZMW_CHILDREN[i].required.x
		+ ZMW_CHILDREN[i].required.width;
	    }
	}
      if ( ZMW_CHILDREN[i].required.height > height )
	height = ZMW_CHILDREN[i].required.height ;
      if ( ZMW_CHILDREN[i].required.y != ZMW_VALUE_UNDEFINED )
	if ( ZMW_CHILDREN[i].required.y + ZMW_CHILDREN[i].required.height
	     > height )
	  height = ZMW_CHILDREN[i].required.y
	    + ZMW_CHILDREN[i].required.height ;

    }

  ZMW_SIZE_MIN.width = width ;
  ZMW_SIZE_MIN.height = height ;
}

void zmw_box_horizontal_children_allocated_size()
{
  int i, last, first, nb_expandable ;

  if (  ZMW_NB_OF_CHILDREN == 0 )
    return ;

  nb_expandable = 0 ;
  for(i=0; i<ZMW_NB_OF_CHILDREN; i++)
    {
      if ( ZMW_CHILDREN[i].used_to_compute_parent_size
	   && ZMW_CHILDREN[i].horizontal_expand
	   )
	nb_expandable++ ;
    }
  
  last = 0 ;
  first = 1 ;
  for(i=0; i<ZMW_NB_OF_CHILDREN; i++)
    {
      if ( !ZMW_CHILDREN[i].used_to_compute_parent_size )
	{
	  ZMW_CHILDREN[i].allocated = ZMW_CHILDREN[i].required ;
	  continue ;
	}

      /*
       * X compute
       */
      
      if ( ZMW_CHILDREN[i].required.x != ZMW_VALUE_UNDEFINED )
	{
	  ZMW_CHILDREN[i].allocated.x = ZMW_SIZE_ALLOCATED.x
	    + ZMW_CHILDREN[i].required.x ;
	}
      else
	{
	  if ( first )
	    {
	      if ( nb_expandable != 0 )
		ZMW_CHILDREN[i].allocated.x = ZMW_SIZE_ALLOCATED.x ;
	      else
		{
		  if ( ZMW_SIZE_HORIZONTAL_ALIGNMENT < 0 )
		    ZMW_CHILDREN[i].allocated.x = ZMW_SIZE_ALLOCATED.x ;
		  else
		    {
		      if ( ZMW_SIZE_HORIZONTAL_ALIGNMENT > 0 )
			ZMW_CHILDREN[i].allocated.x
			= ZMW_SIZE_ALLOCATED.x
			  + ( ZMW_SIZE_ALLOCATED.width
			      - ZMW_SIZE_REQUIRED.width )
			  ;
		      else
			ZMW_CHILDREN[i].allocated.x
			  = ZMW_SIZE_ALLOCATED.x
			  + ( ZMW_SIZE_ALLOCATED.width
			      - ZMW_SIZE_REQUIRED.width ) / 2
			  ;
		    }
		}
	    }
	  else
	    {
	      ZMW_CHILDREN[i].allocated.x =
		ZMW_CHILDREN[last].allocated.x
		+ ZMW_CHILDREN[last].allocated.width ;
	    }
	}
      first = 0 ;
	    
      /*
       * Y and height compute
       */
      if ( ZMW_CHILDREN[i].required.y != ZMW_VALUE_UNDEFINED )
	{
	  ZMW_CHILDREN[i].allocated.y = ZMW_SIZE_ALLOCATED.y
	    + ZMW_CHILDREN[i].required.y ;
	  ZMW_CHILDREN[i].allocated.height = ZMW_SIZE_ALLOCATED.height
	     - ZMW_CHILDREN[i].required.y ;
	}
      else
	{
	  zmw_alignement_vertical_make(&ZMW_CHILDREN[i], 0) ;
	  /*
	  if ( ZMW_CHILDREN[i].vertical_expand )
	    {
	      ZMW_CHILDREN[i].allocated.y = ZMW_SIZE_ALLOCATED.y ;
	      ZMW_CHILDREN[i].allocated.height = ZMW_SIZE_ALLOCATED.height ;
	    }
	  else
	    {
	      if ( ZMW_CHILDREN[i].vertical_alignment < 0 )
		{
		  ZMW_CHILDREN[i].allocated.y = ZMW_SIZE_ALLOCATED.y ;
		  ZMW_CHILDREN[i].allocated.height
		    = ZMW_CHILDREN[i].required.height ;
		}
	      else
		if ( ZMW_CHILDREN[i].vertical_alignment > 0 )
		  {
		    ZMW_CHILDREN[i].allocated.y = ZMW_SIZE_ALLOCATED.y
		      + ZMW_SIZE_ALLOCATED.height
		      - ZMW_CHILDREN[i].required.height ;
		    ZMW_CHILDREN[i].allocated.height
		      = ZMW_CHILDREN[i].required.height ;
		  }
		else
		  {
		    ZMW_CHILDREN[i].allocated.y = ZMW_SIZE_ALLOCATED.y
		      + (ZMW_SIZE_ALLOCATED.height
			 - ZMW_CHILDREN[i].required.height)/2 ;
		    ZMW_CHILDREN[i].allocated.height
		      = ZMW_CHILDREN[i].required.height ;
		  }
	    }
	  */
	}
	  
      /*
       * Width compute
       */
      if ( ZMW_CHILDREN[i].horizontal_expand )
	ZMW_CHILDREN[i].allocated.width
	  = ZMW_CHILDREN[i].required.width
	  + (ZMW_SIZE_ALLOCATED.width-ZMW_SIZE_MIN.width)/nb_expandable ; /* Can't be a 0 divide */
      else
	{
	  /* Should test if ``reductable'' */
	  ZMW_CHILDREN[i].allocated.width = ZMW_CHILDREN[i].required.width;
	}


      last = i ;
    }
}

void zmw_box_horizontal_with_activable(Zmw_Boolean activable)
{
  switch( ZMW_SUBACTION )
    {
    case Zmw_Compute_Required_Size:
      zmw_box_horizontal_required_size() ;
      break ;
    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:
      if ( activable )
	zmw_border_draw( (activable      ? Zmw_Border_Focusable : 0) |
			 (zmw_focused() ? Zmw_Border_Draw_Focus : 0)
			 ) ;

    case Zmw_Compute_Children_Allocated_Size:
      zmw_box_horizontal_children_allocated_size() ;
      break ;
    case Zmw_Input_Event:
      if ( activable )
	{
	  zmw_focusable() ;
	  zmw_activable() ;
	  if ( zmw_key_string() )
	    {
	      zmw.activated = 1 ;
	    }
	}
    default:
      break ;
    }
}

void zmw_box_horizontal()
{
  zmw_box_horizontal_with_activable(Zmw_False) ;
}

void zmw_box_vertical()
{
  zmw_swap_x_y() ;
  zmw_box_horizontal() ;
  zmw_swap_x_y() ;
}

void zmw_box_horizontal_activable()
{
  zmw_box_horizontal_with_activable(Zmw_True) ;
}

void zmw_box_vertical_activable()
{
  zmw_swap_x_y() ;
  zmw_box_horizontal_with_activable(Zmw_True) ;
  zmw_swap_x_y() ;
}






/*
 *
 */

void zmw_box_compute_required_size()
{
  Zmw_Rectangle c ;
  int i ;

  c = ZMW_CHILDREN[0].required ;
  for(i=1; i<ZMW_NB_OF_CHILDREN; i++)
      if ( ZMW_CHILDREN[i].used_to_compute_parent_size )
      c = zmw_rectangle_max(&ZMW_CHILDREN[i].required, &c) ;

  ZMW_SIZE_MIN.width = c.width + c.x  ;
  ZMW_SIZE_MIN.height = c.height + c.y ;
}


void zmw_box()
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
	  ZMW_CHILDREN[i].allocated.x += ZMW_SIZE_ALLOCATED.x ;
	  ZMW_CHILDREN[i].allocated.y += ZMW_SIZE_ALLOCATED.y ;
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



