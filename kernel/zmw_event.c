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
/* -*- outline-minor -*- */

#include <gdk/gdkkeysyms.h>
#include <ctype.h>
#include "zmw/zmw.h"
#include "zmw/zmw_private.h"


static Zmw_Name *global_zmw_selected = NULL ;
static Zmw_Name *global_zmw_selection = NULL ;

#define C(X) if ( (X) == zmw_zmw_event_get()->type ) zmw_printf("%s\n", #X)
void zmw_event_dump()
{
  zmw_printf("EVENT_DUMP\n") ;
  zmw_debug_trace() ;
  zmw_printf("%s\n", zmw_child_string(zmw_size_get())) ;
  zmw_printf("zmw_x y = %d %d\n", zmw_zmw_x_get(), zmw_zmw_y_get()) ;
  zmw_printf("zmw_event_in_rectangle=%d\n", zmw_event_in_rectangle_get()) ;
  zmw_printf("zmw_event_in_children=%d\n", zmw_event_in_children_get()) ;
  zmw_printf("zmw_sensitived_set=%d\n", zmw_sensitived_get()) ;
  zmw_printf("zmw_focus_in() = %d\n", zmw_focus_in()) ;
  zmw_printf("zmw_key_pressed() = %d\n", zmw_key_pressed()) ;
  if ( zmw_key_pressed() )
    zmw_printf("KEY = %c(%d)\n", zmw_zmw_event_get()->key.string[0],
	       zmw_zmw_event_get()->key.string[0]) ;
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
  ZMW_FUNCTION_CALLED_OUTSIDE_ZMW_PARAMETER ;
  return( zmw_event_in_rectangle_get() ) ;
}

/*
 * True if the event should be processed
 */
Zmw_Boolean zmw_event_to_process()
{
  return( zmw_subaction_get() == Zmw_Input_Event
	  && !zmw_zmw_event_removed_get()
	  && zmw_call_number_get() > 0
	  && zmw_event_in_rectangle_get()
	  && zmw_sensitived_get()
	  ) ;
}

Zmw_Boolean zmw_state_change_allowed()
{
  return( zmw_subaction_get() == Zmw_Input_Event
	  || zmw_window_get() == NULL
	  || *zmw_window_get() == NULL
	  ) ;
}
/*
 * True if the last zmw has the focus
 */
Zmw_Boolean zmw_focus_in()
{
  return(
	 zmw_sensitived_get()
	 && zmw_focus_value_get()
	 && zmw_name_is(zmw_focus_get())
	 ) ;
}
Zmw_Boolean zmw_focus_in_by_its_parents()
{
  return(
	 // && *zmw_window_get() == zmw_zmw_event_get()->any.window // No cross-window focus
	 zmw_focus_value_get()
	 && zmw_name_is_inside(zmw_focus_get())
	 && zmw_sensitived_get()
	 ) ;
}
static int zmw_pass_through(int n)
{
  int i ;

  for(i = n ; i>0 && zmw_parent__children_get()[i].pass_through ; i--)
    {
    }
  return i>=0 ? i : 0 ;
}

static void zmw_widget_is_tested_(int n)
{
  int i ;

  if ( zmw_parent__nb_of_children_get() == 0 )
    return ;

  i = zmw_pass_through(zmw_child_number_get() - n) ;

  if ( i >= 0 )
    {
      zmw_parent__children_get()[i].sensitived = Zmw_True ;
    }
}

void zmw_widget_is_tested()
{
  //  ZMW_FUNCTION_CALLED_OUTSIDE_ZMW_PARAMETER ;
  zmw_widget_is_tested_(0) ;
}

void zmw_widget_previous_is_tested()
{
  ZMW_FUNCTION_CALLED_INSIDE_ZMW_PARAMETER ;
  zmw_widget_is_tested_(1) ;
}

/*
 * True if the key is pressed in the widget
 * Or if the widget has the focus.
 */
Zmw_Boolean zmw_key_pressed_unsensitive()
{
  return !zmw_zmw_event_removed_get()
    && zmw_focus_in_by_its_parents()
    && zmw_zmw_event_get()->type == GDK_KEY_PRESS
    ;
}
Zmw_Boolean zmw_key_pressed()
{
  zmw_widget_is_tested() ;
  return zmw_key_pressed_unsensitive() ;
}
/*
 * Same as zmw_key_pressed() but the key should not be a dead one.
 */
Zmw_Boolean zmw_key_string_unsensitive()
{
  return zmw_key_pressed_unsensitive() && zmw_zmw_event_get()->key.string[0] ;
}

/*
 * True if the cursor is pressed in the widget
 */
Zmw_Boolean zmw_button_pressed()
{
  zmw_widget_is_tested() ;
  return( zmw_event_to_process()
	  && zmw_zmw_event_get()->type == GDK_BUTTON_PRESS
	  ) ;
}

/*
 * True if the cursor is released in the widget
 */
Zmw_Boolean zmw_button_released()
{
  return( zmw_event_to_process()
	  && zmw_zmw_event_get()->type == GDK_BUTTON_RELEASE
	  ) ;
}
Zmw_Boolean zmw_button_released_anywhere()
{
  return(  !zmw_zmw_event_removed_get()
	   && zmw_action_get() == zmw_action_dispatch_event
	   && zmw_call_number_get() > 0
	   && zmw_zmw_event_get()->type == GDK_BUTTON_RELEASE ) ;
}
/*
 * True if the previous widget was activated
 * Or itself if in it is called inside the ZMW parameter.
 */
Zmw_Boolean zmw_activated()
{
  zmw_widget_is_tested() ;

  if( zmw_activated_get() )
    {
      if ( zmw_action_get() == zmw_action_dispatch_event )
	return Zmw_True ;
      if ( zmw_action_get() == zmw_action_dispatch_accelerator )
	{
	  zmw_event_remove() ; /* 2004-23-06 in order to no change widget tree */
	  return Zmw_True ;
	}
    }

  if ( zmw_pass_through_get() )
    return zmw_activated_previous() ;

  return Zmw_False ;
}
/*
 * True if the previous previous widget was activated.
 * Or the previous if it is called inside the ZMW parameter.
 */
Zmw_Boolean zmw_activated_previous()
{
  int i ;

  i = zmw_pass_through(zmw_child_number_get() - 1) ;
  return zmw_parent__children_get()[i].activated 
    &&  ( zmw_action_get() == zmw_action_dispatch_event
	  || zmw_action_get() == zmw_action_dispatch_accelerator) ;
}

/*
 * True if event_in_rectangle is true for a not "pass_through" widget
 */
Zmw_Boolean zmw_event_in_rectangle_previous()
{
  int i ;

  i = zmw_pass_through(zmw_child_number_get() - 1) ;
  return zmw_parent__children_get()[i].event_in_rectangle  ;
}

/*
 * True if the previous widget was modified (text, scrollbar)
 */
Zmw_Boolean zmw_changed()
{
  zmw_widget_is_tested() ;
  return( (zmw_changed_get() || zmw_activated_get())
	  && ( zmw_action_get() == zmw_action_dispatch_event
	       || zmw_action_get() == zmw_action_dispatch_accelerator) ) ;
}
/*
 * True is cursor in the zmw and the button pressed
 */
Zmw_Boolean zmw_cursor_in_and_pressed()
{
  zmw_widget_is_tested() ;
  return( zmw_subaction_get() == Zmw_Input_Event
	  && zmw_name_registered(global_zmw_selected) && zmw_cursor_in()
	  && zmw_zmw_event_get()->type != GDK_NOTHING
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
 
static Zmw_Name *global_zmw_cursor ;

Zmw_Boolean zmw_cursor_leave()
{
  if ( 0 )
    zmw_printf("cursor_in %s %d %d %d %s/%d\n"
	       , zmw_name_registered(global_zmw_cursor)
	       , zmw_subaction_get() == Zmw_Input_Event
	       , zmw_name_contains(global_zmw_cursor)
	       , !zmw_event_in_rectangle_get()
	       , zmw_action_name_fct()+11
	       , zmw_call_number_get()
	       ) ;

  if( zmw_subaction_get() == Zmw_Input_Event
      && zmw_name_contains(global_zmw_cursor)
      && !zmw_event_in_rectangle_get()
      )
    {
      if ( 0 )
	zmw_printf("LEAVE!\n") ;
      //	zmw.need_dispatch = Zmw_True ;
      return( Zmw_True ) ;
    }

  return( Zmw_False ) ;
}
       
 
Zmw_Boolean zmw_cursor_enter()
{
  if ( 0 )
    zmw_printf("cursor_in %s\n", zmw_name_registered(global_zmw_cursor)) ;
  
  if( zmw_subaction_get() == Zmw_Input_Event
      && zmw_event_in_rectangle_get()
      && !zmw_name_contains(global_zmw_cursor)
      )
    {
      if ( 0 )
	zmw_printf("ENTER!\n") ;
      return( Zmw_True ) ;
    }

  return( Zmw_False ) ;
}

void zmw_cursor_set(char *name)
{
  if ( (name == NULL && zmw_name_registered(global_zmw_cursor))
       || (name != NULL && !zmw_name_registered(global_zmw_cursor))
       || (name
	   && zmw_name_registered(global_zmw_cursor)
	   && strcmp(name, zmw_name_registered(global_zmw_cursor)))
       )
    zmw_need_repaint() ;

  zmw_name_register_with_name(global_zmw_cursor, name) ;
}

/*
 * In a hierarchic menu.
 *  A -> B -> C -> D -> E -> F
 * With D detached. D is always visible
 * If the user asks to see B, then B is visible
 * If the user asks to see C, then B/C is visible
 * If the user asks to see E, then E is visible
 * If the user asks to see F, then E/F is visible
 */


#define ZMW_PRINTF if (0) printf("%25s %25s/%d ", zmw_name_full, zmw_action_name_fct()+11, zmw_call_number_get()), printf

void zmw_window_update_uppers(int action)
{
  Zmw_State *s ;

  action *= Zmw_Menu_State_New ;
  
  for(s=zmw_state_get()-1; s >= zmw_zmw_table_get(); s--)
    if ( s->u.menu_state )
      {
	*s->u.menu_state |= action ;

	/* If an upper menu is detached, no need to go upper
	 * because it is yet done
	 */
	if ( *s->u.menu_state & Zmw_Menu_Is_Detached )
	  break ;
	
	ZMW_PRINTF("Update %d\n", zmw_state_get() - s) ;
      }
}
/*
 * This function is only callable inside a ZMW parameter.
 * the registered name is the zmw_if one.
 */

static Zmw_Name *global_inner_visible_menu = NULL ;

Zmw_Boolean zmw_window_is_popped_with_detached(int *detached)
{
  Zmw_Boolean visible, continue_recursion ;
  int action ;

  /*
   * So a tip can be defined after the popup window
   */
  zmw_pass_through_set(Zmw_True) ;

  /*
   * Get a pointer on the menu state.
   * It could be a user pointer or a resource pointer.
   * If a menu is called, a ressource is automaticaly created.
   * zmw_menu_state_get() is never NULL after a call to this function.
   * This function is the only one to initialize zmw_menu_state_get()
   *
   * Could be a better idea to store the state in ZMW_SIZE_MENU_STATE
   * to not use resource system so much.
   */
  if ( zmw_menu_state_get() == NULL )
    {
      if ( detached )
	zmw_menu_state_set(detached) ;
      else
	zmw_menu_state_set(zmw_name_get_pointer_on_int_resource("WindowDetached"));
    }
  zmw_widget_previous_is_tested() ;

  /*
   * Widget to unpop are known after the event dispatching.
   * So we unpop badly poped windows when computing required size,
   * so it is done before the widget drawing or event dispatching.
   */

  if ( zmw_action_get() == zmw_action_compute_required_size
       && zmw_call_number_get() == 1
       && !zmw_name_contains(global_inner_visible_menu)
       && ! (*zmw_menu_state_get() & (Zmw_Menu_Contains_A_Detached|Zmw_Menu_Is_Detached))
       )
    {
      ZMW_PRINTF("Unpop before\n") ;
      *zmw_menu_state_get() &= ~Zmw_Menu_Is_Poped ;
    }


  if ( zmw_subaction_get() == Zmw_Input_Event )
    {
      /*
       * It is here AFTER the event dispatch on children
       */
      action = 0 ;
      /*
       * Update state
       */
      ZMW_PRINTF("COMPUTE STATE current=%d\n", *zmw_menu_state_get()) ;
      *zmw_menu_state_get() = *zmw_menu_state_get()/Zmw_Menu_State_New
	| (*zmw_menu_state_get() & Zmw_Menu_Is_Detached) ;
      ZMW_PRINTF("COMPUTE STATE new=%d\n", *zmw_menu_state_get()) ;
      /*
       * Need to be made only once for all widget.
       * This should not be here.
       */
      if ( zmw_zmw_button_pressed_get()
	   && zmw_name_registered(global_inner_visible_menu))
	{
	  zmw_use_window_from_button_press(Zmw_False) ;
	}
      /*
       *
       */
      if ( *zmw_menu_state_get() & Zmw_Menu_Is_Detached )
	{
	  ZMW_PRINTF("I am detached\n") ;
	  action = Zmw_Menu_Contains_A_Detached ;
	  zmw_window_update_uppers(action) ;
	}
      /*
       *
       */
      if ( zmw_name_is(global_inner_visible_menu) )
	{
	  ZMW_PRINTF("poped\n") ;
	  action = Zmw_Menu_Is_Poped ;
	}
      /*
       *
       */
      if ( action )
	{
	  *zmw_menu_state_get() |= Zmw_Menu_Is_Poped ;
	  zmw_window_update_uppers(action) ;
	}
    }

  if ( zmw_action_get() == zmw_action_dispatch_event && zmw_subaction_get() == Zmw_Init)
    {
      /*
       * It is here BEFORE the event dispatch on children
       */
      if (
	  /*
	   * If the menu button was activated we do not
	   * want the unpop.
	   */
	  zmw_activated_previous()
	  /*
	   * The inner visible menu is modified when the cursor
	   * is over a button making it appear.
	   */
	  || (zmw_event_in_rectangle_previous() && zmw_zmw_button_pressed_get() )
	  )
	{
	  ZMW_PRINTF("Registered by Activation\n") ;
	  zmw_name_register(global_inner_visible_menu) ;
	  action = Zmw_Menu_Is_Poped ;
	  zmw_window_update_uppers(action) ;
	  *zmw_menu_state_get() |= Zmw_Menu_Is_Poped ;	  
	}
    }

  visible = *zmw_menu_state_get() & ( Zmw_Menu_Is_Poped | Zmw_Menu_Is_Detached ) ;

  continue_recursion = visible
    || zmw_event_in_children_get()
    || ( *zmw_menu_state_get() & Zmw_Menu_Contains_A_Detached ) ;

  if ( !visible && continue_recursion )
    {
      zmw_do_not_map_window_set(Zmw_True) ;
    }

  ZMW_PRINTF("v=%d c=%d p=%d %s\n"
	     , visible, continue_recursion,zmw_event_in_rectangle_previous()
	     , zmw_name_is(global_inner_visible_menu) ? "***" : ""
	     ) ;

  return continue_recursion ;
}

Zmw_Boolean zmw_window_is_popped()
{
  return(zmw_window_is_popped_with_detached(NULL) ) ;
}

void zmw_window_unpop_all()
{
  ZMW_PRINTF("Unpop all\n") ;

  if ( zmw_name_registered(global_inner_visible_menu) )
    {
      ZMW_PRINTF("Unpop all YES %s\n"
		 , zmw_name_registered(global_inner_visible_menu)) ;
      zmw_name_unregister(global_inner_visible_menu) ;
      zmw_need_repaint() ;
    }
  if ( zmw_debug_get() & Zmw_Debug_Event )
    zmw_printf("Unpop all popup window\n") ;

  zmw_use_window_from_button_press(Zmw_True) ;
}
/*
 * True if the widget is selected (button pressed in the widget
 * but not released
 */
Zmw_Boolean zmw_selected()
{
  return( zmw_name_is(global_zmw_selected) ) ;
}
Zmw_Boolean zmw_selected_by_its_parents()
{
  return( zmw_name_is_inside(global_zmw_selected) ) ;
}
/*
 * True if the widget is selected button pressed on itself or an ancestor
 */

Zmw_Boolean zmw_focused_by_its_parents()
{
  return ( zmw_name_is_inside(zmw_focus_get()) ) ;
}
/*
 * Focus remove
 */
void zmw_focus_remove()
{
  zmw_name_unregister(zmw_focus_get()) ;
}
/*
 * Remove the current event
 */
void zmw_event_remove()
{	
  if ( zmw_debug_get() & Zmw_Debug_Event )
    zmw_printf("**** EVENT **** REMOVE of %s\n", zmw_name_full) ;
  
  zmw_zmw_event_removed_set(Zmw_True) ;
}
/*
 * If your widget can have the focus, call this function
 * at the beginning of the function defining the widget class
 * because it is generally needed by Zmw_Draw and Zmw_Input subaction.
 * It is even possible that a widget has a different size
 * if it is focused.
 */
static void zmw_update_distance(int direction, int distance
				, const Zmw_Rectangle *current)
{
  int x, y, in ;

  if ( distance >= (int)zmw.near[direction].name.value )
    return ;

  if ( 0 )
    {
      GdkRegion *r ;
      int side_x, side_y ;

      gdk_window_get_origin(*zmw_window_get(), &side_x, &side_y) ; 
      x = current->x + current->width/2 - side_x ;
      y = current->y + current->height/2 - side_y  ;
      
      r = gdk_drawable_get_visible_region(*zmw_window_get()) ;
      in = gdk_region_point_in(r, x, y) ;
      gdk_region_destroy(r) ;
      
      if ( !in )
	{
	  return ; // We don't focus on non visible widget
	}
    }
  if ( zmw_debug_get() & Zmw_Debug_Navigation )
    {
      zmw_printf("YES I am nearer on %c  %d ===> %d\n"
		 , "LRUD"[direction], (int)zmw.near[direction].name.value
		 , distance) ;
    }

  zmw_string_copy(&zmw.near[direction].name.name, zmw_name_full) ;

  zmw.near[direction].name.value = (void*)distance ;	      
  zmw.near[direction].name.hash = zmw_hash_key_get() ;
  zmw.near[direction].window = *zmw_window_get() ;
  zmw.near[direction].rectangle = *current ;
}

// 1 and 2 are not symetric
// It is the distance from 2 to 1

static void zmw_interval_distance(int pos1, int length1, int pos2, int length2
				  , int *left_distance
				  , int *right_distance
				  , int *distance
				  )
{
  int middle1, end2 ;

  *left_distance  = 1000000000 ;
  *right_distance = 1000000000 ;
  *distance = 0 ;

  end2 = pos2 + length2 ;
  middle1 = pos1 + length1/2 ;

  if ( pos2 > middle1 )
    *distance = pos2 - middle1 ;
  else if ( end2 < middle1  )
    *distance = middle1 - end2 ;

  // Compute non overlapping distance
  if ( pos1 >= end2 )
    *left_distance = pos1 - end2 + 1 ;
  else if ( pos2 >= pos1 + length1 )
    *right_distance  = pos2 - (pos1 + length1) + 1 ;
}

static void zmw_interval_distance_debug()
{
  int i, pos1, length1, pos2, length2, left, right, distance ;
  char buf[50] ;

  pos1 = 14 ;

  for(length1=6; length1<=12; length1 += 6)
    for(length2=6; length2<=12; length2 += 6)
      for(pos2=pos1-length2-2; pos2<pos1+length1+3; pos2++)
	{
	  for(i=0; i<sizeof(buf); i++)
	    {
	      buf[i] = ' ' ;
	      if ( i>=pos1 && i<pos1+length1)
		buf[i] = '1' ;
	      if ( i>=pos2 && i<pos2+length2)
		{
		  if ( buf[i] == '1' )
		    buf[i] = '#' ;
		  else
		    buf[i] = '2' ;
		}
	    }
	  buf[i-1] = '\0' ;

	  zmw_interval_distance(pos1, length1, pos2, length2
				, &left, &right, &distance);
	  fprintf(stderr, "%s %10d %10d %6d\n", buf, left, right, distance) ;
	}
  exit(1) ;
}


void zmw_focusable()
{
  int left_x, right_x, left_y, right_y, dx, dy ;
  int change_window ;
  Zmw_Rectangle current ;

  //zmw_interval_distance_debug() ;

  /* Search if I am just left/up/down/right to the focused widget */
  if ( zmw_focus_get() == zmw_zmw_focus_with_cursor_get() /* Cursor in the current focus group */
       && zmw_sensitived_get()
       && !zmw_name_is(zmw_focus_get()) /* Not me */
       )
    {
      if ( zmw_subaction_get() == Zmw_Compute_Children_Allocated_Size
	   || zmw_subaction_get() == Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing)
	{
	  /* My position is known, I look if I am near to zmw_zmw_focus_with_cursor_get() */
	  current = zmw_rectangle_min(zmw_allocated_get(), zmw_clipping_get()) ;
	  if ( current.width != 0 && current.height != 0 )
	    {
	      gdk_window_get_origin(*zmw_window_get(), &dx, &dy) ; 
	      current.x += dx ;
	      current.y += dy ;

	      zmw_interval_distance(
				    zmw_zmw_focused_get()->x
				    , zmw_zmw_focused_get()->width
				    , current.x, current.width
				    , &left_x, &right_x, &dx) ;
	      zmw_interval_distance(
				    zmw_zmw_focused_get()->y
				    , zmw_zmw_focused_get()->height
				    , current.y, current.height
				    , &left_y, &right_y, &dy) ;
	      change_window = (*zmw_window_get() != zmw_zmw_raised_get()) ;
	      if ( gdk_window_get_window_type(*zmw_window_get()) == GDK_WINDOW_TEMP)
		change_window = -1 ;
	      

	      if ( zmw_debug_get() & Zmw_Debug_Navigation )
		{
		  zmw_printf("me : %s window=%p\n"
			     , zmw_rectangle_string(&current)
			     , *zmw_window_get()
			     ) ;
		  zmw_printf("focus : %s raised=%p\n"
			     , zmw_rectangle_string(zmw_zmw_focused_get())
			     , zmw_zmw_raised_get()
			     ) ;
		  zmw_printf("rx%d lx%d dx%d  ry%d ly%d dy%d\n"
			     , right_x, left_x, dx,right_y, left_y, dy ) ;
		  zmw_printf("Change window = %d\n", change_window) ;
		}


	      if ( right_x != 1000000000 )
		zmw_update_distance(1, 3*right_x + dy + change_window*10000
				    , &current
				    ) ;
	      if ( left_x != 1000000000 )
		zmw_update_distance(0, 3*left_x + dy + change_window*10000
				    , &current
				    ) ;
	      if ( right_y != 1000000000 )
		zmw_update_distance(3, dx + 3*right_y + change_window*10000
				    , &current
				    ) ;
	      if ( left_y != 1000000000 )
		zmw_update_distance(2, dx + 3*left_y + change_window*10000
				    , &current
				    ) ;
	    }
	}
    }

  if ( zmw_event_to_process() )
    {
      if ( zmw_zmw_event_get()->type == GDK_BUTTON_PRESS
	   || zmw_zmw_event_get()->type == GDK_BUTTON_RELEASE )
	{
	  zmw_name_register(zmw_focus_get()) ;
	}

      if ( zmw_zmw_event_get()->type == GDK_BUTTON_PRESS )
	zmw_need_repaint() ;
      // zmw_event_remove() ; // Removed the 2004-1-4
    }
  if ( zmw_name_is(zmw_focus_get()) )
    {
      zmw_focused_set(Zmw_True) ;

      /* Always recomputed because window may move or be resized */
      if ( zmw_focus_get() == zmw_zmw_focus_with_cursor_get() &&
	   ( zmw_subaction_get() == Zmw_Compute_Children_Allocated_Size
	     || zmw_subaction_get() == Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing
	     )
	   )
	{
	  *zmw_zmw_focused_get() = zmw_rectangle_min(zmw_allocated_get(), zmw_clipping_get()) ;
	  gdk_window_get_origin(*zmw_window_get(), &dx, &dy) ; 
	  *zmw_zmw_focused_x_get_ptr() += dx ;
	  *zmw_zmw_focused_y_get_ptr() += dy ;

	  if ( zmw_debug_get() & Zmw_Debug_Navigation )
	    {
	      zmw_printf("Set Focus to %s\n"
			 , zmw_rectangle_string(zmw_zmw_focused_get())) ;
	    }
	}
    }
}
/*
 * For copy/paste
 */
Zmw_Boolean zmw_selection_have()
{
  return ( zmw_name_is(global_zmw_selection) ) ;
}

void zmw_selection_take()
{
  zmw_name_register(global_zmw_selection) ;
}

void zmw_selection_clear()
{
  zmw_name_unregister(global_zmw_selection) ;
}


/*
 * If your zmw is activable, call zmw_activable in the Zmw_Input subaction
 */

void zmw_event_button_release()
{
  zmw_name_unregister(global_zmw_selected) ;
}

void zmw_activable()
{
  if (  zmw_event_to_process() && zmw_zmw_event_get()->type == GDK_BUTTON_PRESS )
    {
      if ( zmw_name_registered(global_zmw_selected) )
	{
	  /* A children has been selected */
	  return ;
	}
      zmw_name_register(global_zmw_selected) ;
    }
  else
    {
      if ( 
	  zmw_subaction_get() == Zmw_Input_Event
	  && !zmw_zmw_event_removed_get()
	  && zmw_zmw_event_get()
	  && zmw_zmw_event_get()->type == GDK_BUTTON_RELEASE
	  && (zmw_name_is_inside(global_inner_visible_menu) || zmw_selected())
	  )
	{
	  if ( zmw_event_in_rectangle_get() )
	    {
	      zmw_activated_set(Zmw_True) ;
	      zmw_need_repaint() ;
	    }
	}
    }
}
/*
 * Once taken by an accelerator, the key is no more
 * usable by the others.
 */

Zmw_Boolean zmw_accelerator(GdkModifierType state, int character)
{
  if (
      zmw_action_get() == zmw_action_dispatch_accelerator
      && !zmw_zmw_event_removed_get()
      && zmw_zmw_event_get()->type == GDK_KEY_PRESS
      && toupper(zmw_zmw_event_get()->key.keyval) == toupper(character)
      && zmw_zmw_event_get()->key.state == state
      )
    {
      zmw_need_repaint() ;
      zmw_event_remove() ;
      return(1) ;
    }
  return(0) ;
}


/*
 * This function should be used as a ZMW() parameter.
 * "tip_displayed" is the name of the "zmw_if(zmw_tip_visible())"
 *
 * If there is two tips on the same widget the second one
 * must search what the first one has done.
 */
Zmw_Boolean zmw_tip_visible()
{
  int v, i ;

  v = 0 ;

  zmw_pass_through_set(Zmw_True) ;
  
  /* When searching, the tip_displayed is updated
   * In the other cases, test if the tip is displayed
   */
  
  if ( zmw_zmw_table_get()[1].i.action == zmw_action_search )
    {
      /* Set tip on the first inner most widget containing "found" */
      if ( ! zmw_name_registered(zmw_zmw_tip_displayed_get()) )
	{
	  if ( zmw_event_in_rectangle_previous() )
	    {
	      zmw_name_register(zmw_zmw_tip_displayed_get()) ;
	    }
	}
    }
  else
    {
      if ( zmw_zmw_tips_yet_displayed_get() && zmw_name_registered(zmw_zmw_tip_displayed_get()) )
	{
	  if ( zmw_name_is(zmw_zmw_tip_displayed_get()) )
	    {
	      v = Zmw_True ;
	      zmw_tip_visible_set(v) ;
	    }
	  else
	    {
	      /* Search another tip before */
	      for(i = zmw_child_number_get() - 1 ;
		  i>0 && zmw_parent__children_get()[i].pass_through ; i--)
		{
		  if ( zmw_parent__children_get()[i].tip_visible )
		    {
		      v = Zmw_True ;
		      break ;
		    }
		}
	    }
	}
    }

  return(v) ;
}

/*
 *
 */

void zmw_event_initialize()
{
  zmw_name_initialize(&global_zmw_selected      , "Selected" ) ;
  zmw_name_initialize(&global_zmw_selection     , "Selection") ;
  zmw_name_initialize(&global_zmw_cursor        , "Cursor"   ) ;
  zmw_name_initialize(&global_inner_visible_menu, "Menu"     ) ;
}


/*
 *
 */

#define C(X) case X: zmw_label(#X)

void zmw_event_debug_window()
{
  static int display_zmw = 0 ;
  char *found ;
  char buf[9999] ;
  int dir ;
  
  found = zmw_name_registered(zmw_zmw_found_get()) ;

  ZMW(zmw_vbox())
    {
      zmw_check_button_int_with_label(&display_zmw, "zmw") ;
      if ( display_zmw )
	{
	  sprintf(buf, "event=%p", zmw_zmw_event_get()) ;
	  zmw_label(buf) ;	  

	  if ( zmw_zmw_event_get() )
	    switch(zmw_zmw_event_get()->type)
	      {
		C(GDK_KEY_PRESS) ;
		C(GDK_KEY_RELEASE) ;
		sprintf(buf, "state=%d code=%x string=\"%s\""
			, zmw_zmw_event_get()->key.state
			, zmw_zmw_event_get()->key.keyval
			, zmw_zmw_event_get()->key.string
			) ;
		zmw_label(buf) ;
		break ;

		C(GDK_MOTION_NOTIFY) ; break ;
		C(GDK_EXPOSE) ; break ;
		C(GDK_2BUTTON_PRESS) ; break ;
		C(GDK_3BUTTON_PRESS) ; break ;
		C(GDK_BUTTON_PRESS) ; break ;
		C(GDK_BUTTON_RELEASE) ; break ;

	      default:
		sprintf(buf, "type=%d", zmw_zmw_event_get()->type) ;
		zmw_label(buf) ;
		  
	      }

	  sprintf(buf, "x=%d y=%d "
		  , zmw_zmw_x_get()
		  , zmw_zmw_y_get()
		  ) ;
	  zmw_label(buf) ;

	  if ( zmw_name_registered(zmw_zmw_tip_displayed_get()) )
	    {
	      sprintf(buf, "TIP=%s",  zmw_name_registered(zmw_zmw_tip_displayed_get()));
	      zmw_label(buf) ;
	    }
	  sprintf(buf, "inner_menu = %s", zmw_name_registered(global_inner_visible_menu)) ;
	  zmw_label(buf) ;
	  sprintf(buf, "zmw_tips_yet_displayed = %d", zmw_zmw_tips_yet_displayed_get()) ;
	  zmw_label(buf) ;
	  sprintf(buf, "zmw_still_yet_displayed = %d",zmw_zmw_still_yet_displayed_get());
	  zmw_label(buf) ;

	  sprintf(buf,"SELECTED=%s",zmw_name_registered(global_zmw_selected));
	  zmw_label(buf) ;
	  sprintf(buf, "FOUND=%s", found) ;
	  zmw_label(buf) ;

	  for(dir=0; dir<4; dir++)
	    {
	      sprintf(buf, "%c name=%s distance=%d window=%p"
		      , "LRUD"[dir]
		      , zmw.near[dir].name.name
		      , (int)zmw.near[dir].name.value
		      , zmw.near[dir].window
		      ) ;
	      zmw_label(buf) ;
	    }
	  sprintf(buf, "focused: %s"
		  , zmw_rectangle_string(zmw_zmw_focused_get())) ;
	  zmw_label(buf) ;
	}
    }
  zmw_border_embossed_in_draw() ;
}

