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

#include <gdk/gdkkeysyms.h>
#include "zmw/zmw.h"
#include "zmw/zmw_private.h" /* This include is only here for speed up */


static void zmw_anchor_vertical_()
{
  zmw_focusable() ;

  switch( zmw_subaction_get() )
    {
    case Zmw_Compute_Required_Size:
      zmw_min_width_set(2*zmw_border_width_get()) ;
      zmw_min_height_set(1) ;
      zmw_sensitived_set(1) ;
      zmw_vertical_expand(Zmw_True) ;
      break ;

    case Zmw_Post_Drawing:
      zmw_draw_rectangle(Zmw_Color_Border_Dark
			 , Zmw_True
			 , zmw_allocated_x_get()
			 , zmw_allocated_y_get()
			 , zmw_border_width_get()
			 , zmw_allocated_height_get()
			 ) ;
      zmw_draw_rectangle(Zmw_Color_Border_Light
			 , Zmw_True
			 , zmw_allocated_x_get() + zmw_border_width_get()
			 , zmw_allocated_y_get()
			 , zmw_border_width_get()
			 , zmw_allocated_height_get()
			 ) ;
      if ( zmw_focused() )
	{
	  zmw_draw_rectangle(Zmw_Color_Border_Dark
			     , Zmw_True
			     , zmw_allocated_x_get() - (2*zmw_border_width_get())/2
			     , zmw_allocated_y_get()
			     + zmw_allocated_height_get()/2
			     - (3*zmw_border_width_get())/2
			     , 3*zmw_border_width_get()
			     , 3*zmw_border_width_get()
			     ) ;
	  zmw_draw_rectangle(Zmw_Color_Border_Light
			     , Zmw_True
			     , zmw_allocated_x_get() - zmw_border_width_get()/2
			     , zmw_allocated_y_get()
			     + zmw_allocated_height_get()/2
			     - (2*zmw_border_width_get())/2
			     , 2*zmw_border_width_get()
			     , 2*zmw_border_width_get()
			     ) ;
	}
      break ;
    case Zmw_Input_Event:
      zmw_activable() ;
      break ;
    default:
      break ;
    }
}

void zmw_anchor_vertical(int *x)
{
  ZMW( zmw_anchor_vertical_() ) { }

  if ( zmw_key_pressed() )
    {
      if ( zmw_zmw_event_get()->key.keyval == GDK_Right )
	{
	  (*x)++ ;
	  zmw_activated_set(Zmw_True) ;
	}
      if ( zmw_zmw_event_get()->key.keyval == GDK_Left )
	{
	  (*x)-- ;
	  zmw_activated_set(Zmw_True) ;
	}
    }

  if ( zmw_selected() && zmw_subaction_get() == Zmw_Input_Event )
    {
      zmw_changed_set(Zmw_True) ;
      *x = zmw_zmw_x_get() - zmw_parent__allocated_x_get() - 2*zmw_allocated_width_get() ;
    }

  // I do not know why it is necessary here, it bugs with cache
  //  if ( zmw_activated_get() || zmw.changed )
  //  zmw_event_remove() ;
}


void zmw_anchor_vertical_float(float *x)
{
  ZMW( zmw_anchor_vertical_() ) { }

  if ( zmw_key_pressed() )
    {
      if ( zmw_zmw_event_get()->key.keyval == GDK_Right )
	{
	  *x += .01 ;
	  zmw_activated_set(Zmw_True) ;
	}
      if ( zmw_zmw_event_get()->key.keyval == GDK_Left )
	{
	  *x -= .01 ;
	  zmw_activated_set(Zmw_True) ;
	}
    }

  if ( zmw_selected() && zmw_subaction_get() == Zmw_Input_Event )
    {
      zmw_activated_set(Zmw_True) ;
      *x = (zmw_zmw_x_get() - zmw_parent__allocated_x_get())
	/ (float)zmw_parent__allocated_width_get() ;
    }
}


/*
 *
 */

void zmw_coordinates(int corner, int *x, int *y)
{
  *x = zmw_allocated_x_get()
    + zmw_border_width_get()
    + ((corner%3)*(zmw_allocated_width_get()-2*zmw_border_width_get()))/2
    ;
  *y = zmw_allocated_y_get()
    + zmw_border_width_get()
    + ((corner/3)*(zmw_allocated_height_get()-2*zmw_border_width_get()))/2
    ;
}

void zmw_anchor_box_(int *x, int *y, int *width, int *height
		     , int *ix, int *iy, int *iw, int *ih)
{
  int i, xx, yy, nx, ny ;
  static int corner = -1 ;

  zmw_focusable() ;

  switch( zmw_subaction_get() )
    {
    case Zmw_Compute_Required_Size:
      zmw_min_width_set(*width) ;
      zmw_min_height_set(*height) ;
      zmw_min_x_set(*x) ;
      zmw_min_y_set(*y) ;
      zmw_sensitived_set(1) ;
      break ;

    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:
    case Zmw_Compute_Children_Allocated_Size:
      for(i=0; i<zmw_nb_of_children_get(); i++)
	{
	  if ( !zmw_child__used_by_parent_get(i) )
	    continue ;

	  zmw_child__allocated_set(i,zmw_allocated_get()) ;
	  zmw_padding_remove(zmw_child__allocated_get(i)
			     , zmw_child__padding_width_get(i) ) ;
	}
      break ;
    case Zmw_Post_Drawing:

      if ( zmw_focused() )
	{
	  zmw_draw_rectangle(Zmw_Color_Border_Dark
			     , Zmw_False
			     , zmw_allocated_x_get()
			     , zmw_allocated_y_get()
			     , zmw_allocated_width_get()
			     , zmw_allocated_height_get()
			     ) ;
	  for(i=0; i<9; i++)
	    {
	      zmw_coordinates(i, &xx, &yy) ;
	      zmw_draw_rectangle(Zmw_Color_Border_Dark
				 , Zmw_False
				 , xx-zmw_border_width_get()
				 , yy-zmw_border_width_get()
				 , zmw_border_width_get()*2
				 , zmw_border_width_get()*2
				 ) ;
	    }
	}
      break ;
    case Zmw_Input_Event:

      zmw_activable() ;
      
      if ( zmw_button_pressed() )
	{
	  *ix = *x ;
	  *iy = *y ;
	  *iw = *width ;
	  *ih = *height ;
	  for(i=0; i<9; i++)
	    {
	      zmw_coordinates(i, &xx, &yy) ;
	      if ( (zmw_zmw_x_get()-xx)*(zmw_zmw_x_get()-xx) + (zmw_zmw_y_get()-yy)*(zmw_zmw_y_get()-yy) <= 72 )
		{
		  corner = i ;
		  break ;
		}
	    }	  
	}
      else if ( corner != -1 && zmw_button_released_anywhere() && zmw_focused())
	{
	  corner = -1 ;
	  zmw_activated_set(Zmw_True) ;
	}
      else if ( zmw_focused() && corner >= 0 )
	{
	  nx = zmw_zmw_x_get() - zmw_parent__allocated_x_get() ;
	  ny = zmw_zmw_y_get() - zmw_parent__allocated_y_get() ;
	  switch(corner)
	    {
	    case 0:
	      *width = *x + *width - nx ;
	      *height = *y + *height - ny ;
	      *x = nx ;
	      *y = ny ;
	      break ;
	    case 1:
	      *height = *y + *height - ny ;
	      *y = ny ;
	      break ;
	    case 2:
	      *width = nx - *x ;
	      *height = *y + *height - ny ;
	      *y = ny ;
	      break ;
	    case 3:
	      *width = *x + *width - nx ;
	      *x = nx ;
	      break ;
	    case 4:
	      *x = nx - *width/2 ;
	      *y = ny - *height/2 ;
	      break ;
	    case 5:
	      *width = nx - *x ;
	      break ;
	    case 6:
	      *width = *x + *width - nx ;
	      *height = ny - *y ;
	      *x = nx ;
	      break ;
	    case 7:
	      *height = ny - *y ;
	      break ;
	    case 8:
	      *width = nx - *x ;
	      *height = ny - *y ;
	      break ;
	    }
	  zmw_event_remove() ;
	}
      break ;
    default:
      break ;
    }
}

void zmw_anchor_box(int *x, int *y, int *width, int *height)
{
  static int ix, iy, iw, ih ;
  zmw_anchor_box_(x, y, width, height, &ix, &iy, &iw, &ih) ;
}

/*
 * When activated (button release) initial values are returned
 */
void zmw_anchor_box_with_init_values(int *x, int *y, int *width, int *height
				     , int *ix, int *iy, int *iw, int *ih)
{
  zmw_anchor_box_(x, y, width, height, ix, iy, iw, ih) ;
}

/*
 * (ix, iy) is the position before the drag start
 */

void zmw_anchor_move_(int *x, int *y, int *ix, int *iy, Zmw_Boolean movable)
{
  int i, xx, yy, nx, ny ;
  static int corner = -1 ;

  switch( zmw_subaction_get() )
    {
    case Zmw_Compute_Required_Size:
      if ( zmw_nb_of_children_get() )
	zmw_min_set(zmw_child__required_get(0)) ;
      zmw_min_x_set(*x) ;
      zmw_min_y_set(*y) ;
      zmw_sensitived_set(1) ;
      break ;

    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:
    case Zmw_Compute_Children_Allocated_Size:
      for(i=0; i<zmw_nb_of_children_get(); i++)
	{
	  if ( !zmw_child__used_by_parent_get(i) )
	    continue ;

	  zmw_child__allocated_set(i,zmw_allocated_get()) ;
	}
      break ;
    case Zmw_Post_Drawing:

      if ( zmw_focused() )
	{
	  zmw_draw_rectangle(Zmw_Color_Border_Dark
			     , Zmw_False
			     , zmw_allocated_x_get()
			     , zmw_allocated_y_get()
			     , zmw_allocated_width_get()
			     , zmw_allocated_height_get()
			     ) ;

	  zmw_coordinates(4, &xx, &yy) ;
	  zmw_draw_rectangle(Zmw_Color_Border_Dark
			     , Zmw_False
			     , xx-zmw_border_width_get()
			     , yy-zmw_border_width_get()
			     , zmw_border_width_get()*2
			     , zmw_border_width_get()*2
			     ) ;
	  zmw_draw_rectangle(Zmw_Color_Border_Light
			     , Zmw_False
			     , xx-zmw_border_width_get()+1
			     , yy-zmw_border_width_get()+1
			     , zmw_border_width_get()*2-2
			     , zmw_border_width_get()*2-2
			     ) ;
	}
      break ;
    case Zmw_Input_Event:
      if ( !movable )
 	 break ;
      zmw_focusable() ;
      zmw_activable() ;

      if ( zmw_button_pressed() )
	{
	  *ix = *x ;
	  *iy = *y ;
	  zmw_coordinates(4, &xx, &yy) ;
	  if ( (zmw_zmw_x_get()-xx)*(zmw_zmw_x_get()-xx) + (zmw_zmw_y_get()-yy)*(zmw_zmw_y_get()-yy) <= 512 )
	    {
	      corner = 4 ;
	    }
	}
      else if ( corner != -1 && zmw_button_released_anywhere() && zmw_focused() )
	{
	  	  corner = -1 ;
	  	  zmw_activated_set(Zmw_True) ;
	}
      else if ( zmw_focused() && corner >= 0 )
	{
	  nx = zmw_zmw_x_get() - zmw_parent__allocated_x_get() ;
	  ny = zmw_zmw_y_get() - zmw_parent__allocated_y_get() ;
	  *x = nx - zmw_allocated_width_get()/2 ;
	  *y = ny - zmw_allocated_height_get()/2 ;
	  zmw_event_remove() ;
	}
      break ;
    default:
      break ;
    }
}

void zmw_anchor_move(int *x, int *y)
{
  int ix, iy ;
  zmw_anchor_move_(x, y, &ix, &iy, Zmw_True) ;
}

void zmw_anchor_move_not(int x, int y)
{
  int ix, iy ;
  zmw_anchor_move_(&x, &y, &ix, &iy, Zmw_False) ;
}

/*
 * When activated (button release) initial values are returned
 */
void zmw_anchor_move_with_init_values(int *x, int *y, int *ix, int *iy)
{
  zmw_anchor_move_(x, y, ix, iy, Zmw_True) ;
}
