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

/*
 *
 */

static Zmw_Name global_zmw_drag_from = ZMW_NAME_UNREGISTERED("Drag From") ;
static Zmw_Name global_zmw_drag_to = ZMW_NAME_UNREGISTERED("Drag To") ;
static char * global_zmw_drag_data = NULL ;
static Zmw_Boolean global_zmw_drag_accepted = Zmw_False;
static Zmw_Boolean global_zmw_drag_drop = Zmw_False ;

void zmw_drag_printf(char *format, ...)
{
  va_list ap;

  if ( zmw.debug & Zmw_Debug_Drag )
    {
      zmw_printf("drag_to=%s drag_from=%s redispatch=%d accept=%d"
		 , global_zmw_drag_to.name
		 , global_zmw_drag_from.name
		 , zmw.need_dispatch 
		 , global_zmw_drag_accepted
		 ) ;
      zmw_debug_trace() ;
      
      va_start(ap, format);
      vfprintf(stderr, format, ap );
      va_end(ap);
    }
}

#define ZMW_DRAG_RETURN(V,M) ZMW1(zmw_drag_printf("Returns %s :", #V, M) ; return V ; )

void zmw_drag_debug(FILE *f)
{
  fprintf(f, "action  : %p\n", ZMW_ACTION) ;
  fprintf(f, "from    : %s\n", zmw_name_registered(&global_zmw_drag_from)) ;
  fprintf(f, "to      : %s\n", zmw_name_registered(&global_zmw_drag_to)) ;
  fprintf(f, "accepted: %d\n", global_zmw_drag_accepted) ;
  fprintf(f, "data    : %s\n", global_zmw_drag_data) ;
}

void zmw_drag_cancel()
{
  if ( zmw_name_registered(&global_zmw_drag_from)
       && global_zmw_drag_drop == Zmw_False )
    {
      zmw_name_unregister(&global_zmw_drag_from) ;
      zmw_name_unregister(&global_zmw_drag_to) ;
      
      if ( global_zmw_drag_data )
	free(global_zmw_drag_data) ;
      global_zmw_drag_data = NULL ;
      
      zmw_need_repaint() ;
      zmw.need_dispatch = Zmw_True ;
    }
}

/*
 * Return Information to the dragged zmw about the drag
 */
Zmw_Drag_From zmw_drag_from_state()
{
  zmw_widget_is_tested() ;
  
  zmw_next_widget_could_be_transient() ;
  
  /*
   * Start of a drag
   */
  if ( zmw_button_pressed() )
    {
      ZMW_ASSERT( global_zmw_drag_drop == Zmw_False ) ;
      zmw_name_register(&global_zmw_drag_from) ;
      global_zmw_drag_accepted = Zmw_False ;
      zmw_event_remove() ;
      zmw.need_dispatch = Zmw_True ;
      zmw_need_repaint() ;
      ZMW_DRAG_RETURN(Zmw_Drag_From_Begin, "Button pressed") ;
    }
  /*
   * End of a drag
   */
  
  zmw_drag_printf("global_zmw_drag_drop=%d %s name_is = %d\n",
	 global_zmw_drag_drop, zmw_action_name_fct()
	 , zmw_name_is(&global_zmw_drag_from)) ;
  if ( global_zmw_drag_drop
       && ( ZMW_ACTION == zmw_action_dispatch_event
	    || ZMW_ACTION == zmw_action_dispatch_accelerator )
       && zmw_name_is(&global_zmw_drag_from)
       )
    {
      global_zmw_drag_drop = Zmw_False ;
      zmw_drag_cancel() ;
      zmw_event_remove() ;
      ZMW_DRAG_RETURN(Zmw_Drag_From_End, "drag_drop for me") ;
    }
  /*
   * Running drag
   */
  if ( zmw_name_is(&global_zmw_drag_from) )
    {
      zmw.next_is_transient = Zmw_True ;
      ZMW_DRAG_RETURN(Zmw_Drag_From_Running, "Its me" ) ;
    }
  
  ZMW_DRAG_RETURN(Zmw_Drag_From_No, "Not me") ;  
}

/*
 * If the drag begin, the dragged zmw must set this information
 */
void zmw_drag_data_set(const char *data)
{
  if ( global_zmw_drag_data )
    abort() ;
  global_zmw_drag_data = strdup(data) ;
}

/*
 * Call this function if you can accept drop.
 *
 * Do not use cursor_enter/leave because I wanted it to be state change safe.
 */

Zmw_Drag_To zmw_drag_to_state()
{
  zmw_widget_is_tested() ;
  /*
   *
   */
  if ( !zmw_name_registered(&global_zmw_drag_from)
       || ZMW_SUBACTION != Zmw_Input_Event
       || zmw.event->type == GDK_NOTHING
   )
    ZMW_DRAG_RETURN(Zmw_Drag_To_No_Change, "no drag") ;
  /*
   * 
   */   
  if ( zmw_button_released() )
    {
      // zmw_name_unregister(&global_zmw_drag_to) ; not yet
      global_zmw_drag_drop = Zmw_True ;
      zmw_event_remove() ;
      zmw.need_dispatch = Zmw_True ;
      ZMW_DRAG_RETURN(Zmw_Drag_To_End, "Button released") ;
    }
  /*
   * 
   */
  zmw_drag_printf("name_contains %s === %d\ncursor in === %d\n",
  	zmw.found.name, zmw_name_contains(&zmw.found), zmw_cursor_in()) ;

  if ( ZMW_EVENT_IN )
  	{
	  /* The drag area contains the cursor */
	  if ( zmw_name_is(&global_zmw_drag_to) )
	    ZMW_DRAG_RETURN(Zmw_Drag_To_No_Change, "cursor stay in") ;
	  else
	    {
	      if ( zmw_name_contains(&global_zmw_drag_to) )
		ZMW_DRAG_RETURN(Zmw_Drag_To_No_Change, "into inner widget") ;
	      if ( zmw_name_registered(&global_zmw_drag_to) )
		ZMW_DRAG_RETURN(Zmw_Drag_To_No_Change, "not yet?") ;
	      zmw_event_remove() ;
	      zmw_name_register(&global_zmw_drag_to) ;
	      zmw.need_dispatch = Zmw_True ;
	      ZMW_DRAG_RETURN(Zmw_Drag_To_Enter, "Cursor enter on widget") ;
	    }
  	}
  else
    {
      /* The drag area do not contains cursor */
      if ( zmw_name_is(&global_zmw_drag_to) )
	{
	  zmw_event_remove() ;
	  zmw_name_unregister(&global_zmw_drag_to) ;
	  global_zmw_drag_accepted = Zmw_False ;
	  zmw.need_dispatch = Zmw_True ;
	  ZMW_DRAG_RETURN(Zmw_Drag_To_Leave, "Cursor go out") ;
	}
      else
	ZMW_DRAG_RETURN(Zmw_Drag_To_No_Change, "Cursor stay out") ;
    }
  
}

void zmw_drag_to_nothing()
{
  if ( zmw_name_registered(&global_zmw_drag_to) )
    {
      zmw_name_unregister(&global_zmw_drag_to) ;
      // global_zmw_drag_accepted = Zmw_False ;
      zmw.need_dispatch = Zmw_True ;
    }
}

void zmw_drag_accept_set(Zmw_Boolean bool)
{
  zmw_drag_printf("drag_to_accept = %d\n", bool) ;
  global_zmw_drag_accepted = bool;
  zmw_need_repaint() ;
}

Zmw_Boolean zmw_drag_accept_get()
{
   zmw_drag_printf("is accepted return %d\n", global_zmw_drag_accepted) ;
   return( global_zmw_drag_accepted ) ;
}

char *zmw_drag_data_get()
{
  return(global_zmw_drag_data) ;
}



void zmw_drag_debug_window()
{
  static int display_drag_state = 0 ;
  char buf[9999] ;
  
  ZMW(zmw_box_vertical())
    {
      zmw_toggle_with_label(&display_drag_state, "drag state") ;
      if ( display_drag_state )
	{
	  sprintf(buf, "FROM=%s", zmw_name_registered(&global_zmw_drag_from)) ;
	  zmw_text(buf) ;
	  sprintf(buf, "TO=%s", zmw_name_registered(&global_zmw_drag_to)) ;
	  zmw_text(buf) ;
	  sprintf(buf, "accepted=%d", global_zmw_drag_accepted) ;
	  zmw_text(buf) ;
	  sprintf(buf, "dragdrop=%d", global_zmw_drag_drop) ;
	  zmw_text(buf) ;
	  if ( global_zmw_drag_data )
	    {
	      sprintf(buf, "DATA=%20s", global_zmw_drag_data) ;
	      zmw_text(buf) ;
	    }
	}
    }
  zmw_border_embossed_in_draw() ;
}
