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

#include "zmw/zmw.h"
#include "zmw/zmw_private.h"
#include "zmw/socket.h"
#include "zmw/http.h"
#include "zmw/selection.h"
#include <gdk/gdkkeysyms.h>
#include <sys/time.h>
#include <sys/times.h>

struct zmw_main
{
  struct timeval last_cursor_move ; /* To use for shortcut popup */
  struct timeval last_user_action ; /* To use for tips */
  int still_time ;              /* Time since last pointer motion */
  int idle_time ;               /* Time since last user interaction */

  Zmw_Boolean need_repaint ;
  Zmw_Boolean need_dispatch ;           /* for drag and drop */  
  /* If false, the events are associated to the window under the cursor.
   * If true, the events are associated to the window where
   * the cursor had been pressed.
   */
  Zmw_Boolean use_window_from_button_press ;

  Zmw_Boolean accelerator_window_allowed ;

  float frame_per_sec ;
  float frame_per_sec_cpu ;
  int cache_size ;
  gint input_http ;
  int top_level_debug ;

  void (*fct)() ;
} ;

void zmw_need_repaint()
{
  zmw.run->need_repaint = Zmw_True ;
}

void zmw_need_dispatch()
{
  zmw.run->need_dispatch = Zmw_True ;
}




void zmw_use_window_from_button_press(Zmw_Boolean b)
{
  zmw.run->use_window_from_button_press = b ;
}

Zmw_Boolean zmw_use_window_from_button_press_get()
{
  return zmw.run->use_window_from_button_press ;
}


void zmw_debug_flags()
{
  static int display_zmw = 0 ;
  
  zmw_name("DebugFlags") ;
  ZMW(zmw_vbox())
    {
      zmw_check_button_int_with_label(&display_zmw, "Debug flags") ;
      ZMW( zmw_if(display_zmw) )
	{
	  ZMW(zmw_vbox())
	    {
	      zmw_check_button_bits_int_with_label(&zmw.run->top_level_debug
					     , Zmw_Debug_Draw_Cross
					     , "Draw a cross on widget with event") ;
	      zmw_check_button_bits_int_with_label(&zmw.run->top_level_debug
					     , Zmw_Debug_Trace
					     , "Trace all calls") ;
	      zmw_check_button_bits_int_with_label(&zmw.run->top_level_debug
					     , Zmw_Debug_Event
					     , "Trace events") ;
	      zmw_check_button_bits_int_with_label(&zmw.run->top_level_debug
					     , Zmw_Debug_Drag
					     , "Trace drag and drop") ;
	      zmw_check_button_bits_int_with_label(&zmw.run->top_level_debug
					     , Zmw_Debug_Cache_Fast
					     , "Fast checking on cache") ;
	      zmw_check_button_bits_int_with_label(&zmw.run->top_level_debug
					     , Zmw_Debug_Cache_Slow
					     , "Slow checking on cache") ;
#if ZMW_PROFILING
	      zmw_check_button_bits_int_with_label(&zmw.run->top_level_debug
					     , Zmw_Debug_Profiling
					     , "Widget profiling") ;
#endif
#if ZMW_DEBUG_NAME
	      zmw_check_button_bits_int_with_label(&zmw.run->top_level_debug
					     , Zmw_Debug_Name
					     , "Some checking on name handling") ;
#endif
	    }
	}
      if ( zmw_name_registered(&zmw.widget_to_trace) )
	{
	  zmw_label("Widget to debug :") ;
	  zmw_label(zmw_name_registered(&zmw.widget_to_trace)) ;
	}
      else
	  zmw_label("To debug a widget, press Ctrl-F2 on it") ;
    }
  zmw_border_embossed_in_draw() ;
}


static void debug_window()
{
  static GdkWindow *w = NULL ;
  char tmp[99] ;

  if ( zmw.run->top_level_debug & Zmw_Debug_Window_Auto_Resize )
    zmw_auto_resize(1) ;

  ZMW(zmw_window_with_id(&w, "Debug"))
    {
      zmw_horizontal_expand(0) ;
      zmw_vertical_expand(0) ;
      zmw_horizontal_alignment(-1) ;
      ZMW(zmw_vbox())
	{
	  zmw_label("Debug Window") ;
	  zmw_label("------------") ;
	  zmw_event_debug_window() ;
	  zmw_drag_debug_window() ;
	  zmw_name_debug_window() ;
	  zmw_debug_flags() ;

	  sprintf(tmp, "%f Real Frames per Second", zmw.run->frame_per_sec) ;
	  zmw_name("FPS") ;
	  zmw_label(tmp) ;
	  sprintf(tmp, "%f CPU Frames per Second"
		  , zmw.run->frame_per_sec_cpu) ;
	  zmw_label(tmp) ;
	}
    }

  zmw_auto_resize(0) ;
}

void zmw_display_accelerator_window()
{
  if ( zmw.still_yet_displayed && zmw.key_pressed && zmw.event_key.key.state
       && zmw.run->accelerator_window_allowed )
    {
      zmw_accelerators_window(zmw.event_key.key.state
			      &(GDK_SHIFT_MASK
				|GDK_LOCK_MASK
				|GDK_CONTROL_MASK
				|GDK_MOD1_MASK
				|GDK_MOD2_MASK
				)
			      ) ;
    }
}


void zmw_call_widget(void (*fct)(), int (*action)())
{
  static Zmw_Boolean in_redispatch = 0 ;
  int i ;

  zmw_zmw_external_do_not_make_init_set(Zmw_False) ;
  zmw_zmw_event_removed_set(Zmw_False) ;
  zmw.run->need_dispatch = Zmw_False ;
  zmw_action_set(action) ;
  zmw_child_number_set(-1) ;
  zmw_debug_set(zmw.run->top_level_debug) ;

  if ( zmw.run->top_level_debug & Zmw_Debug_Event )
    {
      zmw_printf("CALL %s\n", zmw_action_name_fct()) ;
      if ( zmw_action_get() == zmw_action_dispatch_event )
	zmw_printf("w=%p x=%d y=%d r=%d\n"
		   , zmw_zmw_event_get()->any.window, zmw_zmw_x_get() ,zmw_zmw_y_get(), in_redispatch) ;
    }


  zmw_child_cache_init(zmw.run->cache_size) ;
  if ( zmw.run->top_level_debug & Zmw_Debug_Window )
    {
      zmw_name("DebugWindow") ;
      debug_window() ;
    }
  /* Reinitialize the search for close widgets */
  if ( action != zmw_action_dispatch_accelerator )
    {
      if ( zmw_debug_get() & Zmw_Debug_Navigation )
	zmw_printf("Reset left/right/up/down\n") ;
      for(i=0; i<4; i++)
	{
	  zmw.near[i].name.value = (void*)2000000000 ;
	  ZMW_FREE(zmw.near[i].name.name) ;
	}
    }

  zmw_name("") ;
  (*fct)() ;
  zmw_display_accelerator_window() ;
  
  if ( zmw.run->top_level_debug & Zmw_Debug_Event )
    zmw_printf("ENDCALL nb_childn=%d\n", zmw_state_get()->u.nb_of_children) ;

  if ( zmw_action_get() == zmw_action_dispatch_event )
    {
      /*
       * for zmw_cursor_leave/enter
       */
      zmw_cursor_set(zmw.found.name) ;
      /*
       * The redispatch is here only for the drag and drop.
       * Two passes are needed because two event are received.
       */	
      if ( zmw.run->need_dispatch && zmw_zmw_event_get()->any.type != GDK_MOTION_NOTIFY )
      	{
	  if ( in_redispatch )
	    {
	      zmw_printf("Bug: recursif redispatch\n") ;
	    }
	  else
	    {
	      if ( zmw.run->top_level_debug & Zmw_Debug_Event )
		zmw_printf("BEGIN redispatch\n") ;
	      zmw_zmw_event_get()->any.type = GDK_MOTION_NOTIFY ;
	      in_redispatch = Zmw_True ;
	      zmw_call_widget(fct, action) ;
	      in_redispatch = Zmw_False ;
	      if ( zmw.run->top_level_debug & Zmw_Debug_Event )
		zmw_printf("END redispatch\n") ;
	      // zmw_need_repaint() ;
	    }
      	}
    }
    
    if ( zmw.run->top_level_debug & Zmw_Debug_Event )
      {
    	zmw_printf("ENDCALL REAL\n") ;
	if ( zmw_action_get() == zmw_action_dispatch_event )
	  zmw_printf("w=%p x=%d y=%d r=%d OUT\n"
		     , zmw_zmw_event_get()->any.window, zmw_zmw_x_get() ,zmw_zmw_y_get(), in_redispatch) ;
      }
}

static int zmw_time_from(struct timeval *tv)
{
  struct timeval now ;

  gettimeofday(&now, NULL) ;

  return(  now.tv_usec - tv->tv_usec + (now.tv_sec - tv->tv_sec)*1000000) ;
}



void zmw_draw(void (*fct)())
{
  int i ;
  GList *tlw ;
  struct timeval begin_time ;
  struct tms begin_tms, end_tms ;
  static int clk = 0 ;

  if ( clk == 0 )
    clk = sysconf(_SC_CLK_TCK);



 // zmw_name_unregister(&zmw_zmw_tip_displayed_get()) ;
  zmw.nb_windows = 0 ;
  gettimeofday(&begin_time,NULL) ;
  times(&begin_tms) ;
  zmw_call_widget(fct, zmw_action_draw) ;
  times(&end_tms) ;
  zmw.run->frame_per_sec = 1000000. / zmw_time_from(&begin_time) ;
  zmw.run->frame_per_sec_cpu = 1/((end_tms.tms_utime-begin_tms.tms_utime)/(float)clk) ;
  zmw.run->need_repaint = 0 ;


  for( tlw = gdk_window_get_toplevels() ; tlw ; tlw = g_list_next(tlw) )
    {
      for(i=0; i<zmw.nb_windows; i++)
	{
	  if ( tlw->data == zmw.windows[i] )
	    break ;
	}
      if ( i == zmw.nb_windows )
	{
	  gdk_window_hide(tlw->data) ;
	}
    }

  if ( zmw.run->top_level_debug & Zmw_Debug_Draw )
    {
      static FILE *f = NULL ;
      static int nb_draw = 0 ;

      if ( f == NULL )
	f = fopen("xxx.nb_draw", "w") ;

      rewind(f) ;
      fprintf(f, "%d\n", ++nb_draw) ;
      fflush(f) ;
    }
}

static void zmw_search(void (*fct)())
{
  zmw_name_unregister(zmw_zmw_found_get()) ;
  zmw_call_widget(fct, zmw_action_search) ;
}

/*
 * The search is needed before dispatching an event:
 *    - Enter/Leave widgets
 *    - Find the focus group
 */
static void zmw_dispatch_event(void (*fct)())
{
  zmw_search(fct) ;
  zmw_call_widget(fct, zmw_action_dispatch_event) ;
}

static gboolean timeout(gpointer data)
{
  gint x, y ;
  static gint lx=0, ly=0 ;
  GdkEvent e ;
  void (*fct)() = data ;

  if( gdk_event_peek() )
    return(TRUE) ;

  /*
   * Reset cursor position
   */
  if ( data == NULL )
    {
      lx = 0 ;
      ly = 0 ;
      return(TRUE) ;
    }
  gdk_window_get_pointer(zmw_zmw_window_get(), &x, &y, NULL) ;
  if ( ! zmw.run->use_window_from_button_press || !zmw_zmw_button_pressed_get() )
    e.any.window = gdk_window_at_pointer(&x, &y) ;
  else
    e.any.window = NULL ;


  if ( (lx != x || ly != y) && e.any.window  )
    {
      /* Needed by "tip" */
      zmw_search(fct) ;
    }
  /*
   *
   */
  zmw.run->idle_time = zmw_time_from(&zmw.run->last_user_action) ;
  zmw.run->still_time = zmw_time_from(&zmw.run->last_cursor_move) ;
  if ( 0 & zmw.run->top_level_debug & Zmw_Debug_Event )
    {
      zmw_printf("idle_time=%d still_time %d tips_yet_displayed %d still_yet_displayed=%d\n"
		 , zmw.run->idle_time
		 , zmw.run->still_time
		 , zmw.tips_yet_displayed
		 , zmw.still_yet_displayed
		 ) ;
    }
  /*
   * If the cursor is still, set idle time
   */
  if ( lx == x && ly == y
       	&& e.any.window && !zmw.still_yet_displayed
       	&& zmw.run->still_time > 500000
     //  	&& zmw_name_registered(&zmw.found)
       	 )
    {
      if ( zmw.run->idle_time > 500000 )
	{
	  zmw.tips_yet_displayed = Zmw_True ;
	}
      zmw.still_yet_displayed = Zmw_True ;
      zmw_need_repaint() ;
    }


  if ( zmw.run->need_repaint )
    {
      zmw_draw(fct) ;
    }

  lx = x ;
  ly = y ;

  return(TRUE) ;
}

void user_action()
{
  gettimeofday(&zmw.run->last_user_action,NULL) ;
  
  if ( zmw_name_registered(zmw_zmw_tip_displayed_get()) )
    {
      zmw_name_unregister(zmw_zmw_tip_displayed_get()) ;
      zmw_need_repaint() ;
    }
  zmw.tips_yet_displayed = Zmw_False ;
  zmw.still_yet_displayed = Zmw_False ;   
}

void zmw_unpop(GdkEvent *e)
{
  if ( e->any.window!=NULL 
       && gdk_window_get_type(e->any.window) != GDK_WINDOW_TEMP
       // && ! zmw_zmw_event_removed_get()
       )
    {
      if ( zmw_key_string_unsensitive() )
	return ;
      if ( zmw.run->top_level_debug & Zmw_Debug_Event )
	{
	  zmw_printf("Unpop because event on non-popup window\n") ;
	}
      zmw_window_unpop_all() ;
    }
}  

Zmw_Boolean zmw_keyboard_navigation()
{
  int d ;
  
  if ( zmw_debug_get() & Zmw_Debug_Navigation )
    ZMW_HERE ;
  d = -1 ;
  if ( zmw.event_key.key.keyval == GDK_Left  ) d = 0 ;
  if ( zmw.event_key.key.keyval == GDK_Right ) d = 1 ;
  if ( zmw.event_key.key.keyval == GDK_Up    ) d = 2 ;
  if ( zmw.event_key.key.keyval == GDK_Down  ) d = 3 ;

  if ( d>=0 && zmw.near[d].name.name )
    {
      zmw.focused = zmw.near[d].rectangle ;
      zmw_name_register_with_name(zmw_zmw_focus_with_cursor_get(), zmw.near[d].name.name) ;
      zmw_need_repaint() ;
      zmw_zmw_event_removed_get() = Zmw_True ;
      /*
       * Very very bad behaviour.
       * The correct way is not to raise window but to verify
       * is a widget is visible before letting it have the focus.
       * But GDK/X11 do not allow such question to be asked.
       * I would like the function :
       *    GdkWindow *gdk_window_at_position(int x, int y)
       * It would return the top level window visible at x,y.
       *
       * I don't want to move visualy the cursor here in order to use :
       *    GdkWindow*    gdk_window_at_pointer(gint *win_x, gint *y)
       * Even this is bad because it does not take into account
       * windows not created by the application.
       */
      gdk_window_raise(zmw.near[d].window) ;
      zmw.raised = zmw.near[d].window ;

      return Zmw_True ;
    }
  return Zmw_False ;
}


void event_handler(GdkEvent *e, gpointer o)
{
  void (*fct)() = o ;
  GdkEvent *ee, saved ;
  GdkWindow *w ;

  /* What is this event ? Key on other windows ? */
  if ( e->type == -1 )
    return ;

  saved = *e ;
  /* I want the release event on the real window and not
   * in the window where the button was pressed
   */
  if ( ! zmw.run->use_window_from_button_press )
    {
      if ( e->type == GDK_BUTTON_RELEASE
	   || e->type == GDK_MOTION_NOTIFY
	   )
	{
	  int x, y, origin_x, origin_y ;
	  x = e->button.x_root ;
	  y = e->button.y_root ;
	  w = gdk_window_at_pointer(&x, &y) ;
	  if ( w )
	    {
	      e->any.window = w ;
	      gdk_window_get_origin(e->any.window, &origin_x, &origin_y) ;
	      e->button.x = e->button.x_root - origin_x ;
	      e->button.y = e->button.y_root - origin_y ;
	    }
	}
    }
  zmw_zmw_event_set(e) ;
  
  if ( zmw_handle_selection(e) )
  {
   	zmw_selection_clear() ;
  	zmw_need_repaint() ;
  }


#define ZMW_GET_XY(T) zmw_zmw_x_set(T.x) ; zmw_zmw_y_set(T.y) ; zmw_zmw_x_root_set(T.x_root) ; zmw_zmw_y_root_set(T.y_root)
#define ZMW_GET_XYW(T) ZMW_GET_XY(T) ; zmw_zmw_window_set(T.window)

  switch( e->type )
    {
    case GDK_MOTION_NOTIFY:
      ZMW_GET_XYW(e->motion) ;

      gettimeofday(&zmw.run->last_cursor_move,NULL) ;
      if ( zmw.run->top_level_debug & Zmw_Debug_Event )
	zmw_printf("**** EVENT **** GDK_MOTION_NOTIFY\n") ;

      ee = gdk_event_peek() ;
      if ( ee && ee->type == GDK_MOTION_NOTIFY )
	return ;

      zmw_dispatch_event(fct) ;

      if ( e->motion.state != 0 )
	zmw_need_repaint() ; /* for drag */

      user_action() ;
      break ;

    case GDK_EXPOSE:
      if ( zmw.run->top_level_debug & Zmw_Debug_Event )
	zmw_printf("**** EVENT **** GDK_EXPOSE\n") ;
      zmw_need_repaint() ;
      return ; /* 8/10/2003 do waste less CPU and see less debug garbage */
      break ;

    case GDK_VISIBILITY_NOTIFY:
      if ( zmw.run->top_level_debug & Zmw_Debug_Event )
	zmw_printf("**** EVENT **** GDK_VISIBILITY_NOTIFY\n") ;
      /*
      if ( e->visibility.state == GDK_VISIBILITY_UNOBSCURED )
	zmw.raised = e->visibility.window ;
      */
      zmw_need_repaint() ;
      return ;
      break ;

    case GDK_KEY_PRESS:
      zmw.run->accelerator_window_allowed = Zmw_False ;

      zmw.key_pressed = Zmw_True ;
      zmw.still_yet_displayed = Zmw_False ;
      zmw.event_key = *e ;
      if ( zmw.event_key.key.keyval == GDK_Control_L
      	|| zmw.event_key.key.keyval == GDK_Control_R )
	{
	  zmw.event_key.key.state |= GDK_CONTROL_MASK ;
	  zmw.run->accelerator_window_allowed = Zmw_True ;
	}
      if ( zmw.event_key.key.keyval == GDK_Shift_L
      	|| zmw.event_key.key.keyval == GDK_Shift_R )
	{
	  zmw.event_key.key.state |= GDK_SHIFT_MASK ;
	  zmw.run->accelerator_window_allowed = Zmw_True ;
	}
       if ( zmw.event_key.key.keyval == GDK_Alt_L
      	|| zmw.event_key.key.keyval == GDK_Alt_R )
	 {
	   zmw.event_key.key.state |= GDK_MOD1_MASK ;
	   zmw.run->accelerator_window_allowed = Zmw_True ;
	 }
       if ( zmw.event_key.key.keyval == GDK_Meta_L
      	|| zmw.event_key.key.keyval == GDK_Meta_R )
	 {
	   zmw.event_key.key.state |= GDK_MOD2_MASK ;
	   zmw.run->accelerator_window_allowed = Zmw_True ;
	 }

      zmw_zmw_window_set(e->key.window) ;

      // Keyboard navigation
      if ( zmw.event_key.key.state & GDK_CONTROL_MASK )
	if ( zmw_keyboard_navigation() )
	  break ;
      
      if ( zmw.run->top_level_debug & Zmw_Debug_Event )
	zmw_printf("**** EVENT **** KEY_PRESS\n") ;
      {
	gint x, y ;
	e->any.window = gdk_window_at_pointer(&x, &y) ;
	zmw_zmw_x_set(x) ;
	zmw_zmw_y_set(y) ;
      }
      if ( zmw_zmw_event_get()->type == GDK_KEY_PRESS
	   && ( zmw_zmw_event_get()->key.state & GDK_CONTROL_MASK )
	   &&  zmw_zmw_event_get()->key.keyval == GDK_F1
	   )
	   {
		zmw.run->top_level_debug ^= Zmw_Debug_Window ;	
		zmw_need_repaint() ;
	   }
      // zmw_unpop(e) ;
      zmw_accelerator_init() ;
      zmw_call_widget(fct, zmw_action_dispatch_accelerator) ;
      if ( zmw_zmw_event_removed_get() )
	break ;

      if ( zmw_zmw_event_get()->key.string[0] == '\033' )
	zmw_window_unpop_all() ;
      
      user_action() ;
      // zmw_unpop(e) ;
      zmw_dispatch_event(fct) ;
      // Keyboard navigation, No control if cursor key unused by any widget
      if ( !zmw_zmw_event_removed_get() )
	zmw_keyboard_navigation() ;
      zmw_need_repaint() ;

      break ;

    case GDK_2BUTTON_PRESS:
    case GDK_3BUTTON_PRESS:
    case GDK_BUTTON_PRESS:
    case GDK_BUTTON_RELEASE:
      ZMW_GET_XYW(e->button) ;

      user_action() ;
      if ( e->type == GDK_BUTTON_RELEASE )
	zmw_zmw_button_pressed_set(Zmw_False) ;
      if ( e->type == GDK_BUTTON_PRESS )
	zmw_zmw_button_pressed_set(Zmw_True) ;

      if (zmw.run->top_level_debug & Zmw_Debug_Event)
	zmw_printf("**** EVENT **** BUTTON! %s on window %p\n"
		   , e->type == GDK_BUTTON_RELEASE ? "release" : "press"
		   , e->any.window
		   ) ;
      zmw_unpop(e) ;

      zmw_dispatch_event(fct) ;
      /* You always make repaint because a button release
       * may also change the display state : button press on
       * menu button and release on a menu item.
       * The menu item may be a sub menu and so it is not activated...
       */
      // if ( zmw_zmw_event_get()->type == GDK_NOTHING ) commented 2003/10/27
      zmw_need_repaint() ;
      
      if ( e->type == GDK_BUTTON_RELEASE )
	{
	  zmw_event_button_release() ;
	}

      if ( saved.type == GDK_BUTTON_RELEASE )
	zmw_drag_cancel() ; // In order to stop drag when nobody accept it
      break ;

    case GDK_ENTER_NOTIFY:
      ZMW_GET_XY(e->crossing) ;

      if ( zmw.run->top_level_debug & Zmw_Debug_Event )
	zmw_printf("**** EVENT **** ENTER!\n") ;
      return ; /* 2003/10/27 to waste less CPU and see less debug garbage */
      break ;

    case GDK_LEAVE_NOTIFY:
      ZMW_GET_XY(e->crossing) ;

      if ( zmw.run->top_level_debug & Zmw_Debug_Event )
	zmw_printf("**** EVENT **** LEAVE!\n") ;
      /* Needed by "cursor_leave" */
      zmw_name_unregister(&zmw.found) ;
      zmw_zmw_event_get()->any.type = GDK_MOTION_NOTIFY ;
      zmw_call_widget(fct, zmw_action_dispatch_event) ;
      *e = saved ;
      return ; /* 2003/10/27 to waste less CPU and see less debug garbage */
      break ;

    case GDK_KEY_RELEASE:
      zmw.key_pressed = Zmw_False ;
      zmw.still_yet_displayed = Zmw_False ;
      break ;

    default:
      if ( zmw.run->top_level_debug & Zmw_Debug_Event )
	zmw_printf("**** EVENT **** ???? = %d\n", e->type) ;
      return ; /* 2003/10/8 to waste less CPU and see less debug garbage */
      break ;
    }

    /* Compute the accelerator list */
   if ( gdk_event_peek() == NULL )
   {
      zmw_accelerator_init() ;
      zmw_zmw_event_get()->any.type = GDK_MOTION_NOTIFY ;
      zmw_call_widget(fct, zmw_action_dispatch_accelerator) ;
   }

   *e = saved ;
}

static void zmw_take_int_param(gint *argc, gchar ***argv, char *name
			      , int *value)
{
  int i ;

  for(i=0; i<*argc;)
    {
      if ( strncmp((*argv)[i], name, strlen(name)) == 0 )
	{
	  *value = atoi((*argv)[i] + strlen(name) ) ;
	  memmove(&(*argv)[i], &(*argv)[i+1], (*argc - i)*sizeof((*argv)[i])) ;
	  fprintf(stderr, "%s%d\n", name, *value) ;
	  (*argc)-- ;
	}
      else
	i++ ;
    }
}

static int zmw_global_timeout = 100 ;

void zmw_init(gint *argc, gchar ***argv)
{
  int pango_cache ;

  gdk_init(argc, argv) ;
  gdk_rgb_init() ;

  ZMW_MALLOC(zmw.run, 1) ;

  zmw.run->top_level_debug = Zmw_Debug_Cache_Fast
    | Zmw_Debug_Cache_Slow
    | Zmw_Debug_Name
    // | Zmw_Debug_Profiling
    ;
  zmw.run->cache_size = 0 ;
  pango_cache = 0 ;
  zmw_take_int_param(argc,argv,"--cache-size=" , &zmw.run->cache_size ) ;
  zmw_take_int_param(argc,argv,"--debug="      , &zmw.run->top_level_debug) ;
  zmw_take_int_param(argc,argv,"--mstimeout="  , &zmw_global_timeout) ;
  zmw_take_int_param(argc,argv,"--pango-cache=", &pango_cache) ;

  zmw_text_init(pango_cache) ; // Pango init
}

void zmw_main(void (*fct)())
{
  int i ;
  int socket ;
  Zmw_Name *top_level_focus = NULL ;

  zmw_name_initialize(&top_level_focus, "Top level Focus") ;


  zmw.run->fct = fct ;
  gettimeofday(&zmw.run->last_cursor_move,NULL) ;
  gettimeofday(&zmw.run->last_user_action,NULL) ;
  zmw.run->need_repaint = Zmw_False ;
  zmw_use_window_from_button_press(Zmw_True) ;

  zmw.zmw_table_depth = 2 ;
  ZMW_REALLOC(zmw_zmw_table_get(), zmw.zmw_table_depth) ;
  zmw_state_init(zmw_zmw_table_get()) ;
  zmw_state_init(zmw_zmw_table_get() + 1) ;
  zmw_state_set(zmw_zmw_table_get()) ;
  zmw_event_initialize() ;
  zmw_drag_initialize() ;
  // zmw_used_by_parent_set(Zmw_True) ;
  /*
  zmw_allocated_x_set(0) ;
  zmw_allocated_y_set(0) ;
  */

  zmw_font_family("fixed") ;
  zmw_font_size(8) ;
  zmw_font_weight(500) ;
  zmw_font_style(Zmw_Font_Style_Normal) ;
  //zmw_x(ZMW_VALUE_UNDEFINED) ;
  //zmw_y(ZMW_VALUE_UNDEFINED) ;
  //zmw_width(ZMW_VALUE_UNDEFINED) ;
  //zmw_height(ZMW_VALUE_UNDEFINED) ;
  zmw_name_set(zmw.full_name) ;
  zmw_name_get()[0] = '\0' ;
  zmw_name_index_set(zmw_name_get()) ;
  zmw_call_number_set(0) ;
  zmw_border_width(2) ;
  zmw_focus_width(2) ;
  zmw_focus_set(top_level_focus) ;
  top_level_focus->value = (void*)Zmw_True ; // Always cursor in
  zmw_debug( !!(zmw.run->top_level_debug & Zmw_Debug_Widget) ) ;
  zmw_auto_resize(0) ;
  zmw_sensitive(1) ;
  zmw_rgb(0.75, 0.75, 0.75) ;
  zmw_nb_of_children_set(zmw_state_get()->u.nb_of_children_max = 1) ;
  ZMW_MALLOC(zmw_state_get()->u.children, zmw_state_get()->u.nb_of_children_max) ;
  memset(zmw_children_get(), 0, sizeof(Zmw_Child)) ;
  zmw_window_set(NULL) ;

  zmw_state_push() ;

  zmw_size_get() = &zmw_parent__children_get()[-1];
  zmw_horizontal_expand(1) ;
  zmw_horizontal_alignment(0) ;
  zmw_vertical_expand(1) ;
  zmw_vertical_alignment(0) ;
  zmw_padding_width(2) ;

  zmw_draw(fct) ;

  zmw_zmw_tip_displayed_get()->why = "Tip displayed" ;
  zmw_zmw_found_get()->why = "Found" ;
  zmw_zmw_widget_to_trace_get()->why = "Widget to Trace" ;

  i = 10000 ;
  do
    {
      socket = socket_open(i++) ;
    }
  while( socket < 0 ) ;
  socket_listen(socket) ;
  fprintf(stderr, "socket = %d\n", i-1) ;

  zmw.run->input_http = gdk_input_add(socket
				    , GDK_INPUT_READ|GDK_INPUT_EXCEPTION
				    , (GdkInputFunction)http_connection
				    , (gpointer)fct
				    ) ;

  g_timeout_add(zmw_global_timeout, (GSourceFunc)timeout, (gpointer)fct) ;

  gdk_event_handler_set(event_handler, fct, NULL) ;
  for(;;)
    g_main_iteration(TRUE);
}

void zmw_main_quit(int r)
{
  int i ;

  zmw_call_widget(zmw.run->fct, zmw_action_clean) ;

  gdk_input_remove(zmw.run->input_http) ;

  g_idle_remove_by_data(zmw.run->fct) ;

 // for(i=0; i<ZMW_TABLE_SIZE(zmw_zmw_table_get()[1].i.gc); i++)
 //   gdk_gc_destroy(zmw_zmw_table_get()[1].i.gc[i]) ;


  for(i=0; i<zmw.nb_windows; i++)
    gdk_window_destroy(zmw.windows[i]) ;
  ZMW_FREE(zmw.windows) ;


  for(i=0; i<zmw.zmw_table_depth; i++)
    if ( zmw_zmw_table_get()[i].u.children )
      ZMW_FREE(zmw_zmw_table_get()[i].u.children) ;

  free(zmw_zmw_table_get()) ;

  zmw_text_exit() ;
  zmw_name_free() ;
  zmw_child_cache_free() ;
  gdk_exit(r) ;
}

void zmw_stack_dump()
{
  int i, j ;

  for(i=0; &zmw_zmw_table_get()[i] <= zmw_state_get(); i++)
    {
      fprintf(stderr, "======================== STACK[%d]\n", i) ;
      fprintf(stderr, "NOT HERITABLE\n") ;
      fprintf(stderr, "%s\n", zmw_child_string(zmw_size_get())) ;
      fprintf(stderr, "CallNum=%d NBC=%d NBMC=%d name_sep=%d\n"
	      , zmw_zmw_table_get()[i].u.call_number
	      , zmw_zmw_table_get()[i].u.nb_of_children
	      , zmw_zmw_table_get()[i].u.nb_of_children_max
	      , zmw_zmw_table_get()[i].u.name_separator
	      ) ;
      fprintf(stderr, "End of the name = %s\n", zmw_zmw_table_get()[i].u.name) ;
      fprintf(stderr, "Name Suffix = %s\n", zmw_zmw_table_get()[i].u.name_index) ;
      fprintf(stderr, "Type = %s\n", zmw_zmw_table_get()[i].u.type) ;
      fprintf(stderr, ", subaction = %s", zmw_action_name_(zmw_zmw_table_get()[i].u.subaction)) ;
      fprintf(stderr, ", External = %d", zmw_zmw_table_get()[i].u.external_state) ;
      fprintf(stderr, "%s", zmw_zmw_table_get()[i].u.do_not_execute_pop
	      ? ", Do Not execute Pop" : "") ;
      fprintf(stderr, "\n") ;
      for(j=0; j<zmw_zmw_table_get()[i].u.nb_of_children; j++)
	fprintf(stderr, "\t%s\n"
		, zmw_child_string(&zmw_zmw_table_get()[i].u.children[j])) ;
      fprintf(stderr, "-------------------- HERITABLE\n") ;
    }
}
