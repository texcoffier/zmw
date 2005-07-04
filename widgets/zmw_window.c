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


#include <ctype.h>
#include "zmw/zmw.h"
#include "zmw/zmw_private.h" // The code must on not depends on this line

typedef enum { Zmw_Popup_None, Zmw_Popup_Right, Zmw_Popup_Bottom } Zmw_Popup ;

/*
 * This function is not called on the first pass.
 * A window contains only ONE zmw.
 * So we can use zmw_state_get()[1] without problems.
 */
static void zmw_compute_window_size()
{
  int i, nb, the_child ;

  nb = 0 ;
  the_child = 0 ;  /* To avoid a compiler warning */
  for(i=0; i<zmw_nb_of_children_get(); i++)
    if ( zmw_child__used_by_parent_get(i) )
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
      ZMW_HERE ;
      zmw_stack_print() ;
    }

  if ( zmw_auto_resize_get() || !gdk_window_is_visible(*zmw_window_get()) )
    {
      gdk_window_resize(*zmw_window_get()
			, zmw_child__required_padded_width(the_child)
			, zmw_child__required_padded_height(the_child)
			) ;
      zmw_allocated_width_set(zmw_child__required_padded_width(the_child)) ;
      zmw_allocated_height_set(zmw_child__required_padded_height(the_child)) ;
    }
  else
    {
      int w, h ;
      gdk_window_get_size(*zmw_window_get(), &w, &h) ;
      zmw_allocated_width_set(w) ;
      zmw_allocated_height_set(h) ;
    }
  zmw_allocated_x_set(0) ;
  zmw_allocated_y_set(0) ;
  zmw_used_by_parent_set(Zmw_False) ;
  zmw_child__allocated_set(the_child,zmw_allocated_get()) ;
  zmw_padding_remove(zmw_child__allocated_get(the_child), zmw_child__padding_width_get(the_child)) ;

  zmw_clipping_x_set(0) ;
  zmw_clipping_y_set(0) ;
  zmw_clipping_width_set(zmw_allocated_width_get()) ;
  zmw_clipping_height_set(zmw_allocated_height_get()) ;
}


void zmw_window_generic(GdkWindow **w, Zmw_Popup pop
			, int follow_mouse
			, const char *title, GdkGC **gc)
{
  GdkWindowAttr wa ;
  gint x, y, width, height ;
  Zmw_Child *s ;
  GdkRectangle rect ;

  switch( zmw_subaction_get() )
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
	    c.pixel = zmw_colors_get()[Zmw_Color_Background_Normal] ;
	    gdk_window_set_background(*w, &c) ;
	  }
	}
      zmw_window_set(w) ;
      zmw_gc_set(*gc) ;
      zmw_used_by_parent_set(Zmw_False) ;

      /* If the cursor is in a popped window, the window shouldstay popped */
      if ( pop
	   && !zmw_window_is_detached()
	   && zmw_zmw_event_get()
	   && *zmw_window_get() == zmw_zmw_event_get()->any.window
	   )
	zmw_window_update_uppers(Zmw_Menu_Is_Poped) ;


      /* Here because top level widgets are called only once */
      /* Not nice, may be we should enclose all the application
	 in a special widget */
      zmw_min_width_set(0) ;
      zmw_min_height_set(0) ;
      zmw_required_set(zmw_min_get()) ;

      /* Really not nice */
      zmw_allocated_x_set(0) ;
      zmw_allocated_y_set(0) ;
      gdk_window_get_size(*zmw_window_get()
			  , &width
			  , &height
			  ) ;
      gdk_window_get_size(*zmw_window_get(), &width, &height) ;
      zmw_allocated_width_set(width) ;
      zmw_allocated_height_set(height) ;

      // Keyboard navigation
      /*
      if ( zmw_zmw_raised_get() != *zmw_window_get() && zmw.update_raised )
	{
	  if ( zmw_debug_get() & Zmw_Debug_Navigation )
	    zmw_printf("zmw_zmw_raised_get() is updated to %p\n", *zmw_window_get()) ;
	  zmw_zmw_raised_set(*zmw_window_get()) ;
	  zmw.update_raised = Zmw_False ;
	}
      */
      break ;

    case Zmw_Compute_Children_Allocated_Size:
      zmw_compute_window_size() ;
      break ;

    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:
      zmw_compute_window_size() ;

      // zmw_draw_clip_set() ;

#if GLIB_MAJOR_VERSION > 1
      rect.x = zmw_clipping_x_get() ;
      rect.y = zmw_clipping_y_get() ;
      rect.width = zmw_clipping_width_get() ;
      rect.height = zmw_clipping_height_get() ;

      gdk_window_begin_paint_rect(*zmw_window_get(), &rect) ;
#endif
      
      zmw_draw_rectangle(Zmw_Color_Background_Normal, Zmw_True,
			 0,0,9999,9999) ;
      
      if ( pop && zmw_parent__window_get()
	   && gdk_window_get_type(*zmw_window_get()) == GDK_WINDOW_TEMP
	   && !follow_mouse )
	{
	  gdk_window_get_origin(*zmw_parent__window_get(), &x, &y) ;
	  s = zmw_widget_previous_size() ;
	  gdk_window_move(*zmw_window_get()
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
	gdk_window_set_title(*zmw_window_get(), title) ;


      /*
       * The test is not nice.
       * The do_not_map window should be set in the parent
       * and not by a grand parent.
       */
      if ( !zmw_is_widget_top() && zmw_parent__do_not_map_window_get() )
	{
	  //	  if ( gdk_window_is_visible(*zmw_window_get()) )
	    gdk_window_hide(*zmw_window_get()) ;	  
	}
      else
	{
	  if ( !gdk_window_is_visible(*zmw_window_get()) )
	    gdk_window_show(*zmw_window_get()) ;
	  
	  (*zmw_zmw_nb_windows_get_ptr())++ ;
	  ZMW_REALLOC(*zmw_zmw_windows_get_ptr(), zmw_zmw_nb_windows_get() ) ;
	  zmw_zmw_windows_get()[zmw_zmw_nb_windows_get()-1] = *zmw_window_get();
	}
      break ;
    case Zmw_Post_Drawing:
      // zmw_draw_clip_pop() ;
      if ( follow_mouse && gdk_window_is_visible(*zmw_window_get()) )
	{	
	  /* the -10 is not required.
	   * It should be a parameter (in current state?)
	   */
	  gdk_window_move(*zmw_window_get()
			  , zmw_zmw_x_root_get() - zmw_allocated_width_get() - 10
			  , zmw_zmw_y_root_get() - zmw_allocated_height_get() - 10
			  ) ;
			  

	}
#if GLIB_MAJOR_VERSION > 1
      gdk_window_end_paint(*zmw_window_get()) ;
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
  for(s=zmw_state_get()-1; s >= zmw_zmw_table_get(); s--)
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

  for(s=zmw_state_get(); s >= zmw_zmw_table_get(); s--)
      if ( s->u.menu_state )
	{
	  *s->u.menu_state ^= Zmw_Menu_Is_Detached ;

	  if ( *s->u.menu_state & Zmw_Menu_Is_Detached )
	    {
	      // Just detached : unpop all
	      zmw_window_unpop_all() ;

	      while( --s >= zmw_zmw_table_get() )
		if ( s->u.menu_state )
		  *s->u.menu_state &= ~(Zmw_Menu_Is_Poped*(1+Zmw_Menu_State_New)) ;
	    }
	  else
	    {
	      // Just attached : 
	      if ( *s->u.menu_state & Zmw_Menu_Contains_A_Detached )
		zmw_window_update_uppers(Zmw_Menu_Contains_A_Detached) ;
	    }
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
void zmw_window_popup_bottom_with_id(GdkWindow **w)
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

