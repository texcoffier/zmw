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
#include "socket.h"
#include "http.h"
#include <gdk/gdkkeysyms.h>
#include <sys/time.h>
#include <sys/times.h>

static int global_key_pressed = 0 ;
static float global_frame_per_sec ;
static float global_frame_per_sec2 ;
static int global_cache_size = 0 ;
static gint global_input_http ;
static void (*global_fct)() ;

void zmw_do_nothing()
{
}

void zmw_need_repaint()
{
  zmw.need_repaint = 1 ;
}


void zmw_debug_flags()
{
  static int display_zmw = 0 ;
  
  ZMW(zmw_box_vertical())
    {
      zmw_toggle_with_label(&display_zmw, "Debug flags") ;
      if ( display_zmw )
	{
	  zmw_toggle_bits_with_label(&zmw.debug
				     , Zmw_Debug_Draw_Cross
				     , "Draw a cross on widget with event") ;
	  zmw_toggle_bits_with_label(&zmw.debug
				     , Zmw_Debug_Trace
				     , "Trace all calls") ;
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

  /*  zmw_auto_resize(1) ; */

  ZMW(zmw_window(&w, "Debug"))
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
	  zmw_debug_flags() ;

	  sprintf(tmp, "%f Real Frames per Second", global_frame_per_sec) ;
	  zmw_text(tmp) ;
	  sprintf(tmp, "%f CPU Frames per Second", global_frame_per_sec2) ;
	  zmw_text(tmp) ;
	}
    }
}

void zmw_call_widget(void (*fct)(), int (*action)())
{
  ZMW_ACTION = action ;
  zmw_cache_init(global_cache_size) ;
  if ( zmw.debug & Zmw_Debug_Window )
    {
      zmw_name("DebugWindow") ;
      debug_window() ;
    }
  zmw_name("?") ;
  (*fct)() ;  
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

  zmw_name_unregister(&zmw.tip_displayed) ;
  zmw.nb_windows = 0 ;
  gettimeofday(&begin_time,NULL) ;
  times(&begin_tms) ;
  zmw_call_widget(fct, zmw_action_draw) ;
  times(&end_tms) ;
  global_frame_per_sec = 1000000. / time_from(&begin_time) ;
  global_frame_per_sec2 = 1/((end_tms.tms_utime-begin_tms.tms_utime)/(float)clk) ;

  for( tlw = gdk_window_get_toplevels() ; tlw ; tlw = g_list_next(tlw) )
    {
      for(i=0; i<zmw.nb_windows; i++)
	if ( tlw->data == *zmw.windows[i].window )
	  break ;
      if ( i == zmw.nb_windows )
	{
	  gdk_window_hide(tlw->data) ;
	}
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
  zmw_name_init() ;
  /*
   * Drag and Drop
   */
  if ( lx != x && ly != y )
    {
      zmw.event = &e ;
      zmw.x = x ;
      zmw.y = y ;
      zmw.event->any.type = GDK_MOTION_NOTIFY ;
      zmw_call_widget(fct, zmw_action_dispatch_event) ;
    }
  /*
   * Tips
   */

  if ( lx == x && ly == y && !global_key_pressed
       && time_from(&zmw.last_cursor_move) > 1000000 )
    {
      if ( !zmw_name_registered(&zmw.found) )
	{
	  zmw_call_widget(fct, zmw_action_search) ;
	  zmw_need_repaint() ;
	}
    }
  else
    {
      if ( zmw_name_registered(&zmw.found) )
	{
	  zmw_name_unregister(&zmw.found) ;
	  zmw_need_repaint() ;
	}
    }

  if ( zmw.need_repaint )
    {
      zmw.need_repaint = 0 ;
      zmw_draw(fct) ;
    }

  lx = x ;
  ly = y ;

  return(TRUE) ;
}

void event_handler(GdkEvent *e, gpointer o)
{
  void (*fct)() = o ;
  GdkEvent *ee ;

  zmw.event = e ;
  zmw.activated = 0 ;
  zmw.event_saved = *e ;
  switch( e->type )
    {
    case GDK_MOTION_NOTIFY:
      zmw.x = e->motion.x ;
      zmw.y = e->motion.y ;
      zmw_drag_clear_to_widget() ;
      break ;
    case GDK_2BUTTON_PRESS:
    case GDK_3BUTTON_PRESS:
    case GDK_BUTTON_PRESS:
      global_key_pressed = 1 ;
      zmw.x = e->button.x ;
      zmw.y = e->button.y ;
      break ;
    case GDK_BUTTON_RELEASE:
      global_key_pressed = 0 ;
      zmw.x = e->button.x ;
      zmw.y = e->button.y ;
      break ;
    case GDK_ENTER_NOTIFY:
    case GDK_LEAVE_NOTIFY:
      zmw.x = e->crossing.x ;
      zmw.y = e->crossing.y ;
      break ;
    default:
      break ;
    }

  switch( e->type )
    {
    case GDK_MOTION_NOTIFY:
      if ( zmw.debug & Zmw_Debug_Event )
	zmw_printf("**** EVENT **** GDK_MOTION_NOTIFY\n") ;

      ee = gdk_event_peek() ;
      if ( ee && ee->type == GDK_MOTION_NOTIFY )
	return ;

      zmw_call_widget(fct, zmw_action_dispatch_event) ;

      if ( e->motion.state != 0 )
	zmw_need_repaint() ;

      gettimeofday(&zmw.last_cursor_move,NULL) ;
      break ;
    case GDK_EXPOSE:
      if ( zmw.debug & Zmw_Debug_Event )
	zmw_printf("**** EVENT **** GDK_EXPOSE\n") ;
      zmw_need_repaint() ;
      break ;
    case GDK_KEY_PRESS:
      if ( zmw.debug & Zmw_Debug_Event )
	zmw_printf("**** EVENT **** KEY_PRESS\n") ;
      if ( zmw.event->type == GDK_KEY_PRESS
	   && ( zmw.event->key.state & 4 )
	   &&  zmw.event->key.keyval == GDK_F1
	   )
	zmw.debug ^= Zmw_Debug_Window ;	

      zmw_call_widget(fct, zmw_action_dispatch_accelerator) ;

    case GDK_2BUTTON_PRESS:
    case GDK_3BUTTON_PRESS:
    case GDK_BUTTON_PRESS:
    case GDK_BUTTON_RELEASE:
      gettimeofday(&zmw.last_cursor_move,NULL) ;

      if ( zmw.debug & Zmw_Debug_Event )
	zmw_printf("**** EVENT **** BUTTON\n") ;
      if ( e->type == GDK_BUTTON_RELEASE )
	if ( gdk_window_get_type(e->any.window) != GDK_WINDOW_TEMP )
	  zmw_window_unpop_all() ;

      if ( zmw_name_registered(&zmw.tip_displayed) )
	{
	  zmw_name_unregister(&zmw.tip_displayed) ;
	  timeout(NULL) ; /* So the tip does not reappear immediatly */
	}
      zmw_call_widget(fct, zmw_action_dispatch_event) ;

      if ( zmw.event->type == GDK_NOTHING )
	zmw_need_repaint() ;
      
      if ( e->type == GDK_BUTTON_RELEASE )
	{
	  zmw_event_button_release() ;
	  zmw_drag_cancel() ;
	}
      break ;
    default:
      break ;
    }
}

void zmw_run(gint *argc, gchar ***argv, void (*fct)())
{
  int i ;
  int socket ;
  Zmw_Name top_level_focus = ZMW_NAME_UNREGISTERED("Top level Focus") ;

  gdk_init(argc, argv) ;
  gdk_rgb_init() ;

  global_fct = fct ;

  for(i=0; i<*argc; i++)
    {
      if ( strncmp((*argv)[i], "--cache_size=", 13) == 0 )
	global_cache_size = atoi((*argv)[i] + 13 ) ;
      if ( strncmp((*argv)[i], "--debug=", 8) == 0 )
	zmw.debug = atoi((*argv)[i] + 8 ) ;
    }

  fprintf(stderr, "cache size = %d\n", global_cache_size) ;

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
  zmw_debug(0) ;
  zmw_auto_resize(0) ;
  zmw_sensible(1) ;

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
  printf("socket = %d\n", i-1) ;

  global_input_http = gdk_input_add(socket
				    , GDK_INPUT_READ|GDK_INPUT_EXCEPTION
				    , (GdkInputFunction)http_connection
				    , (gpointer)fct
				    ) ;

  g_timeout_add(100, (GSourceFunc)timeout, (gpointer)fct) ;

  gdk_event_handler_set(event_handler, fct, NULL) ;
  for(;;)
    g_main_iteration(TRUE);
}

void zmw_exit(int r)
{
  int i ;

  zmw_call_widget(global_fct, zmw_action_clean) ;

  gdk_input_remove(global_input_http) ;

  g_idle_remove_by_data(global_fct) ;

  for(i=0; i<ZMW_TABLE_SIZE(zmw.zmw_table[1].i.gc); i++)
    gdk_gc_destroy(zmw.zmw_table[1].i.gc[i]) ;


  for(i=0; i<zmw.nb_windows; i++)
    gdk_window_destroy(*zmw.windows[i].window) ;
  ZMW_FREE(zmw.windows) ;


  for(i=0; i<ZMW_MAX_DEPTH; i++)
    if ( zmw.zmw_table[i].u.children )
      ZMW_FREE(zmw.zmw_table[i].u.children) ;

  zmw_font_free() ;
  zmw_name_free() ;
  gdk_exit(r) ;
}
