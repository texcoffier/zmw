/*
    ZMW: A Zero Memory Widget Library
    Copyright (C) 2002-2003  Thierry EXCOFFIER, LIRIS

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
#include "zmw.h"


static void zmw_anchor_vertical_()
{
  switch( ZMW_SUBACTION )
    {
    case Zmw_Compute_Required_Size:
      ZMW_SIZE_MIN.width = 2*ZMW_BORDER_WIDTH ;
      ZMW_SIZE_MIN.height = 1 ;
      ZMW_SIZE_SENSIBLE = 1 ;
      break ;

    case Zmw_Post_Drawing:
      gdk_draw_rectangle(ZMW_WINDOW
			 , ZMW_GC[ZMW_BORDER_DARK]
			 , 1
			 , ZMW_SIZE_ALLOCATED.x
			 , zMw[-1].u.size.allocated.y
			 , ZMW_BORDER_WIDTH
			 , zMw[-1].u.size.allocated.height
			 ) ;
      gdk_draw_rectangle(ZMW_WINDOW
			 , ZMW_GC[ZMW_BORDER_LIGHT]
			 , 1
			 , ZMW_SIZE_ALLOCATED.x + ZMW_BORDER_WIDTH
			 , zMw[-1].u.size.allocated.y
			 , ZMW_BORDER_WIDTH
			 , zMw[-1].u.size.allocated.height
			 ) ;
      if ( zmw_focused() )
	{
	  gdk_draw_rectangle(ZMW_WINDOW
			     , ZMW_GC[ZMW_BORDER_DARK]
			     , 1
			     , ZMW_SIZE_ALLOCATED.x - (2*ZMW_BORDER_WIDTH)/2
			     , zMw[-1].u.size.allocated.y
			     + zMw[-1].u.size.allocated.height/2
			     - (3*ZMW_BORDER_WIDTH)/2
			     , 3*ZMW_BORDER_WIDTH
			     , 3*ZMW_BORDER_WIDTH
			     ) ;
	  gdk_draw_rectangle(ZMW_WINDOW
			     , ZMW_GC[ZMW_BORDER_LIGHT]
			     , 1
			     , ZMW_SIZE_ALLOCATED.x - ZMW_BORDER_WIDTH/2
			     , zMw[-1].u.size.allocated.y
			     + zMw[-1].u.size.allocated.height/2
			     - (2*ZMW_BORDER_WIDTH)/2
			     , 2*ZMW_BORDER_WIDTH
			     , 2*ZMW_BORDER_WIDTH
			     ) ;
	}
      break ;
    case Zmw_Input_Event:
      zmw_focusable() ;
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
	  zmw.dragged = Zmw_True ;
	  zmw.activated = Zmw_True ;
	  zmw_event_remove() ;
	}
      if ( zmw.event->key.keyval == GDK_Left )
	{
	  (*x)-- ;
	  zmw.dragged = Zmw_True ;
	  zmw.activated = Zmw_True ;
	  zmw_event_remove() ;
	}
    }

  if ( zmw_selected() && ZMW_ACTION == zmw_action_dispatch_event )
    {
      zmw.dragged = Zmw_True ;
      zmw.activated = Zmw_True ;
      *x = zmw.x - zMw[-1].u.size.allocated.x - 2*ZMW_SIZE_ALLOCATED.width ;
    }

}


void zmw_anchor_vertical_float(float *x)
{
  ZMW( zmw_anchor_vertical_() ) { }

  if ( zmw_key_pressed() )
    {
      if ( zmw.event->key.keyval == GDK_Right )
	{
	  *x += .01 ;
	  zmw.dragged = Zmw_True ;
	  zmw.activated = Zmw_True ;
	  zmw_event_remove() ;
	}
      if ( zmw.event->key.keyval == GDK_Left )
	{
	  *x -= .01 ;
	  zmw.dragged = Zmw_True ;
	  zmw.activated = Zmw_True ;
	  zmw_event_remove() ;
	}
    }

  if ( zmw_selected() && ZMW_ACTION == zmw_action_dispatch_event )
    {
      zmw.dragged = Zmw_True ;
      zmw.activated = Zmw_True ;
      *x = (zmw.x - zMw[-1].u.size.allocated.x)
	/ (float)zMw[-1].u.size.allocated.width ;
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
	}
      break ;
    case Zmw_Post_Drawing:

      if ( zmw_focused() )
	{
	  gdk_draw_rectangle(ZMW_WINDOW
			     , ZMW_GC[ZMW_BORDER_DARK]
			     , 0
			     , ZMW_SIZE_ALLOCATED.x
			     , ZMW_SIZE_ALLOCATED.y
			     , ZMW_SIZE_ALLOCATED.width
			     , ZMW_SIZE_ALLOCATED.height
			     ) ;
	  for(i=0; i<9; i++)
	    {
	      zmw_coordinates(i, &xx, &yy) ;
	      gdk_draw_rectangle(ZMW_WINDOW
				 , ZMW_GC[ZMW_BORDER_DARK]
				 , 0
				 , xx-ZMW_BORDER_WIDTH
				 , yy-ZMW_BORDER_WIDTH
				 , ZMW_BORDER_WIDTH*2
				 , ZMW_BORDER_WIDTH*2
				 ) ;
	    }
	}
      break ;
    case Zmw_Input_Event:

      zmw_focusable() ;

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
	  zmw_event_remove() ;
	}
      else if ( corner != -1 && zmw_button_released_anywhere() )
	{
	  corner = -1 ;
	  zmw.activated = Zmw_True ;
	  zmw_event_remove() ;
	}
      else if ( zmw_focused() && corner >= 0 )
	{
	  nx = zmw.x - zMw[-1].u.size.allocated.x ;
	  ny = zmw.y - zMw[-1].u.size.allocated.y ;
	  zmw.dragged = Zmw_True ;
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



void zmw_anchor_move_(int *x, int *y, int *ix, int *iy)
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
	  gdk_draw_rectangle(ZMW_WINDOW
			     , ZMW_GC[ZMW_BORDER_DARK]
			     , 0
			     , ZMW_SIZE_ALLOCATED.x
			     , ZMW_SIZE_ALLOCATED.y
			     , ZMW_SIZE_ALLOCATED.width
			     , ZMW_SIZE_ALLOCATED.height
			     ) ;

	  zmw_coordinates(4, &xx, &yy) ;
	  gdk_draw_rectangle(ZMW_WINDOW
			     , ZMW_GC[ZMW_BORDER_DARK]
			     , 0
			     , xx-ZMW_BORDER_WIDTH
			     , yy-ZMW_BORDER_WIDTH
			     , ZMW_BORDER_WIDTH*2
			     , ZMW_BORDER_WIDTH*2
			     ) ;
	}
      break ;
    case Zmw_Input_Event:

      zmw_focusable() ;

      if ( zmw_button_pressed() )
	{
	  *ix = *x ;
	  *iy = *y ;
	  zmw_coordinates(4, &xx, &yy) ;
	  if ( (zmw.x-xx)*(zmw.x-xx) + (zmw.y-yy)*(zmw.y-yy) <= 512 )
	    {
	      corner = 4 ;
	      zmw_event_remove() ;
	    }
	}
      else if ( corner != -1 && zmw_button_released_anywhere() )
	{
	  corner = -1 ;
	  zmw.activated = Zmw_True ;
	  zmw_event_remove() ;
	}
      else if ( zmw_focused() && corner >= 0 )
	{
	  nx = zmw.x - zMw[-1].u.size.allocated.x ;
	  ny = zmw.y - zMw[-1].u.size.allocated.y ;
	  zmw.dragged = Zmw_True ;
	  *x = nx - ZMW_SIZE_ALLOCATED.width/2 ;
	  *y = ny - ZMW_SIZE_ALLOCATED.height/2 ;
	}
      break ;
    default:
      break ;
    }
}

void zmw_anchor_move(int *x, int *y)
{
  static int ix, iy ;
  zmw_anchor_move_(x, y, &ix, &iy) ;
}

/*
 * When activated (button release) initial values are returned
 */
void zmw_anchor_move_with_init_values(int *x, int *y, int *ix, int *iy)
{
  zmw_anchor_move_(x, y, ix, iy) ;
}
