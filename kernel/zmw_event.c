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

#include <ctype.h>
#include "zmw.h"

static Zmw_Name global_zmw_selected = ZMW_NAME_UNREGISTERED("Selected") ;

/*
 * True if cursor in
 */
Zmw_Boolean zmw_cursor_in()
{
  return( ZMW_EVENT_IN && !ZMW_EVENT_IN_MASKED ) ;
}

/*
 * True if the event should be processed
 */
Zmw_Boolean zmw_event_to_process()
{
  return( ZMW_ACTION == zmw_action_dispatch_event
	  && ZMW_CALL_NUMBER > 0
	  && ZMW_EVENT_IN
	  && !ZMW_EVENT_IN_MASKED
	  && ZMW_SIZE_SENSIBLE
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
	 && zmw_name_is(ZMW_FOCUS)
	 ) ;
}
void zmw_widget_is_tested()
{
  if ( zMw[-1].u.nb_of_children )
    zMw[-1].u.children[zMw[-1].u.nb_of_children-1].sensible = 1 ;
}

/*
 * True if the key is pressed in the zmw
 * Or if the zmw has the focus.
 */
Zmw_Boolean zmw_key_pressed()
{
  zmw_widget_is_tested() ;
  return( zmw_focus_in()
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
 * True if the cursor is pressed and released in the zmw
 */
Zmw_Boolean zmw_activated()
{
  zmw_widget_is_tested() ;
  return( zmw.activated
	  && ( ZMW_ACTION == zmw_action_dispatch_event
	       || ZMW_ACTION == zmw_action_dispatch_accelerator) ) ;
}
/*
 * True if the anchor widget is beeing dragged
 */
Zmw_Boolean zmw_dragged()
{
  return( zmw.dragged ) ;
}
/*
 * True if the popup menu is activated of visible or contains
 * a visible sub menu
 */

static Zmw_Name global_inner_visible_menu = ZMW_NAME_UNREGISTERED("Inner menu") ;

Zmw_Boolean zmw_window_is_popped(int detached)
{

  if ( zmw_activated() )
    {
      zmw_name_register(&global_inner_visible_menu) ;
      zmw_event_remove() ;
      return( 1 ) ;
    }
  if ( zmw_name_is(&global_inner_visible_menu)
       || zmw_name_next_contains(&global_inner_visible_menu)
       || detached
       || ZMW_ACTION == zmw_action_dispatch_accelerator
       )
    {
      return(1) ;
    }
  zmw_increment_index() ;
  return(0) ;
}

void zmw_window_unpop_all()
{
  zmw_name_unregister(&global_inner_visible_menu) ;
}
/*
 * True if the zmw is selected (button pressed in the zmw
 * but not released
 */
Zmw_Boolean zmw_selected()
{
  return( zmw_name_is(&global_zmw_selected) ) ;
}
/*
 * True if the zmw has the focus
 */
Zmw_Boolean zmw_focused()
{
  return ( ZMW_FOCUS && zmw_name_is(ZMW_FOCUS) ) ;
}
/*
 * Remove the current event
 */
void zmw_event_remove()
{
  zmw.event->type = GDK_NOTHING ;
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
    }
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
      if ( zmw_name_registered(&global_zmw_selected) )
	fprintf(stderr, "Button (%s) press 2 times without releasing\n"
		, zmw_name_full) ;
      zmw_name_register(&global_zmw_selected) ;
      zmw_event_remove() ;
    }
  else
    if ( zmw_selected()
	 && ZMW_ACTION == zmw_action_dispatch_event
	 && !ZMW_EVENT_IN_MASKED
	 && zmw.event
	 && zmw.event->type == GDK_BUTTON_RELEASE
	 )
      {
	if ( ZMW_EVENT_IN )
	  {
	    zmw.activated = 1 ;
	    zmw_need_repaint() ;
	    zmw_event_remove() ;
	  }
	zmw_event_button_release() ;
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

  v = 0 ;
  if ( zmw_name_registered(&zmw.found) && ZMW_ACTION != zmw_action_search )
    {
      if ( zmw_name_registered(&zmw.tip_displayed) )
	{
	  v = zmw_name_is(&zmw.tip_displayed) ;
	}
      else
	{
	  if ( zmw_name_contains(&zmw.found) )
	    {
	      v = 1 ;
	      zmw_name_register(& zmw.tip_displayed) ;
	    }
	}
    }
  /*
   * To avoid change of name when the tip doesn't appear.
   * This imply that the tip display EXACTLY ONE zmw widget (no more, no less)
   */
  if ( v )
    {
      zmw_decrement_index() ;
      zMw[-1].u.children[zMw[-1].u.nb_of_children-1].sensible =
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
      zmw_toggle_with_label(&display_zmw, "zmw") ;
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
	  if ( zmw_name_registered(&global_zmw_selected) )
	    {
	      sprintf(buf, "SELECTED=%s", zmw_name_registered(&global_zmw_selected)) ;
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

