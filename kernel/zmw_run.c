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
#include "zmw/socket.h"
#include "zmw/http.h"
#include "zmw/selection.h"
#include <gdk/gdkkeysyms.h>
#include <sys/time.h>
#include <sys/times.h>

struct zmw_run
{
  struct timeval last_cursor_move ; /* To use for shortcut popup */
  struct timeval last_user_action ; /* To use for tips */
  int still_time ;              /* Time since last pointer motion */
  int idle_time ;               /* Time since last user interaction */
  int need_repaint ;
  int need_dispatch ;           /* for drag and drop */  

  float frame_per_sec ;
  float frame_per_sec_cpu ;
  int cache_size ;
  gint input_http ;
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


void zmw_debug_flags()
{
  static int display_zmw = 0 ;
  
  ZMW(zmw_box_vertical())
    {
      zmw_toggle_int_with_label(&display_zmw, "Debug flags") ;
      if ( display_zmw )
	{
	  zmw_toggle_bits_int_with_label(&zmw.debug
				     , Zmw_Debug_Draw_Cross
				     , "Draw a cross on widget with event") ;
	  zmw_toggle_bits_int_with_label(&zmw.debug
				     , Zmw_Debug_Trace
				     , "Trace all calls") ;
	  zmw_toggle_bits_int_with_label(&zmw.debug
				     , Zmw_Debug_Event
				     , "Trace events") ;
	  zmw_toggle_bits_int_with_label(&zmw.debug
				     , Zmw_Debug_Drag
				     , "Trace drag and drop") ;
	  zmw_toggle_bits_int_with_label(&zmw.debug
				     , Zmw_Debug_Cache_Fast
				     , "Fast checking on cache") ;
	  zmw_toggle_bits_int_with_label(&zmw.debug
				     , Zmw_Debug_Cache_Slow
				     , "Slow checking on cache") ;
#if ZMW_DEBUG_NAME
	  zmw_toggle_bits_int_with_label(&zmw.debug
				     , Zmw_Debug_Name
				     , "Some checking on name handling") ;
#endif
	}
      if ( zmw_name_registered(&zmw.widget_to_trace) )
	{
	  zmw_text("Widget to debug :") ;
	  zmw_text(zmw_name_registered(&zmw.widget_to_trace)) ;
	}
      else
	  zmw_text("To debug a widget, press Ctrl-F2 on it") ;
    }
  zmw_border_embossed_in_draw() ;
}


static void debug_window()
{
  static GdkWindow *w = NULL ;
  char tmp[99] ;

  if ( zmw.debug & Zmw_Debug_Window_Auto_Resize )
    zmw_auto_resize(1) ;

  ZMW(zmw_window_with_id(&w, "Debug"))
    {
      zmw_horizontal_expand(0) ;
      zmw_vertical_expand(0) ;
      zmw_horizontal_alignment(-1) ;
      ZMW(zmw_box_vertical())
	{
	  zmw_text("Debug Window") ;
	  zmw_text("------------") ;
	  zmw_event_debug_window() ;
	  zmw_drag_debug_window() ;
	  zmw_name_debug_window() ;
	  zmw_debug_flags() ;

	  sprintf(tmp, "%f Real Frames per Second", zmw.run->frame_per_sec) ;
	  zmw_text(tmp) ;
	  sprintf(tmp, "%f CPU Frames per Second", zmw.run->frame_per_sec_cpu) ;
	  zmw_text(tmp) ;
	}
    }

  zmw_auto_resize(0) ;
}

void zmw_display_accelerator_window()
{
  char buf[999] ;
  
  if ( zmw.still_yet_displayed && zmw.key_pressed && zmw.event_key.key.state )
    {
      
      buf[0] = '\0' ;
      if ( zmw.event_key.key.state & GDK_SHIFT_MASK)  strcat(buf, "Shift+") ;
      if ( zmw.event_key.key.state & GDK_LOCK_MASK)   strcat(buf, "Lock+") ;
      if ( zmw.event_key.key.state & GDK_CONTROL_MASK)strcat(buf, "Control+") ;
      if ( zmw.event_key.key.state & GDK_MOD1_MASK)   strcat(buf, "Meta+") ;
      if ( zmw.event_key.key.state & GDK_MOD2_MASK)   strcat(buf, "Alt+") ;
      
      zmw_accelerators_window(buf) ;
    }
}


void zmw_call_widget(void (*fct)(), int (*action)())
{
  if ( zmw.debug & Zmw_Debug_Event )
    zmw_printf("CALL %s index=%d\n", zmw_action_name_fct(), zmw.index_last) ;

  zmw.external_do_not_make_init = Zmw_False ;
  zmw.run->need_dispatch = Zmw_False ;
  ZMW_ACTION = action ;
  zmw_cache_init(zmw.run->cache_size) ;
  zmw_name_init() ;
  if ( zmw.debug & Zmw_Debug_Window )
    {
      zmw_name("DebugWindow") ;
      debug_window() ;
    }
  zmw_name("?") ;
  (*fct)() ;
  zmw_display_accelerator_window() ;
  
  if ( zmw.debug & Zmw_Debug_Event )
    zmw_printf("ENDCALL size_index=%d last_index=%d last_child=%d lcn=%d lcn0=%d\n"
	       ,ZMW_SIZE_INDEX
	       ,zmw.index_last
	       ,zMw->u.children[zMw->u.nb_of_children_0-1].index
	       ,zMw->u.nb_of_children
	       ,zMw->u.nb_of_children_0
	       ) ;

  ZMW_SIZE_INDEX = zmw.index_last + 1 ;

  if ( action != zmw_action_dispatch_accelerator )
    zmw.index_last = zMw->u.children[zMw->u.nb_of_children_0-1].index ;

  zmw.remove_event = Zmw_False ;

  if ( ZMW_SUBACTION == Zmw_Input_Event )
    {
      ZMW_SIZE_SENSIBLE = Zmw_True ;
      ZMW_EVENT_IN = Zmw_True ;

      if ( 0 )
	{
	  fprintf(stderr,"button released = %d\n", zmw_button_released()) ;
	  fprintf(stderr,"CN = %d\n", ZMW_CALL_NUMBER) ;
	  fprintf(stderr,"EI = %d\n", ZMW_EVENT_IN) ;
	  fprintf(stderr,"EIM = %d\n", ZMW_EVENT_IN_MASKED) ;
	  fprintf(stderr,"SS = %d\n", ZMW_SIZE_SENSIBLE) ;
	}
      /* Commented the 08/10/2003, because it doesn't allow
       * drag and drop between windows
       */
      /*
      // Do a drag to enter in a widget that does not exists
      if ( zmw_drag_to_state() == Zmw_Drag_To_Enter )
	zmw_drag_accept_set(Zmw_False) ;
      */

      if ( !zmw_name_registered(&zmw.found) ) // (:1:)
	zmw_drag_to_nothing() ;
      
      zmw_cursor_set(zmw.found.name) ;
	
      if ( zmw.run->need_dispatch && zmw.event->any.type != GDK_MOTION_NOTIFY )
      	{
         zmw.event->any.type = GDK_MOTION_NOTIFY ;
	 zmw_call_widget(fct, zmw_action_search) ;  // Needed by (:1:)  
         zmw_call_widget(fct, action) ;
         // zmw_need_repaint() ;
      	}
    }
    
    if ( zmw.debug & Zmw_Debug_Event )
    	zmw_printf("ENDCALL REAL\n") ;
}

static int time_from(struct timeval *tv)
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

 // zmw_name_unregister(&zmw.tip_displayed) ;
  zmw.nb_windows = 0 ;
  gettimeofday(&begin_time,NULL) ;
  times(&begin_tms) ;
  zmw_call_widget(fct, zmw_action_draw) ;
  times(&end_tms) ;
  zmw.run->frame_per_sec = 1000000. / time_from(&begin_time) ;
  zmw.run->frame_per_sec_cpu = 1/((end_tms.tms_utime-begin_tms.tms_utime)/(float)clk) ;

  for( tlw = gdk_window_get_toplevels() ; tlw ; tlw = g_list_next(tlw) )
    {
      for(i=0; i<zmw.nb_windows; i++)
	{
	  if ( tlw->data == zmw.windows[i].window )
	    break ;
	}
      if ( i == zmw.nb_windows )
	{
	  gdk_window_hide(tlw->data) ;
	}
    }

  if ( zmw.debug & Zmw_Debug_Draw )
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

  gdk_window_get_pointer(gdk_window_get_toplevels()->data, &x, &y, NULL) ;
  e.any.window = gdk_window_at_pointer(&x, &y) ;
  /*
   *
   */
  zmw.run->idle_time = time_from(&zmw.run->last_user_action) ;
  zmw.run->still_time = time_from(&zmw.run->last_cursor_move) ;

  /*
   * Drag and Drop
   */
  if ( (lx != x || ly != y) && e.any.window  )
    {
      zmw.event = &e ;
      zmw.x = x ;
      zmw.y = y ;
      zmw.event->any.type = GDK_MOTION_NOTIFY ;
      zmw.run->idle_time = 0 ;
      
      if ( zmw.tips_yet_displayed || zmw.still_yet_displayed )
      	zmw_need_repaint() ;
      zmw.tips_yet_displayed = Zmw_False ;
      zmw.still_yet_displayed = Zmw_False ;

      zmw_call_widget(fct, zmw_action_search) ;      
      zmw_call_widget(fct, zmw_action_dispatch_event) ; // Drag and Drop
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
      zmw.run->need_repaint = 0 ;
      zmw_draw(fct) ;
    }

  lx = x ;
  ly = y ;

  return(TRUE) ;
}

void user_action()
{
  zmw_name_unregister(&zmw.found) ;
  gettimeofday(&zmw.run->last_user_action,NULL) ;
  
  if ( zmw_name_registered(&zmw.tip_displayed) )
    {
      zmw_name_unregister(&zmw.tip_displayed) ;
      zmw_need_repaint() ;
    }
}

void zmw_unpop(GdkEvent *e)
{
  if ( e->any.window!=NULL 
       && gdk_window_get_type(e->any.window) != GDK_WINDOW_TEMP )
    {
      if ( zmw.debug & Zmw_Debug_Event )
	{
	  zmw_printf("Unpop because event on non-popup window\n") ;
	}
      zmw_window_unpop_all() ;
    }
}  


void event_handler(GdkEvent *e, gpointer o)
{
  void (*fct)() = o ;
  GdkEvent *ee ;
  GdkWindow *w ;
  
  zmw.event_saved = *e ;

#if GLIB_MAJOR_VERSION > 1
  /* I want the release event on the real window and not
   * in the window where the button was pressed
   */
  if ( e->type == GDK_BUTTON_RELEASE )
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
#endif
  if ( e->type == GDK_KEY_PRESS )
    {
      e->any.window = gdk_window_at_pointer(&zmw.x, &zmw.y) ;      
    }

  zmw.event = e ;
  zmw.activated = Zmw_False ;
  zmw.changed = Zmw_False ;
  zmw.child_activated = Zmw_False ;
  zmw.remove_event = Zmw_False ;
  zmw.next_is_transient = Zmw_False ;
  
  if ( zmw_handle_selection(e) )
  {
   	zmw_selection_clear() ;
  	zmw_need_repaint() ;
  }
  switch( e->type )
    {
    case GDK_MOTION_NOTIFY:
      zmw.x = e->motion.x ;
      zmw.y = e->motion.y ;
      zmw.x_root = e->motion.x_root ;
      zmw.y_root = e->motion.y_root ;
      break ;
    case GDK_2BUTTON_PRESS:
    case GDK_3BUTTON_PRESS:
    case GDK_BUTTON_PRESS:
      zmw.x = e->button.x ;
      zmw.y = e->button.y ;
      zmw.x_root = e->button.x_root ;
      zmw.y_root = e->button.y_root ;
      break ;
    case GDK_BUTTON_RELEASE:
      zmw.x = e->button.x ;
      zmw.y = e->button.y ;
      zmw.x_root = e->button.x_root ;
      zmw.y_root = e->button.y_root ;
      break ;
    case GDK_ENTER_NOTIFY:
    case GDK_LEAVE_NOTIFY:
      zmw.x = e->crossing.x ;
      zmw.y = e->crossing.y ;
      zmw.x_root = e->crossing.x_root ;
      zmw.y_root = e->crossing.y_root ;
      break ;
    case GDK_KEY_PRESS:
      zmw.key_pressed = Zmw_True ;
      zmw.still_yet_displayed = Zmw_False ;
      zmw.event_key = *e ;
      if ( zmw.event_key.key.keyval == GDK_Control_L
      	|| zmw.event_key.key.keyval == GDK_Control_R )
      	zmw.event_key.key.state |= GDK_CONTROL_MASK ;
      if ( zmw.event_key.key.keyval == GDK_Shift_L
      	|| zmw.event_key.key.keyval == GDK_Shift_R )
      	zmw.event_key.key.state |= GDK_SHIFT_MASK ;
       if ( zmw.event_key.key.keyval == GDK_Meta_L
      	|| zmw.event_key.key.keyval == GDK_Meta_R )
      	zmw.event_key.key.state |= GDK_MOD1_MASK ;
       if ( zmw.event_key.key.keyval == GDK_Alt_L
      	|| zmw.event_key.key.keyval == GDK_Alt_R )
      	zmw.event_key.key.state |= GDK_MOD2_MASK ;
      break ;
    case GDK_KEY_RELEASE:
      zmw.key_pressed = Zmw_False ;
      zmw.still_yet_displayed = Zmw_False ;
      break ;
    default:
      break ;
    }

  switch( e->type )
    {
    case GDK_MOTION_NOTIFY:
      gettimeofday(&zmw.run->last_cursor_move,NULL) ;
      if ( zmw.debug & Zmw_Debug_Event )
	zmw_printf("**** EVENT **** GDK_MOTION_NOTIFY\n") ;

      ee = gdk_event_peek() ;
      if ( ee && ee->type == GDK_MOTION_NOTIFY )
	return ;

      zmw_call_widget(fct, zmw_action_dispatch_event) ;

      if ( e->motion.state != 0 )
	zmw_need_repaint() ; /* for drag */

      user_action() ;
      zmw.run->last_user_action = zmw.run->last_cursor_move ;
      break ;
    case GDK_EXPOSE:
      if ( zmw.debug & Zmw_Debug_Event )
	zmw_printf("**** EVENT **** GDK_EXPOSE\n") ;
      zmw_need_repaint() ;
      return ; /* 8/10/2003 do waste less CPU and see less debug garbage */
      break ;
    case GDK_KEY_PRESS:
      if ( zmw.debug & Zmw_Debug_Event )
	zmw_printf("**** EVENT **** KEY_PRESS\n") ;
      if ( zmw.event->type == GDK_KEY_PRESS
	   && ( zmw.event->key.state & GDK_CONTROL_MASK )
	   &&  zmw.event->key.keyval == GDK_F1
	   )
	   {
		zmw.debug ^= Zmw_Debug_Window ;	
		zmw_need_repaint() ;
	   }
      zmw_unpop(e) ;
      zmw_accelerator_init() ;
      zmw_call_widget(fct, zmw_action_dispatch_accelerator) ;

    case GDK_2BUTTON_PRESS:
    case GDK_3BUTTON_PRESS:
    case GDK_BUTTON_PRESS:
    case GDK_BUTTON_RELEASE:
      user_action() ;

      if ( (zmw.debug & Zmw_Debug_Event) && e->type != GDK_KEY_PRESS )
	zmw_printf("**** EVENT **** BUTTON! %s on window %p\n"
		   , e->type == GDK_BUTTON_RELEASE ? "release" : "press"
		   , e->any.window
		   ) ;
      zmw_unpop(e) ;

      zmw_call_widget(fct, zmw_action_dispatch_event) ;
      /* You always make repaint because a button release
       * may also change the display state : button press on
       * menu button and release on a menu item.
       * The menu item may be a sub menu and so it is not activated...
       */
      // if ( zmw.event->type == GDK_NOTHING ) commented 2003/10/27
      zmw_need_repaint() ;
      
      if ( zmw.event_saved.type == GDK_BUTTON_RELEASE )
	{
	  zmw_event_button_release() ;
	}

      if ( e->type == GDK_BUTTON_RELEASE )
	zmw_drag_cancel() ; // In order to stop drag when nobody accept it
      break ;
    case GDK_ENTER_NOTIFY:
      if ( zmw.debug & Zmw_Debug_Event )
	zmw_printf("**** EVENT **** ENTER!\n") ;
      return ; /* 2003/10/27 to waste less CPU and see less debug garbage */
      break ;
    case GDK_LEAVE_NOTIFY:
      if ( zmw.debug & Zmw_Debug_Event )
	zmw_printf("**** EVENT **** LEAVE!\n") ;
      return ; /* 2003/10/27 to waste less CPU and see less debug garbage */
      break ;
    default:
      if ( zmw.debug & Zmw_Debug_Event )
	zmw_printf("**** EVENT **** ???? = %d\n", e->type) ;
      return ; /* 2003/10/8 to waste less CPU and see less debug garbage */
      break ;
    }
    /* Compute the accelerator list */
   if ( gdk_event_peek() == NULL )
   {
      zmw_accelerator_init() ;
      zmw.event->any.type = GDK_MOTION_NOTIFY ;
      zmw.activated = Zmw_False ;
      zmw.child_activated = Zmw_False ;
      zmw_call_widget(fct, zmw_action_dispatch_accelerator) ;
   }
  *e = zmw.event_saved ;
}

static void zmw_take_int_param(gint *argc, gchar ***argv, int *i, char *name
			      , int *value)
{
  if ( *i<*argc && strncmp((*argv)[*i], name, strlen(name)) == 0 )
    {
      *value = atoi((*argv)[*i] + strlen(name) ) ;
      memmove(&(*argv)[*i], &(*argv)[*i+1], *argc - *i - 1) ;
      fprintf(stderr, "%s%d\n", name, *value) ;
      (*argc)-- ;
      (*i)-- ;
    }
}

static int zmw_global_timeout = 100 ;

void zmw_init(gint *argc, gchar ***argv)
{
  int i ;

  gdk_init(argc, argv) ;
  gdk_rgb_init() ;

  ZMW_MALLOC(zmw.run, 1) ;

  zmw.debug = Zmw_Debug_Cache_Fast | Zmw_Debug_Cache_Slow | Zmw_Debug_Name ;

  for(i=0; i<*argc; i++)
    {
      zmw_take_int_param(argc,argv,&i,"--cache_size=", &zmw.run->cache_size ) ;
      zmw_take_int_param(argc,argv,&i,"--debug="     , &zmw.debug         ) ;
      zmw_take_int_param(argc,argv,&i,"--mstimeout=" , &zmw_global_timeout) ;
    }
}

void zmw_run(void (*fct)())
{
  int i ;
  int socket ;
  Zmw_Name top_level_focus = ZMW_NAME_UNREGISTERED("Top level Focus") ;

  zmw.run->fct = fct ;

  zMw = zmw.zmw_table ;
  ZMW_USED_TO_COMPUTE_PARENT_SIZE = Zmw_True ;
  /*
  ZMW_SIZE_ALLOCATED.x = 0 ;
  ZMW_SIZE_ALLOCATED.y = 0 ;
  */

  zmw_font("6x13") ;
  zmw_x(ZMW_VALUE_UNDEFINED) ;
  zmw_y(ZMW_VALUE_UNDEFINED) ;
  zmw_width(ZMW_VALUE_UNDEFINED) ;
  zmw_height(ZMW_VALUE_UNDEFINED) ;
  ZMW_NAME = zmw.full_name ;
  ZMW_NAME[0] = '\0' ;
  ZMW_NAME_INDEX = ZMW_NAME ;
  zmw_horizontal_expand(1) ;
  zmw_vertical_expand(1) ;
  zmw_horizontal_expand(1) ;
  zmw_vertical_alignment(0) ;
  zmw_horizontal_alignment(0) ;
  zmw_padding_width(2) ;
  zmw_border_width(2) ;
  zmw_focus_width(2) ;
  zmw_focus(&top_level_focus) ;
  ZMW_EVENT_IN_FOCUS = 1 ;
  zmw_debug( !!(zmw.debug & Zmw_Debug_Widget) ) ;
  zmw_auto_resize(0) ;
  zmw_sensible(1) ;
  zmw_rgb(0.75, 0.75, 0.75) ;

  zmw.activated = Zmw_False ;
  zmw.changed = Zmw_False ;

  zmw_state_push() ;
  zmw_draw(fct) ;

  zmw.tip_displayed.why = "Tip displayed" ;
  zmw.found.why = "Found" ;
  zmw.widget_to_trace.why = "Widget to Trace" ;

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

void zmw_exit(int r)
{
  int i ;

  zmw.index_last = 2000000000 ; /* FIXME */

  zmw_call_widget(zmw.run->fct, zmw_action_clean) ;

  gdk_input_remove(zmw.run->input_http) ;

  g_idle_remove_by_data(zmw.run->fct) ;

 // for(i=0; i<ZMW_TABLE_SIZE(zmw.zmw_table[1].i.gc); i++)
 //   gdk_gc_destroy(zmw.zmw_table[1].i.gc[i]) ;


  for(i=0; i<zmw.nb_windows; i++)
    gdk_window_destroy(zmw.windows[i].window) ;
  ZMW_FREE(zmw.windows) ;


  for(i=0; i<ZMW_MAX_DEPTH; i++)
    if ( zmw.zmw_table[i].u.children )
      ZMW_FREE(zmw.zmw_table[i].u.children) ;

  zmw_font_free() ;
  zmw_name_free() ;
  zmw_cache_free() ;
  gdk_exit(r) ;
}

void zmw_stack_dump()
{
  int i, j ;

  for(i=0; &zmw.zmw_table[i] <= zMw; i++)
    {
      fprintf(stderr, "======================== STACK[%d]\n", i) ;
      fprintf(stderr, "NOT HERITABLE\n") ;
      fprintf(stderr, "%s\n", zmw_size_string(&zmw.zmw_table[i].u.size)) ;
      fprintf(stderr, "CallNum=%d NBC=%d NBMC=%d NBC0=%d name_sep=%d trans_sep=%d\n"
	      , zmw.zmw_table[i].u.call_number
	      , zmw.zmw_table[i].u.nb_of_children
	      , zmw.zmw_table[i].u.nb_of_children_max
	      , zmw.zmw_table[i].u.nb_of_children_0
	      , zmw.zmw_table[i].u.name_separator
	      , zmw.zmw_table[i].u.transient_separator
	      ) ;
      fprintf(stderr, "End of the name = %s\n", zmw.zmw_table[i].u.name) ;
      fprintf(stderr, "Name Suffix = %s\n", zmw.zmw_table[i].u.name_index) ;
      fprintf(stderr, "Type = %s\n", zmw.zmw_table[i].u.type) ;
      fprintf(stderr, ", subaction = %s", zmw_action_name_(zmw.zmw_table[i].u.subaction)) ;
      fprintf(stderr, ", External = %d", zmw.zmw_table[i].u.external_state) ;
      fprintf(stderr, "%s", zmw.zmw_table[i].u.do_not_execute_pop
	      ? ", Do Not execute Pop" : "") ;
      fprintf(stderr, "%s", zmw.zmw_table[i].u.font_copy_on_write
	      ? ", Font copy" : "") ;
      fprintf(stderr, "\n") ;
      for(j=0; j<zmw.zmw_table[i].u.nb_of_children_0; j++)
	fprintf(stderr, "\t%s\n"
		, zmw_size_string(&zmw.zmw_table[i].u.children[j])) ;
      fprintf(stderr, "-------------------- HERITABLE\n") ;
    }
}
