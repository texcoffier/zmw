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


static void zmw_anchor_vertical_()
{
  zmw_focusable() ;

  switch( ZMW_SUBACTION )
    {
    case Zmw_Compute_Required_Size:
      ZMW_SIZE_MIN.width = 2*ZMW_BORDER_WIDTH ;
      ZMW_SIZE_MIN.height = 1 ;
      ZMW_SIZE_SENSIBLE = 1 ;
      zmw_vertical_expand(Zmw_True) ;
      break ;

    case Zmw_Post_Drawing:
      zmw_draw_rectangle(Zmw_Color_Border_Dark
			 , Zmw_True
			 , ZMW_SIZE_ALLOCATED.x
			 , ZMW_SIZE_ALLOCATED.y
			 , ZMW_BORDER_WIDTH
			 , ZMW_SIZE_ALLOCATED.height
			 ) ;
      zmw_draw_rectangle(Zmw_Color_Border_Light
			 , Zmw_True
			 , ZMW_SIZE_ALLOCATED.x + ZMW_BORDER_WIDTH
			 , ZMW_SIZE_ALLOCATED.y
			 , ZMW_BORDER_WIDTH
			 , ZMW_SIZE_ALLOCATED.height
			 ) ;
      if ( zmw_focused() )
	{
	  zmw_draw_rectangle(Zmw_Color_Border_Dark
			     , Zmw_True
			     , ZMW_SIZE_ALLOCATED.x - (2*ZMW_BORDER_WIDTH)/2
			     , ZMW_SIZE_ALLOCATED.y
			     + ZMW_SIZE_ALLOCATED.height/2
			     - (3*ZMW_BORDER_WIDTH)/2
			     , 3*ZMW_BORDER_WIDTH
			     , 3*ZMW_BORDER_WIDTH
			     ) ;
	  zmw_draw_rectangle(Zmw_Color_Border_Light
			     , Zmw_True
			     , ZMW_SIZE_ALLOCATED.x - ZMW_BORDER_WIDTH/2
			     , ZMW_SIZE_ALLOCATED.y
			     + ZMW_SIZE_ALLOCATED.height/2
			     - (2*ZMW_BORDER_WIDTH)/2
			     , 2*ZMW_BORDER_WIDTH
			     , 2*ZMW_BORDER_WIDTH
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
      if ( zmw.event->key.keyval == GDK_Right )
	{
	  (*x)++ ;
	  ZMW_SIZE_ACTIVATED = Zmw_True ;
	}
      if ( zmw.event->key.keyval == GDK_Left )
	{
	  (*x)-- ;
	  ZMW_SIZE_ACTIVATED = Zmw_True ;
	}
    }

  if ( zmw_selected() && ZMW_SUBACTION == Zmw_Input_Event )
    {
      ZMW_SIZE_CHANGED = Zmw_True ;
      *x = zmw.x - ZMW_PARENT_SIZE.allocated.x - 2*ZMW_SIZE_ALLOCATED.width ;
    }

  // I do not know why it is necessary here, it bugs with cache
  //  if ( ZMW_SIZE_ACTIVATED || zmw.changed )
  //  zmw_event_remove() ;
}


void zmw_anchor_vertical_float(float *x)
{
  ZMW( zmw_anchor_vertical_() ) { }

  if ( zmw_key_pressed() )
    {
      if ( zmw.event->key.keyval == GDK_Right )
	{
	  *x += .01 ;
	  ZMW_SIZE_ACTIVATED = Zmw_True ;
	}
      if ( zmw.event->key.keyval == GDK_Left )
	{
	  *x -= .01 ;
	  ZMW_SIZE_ACTIVATED = Zmw_True ;
	}
    }

  if ( zmw_selected() && ZMW_SUBACTION == Zmw_Input_Event )
    {
      ZMW_SIZE_ACTIVATED = Zmw_True ;
      *x = (zmw.x - ZMW_PARENT_SIZE.allocated.x)
	/ (float)ZMW_PARENT_SIZE.allocated.width ;
    }
}


/*
 *
 */

void zmw_coordinates(int corner, int *x, int *y)
{
  *x = ZMW_SIZE_ALLOCATED.x
    + ZMW_BORDER_WIDTH
    + ((corner%3)*(ZMW_SIZE_ALLOCATED.width-2*ZMW_BORDER_WIDTH))/2
    ;
  *y = ZMW_SIZE_ALLOCATED.y
    + ZMW_BORDER_WIDTH
    + ((corner/3)*(ZMW_SIZE_ALLOCATED.height-2*ZMW_BORDER_WIDTH))/2
    ;
}

void zmw_anchor_box_(int *x, int *y, int *width, int *height
		     , int *ix, int *iy, int *iw, int *ih)
{
  int i, xx, yy, nx, ny ;
  static int corner = -1 ;

  zmw_focusable() ;

  switch( ZMW_SUBACTION )
    {
    case Zmw_Compute_Required_Size:
      ZMW_SIZE_MIN.width = *width ;
      ZMW_SIZE_MIN.height = *height ;
      ZMW_SIZE_MIN.x = *x ;
      ZMW_SIZE_MIN.y = *y ;
      ZMW_SIZE_SENSIBLE = 1 ;
      break ;

    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:
    case Zmw_Compute_Children_Allocated_Size:
      for(i=0; i<ZMW_NB_OF_CHILDREN; i++)
	{
	  if ( !ZMW_CHILDREN[i].used_to_compute_parent_size )
	    continue ;

	  ZMW_CHILDREN[i].allocated = ZMW_SIZE_ALLOCATED ;
	  zmw_padding_remove(&ZMW_CHILDREN[i].allocated
			     , ZMW_CHILDREN[i].current_state.padding_width ) ;
	}
      break ;
    case Zmw_Post_Drawing:

      if ( zmw_focused() )
	{
	  zmw_draw_rectangle(Zmw_Color_Border_Dark
			     , Zmw_False
			     , ZMW_SIZE_ALLOCATED.x
			     , ZMW_SIZE_ALLOCATED.y
			     , ZMW_SIZE_ALLOCATED.width
			     , ZMW_SIZE_ALLOCATED.height
			     ) ;
	  for(i=0; i<9; i++)
	    {
	      zmw_coordinates(i, &xx, &yy) ;
	      zmw_draw_rectangle(Zmw_Color_Border_Dark
				 , Zmw_False
				 , xx-ZMW_BORDER_WIDTH
				 , yy-ZMW_BORDER_WIDTH
				 , ZMW_BORDER_WIDTH*2
				 , ZMW_BORDER_WIDTH*2
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
	      if ( (zmw.x-xx)*(zmw.x-xx) + (zmw.y-yy)*(zmw.y-yy) <= 72 )
		{
		  corner = i ;
		  break ;
		}
	    }	  
	}
      else if ( corner != -1 && zmw_button_released_anywhere() && zmw_focused())
	{
	  corner = -1 ;
	  ZMW_SIZE_ACTIVATED = Zmw_True ;
	}
      else if ( zmw_focused() && corner >= 0 )
	{
	  nx = zmw.x - ZMW_PARENT_SIZE.allocated.x ;
	  ny = zmw.y - ZMW_PARENT_SIZE.allocated.y ;
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

  switch( ZMW_SUBACTION )
    {
    case Zmw_Compute_Required_Size:
      if ( ZMW_NB_OF_CHILDREN )
	ZMW_SIZE_MIN = ZMW_CHILDREN[0].required ;
      ZMW_SIZE_MIN.x = *x ;
      ZMW_SIZE_MIN.y = *y ;
      ZMW_SIZE_SENSIBLE = 1 ;
      break ;

    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:
    case Zmw_Compute_Children_Allocated_Size:
      for(i=0; i<ZMW_NB_OF_CHILDREN; i++)
	{
	  if ( !ZMW_CHILDREN[i].used_to_compute_parent_size )
	    continue ;

	  ZMW_CHILDREN[i].allocated = ZMW_SIZE_ALLOCATED ;
	}
      break ;
    case Zmw_Post_Drawing:

      if ( zmw_focused() )
	{
	  zmw_draw_rectangle(Zmw_Color_Border_Dark
			     , Zmw_False
			     , ZMW_SIZE_ALLOCATED.x
			     , ZMW_SIZE_ALLOCATED.y
			     , ZMW_SIZE_ALLOCATED.width
			     , ZMW_SIZE_ALLOCATED.height
			     ) ;

	  zmw_coordinates(4, &xx, &yy) ;
	  zmw_draw_rectangle(Zmw_Color_Border_Dark
			     , Zmw_False
			     , xx-ZMW_BORDER_WIDTH
			     , yy-ZMW_BORDER_WIDTH
			     , ZMW_BORDER_WIDTH*2
			     , ZMW_BORDER_WIDTH*2
			     ) ;
	  zmw_draw_rectangle(Zmw_Color_Border_Light
			     , Zmw_False
			     , xx-ZMW_BORDER_WIDTH+1
			     , yy-ZMW_BORDER_WIDTH+1
			     , ZMW_BORDER_WIDTH*2-2
			     , ZMW_BORDER_WIDTH*2-2
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
	  if ( (zmw.x-xx)*(zmw.x-xx) + (zmw.y-yy)*(zmw.y-yy) <= 512 )
	    {
	      corner = 4 ;
	    }
	}
      else if ( corner != -1 && zmw_button_released_anywhere() && zmw_focused() )
	{
	  	  corner = -1 ;
	  	  ZMW_SIZE_ACTIVATED = Zmw_True ;
	}
      else if ( zmw_focused() && corner >= 0 )
	{
	  nx = zmw.x - ZMW_PARENT_SIZE.allocated.x ;
	  ny = zmw.y - ZMW_PARENT_SIZE.allocated.y ;
	  *x = nx - ZMW_SIZE_ALLOCATED.width/2 ;
	  *y = ny - ZMW_SIZE_ALLOCATED.height/2 ;
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
