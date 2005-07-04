/*
  ZMW: A Zero Memory Widget Library
  Copyright (C) 2003-2005 Thierry EXCOFFIER, Université Claude Bernard, LIRIS

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
#include <stdarg.h>

static int zmw_decorator_border(int options)
{
  int border_width ;

  border_width = 0 ;
  if ( options & Zmw_Decorator_Focus_Any )
    border_width += zmw_focus_width_get() ;
  if ( options & Zmw_Decorator_Border_Any )
    border_width += zmw_border_width_get() ;

  return( border_width ) ;
}

void zmw_decorator(int options, ...)
{
  int border, border_width ;
  int tx, ty ;
  va_list ap;
  Zmw_Rectangle r ;

  tx = ty = 0 ; /* to remove a compiler warning */

  va_start(ap, options);
  if ( options & Zmw_Decorator_Translate )
    {
      tx = va_arg(ap, int) ;
      ty = va_arg(ap, int) ;
    }
  va_end(ap);


  border_width = zmw_decorator_border(options) ;

  if ( options & Zmw_Decorator_Focusable )
    zmw_focusable() ;

  if ( options & Zmw_Decorator_Clip )
    {
      r.x = zmw_allocated_x_get() + border_width ;
      r.y = zmw_allocated_y_get() + border_width ;
      r.width = zmw_allocated_width_get() - 2*border_width ;
      r.height = zmw_allocated_height_get() - 2*border_width ;
      *zmw_clipping_get() = zmw_rectangle_min(zmw_clipping_get(), &r) ;
    }

  switch( zmw_subaction_get() )
    {
    case Zmw_Compute_Required_Size:
      if ( zmw_nb_of_children_get() )
	{
	  zmw_min_set(zmw_child__required_get(0)) ;
	  *zmw_min_width_get_ptr() += 2 * border_width ;
	  *zmw_min_height_get_ptr() += 2 * border_width ;
	  zmw_padding_add(zmw_min_get(), zmw_child__padding_width_get(0)) ;
	}
      else
	{
	  zmw_min_width_set(2 * border_width + 6) ;
	  zmw_min_height_set(2 * border_width + 6) ;
	}

      break ;

    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:
      border = 0 ;
      if ( ((options & Zmw_Decorator_Focusable) && zmw_focused())
	   || (options & Zmw_Decorator_Border_Focus)
	   )
	border |= Zmw_Border_Draw_Focus ;

      if ( options & Zmw_Decorator_Focusable )
	border |= Zmw_Border_Focusable ;

      if ( options & Zmw_Decorator_Pushable )
	{
	  if ( zmw_selected() )
	    border |= Zmw_Border_In ;
	  else
	    border |= Zmw_Border_Out ;
	}
      if ( options & Zmw_Decorator_Border_Relief )
	border |= Zmw_Border_Relief ;
      if ( options & Zmw_Decorator_Border_Solid )
	border |= Zmw_Border_Solid ;
      if ( options & Zmw_Decorator_Border_Embossed )
	border |= Zmw_Border_Embossed ;
      if ( options & Zmw_Decorator_Border_In )
	border |= Zmw_Border_In ;
      if ( options & Zmw_Decorator_Border_Out )
	border |= Zmw_Border_Out ;
      if ( options & Zmw_Decorator_Interior )
	border |= Zmw_Border_Background ;
      if ( (options & Zmw_Decorator_Feedback)
	   && zmw_sensitived_get()
	   && zmw_event_in_rectangle_get() )
	border |= Zmw_Border_In ;
      zmw_border_draw(border) ;


      if ( options & Zmw_Decorator_Clip )
	zmw_draw_clip_set() ;

      /* fall thru */

    case Zmw_Compute_Children_Allocated_Size:
      if ( zmw_nb_of_children_get() )
	{
	  /* This case is for zmw_viewport, because in this
	   * case the decorator must not put its child where it want
	   */
	  if ( options & Zmw_Decorator_Translate )
	    {
	      zmw_child__allocated_x_set(0,zmw_allocated_x_get() + tx + zmw_child__padding_width_get(0)) ;
	      zmw_child__allocated_y_set(0,zmw_allocated_y_get() + ty + zmw_child__padding_width_get(0)) ;
	      zmw_child__allocated_width_set(0,zmw_child__required_width_get(0));
	      zmw_child__allocated_height_set(0,zmw_child__required_height_get(0));
	    }
	  else
	    {
	      zmw_alignment_vertical_make(0, border_width) ;
	      zmw_alignment_horizontal_make(0, border_width) ;
	    }
	}
      break ;

    case Zmw_Post_Drawing:
      if ( options & Zmw_Decorator_Clip )
	{
	  /* Not clean to restore the clipping rectangle
	   * before the next widget */
	  zmw_clipping_set(zmw_parent__clipping_get()) ;
	  zmw_draw_clip_set() ;
	}
      break ;

    case Zmw_Input_Event:
      if ( options & Zmw_Decorator_Activable )
	{
	  zmw_activable() ;
	}
      if ( options & Zmw_Decorator_Activable_By_Key )
	if ( zmw_key_string_unsensitive()
	     && zmw_zmw_event_get()->key.string[0] != '\033' )
	  {
	    zmw_activated_set(Zmw_True) ;
	  }

      if ( zmw_activated_get() )
	{
	  zmw_window_unpop_all() ;
	}
      break ;

    default:
      break ;
    }
}
