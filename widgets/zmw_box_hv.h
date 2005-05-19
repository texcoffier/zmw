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
 * This file is intented to be included only by "zmw_fixed.c"
 * On inclusion, it will define "horizontal" box, the other "vertical" box
 *
 * ZMW_VERTICAL(A,B) inserts the word vertical (or horizontal) between A and B
 * ZMW_HORIZONTAL(A,B) make the reverse.
 */


/*
 *
 */
void ZMW_H(zmw_, box_required_size)(int focusable)
{
  int width, height, i ;
  Zmw_Rectangle r ;

  width = 0 ;
  height = 0 ;
  for(i=0; i<ZMW_NB_OF_CHILDREN; i++)
    {
      if ( !ZMW_CHILDREN[i].used_to_compute_parent_size )
	continue ;

      r = ZMW_CHILDREN[i].required ;
      zmw_padding_add(&r, ZMW_CHILDREN[i].current_state.padding_width) ;

      if ( r.X == ZMW_VALUE_UNDEFINED )
	{
	  width += r.WIDTH ;
	}
      else
	{
	  if ( r.X + r.WIDTH > width )
	    width = r.X + r.WIDTH ;
	}
      if ( r.HEIGHT > height )
	height = r.HEIGHT ;
      if ( r.Y != ZMW_VALUE_UNDEFINED )
	if ( r.Y + r.HEIGHT > height )
	  height = r.Y + r.HEIGHT ;
    }
  ZMW_SIZE_MIN.WIDTH = width ;
  ZMW_SIZE_MIN.HEIGHT = height ;
  if ( focusable )
    {
      ZMW_SIZE_MIN.WIDTH += 2 * ZMW_FOCUS_WIDTH ;
      ZMW_SIZE_MIN.HEIGHT += 2 * ZMW_FOCUS_WIDTH ;
    }
}

void ZMW_H(zmw_, box_children_allocated_size)(int focusable)
{
  int i, last, first, nb_expandable ;

  if (  ZMW_NB_OF_CHILDREN == 0 )
    return ;

  nb_expandable = 0 ;
  for(i=0; i<ZMW_NB_OF_CHILDREN; i++)
    {
      if ( ZMW_CHILDREN[i].used_to_compute_parent_size
	   && ZMW_CHILDREN[i].ZMW_HORIZONTAL(,_expand)
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
      if ( ZMW_CHILDREN[i].required.X != ZMW_VALUE_UNDEFINED )
	{
	  ZMW_CHILDREN[i].allocated.X = ZMW_SIZE_ALLOCATED.X
	    + ZMW_CHILDREN[i].required.X
	    + ZMW_CHILDREN[i].current_state.padding_width ;
	}
      else
	{
	  if ( first )
	    {
	      ZMW_CHILDREN[i].allocated.X = ZMW_SIZE_ALLOCATED.X
		+ ZMW_CHILDREN[i].current_state.padding_width ;
	      if ( focusable )
		{
		  ZMW_CHILDREN[i].allocated.X += ZMW_FOCUS_WIDTH ;
		  ZMW_CHILDREN[i].allocated.Y += ZMW_FOCUS_WIDTH ;
		}
	      if ( nb_expandable == 0 )
		{
		  if ( ZMW_SIZE_CS.ZMW_HORIZONTAL(,_alignment) >= 0 )
		    {
		      if ( ZMW_SIZE_CS.ZMW_HORIZONTAL(,_alignment) > 0 )
			ZMW_CHILDREN[i].allocated.X
			= ZMW_SIZE_ALLOCATED.X
			  + ZMW_CHILDREN[i].current_state.padding_width
			  + ( ZMW_SIZE_ALLOCATED.WIDTH
			      - ZMW_SIZE_REQUIRED.WIDTH )
			  ;
		      else
			ZMW_CHILDREN[i].allocated.X
			  = ZMW_SIZE_ALLOCATED.X
			  + ZMW_CHILDREN[i].current_state.padding_width
			  + ( ZMW_SIZE_ALLOCATED.WIDTH
			      - ZMW_SIZE_REQUIRED.WIDTH ) / 2 ;
			  ;
		    }
		}
	    }
	  else
	    {
	      ZMW_CHILDREN[i].allocated.X =
		ZMW_CHILDREN[last].allocated.X
		+ ZMW_CHILDREN[last].allocated.WIDTH
		+ ZMW_CHILDREN[last].current_state.padding_width
		+ ZMW_CHILDREN[i].current_state.padding_width
		;
	    }
	}
      first = 0 ;
	    
      /*
       * Y and height compute
       */
      if ( ZMW_CHILDREN[i].required.Y != ZMW_VALUE_UNDEFINED )
	{
	  ZMW_CHILDREN[i].allocated.Y = ZMW_SIZE_ALLOCATED.Y
	    + ZMW_CHILDREN[i].current_state.padding_width
	    + ZMW_CHILDREN[i].required.Y ;
	  ZMW_CHILDREN[i].allocated.HEIGHT = ZMW_SIZE_ALLOCATED.Y
	    + ZMW_SIZE_ALLOCATED.HEIGHT
	    - ZMW_CHILDREN[i].allocated.Y
	    - ZMW_CHILDREN[i].current_state.padding_width
	    ;

	}
      else
	{
	  ZMW_VERTICAL(zmw_alignement_,_make)(&ZMW_CHILDREN[i]
				       , focusable ? ZMW_FOCUS_WIDTH : 0) ;
	}
	  
      /*
       * Width compute
       */
      if ( ZMW_CHILDREN[i].ZMW_HORIZONTAL(,_expand) )
	ZMW_CHILDREN[i].allocated.WIDTH
	  = ZMW_CHILDREN[i].required.WIDTH
	  + (ZMW_SIZE_ALLOCATED.WIDTH-ZMW_SIZE_MIN.WIDTH)/nb_expandable ; /* Can't be a 0 divide */
      else
	{
	  /* Should test if ``reductable'' */
	  ZMW_CHILDREN[i].allocated.WIDTH = ZMW_CHILDREN[i].required.WIDTH;
	}

      
      last = i ;
    }
}

void ZMW_H(zmw_, box_with_activable)(Zmw_Boolean activable)
{
  if ( activable )
    {
      zmw_focusable() ;
    }

  switch( ZMW_SUBACTION )
    {
    case Zmw_Compute_Required_Size:
      ZMW_H(zmw_, box_required_size)(activable) ;
      break ;
    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:
      if ( activable )
	zmw_border_draw( (activable      ? Zmw_Border_Focusable : 0) |
			 (zmw_focused() ? Zmw_Border_Draw_Focus : 0)
			 ) ;

    case Zmw_Compute_Children_Allocated_Size:
      ZMW_H(zmw_, box_children_allocated_size)(activable) ;
      break ;
    case Zmw_Input_Event:
      if ( activable )
	{
	  zmw_activable() ;
	  if ( zmw_key_string_unsensitive() )
	    {
	      ZMW_SIZE_ACTIVATED = Zmw_True ;
	    }
	}
    default:
      break ;
    }
}

void ZMW_H(zmw_, box)()
{
  ZMW_H(zmw_, box_with_activable)(Zmw_False) ;
}

void ZMW_H(zmw_, box_activable)()
{
  ZMW_H(zmw_, box_with_activable)(Zmw_True) ;
}





void ZMW_HORIZONTAL(zmw_alignement_,_make)(Zmw_Child *s, int border_width)
{
  int free_space ;

  s->allocated.X = ZMW_SIZE_ALLOCATED.X + border_width + s->current_state.padding_width ;
  s->allocated.WIDTH = ZMW_SIZE_ALLOCATED.WIDTH	- 2 * border_width - 2 * s->current_state.padding_width;

  if ( ! s->ZMW_HORIZONTAL(,_expand) )
    {
      free_space = ZMW_SIZE_ALLOCATED.WIDTH - 2 * border_width
	- s->required.WIDTH - 2 * s->current_state.padding_width ;
      s->allocated.WIDTH -= free_space ;
      switch(s->current_state.ZMW_HORIZONTAL(,_alignment) )
	    {
	    case -1:
	      break ;
	    case 0:
	      s->allocated.X += free_space / 2 ;
	      break ;
	    case 1:
	      s->allocated.X += free_space ;
	      break ;
	    }
    }
}

