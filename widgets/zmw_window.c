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

#include "zmw/zmw.h"

typedef enum { Zmw_Popup_None, Zmw_Popup_Right, Zmw_Popup_Bottom } Zmw_Popup ;

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

/* This stack is not nice because redondent with zmw one.
 * It is needed because the widget can't execute code
 * just before the first child.
 * If it was possible, the "gc" change should be done at this place
 */
static GdkGC *global_gc_stack[9][ZMW_TABLE_SIZE(ZMW_GC_COPY_ON_WRITE)] ;
static Zmw_Boolean global_copy[9][ZMW_TABLE_SIZE(ZMW_GC_COPY_ON_WRITE)] ;
static int global_gc_nb = 0 ;

void zmw_gc_push()
{
  int i ;
  
  for(i=0; i<ZMW_TABLE_SIZE(ZMW_GC_COPY_ON_WRITE); i++)
    {
      global_gc_stack[global_gc_nb][i] = ZMW_GC[i] ;
      global_copy[global_gc_nb][i] = ZMW_GC_COPY_ON_WRITE[i] ;
      ZMW_GC_COPY_ON_WRITE[i] = Zmw_True ;
    }
  global_gc_nb++ ;
}

void zmw_gc_pop()
{
  int i ;
  
  global_gc_nb-- ;
  for(i=0; i<ZMW_TABLE_SIZE(ZMW_GC_COPY_ON_WRITE); i++)
    {
      if ( ZMW_GC[i] != global_gc_stack[global_gc_nb][i] )
      {
	      gdk_gc_destroy(ZMW_GC[i]) ;
      	      ZMW_GC[i] = global_gc_stack[global_gc_nb][i] ;
              ZMW_GC_COPY_ON_WRITE[i] = global_copy[global_gc_nb][i] ;
      }
    }
}



void zmw_window_generic(GdkWindow **w, Zmw_Popup pop
			, const int *detached, int follow_mouse
			, const char *title)
{
  GdkWindowAttr wa ;
  gint x, y ;
  Zmw_Size *s ;
  Zmw_Resource r = { "WindowID", 0 } ;
  GdkRectangle rect ;
  int i ;

  zmw_resource_pointer_get((void**)&w, &r) ;

  switch( ZMW_SUBACTION )
    {
    case Zmw_Init:
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
	  gdk_window_set_events(*w, GDK_ALL_EVENTS_MASK) ;

	  /*
	  if ( gdk_drawable_get_colormap(*w) == NULL )
	    {
	      static GdkColormap *cm = NULL ;
	      if ( cm == NULL )
		{
		  cm = gdk_colormap_new(gdk_visual_get_best(),1) ;
		  if ( cm == NULL )
		    ZMW_ABORT ;
		}
	      gdk_drawable_set_colormap(*w, cm) ;
	    }
	  */

	  {
	    GdkColor c ;
	    if ( pop )
	      c.pixel = 0xD0D0D0D0 ;
	    else
	      c.pixel = 0xC0C0C0C0 ;
	    gdk_window_set_background(*w, &c) ;
	  }

	  zmw_resource_set(&r) ;
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

      zmw_gc_push() ;
      
      /* This should not be done.
       * Because it destroy current color and so, we can't change window
       * color. It is done like this because :
       *    - New GC must be created for the window
       *    - We don't know the current color
       *    - There is not gc_get_background
       * So it must be corrected.
       */
      
      zmw_color(ZMW_FOREGROUND, 0x00000000) ;
      if ( pop )
	{
	  zmw_color(ZMW_BACKGROUND_NORMAL, 0xD0D0D0D0) ;
	  zmw_color(ZMW_BACKGROUND_PUSHED, 0xC0C0C0C0) ;
	  zmw_color(ZMW_BACKGROUND_POPED, 0xE0E0E0E0) ;
	  zmw_color(ZMW_BORDER_LIGHT, 0xF0F0F0F0) ;
	  zmw_color(ZMW_BORDER_DARK, 0x70707070) ;
	}
      else
	{
	  zmw_color(ZMW_BACKGROUND_NORMAL, 0xC0C0C0C0) ;
	  zmw_color(ZMW_BACKGROUND_PUSHED, 0xB0B0B0B0) ;
	  zmw_color(ZMW_BACKGROUND_POPED, 0xD0D0D0D0) ;
	  zmw_color(ZMW_BORDER_LIGHT, 0xE0E0E0E0) ;
	  zmw_color(ZMW_BORDER_DARK, 0x60606060) ;
	}
     
      zmw_compute_window_size() ;

      /* Remove the clipping rectangle */
      
      rect.x = 0 ;
      rect.y = 0 ;
      rect.width = ZMW_SIZE_ALLOCATED.width ;
      rect.height = ZMW_SIZE_ALLOCATED.height ;
      for(i=0; i<ZMW_TABLE_SIZE(ZMW_GC_COPY_ON_WRITE); i++)
	{
	  zmw_modify_gc(gdk_gc_set_clip_rectangle(ZMW_GC[i], &rect);, i) ;
	}

#if GLIB_MAJOR_VERSION > 1
      gdk_window_begin_paint_rect(ZMW_WINDOW, &rect) ;
#endif
      gdk_draw_rectangle(ZMW_WINDOW, ZMW_GC[ZMW_BACKGROUND_NORMAL], 1,
			 0,0,9999,9999) ;

      if ( !gdk_window_is_visible(*w) )
	{
	  if ( pop )
	    {
	      if ( zMw[-1].i.window )
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
/*	      else
	      {
		      gdk_window_move(ZMW_WINDOW, zmw.x, zmw.y) ;	      	
	      }  */
	    }
	  /* (1) */
	  gdk_window_show(*w) ;
	}
      /* Was in (1) */
	  if ( title && ( !pop || zmw_window_detached(detached, Zmw_Detached_Here) ) )
	    gdk_window_set_title(*w, title) ;

      zmw.nb_windows++ ;
      ZMW_REALLOC(zmw.windows, zmw.nb_windows ) ;
      zmw.windows[zmw.nb_windows-1].window = *w;
      break ;
    case Zmw_Post_Drawing:
      if ( follow_mouse && gdk_window_is_visible(*w) )
	{	
	  /* the -10 so the cursor don't easely go over the popup
	   *  when moved, because in this case (window,x,y) are false.
	   * I don't know how to fix this.
	   */
	  gdk_window_move(ZMW_WINDOW
			  , zmw.x_root - ZMW_SIZE_ALLOCATED.width - 10
			  , zmw.y_root - ZMW_SIZE_ALLOCATED.height - 10
			  ) ;
			  

	}
      zmw_gc_pop() ;
#if GLIB_MAJOR_VERSION > 1
      gdk_window_end_paint(ZMW_WINDOW) ;
#endif
      break ;
    default:
      break ;
    }
}


char* zmw_window_name()
{
  Zmw_State *s ;
  static char *name = NULL ;
  char *pc, save ;
  /* Search the window */
  s = zmw.ptr ;
  while( s->i.window == zmw.ptr->i.window )
    s-- ;
  s++ ;
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
  		name = strdup(zmw_name_full) ;
  		break ;
  	case Zmw_Detached_Next:
		zmw_name_of_the_transient_begin() ;
  		name = strdup(zmw_name_full) ;
		zmw_name_of_the_transient_end() ;
		break ;
  	case Zmw_Detached_Up:
  		name = zmw_window_name() ;
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
		free(name) ;
	}
}



void zmw_window_with_id(GdkWindow **w, const char *title)
{
  zmw_window_generic(w, Zmw_Popup_None, 0, 0, title) ;
}
void zmw_window(const char *title)
{
  zmw_window_with_id(NULL, title) ;
}


void zmw_window_popup_right_with_id_and_detached(GdkWindow **w, const int *detached)
{
  zmw_window_generic(w, Zmw_Popup_Right, detached, 0, NULL) ;
}
void zmw_window_popup_right_with_id(GdkWindow **w)
{
  zmw_window_generic(w, Zmw_Popup_Right, NULL, 0, NULL) ;
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
  zmw_window_generic(w, Zmw_Popup_Bottom, detached, 0, NULL) ;
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
  zmw_window_generic(w, Zmw_Popup_Right, detached, 0, title) ;
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
  zmw_window_generic(w, Zmw_Popup_Bottom, detached, 0, title) ;
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
  zmw_window_generic(w, Zmw_Popup_Right, 0, 1, NULL) ;
}

void zmw_window_drag()
{
  zmw_window_drag_with_id(NULL) ;
}

