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

#include "zmw.h"

/*
 * This function is not called on the first pass.
 * A window contains only ONE zmw.
 * So we can use zMw[1] without problems.
 */
static void zmw_compute_window_size()
{
  int i, nb, the_child ;

  nb = 0 ;
  the_child = 0 ;  /* To avoid a compiler warning */
  for(i=0; i<ZMW_NB_OF_CHILDREN; i++)
    if ( ZMW_CHILDREN[i].used_to_compute_parent_size )
      {
	nb++ ;
	the_child = i ;
      }

  if ( nb == 0 )
    return ; /* Aborted event */

  if ( nb > 1 )
    {
      fprintf(stderr, "Window have more than one child (%d)\n", nb) ;
      // abort() ;
    }

  if ( ZMW_AUTO_RESIZE || !gdk_window_is_visible(zMw[1].i.window) )
    gdk_window_resize(zMw[1].i.window
		      , ZMW_CHILDREN[the_child].required.width
		      , ZMW_CHILDREN[the_child].required.height) ;

  gdk_window_get_size(zMw[1].i.window
		      , &ZMW_SIZE_ALLOCATED.width
		      , &ZMW_SIZE_ALLOCATED.height
		      ) ;
  ZMW_SIZE_ALLOCATED.x = 0 ;
  ZMW_SIZE_ALLOCATED.y = 0 ;
  ZMW_USED_TO_COMPUTE_PARENT_SIZE = Zmw_False ;
  ZMW_CHILDREN[the_child].allocated = ZMW_SIZE_ALLOCATED ;
}



void zmw_window_generic(GdkWindow **w, int pop, int detached, int follow_mouse
			, const char *title)
{
  GdkWindowAttr wa ;
  gint x, y ;

  switch( ZMW_SUBACTION )
    {
    case Zmw_Init:
      if ( *w == NULL )
	{
	  wa.title = "toto" ;
	  wa.width = 350 ;
	  wa.height = 500 ;
	  wa.wclass = GDK_INPUT_OUTPUT ;
	  
	  if ( pop && !detached )
	    {
	      wa.window_type = GDK_WINDOW_TEMP ;
	    }
	  else
	    wa.window_type = GDK_WINDOW_TOPLEVEL ;
	  
	  /* wa.cursor = ; */
	  wa.wmclass_name = "wmclass_name" ;
	  wa.wmclass_class = "wmclass_class" ;
	  wa.colormap = gdk_rgb_get_cmap() ;
	  wa.visual = gdk_rgb_get_visual() ;
	  wa.override_redirect = 0 ;
	  
	  *w = gdk_window_new(NULL, &wa, GDK_WA_COLORMAP|GDK_WA_VISUAL) ;
	  gdk_window_set_events(*w, GDK_ALL_EVENTS_MASK) ;
	}
      ZMW_WINDOW = *w ;
      ZMW_USED_TO_COMPUTE_PARENT_SIZE = Zmw_False ;
      
      break ;

    case Zmw_Compute_Required_Size:
      break ;

    case Zmw_Compute_Children_Allocated_Size:
      zmw_compute_window_size() ;
      break ;

    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:

      zmw_foreground(ZMW_FOREGROUND, 0x00000000) ;
      if ( pop )
	{
	  zmw_foreground(ZMW_BACKGROUND_NORMAL, 0xD0D0D0D0) ;
	  zmw_foreground(ZMW_BACKGROUND_PUSHED, 0xC0C0C0C0) ;
	  zmw_foreground(ZMW_BACKGROUND_POPED, 0xE0E0E0E0) ;
	  zmw_foreground(ZMW_BORDER_LIGHT, 0xF0F0F0F0) ;
	  zmw_foreground(ZMW_BORDER_DARK, 0x70707070) ;
	}
      else
	{
	  zmw_foreground(ZMW_BACKGROUND_NORMAL, 0xC0C0C0C0) ;
	  zmw_foreground(ZMW_BACKGROUND_PUSHED, 0xB0B0B0B0) ;
	  zmw_foreground(ZMW_BACKGROUND_POPED, 0xD0D0D0D0) ;
	  zmw_foreground(ZMW_BORDER_LIGHT, 0xE0E0E0E0) ;
	  zmw_foreground(ZMW_BORDER_DARK, 0x60606060) ;
	}

      zmw_compute_window_size() ;

      gdk_draw_rectangle(ZMW_WINDOW, ZMW_GC[ZMW_BACKGROUND_NORMAL], 1,
			 0,0,9999,9999) ;

      if ( !gdk_window_is_visible(*w) )
	{
	  if ( pop )
	    {
	      gdk_window_get_origin(zMw[-1].i.window, &x, &y) ;
	      gdk_window_move(ZMW_WINDOW
			      , x
			      + zMw[-1].u.children[zMw[-1].u.nb_of_children-2].allocated.x
			      + zMw[-1].u.children[zMw[-1].u.nb_of_children-2].allocated.width
			      , y
			      + zMw[-1].u.children[zMw[-1].u.nb_of_children-2].allocated.y
			      ) ;
	    }
	  /* 1 */
	  gdk_window_show(*w) ;
	}
      /* c'était en 1 */
	  if ( title && ( !pop || detached ) )
	    gdk_window_set_title(*w, title) ;

      zmw.nb_windows++ ;
      ZMW_REALLOC(zmw.windows, zmw.nb_windows ) ;
      zmw.windows[zmw.nb_windows-1].window = w;

      break ;
    case Zmw_Post_Drawing:
      if ( follow_mouse && gdk_window_is_visible(*w) )
	{
	  gdk_window_get_origin(zMw[-2].i.window, &x, &y) ;
	  gdk_window_move(ZMW_WINDOW
			  , zmw.x - ZMW_SIZE_ALLOCATED.width + x
			  , zmw.y - ZMW_SIZE_ALLOCATED.height + y
			  ) ;
	}
      break ;
    default:
      break ;
    }
}

void zmw_window(GdkWindow **w, const char *title)
{
  zmw_window_generic(w, 0, 0, 0, title) ;
}

void zmw_window_popup(GdkWindow **w, int detached)
{
  zmw_window_generic(w, 1, detached, 0, NULL) ;
}

void zmw_window_popup_with_title(GdkWindow **w, int detached
				 , const char *title)
{
  zmw_window_generic(w, 1, detached, 0, title) ;
}

void zmw_window_drag(GdkWindow **w)
{
  zmw_window_generic(w, 1, 0, 1, NULL) ;
}

