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

static Zmw_Name global_zmw_selected = ZMW_NAME_UNREGISTERED("Selected") ;
static Zmw_Name global_zmw_selected_next = ZMW_NAME_UNREGISTERED("Selected_next") ;
static Zmw_Name global_zmw_selection = ZMW_NAME_UNREGISTERED("Selection") ;

#define C(X) if ( (X) == zmw.event->type ) zmw_printf("%s\n", #X)
void zmw_event_dump()
{
	zmw_printf("EVENT_DUMP\n") ;
	zmw_debug_trace() ;
	zmw_printf("%s\n", zmw_size_string(&ZMW_SIZE)) ;
	zmw_printf("zmw.x = %d %d\n", zmw.x, zmw.y) ;
	zmw_printf("ZMW_EVENT_IN = %d\n", ZMW_EVENT_IN) ;
	zmw_printf("ZMW_EVENT_IN_MASKED = %d\n", ZMW_EVENT_IN_MASKED) ;
	zmw_printf("ZMW_EVENT_IN_FOCUS = %d\n", ZMW_EVENT_IN_FOCUS) ;
	zmw_printf("ZMW_SIZE_SENSIBLE = %d\n", ZMW_SIZE_SENSIBLE) ;
	zmw_printf("zmw_focus_in() = %d\n", zmw_focus_in()) ;
	zmw_printf("zmw_key_pressed() = %d\n", zmw_key_pressed()) ;
	if ( zmw_key_pressed() )
	  zmw_printf("KEY = %c(%d)\n", zmw.event->key.string[0],
		     zmw.event->key.string[0]) ;
		  C(GDK_KEY_RELEASE) ;
		  C(GDK_KEY_PRESS) ;
		  C(GDK_MOTION_NOTIFY) ;
		  C(GDK_EXPOSE) ;
		  C(GDK_2BUTTON_PRESS) ;
		  C(GDK_3BUTTON_PRESS) ;
		  C(GDK_BUTTON_PRESS) ;
		  C(GDK_BUTTON_RELEASE) ;
	zmw_printf("\n") ;
}
#undef C

/*
 * True if cursor in
 */
Zmw_Boolean zmw_cursor_in()
{
  return( ZMW_EVENT_IN && !ZMW_EVENT_IN_MASKED ) ;
  /* zmw.event->type != GDK_NOTHING */
}

/*
 * True if the event should be processed
 */
Zmw_Boolean zmw_event_to_process()
{
  return( ZMW_SUBACTION == Zmw_Input_Event
	  && ZMW_CALL_NUMBER > 0
	  && ZMW_EVENT_IN
	  && !ZMW_EVENT_IN_MASKED
	  && ZMW_SIZE_SENSIBLE
	  && ZMW_SENSIBLE
	  ) ;
}

Zmw_Boolean zmw_state_change_allowed()
{
  return( ZMW_SUBACTION == Zmw_Input_Event
	  || ZMW_WINDOW == NULL
	  ) ;
}
/*
 * True if the last zmw has the focus
 */
Zmw_Boolean zmw_focus_in()
{
  return(
	 ZMW_EVENT_IN_FOCUS
	 && !ZMW_EVENT_IN_MASKED
	 // && ZMW_WINDOW == zmw.event->any.window // No cross-window focus
	 && ZMW_SENSIBLE
	 && zmw_name_is(ZMW_FOCUS)
	 ) ;
}
Zmw_Boolean zmw_focus_in_by_its_parents()
{
  return(
	 ZMW_EVENT_IN_FOCUS
	 && !ZMW_EVENT_IN_MASKED
	 // && ZMW_WINDOW == zmw.event->any.window // No cross-window focus
	 && zmw_name_is_inside(ZMW_FOCUS)
	 && ZMW_SENSIBLE
	 ) ;
}
void zmw_widget_is_tested()
{
  if ( zMw[-1].u.nb_of_children )
    zMw[-1].u.children[zMw[-1].u.nb_of_children-1].sensible = 1 ;
//  ZMW_SIZE_SENSIBLE = Zmw_True ;
}

/*
 * True if the key is pressed in the zmw
 * Or if the zmw has the focus.
 */
Zmw_Boolean zmw_key_pressed()
{
  zmw_widget_is_tested() ;
  return( zmw_focus_in_by_its_parents()
	  && zmw.event->type == GDK_KEY_PRESS
	  ) ;
}
/*
 * True if the NON NUL key is pressed in the zmw
 * Or if the zmw has the focus.
 */
Zmw_Boolean zmw_key_string()
{
  return( zmw_focus_in()
	  && zmw.event->type == GDK_KEY_PRESS
	  && zmw.event->key.string[0]
	  ) ;
}
/*
 * True if the cursor is pressed in the zmw
 */
Zmw_Boolean zmw_button_pressed()
{
  zmw_widget_is_tested() ;
  return( zmw_event_to_process()
	  && zmw.event->type == GDK_BUTTON_PRESS
	  ) ;
}
/*
 * True if the cursor is released in the zmw
 */
Zmw_Boolean zmw_button_released()
{
  return( zmw_event_to_process()
	  && zmw.event->type == GDK_BUTTON_RELEASE
	  ) ;
}
Zmw_Boolean zmw_button_released_anywhere()
{
  return(ZMW_ACTION == zmw_action_dispatch_event
	 && ZMW_CALL_NUMBER > 0
	 && zmw.event->type == GDK_BUTTON_RELEASE ) ;
}
/*
 * True if the previous widget was activated
 */
Zmw_Boolean zmw_activated()
{
  zmw_widget_is_tested() ;
  return( zmw.activated
	  && ( ZMW_ACTION == zmw_action_dispatch_event
	       || ZMW_ACTION == zmw_action_dispatch_accelerator) ) ;
}
/*
 * True if the previous widget was modified (text, scrollbar)
 */
Zmw_Boolean zmw_changed()
{
  zmw_widget_is_tested() ;
  return( (zmw.changed || zmw.activated)
	  && ( ZMW_ACTION == zmw_action_dispatch_event
	       || ZMW_ACTION == zmw_action_dispatch_accelerator) ) ;
}
/*
 * True is cursor in the zmw and the button pressed
 */
Zmw_Boolean zmw_cursor_in_and_pressed()
{
  zmw_widget_is_tested() ;
  return( ZMW_SUBACTION == Zmw_Input_Event
	  && global_zmw_selected.name && zmw_cursor_in()
	  && zmw.event->type != GDK_NOTHING
	  ) ;
}
/*
 * Cursor enter/leave the widget.
 *
 * global_zmw_cursor : contains the last registered cursor.
 *
 * The first pass "leave" all the widgets.
 * "zmw_cursor_set" is called by zmw_call_widget.
 * We enter now all the widgets.
 */
 
static Zmw_Name global_zmw_cursor = ZMW_NAME_UNREGISTERED("Cursor") ;

Zmw_Boolean zmw_cursor_leave()
{
  if( ZMW_SUBACTION == Zmw_Input_Event
   	&& zmw_name_contains(&global_zmw_cursor)
  	 && !ZMW_EVENT_IN
	&& !ZMW_EVENT_IN_MASKED  	 )
  	 {
	   //	zmw_printf("leave %s\n", global_zmw_cursor.name) ;
		//	zmw.need_dispatch = Zmw_True ;
   	 	return( Zmw_True ) ;
  	 }

   return( Zmw_False ) ;
}
       
 
Zmw_Boolean zmw_cursor_enter()
{
  if( ZMW_SUBACTION == Zmw_Input_Event
  	&& ZMW_EVENT_IN
	&& !ZMW_EVENT_IN_MASKED
  	 && !zmw_name_contains(&global_zmw_cursor)
  	 )
  	 {
	   //	zmw_printf("enter %s\n", zmw_name_full) ;
  	 	return( Zmw_True ) ;
  	 }

   return( Zmw_False ) ;
}

void zmw_cursor_set(char *name)
{
  //	ZMW_HERE;
		//	zmw_printf("name=%s\n", name) ;
	zmw_name_register_with_name(&global_zmw_cursor, name) ;
}

/*
 * True if the popup menu is activated of visible or contains
 * a visible sub menu
 */

static Zmw_Name global_inner_visible_menu = ZMW_NAME_UNREGISTERED("Inner menu") ;

Zmw_Boolean zmw_window_is_popped_with_detached(const int *detached)
{
  zmw_next_widget_could_be_transient() ;
  if ( zmw_cursor_in_and_pressed() || zmw_activated() )
    {
      if ( zmw.debug & Zmw_Debug_Event )
	zmw_printf("Make popup appear\n") ;

      zmw_name_register(&global_inner_visible_menu) ;
      zmw.next_is_transient = Zmw_True ;
      zmw.activated = Zmw_True ;
      return( Zmw_True ) ;
    }

  if ( zmw_name_is(&global_inner_visible_menu)
       || zmw_name_next_contains(&global_inner_visible_menu)
       || zmw_window_detached(detached, Zmw_Detached_Next)
       || ZMW_ACTION == zmw_action_dispatch_accelerator
       )
    {
      if ( zmw.debug & Zmw_Debug_Event )
	zmw_printf("Popup is visible\n") ;

      zmw.next_is_transient = Zmw_True ;
      return( Zmw_True ) ;
    }
  if ( zmw.debug & Zmw_Debug_Event )
    zmw_printf("Popup is NOT visible\n") ;

  return(0) ;
}
Zmw_Boolean zmw_window_is_popped()
{
	return(zmw_window_is_popped_with_detached(NULL) ) ;
}

void zmw_window_unpop_all()
{
  if ( zmw.debug & Zmw_Debug_Event )
    zmw_printf("Unpop all popup window\n") ;
  zmw_name_unregister(&global_inner_visible_menu) ;
}
/*
 * True if the widget is selected (button pressed in the widget
 * but not released
 */
Zmw_Boolean zmw_selected()
{
  return( zmw_name_is(&global_zmw_selected) ) ;
}
Zmw_Boolean zmw_selected_by_its_parents()
{
  return( zmw_name_is_inside(&global_zmw_selected) ) ;
}
Zmw_Boolean zmw_selected_by_a_children()
{
  return( zmw_name_contains(&global_zmw_selected) ) ;
}
/*
 * True if the widget is selected button pressed on itself or an ancestor
 */
Zmw_Boolean zmw_selected_next_by_its_parents()
{
  return( zmw_name_is_inside(&global_zmw_selected_next) ) ;
}


/*
 * True if the widget has the focus
 */
Zmw_Boolean zmw_focused()
{
  return ( ZMW_FOCUS && zmw_name_is(ZMW_FOCUS) ) ;
}

Zmw_Boolean zmw_focused_by_its_parents()
{
  return ( ZMW_FOCUS && zmw_name_is_inside(ZMW_FOCUS) ) ;
}
/*
 * Focus remove
 */
void zmw_focus_remove()
{
	zmw_name_unregister(ZMW_FOCUS) ;
}
/*
 * Remove the current event
 */
void zmw_event_remove()
{	
   if ( zmw.debug & Zmw_Debug_Event )
	   zmw_printf("**** EVENT **** REMOVE of %s\n", zmw_name_full) ;
	
     zmw.remove_event = Zmw_True ;
  // zmw.event->type = GDK_NOTHING ;
}
/*
 * If your zmw can have the focus, call this function
 */
void zmw_focusable()
{
  if ( zmw_button_pressed() )
    {
      zmw_name_register(ZMW_FOCUS) ;
      zmw_need_repaint() ;
      zmw_event_remove() ;
     }
}
/*
 * For copy/paste
 */
Zmw_Boolean zmw_selection_have()
{
  return ( zmw_name_is(&global_zmw_selection) ) ;
}

void zmw_selection_take()
{
  zmw_name_register(&global_zmw_selection) ;
}

void zmw_selection_clear()
{
  zmw_name_unregister(&global_zmw_selection) ;
}


/*
 * If your zmw is activable, call this function
 */

void zmw_event_button_release()
{
  zmw_name_unregister(&global_zmw_selected) ;
}

void zmw_activable()
{
  if ( zmw_button_pressed() )
    {
      if ( global_zmw_selected.name )
	return ;
      /*
      if ( zmw_name_registered(&global_zmw_selected) )
	fprintf(stderr, "Button (%s) press 2 times without releasing\n"
		, zmw_name_full) ;
      */
      zmw_name_register(&global_zmw_selected) ;

      /* Dirty : store the name of the next widget.
       * This is used by zmw_selected_by_its_parents
       */
      ZMW_TRANSIENT_SEPARATOR++ ;
      zmw_name_of_the_transient_begin() ;
      zmw_name_register(&global_zmw_selected_next) ;
      ZMW_TRANSIENT_SEPARATOR-- ;
      zmw_name_of_the_transient_end() ;
    }
  else
  {
    
    if( 0 && ZMW_SUBACTION == Zmw_Input_Event &&zmw.event->type == GDK_BUTTON_RELEASE )
      zmw_printf("%s\n%s\n event_in_masked=%d sp=%d s=%d event_in=%d\n"
		 , zmw_name_full
		 , global_zmw_selected.name?global_zmw_selected.name:"???"
		 , ZMW_EVENT_IN_MASKED
		 , zmw_selected_by_its_parents()
		 , zmw_selected()
		 , ZMW_EVENT_IN
		 );
  				
    zmw.activated = Zmw_False ; // To de-activate child activation
    if ( 
	 ZMW_SUBACTION == Zmw_Input_Event
	 && !ZMW_EVENT_IN_MASKED
	 && zmw.event
	 && zmw.event->type == GDK_BUTTON_RELEASE
	 && ( zmw_selected_next_by_its_parents() || zmw_selected() )
	 )
      {
	if ( ZMW_EVENT_IN )
	  {
	    zmw.activated = Zmw_True ;
	    zmw_need_repaint() ;
	  }
      }
  }
}
/*
 *
 */

Zmw_Boolean zmw_accelerator(GdkModifierType state, int character)
{
  if (
      ZMW_ACTION == zmw_action_dispatch_accelerator
      && zmw.event->type == GDK_KEY_PRESS
      && toupper(zmw.event->key.keyval) == toupper(character)
      && zmw.event->key.state == state
      )
    {
      zmw_need_repaint() ;
      zmw_event_remove() ;
       return(1) ;
    }
  return(0) ;
}


/*
 *
 */
Zmw_Boolean zmw_tip_visible()
{
  int v ;

  if ( zmw_debug_message() )
    {
      http_printf(" current=(%s) tip_displayed=(%s) yet=%d "
		  , zmw_name_full, zmw.tip_displayed.name
		  , zmw.tips_yet_displayed) ;
      http_printf("[%d-%d-%d]<BR>\n"
		  , ZMW_INDEX
		  , ZMW_SIZE_INDEX
		  , zmw.index_last
		  ) ;
    }
  
  zmw_next_widget_could_be_transient() ;
  v = 0 ;
  
  /* When searching, the tip_displayed is updated
   * In the other cases, test if the tip is displayed
   */
  
   if ( zmw.zmw_table[1].i.action == zmw_action_search )
    {
    	/* Set tip on the inner most widget containing "found" */
      if ( ! zmw_name_registered(&zmw.tip_displayed) )
	{
	  if ( zmw_name_contains(&zmw.found) )
	    {
	      zmw_name_register(& zmw.tip_displayed) ;
	    }
	}
    }
    else
    {
      if ( zmw.tips_yet_displayed && zmw_name_registered(&zmw.tip_displayed) )
	{
	  if ( zmw_debug_message() )
	    {
	      http_printf(" call name_is (%s)==(%s) ? %d<BR>\n", zmw_name_full
			  , zmw.tip_displayed.name,
			  strcmp(zmw_name_full, zmw.tip_displayed.name)
			  );
	    }
	  v = zmw_name_is(&zmw.tip_displayed) ;
	}
    }
  /*
   * To avoid change of name when the tip doesn't appear.
   * This imply that the tip display EXACTLY ONE zmw widget (no more, no less)
   */
  if ( zmw_debug_message() )
    {
      http_printf(" v = %d<BR>\n", v) ;
    }
  
  if ( v )
    {
      zmw.next_is_transient = Zmw_True ;
      
      zMw[-1].u.children[zMw[-1].u.nb_of_children-1].sensible |=
	zMw[-1].u.children[zMw[-1].u.nb_of_children].sensible ;
    }
  return(v) ;
}


/*
 *
 */

#define C(X) case X: zmw_text(#X)

void zmw_event_debug_window()
{
  static int display_zmw = 0 ;
  char buf[9999] ;
  
  ZMW(zmw_box_vertical())
    {
      zmw_toggle_int_with_label(&display_zmw, "zmw") ;
      if ( display_zmw )
	{
	  sprintf(buf, "event=%p", zmw.event) ;
	  zmw_text(buf) ;	  


	  switch(zmw.event_saved.type)
		{
		  C(GDK_KEY_PRESS) ;
		  C(GDK_KEY_RELEASE) ;
		  sprintf(buf, "state=%d code=%x string=\"%s\""
			  , zmw.event_saved.key.state
			  , zmw.event_saved.key.keyval
			  , zmw.event_saved.key.string
			  ) ;
		  zmw_text(buf) ;
		  break ;

		  C(GDK_MOTION_NOTIFY) ; break ;
		  C(GDK_EXPOSE) ; break ;
		  C(GDK_2BUTTON_PRESS) ; break ;
		  C(GDK_3BUTTON_PRESS) ; break ;
		  C(GDK_BUTTON_PRESS) ; break ;
		  C(GDK_BUTTON_RELEASE) ; break ;

		default:
		  sprintf(buf, "type=%d", zmw.event_saved.type) ;
		  zmw_text(buf) ;
		  
		}

	  sprintf(buf, "x=%d y=%d activated=%d"
		  , zmw.x
		  , zmw.y
		  , zmw.activated
		  ) ;
	  zmw_text(buf) ;

	  if ( zmw_name_registered(&zmw.tip_displayed) )
	    {
	      sprintf(buf, "TIP=%s",  zmw_name_registered(&zmw.tip_displayed));
	      zmw_text(buf) ;
	    }
	  sprintf(buf, "zmw.tips_yet_displayed = %d", zmw.tips_yet_displayed) ;
	  zmw_text(buf) ;
	  sprintf(buf, "zmw.still_yet_displayed = %d",zmw.still_yet_displayed);
	  zmw_text(buf) ;

	  if ( zmw_name_registered(&global_zmw_selected) )
	    {
	      sprintf(buf, "SELECTED=%s", zmw_name_registered(&global_zmw_selected)) ;
	      zmw_text(buf) ;
	    }
	  if ( zmw_name_registered(&global_zmw_selected) )
	    {
	      sprintf(buf, "SELECTED_NEXT=%s", zmw_name_registered(&global_zmw_selected_next)) ;
	      zmw_text(buf) ;
	    }
	  if ( zmw_name_registered(&global_inner_visible_menu) )
	    {
	      sprintf(buf, "INNERMENU=%s", zmw_name_registered(&global_inner_visible_menu)) ;
	      zmw_text(buf) ;
	    }
	  if ( zmw_name_registered(&zmw.found) )
	    {
	      sprintf(buf, "FOUND=%s", zmw_name_registered(&zmw.found)) ;
	      zmw_text(buf) ;
	    }
	}
    }
  zmw_border_embossed_in_draw() ;
}

