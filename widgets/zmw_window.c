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


#include <ctype.h>
#include "zmw/zmw.h"

typedef enum { Zmw_Popup_None, Zmw_Popup_Right, Zmw_Popup_Bottom } Zmw_Popup ;

/*
 * This function is called from "zmw.c/zmw_action_do_no_enter"
 * This could be considered as a current state from parent to child
 * but not between siblings.
 */

void zmw_window_restore()
{
  ZMW_WINDOW = zMw[-1].i.window ; /* Restore window */
  ZMW_GC = zMw[-1].i.gc ; /* Restore GC */
  ZMW_CLIPPING = zMw[-1].i.clipping ;
}

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

  /* I don't understand this test, always necessary ? */
  if ( nb == 0 )
    return ; /* Aborted event */

  if ( nb != 1 )
    {
      fprintf(stderr, "Window have more than one child (%d)\n", nb) ;
      ZMW_ABORT ;
    }

  if ( ZMW_AUTO_RESIZE || !gdk_window_is_visible(*ZMW_WINDOW) )
    {
      gdk_window_resize(*ZMW_WINDOW
			, ZMW_CHILD_REQUIRED_PADDED_WIDTH(the_child)
			, ZMW_CHILD_REQUIRED_PADDED_HEIGHT(the_child)
			) ;
      ZMW_SIZE_ALLOCATED.width = ZMW_CHILD_REQUIRED_PADDED_WIDTH(the_child) ;
      ZMW_SIZE_ALLOCATED.height = ZMW_CHILD_REQUIRED_PADDED_HEIGHT(the_child) ;
    }
  else
    {
      gdk_window_get_size(*ZMW_WINDOW
			  , &ZMW_SIZE_ALLOCATED.width
			  , &ZMW_SIZE_ALLOCATED.height
			  ) ;
    }
  ZMW_SIZE_ALLOCATED.x = 0 ;
  ZMW_SIZE_ALLOCATED.y = 0 ;
  ZMW_USED_TO_COMPUTE_PARENT_SIZE = Zmw_False ;
  ZMW_CHILDREN[the_child].allocated = ZMW_SIZE_ALLOCATED ;
  zmw_padding_remove(&ZMW_CHILDREN[the_child].allocated, ZMW_CHILDREN[the_child].current_state.padding_width) ;
}


void zmw_window_generic(GdkWindow **w, Zmw_Popup pop
			, int follow_mouse
			, const char *title, GdkGC **gc)
{
  GdkWindowAttr wa ;
  gint x, y ;
  Zmw_Size *s ;
  GdkRectangle rect ;

  switch( ZMW_SUBACTION )
    {
    case Zmw_Init:
      zmw_resource_pointer_get((void**)&w, "WindowID", NULL) ;
      zmw_resource_pointer_get((void**)&gc, "WindowGC", 0) ;
      if ( *w == NULL )
	{
	  wa.title = "toto" ;
	  wa.width = 350 ;
	  wa.height = 500 ;
	  wa.wclass = GDK_INPUT_OUTPUT ;

	  if ( pop && !zmw_window_is_detached() )
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
	  // zmw_printf("colormap %p visual %p\n", wa.colormap, wa.visual) ;
	  
	  *w = gdk_window_new(NULL, &wa, GDK_WA_COLORMAP|GDK_WA_VISUAL) ;
	  gdk_window_set_events(*w,
				GDK_ALL_EVENTS_MASK
				& ~GDK_PROPERTY_CHANGE_MASK
				& ~GDK_STRUCTURE_MASK
				& ~GDK_FOCUS_CHANGE_MASK
				& ~GDK_SUBSTRUCTURE_MASK
				& ~GDK_SCROLL_MASK
				& ~GDK_PROXIMITY_IN_MASK
				& ~GDK_PROXIMITY_OUT_MASK
				) ;
	  *gc = gdk_gc_new(*w) ;

	  {
	    GdkColor c ;
	    c.pixel = ZMW_COLORS[Zmw_Color_Background_Normal] ;
	    gdk_window_set_background(*w, &c) ;
	  }
	}
      ZMW_WINDOW = w ;
      ZMW_GC = *gc ;
      ZMW_USED_TO_COMPUTE_PARENT_SIZE = Zmw_False ;

      /* If the cursor is in a popped window, the window shouldstay popped */
      if ( pop && !zmw_window_is_detached() && zmw.event && *ZMW_WINDOW == zmw.event->any.window )
	zmw_window_update_uppers(Zmw_Menu_Is_Poped * (1+Zmw_Menu_State_New)) ;


      /* Here because top level widgets are called only once */
      /* Not nice, may be we should enclose all the application
	 in a special widget */
      ZMW_SIZE_MIN.width = 0 ;
      ZMW_SIZE_MIN.height = 0 ;
      ZMW_SIZE_REQUIRED = ZMW_SIZE_MIN ;
      break ;

    case Zmw_Compute_Required_Size:
      break ;

    case Zmw_Compute_Children_Allocated_Size:
      zmw_compute_window_size() ;
      break ;

    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:
      zmw_compute_window_size() ;

      /* Remove the clipping rectangle */
      
      ZMW_CLIPPING.x = 0 ;
      ZMW_CLIPPING.y = 0 ;
      ZMW_CLIPPING.width = ZMW_SIZE_ALLOCATED.width ;
      ZMW_CLIPPING.height = ZMW_SIZE_ALLOCATED.height ;
      zmw_draw_clip_push(&ZMW_CLIPPING) ;

#if GLIB_MAJOR_VERSION > 1
      rect.x = ZMW_CLIPPING.x ;
      rect.y = ZMW_CLIPPING.y ;
      rect.width = ZMW_CLIPPING.width ;
      rect.height = ZMW_CLIPPING.height ;

      gdk_window_begin_paint_rect(*ZMW_WINDOW, &rect) ;
#endif
      
      zmw_draw_rectangle(Zmw_Color_Background_Normal, Zmw_True,
			 0,0,9999,9999) ;
      
      if ( pop && zMw[-1].i.window
	   && gdk_window_get_type(*ZMW_WINDOW) == GDK_WINDOW_TEMP
	   && !follow_mouse )
	{
	  gdk_window_get_origin(*zMw[-1].i.window, &x, &y) ;
	  s = zmw_widget_previous_size() ;
	  gdk_window_move(*ZMW_WINDOW
			  , x
			  + s->allocated.x - s->current_state.padding_width
			  + (pop==Zmw_Popup_Right ? s->allocated.width + 2*s->current_state.padding_width  : 0 )
			  , y
			  + s->allocated.y - s->current_state.padding_width
			  + (pop==Zmw_Popup_Bottom ? s->allocated.height + 2*s->current_state.padding_width  : 0)
			  ) ;
	}
      if ( title
	   && ( !pop || zmw_window_is_detached() ) )
	gdk_window_set_title(*ZMW_WINDOW, title) ;


      /*
       * The test is not nice.
       * The do_not_map window should be set in the parent
       * and not by a grand parent.
       */
      if ( !ZMW_WIDGET_TOP && ZMW_PARENT_SIZE.do_not_map_window )
	{
	  //	  if ( gdk_window_is_visible(*ZMW_WINDOW) )
	    gdk_window_hide(*ZMW_WINDOW) ;	  
	}
      else
	{
	  if ( !gdk_window_is_visible(*ZMW_WINDOW) )
	    gdk_window_show(*ZMW_WINDOW) ;
	  
	  zmw.nb_windows++ ;
	  ZMW_REALLOC(zmw.windows, zmw.nb_windows ) ;
	  zmw.windows[zmw.nb_windows-1].window = *ZMW_WINDOW;
	}
      break ;
    case Zmw_Post_Drawing:
      zmw_draw_clip_pop() ;
      if ( follow_mouse && gdk_window_is_visible(*ZMW_WINDOW) )
	{	
	  /* the -10 is not required.
	   * It should be a parameter (in current state?)
	   */
	  gdk_window_move(*ZMW_WINDOW
			  , zmw.x_root - ZMW_SIZE_ALLOCATED.width - 10
			  , zmw.y_root - ZMW_SIZE_ALLOCATED.height - 10
			  ) ;
			  

	}
#if GLIB_MAJOR_VERSION > 1
      gdk_window_end_paint(*ZMW_WINDOW) ;
#endif
      break ;
    default:
      break ;
    }
}

Zmw_Boolean zmw_window_is_detached()
{
  Zmw_State *s ;
  /*
   * Search a state up
   */
  for(s=zMw; s >= zmw.zmw_table; s--)
    {
      if ( s->u.menu_state )
	{
	  if ( *s->u.menu_state & Zmw_Menu_Is_Detached )
	    return Zmw_True ;
	  else
	    return Zmw_False ;
	}
    }
  return Zmw_False ;
}

/* Update the new state because we are dispatching event */
void zmw_window_detached_toggle()
{
  Zmw_State *s ;

  for(s=zMw; s >= zmw.zmw_table; s--)
      if ( s->u.menu_state )
	{
	  *s->u.menu_state ^= Zmw_Menu_Is_Detached * Zmw_Menu_State_New ;
	  *s->u.menu_state ^= Zmw_Menu_Is_Detached ;
	  break ;
	}
}



void zmw_window_with_id(GdkWindow **w, const char *title)
{
  zmw_window_generic(w, Zmw_Popup_None, 0, title, NULL) ;
}
void zmw_window(const char *title)
{
  zmw_window_with_id(NULL, title) ;
}





void zmw_window_popup_right_with_id_and_title(GdkWindow **w, const char *title)
{
  zmw_window_generic(w, Zmw_Popup_Right, 0, title, NULL) ;
}
void zmw_window_popup_right_with_title(const char *title)
{
  zmw_window_popup_right_with_id_and_title(NULL, title) ;
}
void zmw_window_popup_right_with_id(GdkWindow **w)
{
  zmw_window_popup_right_with_id_and_title(w, NULL) ;
}
void zmw_window_popup_right()
{
  zmw_window_popup_right_with_id(NULL) ;
}


void zmw_window_popup_bottom_with_id_and_title(GdkWindow **w,const char *title)
{
  zmw_window_generic(w, Zmw_Popup_Bottom, 0, title, NULL) ;
}
void zmw_window_popup_bottom_with_title(const char *title)
{
  zmw_window_popup_bottom_with_id_and_title(NULL, title) ;
}
void zmw_window_popup_nottom_with_id(GdkWindow **w)
{
  zmw_window_popup_bottom_with_id_and_title(w, NULL) ;
}
void zmw_window_popup_bottom()
{
  zmw_window_generic(NULL, Zmw_Popup_Bottom, 0, NULL, NULL) ;
}


void zmw_window_drag_with_id(GdkWindow **w)
{
  zmw_window_generic(w, Zmw_Popup_Right, 1, NULL, NULL) ;
}

void zmw_window_drag()
{
  zmw_window_drag_with_id(NULL) ;
}

