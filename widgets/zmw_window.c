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

  if ( ZMW_AUTO_RESIZE || !gdk_window_is_visible(ZMW_WINDOW) )
    {
      gdk_window_resize(ZMW_WINDOW
			, ZMW_CHILDREN[the_child].required.width
			, ZMW_CHILDREN[the_child].required.height) ;
      ZMW_SIZE_ALLOCATED.width = ZMW_CHILDREN[the_child].required.width ;
      ZMW_SIZE_ALLOCATED.height = ZMW_CHILDREN[the_child].required.height ;
    }
  else
    {
      gdk_window_get_size(ZMW_WINDOW
			  , &ZMW_SIZE_ALLOCATED.width
			  , &ZMW_SIZE_ALLOCATED.height
			  ) ;
    }
  ZMW_SIZE_ALLOCATED.x = 0 ;
  ZMW_SIZE_ALLOCATED.y = 0 ;
  ZMW_USED_TO_COMPUTE_PARENT_SIZE = Zmw_False ;
  ZMW_CHILDREN[the_child].allocated = ZMW_SIZE_ALLOCATED ;
}


void zmw_window_generic(GdkWindow **w, Zmw_Popup pop
			, const int *detached, int follow_mouse
			, const char *title, GdkGC **gc)
{
  GdkWindowAttr wa ;
  gint x, y ;
  Zmw_Size *s ;
  Zmw_Resource r = { "WindowID", 0 } ;
  Zmw_Resource rgc = { "WindowGC", 0 } ;
  GdkRectangle rect ;

  switch( ZMW_SUBACTION )
    {
    case Zmw_Init:
      if ( ZMW_ACTION == zmw_action_dispatch_accelerator )
	{
	  ZMW_WINDOW++ ; // Kludge to make zmw_window_name works
	  break ;
	}

      zmw_resource_pointer_get((void**)&w, &r) ;
      zmw_resource_pointer_get((void**)&gc, &rgc) ;
      if ( *w == NULL )
	{
	  wa.title = "toto" ;
	  wa.width = 350 ;
	  wa.height = 500 ;
	  wa.wclass = GDK_INPUT_OUTPUT ;
	  
	  if ( pop && !zmw_window_detached(detached, Zmw_Detached_Here) )
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

	  zmw_resource_set(&r) ;
	  zmw_resource_set(&rgc) ;
	}
      ZMW_WINDOW = *w ;
      ZMW_GC = *gc ;
      ZMW_USED_TO_COMPUTE_PARENT_SIZE = Zmw_False ;
      
      break ;

    case Zmw_Compute_Required_Size:
      /* To make cache checking happy */
      ZMW_SIZE_MIN.width = 0 ;
      ZMW_SIZE_MIN.height = 0 ;
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

      gdk_window_begin_paint_rect(ZMW_WINDOW, &rect) ;
#endif
      
      zmw_draw_rectangle(Zmw_Color_Background_Normal, Zmw_True,
			 0,0,9999,9999) ;
      
      if ( pop && zMw[-1].i.window
	   && gdk_window_get_type(ZMW_WINDOW) == GDK_WINDOW_TEMP
	   && !follow_mouse )
	{
	  gdk_window_get_origin(zMw[-1].i.window, &x, &y) ;
	  s = zmw_widget_previous_size() ;
	  gdk_window_move(ZMW_WINDOW
			  , x
			  + s->allocated.x
			  + (pop==Zmw_Popup_Right ? s->allocated.width : 0 )
			  , y
			  + s->allocated.y
			  + (pop==Zmw_Popup_Bottom ? s->allocated.height : 0)
			  ) ;
	}
      if ( title
	   && ( !pop || zmw_window_detached(detached, Zmw_Detached_Here) ) )
	gdk_window_set_title(ZMW_WINDOW, title) ;



      if ( ! ZMW_SIZE_DO_NOT_MAP_WINDOW )
	{
	  if ( !gdk_window_is_visible(ZMW_WINDOW) )
	    gdk_window_show(ZMW_WINDOW) ;
	  
	  zmw.nb_windows++ ;
	  ZMW_REALLOC(zmw.windows, zmw.nb_windows ) ;
	  zmw.windows[zmw.nb_windows-1].window = ZMW_WINDOW;
	}
      break ;
    case Zmw_Post_Drawing:
      zmw_draw_clip_pop() ;
      if ( follow_mouse && gdk_window_is_visible(ZMW_WINDOW) )
	{	
	  /* the -10 is not required.
	   * It should be a parameter.
	   */
	  gdk_window_move(ZMW_WINDOW
			  , zmw.x_root - ZMW_SIZE_ALLOCATED.width - 10
			  , zmw.y_root - ZMW_SIZE_ALLOCATED.height - 10
			  ) ;
			  

	}
#if GLIB_MAJOR_VERSION > 1
      gdk_window_end_paint(ZMW_WINDOW) ;
#endif
      break ;
    default:
      break ;
    }
}


char* zmw_window_name(Zmw_Boolean up)
{
  Zmw_State *s ;
  static char *name = NULL ;
  char *pc, save ;

  /* Search the window */
  s = zmw.ptr ;

  if ( up )
    {
      while( s->i.window == ZMW_WINDOW )
	s-- ;
      s++ ;
      /* We are on the toplevel window */
    }

  // And now we go up until a transient
  // It is for the popup windows contained in a zmw_void.
  if ( s->u.transient_separator == 0 )
    s-- ;
 
  /* Take the window name */
  pc = s->u.name_index ;
  while( *pc != '/' && *pc != '\0' )
    pc++ ;
  save = *pc ;
  *pc = '\0' ;
  name = strdup(zmw_name_full) ;
  *pc = save ;
  
  return(name) ;
}

static char * zmw_detached_name(Zmw_Detached where)
{
  char *name ;
  
  switch(where)
  {
  	case Zmw_Detached_Here:
  		name = zmw_window_name(Zmw_False) ;
  		// name = strdup(zmw_name_full) ;
  		break ;
  	case Zmw_Detached_Next:
		zmw_name_of_the_transient_begin() ;
  		name = strdup(zmw_name_full) ;
		zmw_name_of_the_transient_end() ;
		break ;
  	case Zmw_Detached_Up:
  		name = zmw_window_name(Zmw_True) ;
  		break ;
  }
  return(name) ;
}

Zmw_Boolean zmw_window_detached(const int *detached
				, Zmw_Detached where)
{
  int d ;
  char *name ;

  if ( detached )
    return( *detached ) ;

  name = zmw_detached_name(where) ;

  d = Zmw_False ;
  zmw_name_get_value_int_with_name(name, "WindowDetached", &d) ;

  free(name) ;

  return(d) ;
}

/*
 * Search the deepest detached window containing name.
 * It is used to compute if a menu should be displayed or not.
 * Return NULL if there is no such window.
 */
Zmw_Name* zmw_window_detached_containing(const char *name)
{
  int d, len ;
  char *n ;
  Zmw_Name *na ;

  n = strdup(name) ;
  d = Zmw_False ;
  len = strlen(n) ;

  do
    {
      if ( zmw_name_is_transient(n, len) )
	{
	  na = zmw_name_get_name(n, "WindowDetached") ;
	  if ( na && na->value )
	    {
	      free(n) ;
	      return na ;
	    }
	}
      zmw_name_cut_last(n, &len) ;
    }
  while(len > 0 ) ;
  free(n) ;
  return NULL ;
}

/*
 * Returns true if the next window (transient) contains
 * a detached window.
 * In fact returns the number of detached menu contained.
 */

int zmw_window_contains_detached()
{
  int d ;
  char *name ;

  name = zmw_detached_name(Zmw_Detached_Next) ;

  d = 0 ;
  zmw_name_get_value_int_with_name(name, "WindowContainsDetached", &d) ;

  free(name) ;

  return(d) ;
}


void zmw_mark_windows_rec(char *name, int detached, int len)
{
  int n ;

  zmw_name_cut_last(name, &len) ;
  if ( len < 0 )
    return ;

  if ( zmw_name_is_transient(name, len) )
    {
      n = 0 ;
      zmw_name_get_value_int_with_name(name, "WindowContainsDetached", &n);
      if ( detached )
	{
	  zmw_name_set_value_int_with_name(name, "WindowContainsDetached",n+1);
	}
      else
	{
	  if ( n > 1 )
	    zmw_name_set_value_int_with_name(name, "WindowContainsDetached"
					     ,n-1);
	  else
	    zmw_name_unregister_value(name, "WindowContainsDetached") ;
	}
    }

  zmw_mark_windows_rec(name, detached, len) ;
  name[len] = '/' ;
}

void zmw_mark_windows(char *name, int detached)
{
  zmw_mark_windows_rec(name, detached, strlen(name)) ;
}

void zmw_window_detached_toggle(int *detached, Zmw_Detached where)
{
	int value ;
	char *name ;
	
	if ( detached )
	      *detached = 1 - *detached ;
	else
	{
		value = Zmw_False ;
		name = zmw_detached_name(where) ;
		zmw_name_get_value_int_with_name(name,"WindowDetached", &value) ;
		zmw_name_set_value_int_with_name(name, "WindowDetached", 1 - value) ;
		zmw_mark_windows(name, 1 - value) ;
		free(name) ;
	}
}



void zmw_window_with_id(GdkWindow **w, const char *title)
{
  zmw_window_generic(w, Zmw_Popup_None, 0, 0, title, NULL) ;
}
void zmw_window(const char *title)
{
  zmw_window_with_id(NULL, title) ;
}


void zmw_window_popup_right_with_id_and_detached(GdkWindow **w, const int *detached)
{
  zmw_window_generic(w, Zmw_Popup_Right, detached, 0, NULL, NULL) ;
}
void zmw_window_popup_right_with_id(GdkWindow **w)
{
  zmw_window_generic(w, Zmw_Popup_Right, NULL, 0, NULL, NULL) ;
}
void zmw_window_popup_right_with_detached(const int *detached)
{
  zmw_window_popup_right_with_id_and_detached(NULL, detached) ;
}
void zmw_window_popup_right()
{
  zmw_window_popup_right_with_id_and_detached(NULL, NULL) ;
}

void zmw_window_popup_bottom_with_id_and_detached(GdkWindow **w, const int *detached)
{
  zmw_window_generic(w, Zmw_Popup_Bottom, detached, 0, NULL, NULL) ;
}
void zmw_window_popup_bottom_with_detached(const int *detached)
{
  zmw_window_popup_bottom_with_id_and_detached(NULL, detached) ;
}
void zmw_window_popup_bottom()
{
  zmw_window_popup_bottom_with_id_and_detached(NULL, NULL) ;
}


void zmw_window_popup_right_with_id_and_detached_and_title(GdkWindow **w, const int *detached
					      , const char *title)
{
  zmw_window_generic(w, Zmw_Popup_Right, detached, 0, title, NULL) ;
}
void zmw_window_popup_right_with_detached_and_title(const int *detached, const char *title)
{
  zmw_window_popup_right_with_id_and_detached_and_title(NULL, detached, title) ;
}
void zmw_window_popup_right_with_title(const char *title)
{
  zmw_window_popup_right_with_id_and_detached_and_title(NULL, NULL, title) ;
}


void zmw_window_popup_bottom_with_id_and_detached_and_title(GdkWindow **w
						, const int *detached
					      , const char *title)
{
  zmw_window_generic(w, Zmw_Popup_Bottom, detached, 0, title, NULL) ;
}
void zmw_window_popup_bottom_with_detached_and_title(const int *detached, const char *title)
{
  zmw_window_popup_bottom_with_id_and_detached_and_title(NULL, detached, title) ;
}
void zmw_window_popup_bottom_with_title(const char *title)
{
  zmw_window_popup_bottom_with_id_and_detached_and_title(NULL, NULL, title) ;
}


void zmw_window_drag_with_id(GdkWindow **w)
{
  zmw_window_generic(w, Zmw_Popup_Right, 0, 1, NULL, NULL) ;
}

void zmw_window_drag()
{
  zmw_window_drag_with_id(NULL) ;
}

