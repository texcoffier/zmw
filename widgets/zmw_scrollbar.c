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

#include <math.h>
#include "zmw/zmw.h"
#include <gdk/gdkkeysyms.h>

static void zmw_scrollbar2_(Zmw_Float_0_1 *x, Zmw_Float_0_1 x_size, Zmw_Float_0_1 x_delta, Zmw_Float_0_1 *y, Zmw_Float_0_1 y_size, Zmw_Float_0_1 y_delta)
{
  Zmw_Rectangle r ;
  int border ;
  
  border = ZMW_BORDER_WIDTH + ZMW_FOCUS_WIDTH ;
  switch( ZMW_SUBACTION )
    {
    case Zmw_Compute_Required_Size:
      ZMW_SIZE_MIN.width = 16  ;
      ZMW_SIZE_MIN.height = 16 ;
     break ;

    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:
      zmw_border_draw(Zmw_Border_Relief_In | Zmw_Border_Background
		      | Zmw_Border_Focusable
		      | (zmw_focused() ? Zmw_Border_Draw_Focus:0)
		      ) ;

      r.x = ZMW_SIZE_ALLOCATED.x + border +
	(int)rint((ZMW_SIZE_ALLOCATED.width - 2*border) * *x + 0.5) ;
      r.y = ZMW_SIZE_ALLOCATED.y + border +
	(int)rint((ZMW_SIZE_ALLOCATED.height - 2*border) * *y + 0.5) ;

      r.width  = x_size * (ZMW_SIZE_ALLOCATED.width - 2*border) ;
      r.height = y_size * (ZMW_SIZE_ALLOCATED.height - 2*border) ;   
      if ( r.width < 2*ZMW_BORDER_WIDTH )
	{
	  if ( *x > 0.5 )
	    r.x -= 2*ZMW_BORDER_WIDTH - r.width ;
	  r.width = 2*ZMW_BORDER_WIDTH ;
	}
      if ( r.height < 2*ZMW_BORDER_WIDTH )
	{
	  if ( *y > 0.5 )
	    r.y -= 2*ZMW_BORDER_WIDTH - r.height ;
	  r.height = 2*ZMW_BORDER_WIDTH ;
	}


      zmw_border_draw_with_rectangle(Zmw_Border_Relief |Zmw_Border_Background
				     , &r);
      break ;
      
    case Zmw_Input_Event:
      
      zmw_focusable() ;
      zmw_activable() ;

      if ( zmw_selected() )
	{
	  zmw.dragged = Zmw_True ;
	  zmw.changed = Zmw_True ;
	  *x = (zmw.x - ZMW_SIZE_ALLOCATED.x - border)
	    / (float)(ZMW_SIZE_ALLOCATED.width - 2*border) - x_size/2 ;
	  *y = (zmw.y - ZMW_SIZE_ALLOCATED.y -border)
	    / (float)(ZMW_SIZE_ALLOCATED.height - 2*border) - y_size/2 ;
	  zmw_event_remove() ;
	}

      if ( zmw_key_pressed() )
	{	    
	  if ( zmw.event->key.keyval == GDK_Right )
	    {
	      *x += x_delta*x_size ;
	      zmw.dragged = Zmw_True ;
	      zmw.activated = Zmw_True ;
	      zmw_event_remove() ;
	    }
	  if ( zmw.event->key.keyval == GDK_Left )
	    {
	      *x -= x_delta*x_size ;
	      zmw.dragged = Zmw_True ;
	      zmw.activated = Zmw_True ;
	      zmw_event_remove() ;
	    }
	  if ( zmw.event->key.keyval == GDK_Up )
	    {
	      *y -= y_delta*y_size ;
	      zmw.dragged = Zmw_True ;
	      zmw.activated = Zmw_True ;
	      zmw_event_remove() ;
	    }
	  if ( zmw.event->key.keyval == GDK_Page_Up )
	    {
	      *y -= y_size ;
	      zmw.dragged = Zmw_True ;
	      zmw.activated = Zmw_True ;
	      zmw_event_remove() ;
	    }
	  if ( zmw.event->key.keyval == GDK_Down )
	    {
	      *y += y_delta*y_size ;
	      zmw.dragged = Zmw_True ;
	      zmw.activated = Zmw_True ;
	      zmw_event_remove() ;
	    }
	  if ( zmw.event->key.keyval == GDK_Page_Down )
	    {
	      *y += y_size ;
	      zmw.dragged = Zmw_True ;
	      zmw.activated = Zmw_True ;
	      zmw_event_remove() ;
	    }
	}
      ZMW_CLAMP(*x,0,1-x_size) ;
      ZMW_CLAMP(*y,0,1-y_size) ;
      break ;

    default:
      break ;
    }
}

void zmw_scrollbar2_with_delta(Zmw_Float_0_1 *x, Zmw_Float_0_1 x_size, Zmw_Float_0_1 x_delta, Zmw_Float_0_1 *y, Zmw_Float_0_1 y_size, Zmw_Float_0_1 y_delta)
{
  ZMW(zmw_scrollbar2_(x, x_size, x_delta, y, y_size, y_delta))
    {
    }
  zmw_widget_is_tested() ;	
}

void zmw_scrollbar2(Zmw_Float_0_1 *x, Zmw_Float_0_1 x_size, Zmw_Float_0_1 *y, Zmw_Float_0_1 y_size)
{
  zmw_scrollbar2_with_delta(x, x_size, 0.1, y, y_size, 0.1) ;
}

void zmw_scrollbar_horizontal(Zmw_Float_0_1 *x, Zmw_Float_0_1 x_size)
{
  Zmw_Float_0_1 y = 0 ;	
  zmw_scrollbar2(x, x_size, &y, 1) ;
}

void zmw_scrollbar_vertical(Zmw_Float_0_1 *y, Zmw_Float_0_1 y_size)
{
  Zmw_Float_0_1 x = 0 ;	
  zmw_scrollbar2(&x, 1, y, y_size) ;
}

void zmw_scrollbar_horizontal_with_delta(Zmw_Float_0_1 *x, Zmw_Float_0_1 x_size, Zmw_Float_0_1 x_delta)
{
  Zmw_Float_0_1 y = 0 ;	
  zmw_scrollbar2_with_delta(x, x_size, x_delta, &y, 1, 1) ;
}

void zmw_scrollbar_vertical_with_delta(Zmw_Float_0_1 *y, Zmw_Float_0_1 y_size, Zmw_Float_0_1 y_delta)
{
  Zmw_Float_0_1 x = 0 ;	
  zmw_scrollbar2_with_delta(&x, 1, 1, y, y_size, y_delta) ;
}

