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

void zmw_text_general(char **text, int option)
{
  gint lbearing, rbearing, width, ascent, descent ;
  int border, activable, focusable, editable ;
  int x, y ;

  border = !!(option & Zmw_Option_Border) ;
  editable = !!(option & Zmw_Option_Editable) ;
  focusable = !!(option & Zmw_Option_Focusable) ;
  activable = !!(option & Zmw_Option_Activable) ;

  switch( ZMW_SUBACTION )
    {
    case Zmw_Compute_Required_Size:
      if ( *text == NULL )
	abort() ;

      gdk_string_extents(ZMW_FONT, *text
			 , &lbearing, &rbearing, &width, &ascent, &descent
			 ) ;
      ZMW_SIZE_MIN.width = width
	+ 2*(border*ZMW_BORDER_WIDTH+focusable*ZMW_FOCUS_WIDTH) ;
      ZMW_SIZE_MIN.height = ascent + descent
	+ 2*(border*ZMW_BORDER_WIDTH+focusable*ZMW_FOCUS_WIDTH) ;

      if ( editable )
	ZMW_SIZE_SENSIBLE = 1 ;
      break ;

    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:
      if ( *text == NULL )
	abort() ;

      zmw_border_draw
	((border                            ? Zmw_Border_Relief : 0)
	 | (focusable                       ? Zmw_Border_Focusable : 0)
	 | (zmw_focused()                   ? Zmw_Border_Draw_Focus : 0)
	 | (border                          ? Zmw_Border_Background : 0)
	 | (((activable && zmw_selected()) || editable) ? Zmw_Border_In : 0)
	 ) ;
      gdk_string_extents(ZMW_FONT, *text
			 , &lbearing, &rbearing, &width, &ascent, &descent
			 ) ;

      x = ZMW_SIZE_ALLOCATED.x	+ border*ZMW_BORDER_WIDTH + focusable*ZMW_FOCUS_WIDTH ;
      if ( ZMW_SIZE_HORIZONTAL_ALIGNMENT != -1 )
	x += (ZMW_SIZE_ALLOCATED.width-width-2*(border*ZMW_BORDER_WIDTH+focusable*ZMW_FOCUS_WIDTH))
	  / (ZMW_SIZE_HORIZONTAL_ALIGNMENT==0?2:1) ;

      y = ZMW_SIZE_ALLOCATED.y + ZMW_SIZE_ALLOCATED.height
	- border*ZMW_BORDER_WIDTH - focusable*ZMW_FOCUS_WIDTH
	- descent ;
      if ( ZMW_SIZE_VERTICAL_ALIGNMENT != -1 )
	y -= (ZMW_SIZE_ALLOCATED.height-ascent-descent-2*(border*ZMW_BORDER_WIDTH+focusable*ZMW_FOCUS_WIDTH))
	  / (ZMW_SIZE_VERTICAL_ALIGNMENT==0?2:1) ;


      if ( ZMW_SIZE_SENSIBLE || (!activable && !editable) )
	gdk_draw_string(ZMW_WINDOW, ZMW_FONT, ZMW_GC[ZMW_FOREGROUND]
			, x, y, *text) ;
      
      else
	{
	  gdk_draw_string(ZMW_WINDOW, ZMW_FONT, ZMW_GC[ZMW_BORDER_LIGHT]
			  , x-1, y-1, *text) ;
	  gdk_draw_string(ZMW_WINDOW, ZMW_FONT, ZMW_GC[ZMW_BORDER_DARK]
			  , x, y, *text) ;
	}

      break ;
    case Zmw_Input_Event:
      if ( focusable )
	zmw_focusable() ;
      if ( activable )
	zmw_activable() ;
      if ( zmw_key_pressed() )
	{
	  if ( editable )
	    {
	      if ( zmw.event->key.keyval == GDK_BackSpace )
		{
		  if ( *text )
		    (*text)[strlen(*text)-1] = '\0' ;
		}
	      else
		{
		  ZMW_REALLOC(*text, strlen(*text)+2) ;
		  strcat(*text, zmw.event->key.string) ;
		}
	    }
	  if ( activable && zmw.event->key.string[0] )
	      zmw.activated = 1 ;
	  
	  zmw_event_remove() ;
	}
      if ( zmw.activated )
	zmw_window_unpop_all() ;
      break ;
    default:
      break ;
    }
}

void zmw_text(const char *text)
{
  ZMW(zmw_text_general( (char**)&text, 0)) { }
}


void zmw_text_editable(char **text)
{
  ZMW(zmw_text_general(text
		       , Zmw_Option_Editable
		       |Zmw_Option_Focusable
		       |Zmw_Option_Border)) { }
}

void zmw_int_editable(int *i)
{
  char buf[20], *text ;
  

  sprintf(buf, "%d", *i) ;
  text = strdup(buf) ;
  zmw_text_editable(&text) ;
  *i = atoi(text) ;
  free(text) ;
}

void zmw_button(const char *text)
{
  ZMW(zmw_text_general((char**)&text
		       , Zmw_Option_Focusable
		       |Zmw_Option_Border
		       |Zmw_Option_Activable)) { }
}

void zmw_text_activable(const char *text)
{
  ZMW(zmw_text_general((char**)&text
		       , Zmw_Option_Focusable
		       |Zmw_Option_Activable)) { }
}

void zmw_button_with_accelerator(const char *text
				 , GdkModifierType state, int character
				 )
{
  char buf[strlen(text)+20] ;

  sprintf(buf, "%s   %s%s%s%s%c"
	  , text
	  , (state & GDK_SHIFT_MASK) ? "Shift+" : ""
	  , (state & GDK_LOCK_MASK) ? "Lock+" : ""
	  , (state & GDK_CONTROL_MASK) ? "Control+" : ""
	  , (state & GDK_MOD1_MASK) ? "Alt+" : ""
	  , character
	  ) ;
  zmw_button(buf) ;
  if ( !zmw.activated && zmw_accelerator(state, character) )
    zmw.activated = 1 ;
}

void zmw_tearoff(int *detached, GdkWindow **w)
{
  if ( *detached )
    zmw_button("«----------") ;
  else
    zmw_button("----------»") ;
  if ( zmw_activated() )
    {
      *w = NULL ;
      *detached = 1 - *detached ;
    }
}

