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

#include "zmw/zmw.h"
#include "zmw/zmw_private.h"
#include <gdk/gdkkeysyms.h>


#define ZMW_PRINTF if(0) zmw_printf

/*
 *
 * Draw Zmw :
 *   - Compute required size for children and itself
 *   - Compute allocated size for children
 */


int zmw_printf(const char *format, ...)
{
    va_list ap;
    int i ;

    fprintf(stderr, "%s        ", zmw_name_full) ;
    va_start(ap, format);
    i = vfprintf(stderr, format, ap) ;
    va_end(ap);
    return(i) ;
}

#define ZMW_AC(X) if ( action == X ) return( #X )

const char *zmw_action_name_fct_(int (*action)(void))
{
  ZMW_AC(zmw_action_compute_required_size) ;
  ZMW_AC(zmw_action_draw) ;
  ZMW_AC(zmw_action_dispatch_event) ;
  ZMW_AC(zmw_action_dispatch_accelerator) ;
  ZMW_AC(zmw_action_search) ;
  return("zmw_action_???") ;
}

const char *zmw_action_name_fct()
{
  return zmw_action_name_fct_(zmw_action_get()) ;
}

const char *zmw_action_name_(Zmw_Subaction sa)
{
  switch( sa )
    {
    case Zmw_Init :
      return("Init") ;
    case Zmw_Compute_Required_Size :
      return("Required_Size") ;
    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing :
      return("Children_Allocated_Size_And_Pre_Drawing") ;
    case Zmw_Compute_Children_Allocated_Size :
      return("Compute_Children_Allocated_Size") ;
    case Zmw_Post_Drawing :
      return("Post_Drawing") ;
    case Zmw_Input_Event :
      return("Input_Event") ;
    case Zmw_Nothing :
      return("Nothing") ;
    case Zmw_Clean :
      return("Clean") ;
    case Zmw_Debug_Message :
      return("Message") ;
    case Zmw_Subaction_Last:
      return("Zmw_Subaction_Last!!! Bug?") ;
    }
  ZMW_ABORT ;
}
const char *zmw_action_name()
{
  return zmw_action_name_( zmw_subaction_get() ) ;
}


int zmw_event_in()
{
  /* If there is no event : no inside a widget */
  if ( zmw_zmw_event_get() == NULL )
    return Zmw_False ;

  /* A top level can be a zmw_if so there is no window */
  if ( zmw_window_get() == NULL )
    return Zmw_False ;

  /* Because of viewport, the event must be in the upper window (no clip) */
  if ( zmw_parent__window_get() == zmw_window_get()
       && ! zmw_parent__event_in_rectangle_get() )
    return Zmw_False ;

  /* The current window should be window receiving the event */
  // if ( zmw_zmw_event_get()->any.window != *zmw_window_get() )
  if ( zmw_zmw_window_get() != *zmw_window_get() )
    return Zmw_False ;

  return
    ( 
      zmw_zmw_x_get() >= zmw_allocated_x_get() - zmw_padding_width_get()
      && zmw_zmw_x_get()
       < zmw_allocated_x_get() + zmw_allocated_width_get() + zmw_padding_width_get()
      && zmw_zmw_y_get() >= zmw_allocated_y_get() - zmw_padding_width_get()
      && zmw_zmw_y_get()
      < zmw_allocated_y_get() + zmw_allocated_height_get() + zmw_padding_width_get()
     ) ;
}

/*
 * The parent manage the focus of its children
 */

void zmw_focus(Zmw_Name *focus)
{
  zmw_focus_set(focus) ;
  zmw_focus_value_set((void*) ( zmw_focus_get() == zmw_zmw_focus_with_cursor_get() ) ) ;
}




void zmw_name(const char *s)
{
  strcpy(zmw_name_get(), s) ;
  zmw_name_index_set(zmw_name_get() + strlen(zmw_name_get())) ;
  *(*zmw_name_index_get_ptr())++ = '.' ;
  zmw_name_separator_set(-1) ;
}

Zmw_Hash zmw_hash(Zmw_Hash start, const char *string)
{
  while(*string)
    {
      if ( *string != '/' )
	start = start*532483 + *string ;
      string++ ;
    }
  return start ;
}


/*
 * This string copy, from into to.
 * But if to is too small (or a NULL pointer) it is reallocated.
 */
void zmw_string_copy(char **to, const char *from)
{
  char *c ;
  int i ;

  if ( 0 )
    {
      i = strlen(from) + 1 ;
      ZMW_REALLOC(*to, i) ;
      memcpy(*to, from, i) ;
      return ;
    }

  /* The following lines are an optimization of above lines */

  if ( *to == NULL )
    {
      *to = strdup(from) ;
      return ;
    }
  
  c = *to ;
  while(*c)
    {
      *c++ = *from ;
      if ( *from == '\0' )
	return ; /* Do not release memory if shorter */
      from++ ;
    }
  if ( *from == '\0' )
    return ; /* Same size */

  i = c - *to ;
  ZMW_REALLOC(*to, i + strlen(from) + 1) ;
  strcpy(*to + i, from) ;
}


void zmw_init_widget()
{
  ZMW_PRINTF("init_widget parent callnumber %d do not make init: %d\n", zmw_parent__call_number_get(), zmw_zmw_external_do_not_make_init_get()) ;

  if ( zmw_zmw_external_do_not_make_init_get() )
    {
      /* The init was yet done by the widget calling the external compose */
      zmw_zmw_external_do_not_make_init_set(Zmw_False) ;
      return ;
    }
  /* If the previous widget was activated, remove event */
  if ( zmw_child_number_get() != -1 && (zmw_activated_get() || zmw_changed_get()) )
    {
      zmw_event_remove() ;
    }

  /* Inherit data from parent */
  zmw_parent_to_child_set(zmw_parent__parent_to_child_get()) ;

  /* Init some variables */
  zmw_call_number_set(0) ;
  (*zmw_child_number_get_ptr())++ ;
  
  zmw_size_get() = &zmw_parent__children_get()[zmw_child_number_get()] ; //for fast access
  zmw_subaction_set(Zmw_Init) ;
  zmw_menu_state_set(NULL) ;

  /* Append separator to the current name */
  (*zmw_name_separator_get_ptr())++ ;
  sprintf(zmw_name_index_get(), "%d",  zmw_name_separator_get()) ;


  /* First pass on the children, initialize their data */
  if ( zmw_parent__call_number_get() <= 1  )
    {
      /* Go here only once.
       * if zmw_parent__call_number_get() == 0 for top level widgets
       *                               it will never be 1 (no loop)
       *    zmw_parent__call_number_get() == 1 for other widgets
       *                               it can't be 0
       *
       * There is one element reserved for spare.
       * It is used to store the current state.
       */
      if ( zmw_parent__nb_of_children_get() + 1 >= zmw_parent__nb_of_children_max_get() )
	{
	  zmw_parent__nb_of_children_max_set(10 + 2*zmw_parent__nb_of_children_max_get() ) ;
	  ZMW_REALLOC(zMw[-1].u.children, zmw_parent__nb_of_children_max_get()) ;
	  zmw_size_get()= &zmw_parent__children_get()[zmw_child_number_get()] ;
	}
      (*zmw_parent__nb_of_children_get_ptr())++ ;
      
      if ( zmw_is_widget_top() )
	{
	  zmw_hash_key_set(zmw_hash(0, zmw_name_get()-1)) ;
	}
      else
	{
	  zmw_hash_key_set(zmw_hash(zmw_parent__hash_key_get(), zmw_name_get()-1)) ;
	}
      zmw_min_x_set(ZMW_VALUE_UNDEFINED) ;
      zmw_min_y_set(ZMW_VALUE_UNDEFINED) ;
      zmw_used_by_parent_set    (Zmw_True) ;
      zmw_event_in_rectangle_set(Zmw_False) ;
      zmw_event_in_children_set (Zmw_False) ;
      zmw_invisible_set         (Zmw_False) ;
      zmw_sensitived_set        (Zmw_False) ;
      zmw_do_not_map_window_set (Zmw_False) ;
      zmw_activated_set         (Zmw_False) ;
      zmw_changed_set           (Zmw_False) ;
      zmw_children_activated_set(Zmw_False) ;
      zmw_tip_visible_set       (Zmw_False) ;
      zmw_focused_set           (Zmw_False) ;
      zmw_pass_through_set      (Zmw_False) ;
      /*
       * Copy the widget current state to the next widget
       */
      zmw_cs_current_state_set(zmw_current_state_get()) ;
    }
  /*
   * If it is not the case it indicates that a new widget
   * was created on the fly when an event was received.
   * No initialization have been done for it.
   * So the program must stop.
   * We should indicates to the user how to fix its program :
   *   - Use zmw_if
   *   - Change state of the program at the begin/end of the widget tree
   *   - Put the transient widgets outside any widget.
   * Or we can modify this library in order to not enter into
   * widgets newly created.
   */
  ZMW_ASSERT(
	     zmw_is_widget_top()
	     || (zmw_action_get() == zmw_action_dispatch_event
		 && zmw_zmw_event_removed_get() )
	     || zmw_action_get() == zmw_action_clean
	     || zmw_hash_key_get() == zmw_hash(zmw_parent__hash_key_get(), zmw_name_get()-1) ) ;
  /*
   * Debug widget
   */
  if ( zmw_name_is(zmw_zmw_widget_to_trace_get()) )
    {
      zmw_debug(1) ;
      zmw_printf("%s\n", zmw_action_name()) ;
    }

  ZMW_PRINTF("init_widget fin child_number=%d\n", zmw_child_number_get()) ;
}




int zmw_action_compute_required_size()
{
  int i ;

  ZMW_PRINTF("compute_required call=%d child=%d\n", zmw_call_number_get(), zmw_child_number_get()) ;

  ZMW_EXTERNAL_HANDLING ;

  if ( (*zmw_call_number_get_ptr())++ == 0 /* && zmw_used_by_parent_get() */ )
    {
      if ( zmw_child_cache_get_size(zmw_size_get()) )
	{
	  zmw_subaction_set(Zmw_Compute_Required_Size) ;
	  zmw_state_push() ;
	  return(1) ;
	}
      /* So ZMW_SIZE is the cached value */
    }
  else
    {
      if ( zmw_zmw_event_get() )
	{
	  /*
	   * True if it is the window with the cursor.
	   * Not yet its content.
	   * It will be added in zmw_second_pass
	   */
	  zmw_event_in_children_set(
				    zmw_event_in_children_get()
				    || (*zmw_window_get() == zmw_zmw_window_get()
					&& zmw_parent__window_get() != zmw_window_get()
					)
				    ) ;
	  /*
	   * Also true if one of the child contains the window with the cursor
	   */
	  if ( !zmw_event_in_children_get() )
	    for(i=0;i<zmw_nb_of_children_get();i++)
	      zmw_event_in_children_set(zmw_event_in_children_get()
					|| zmw_child__event_in_children_get(i)
					) ;
	}

      /*
       * Asked size set required size
       * These sizes are random if !zmw_used_by_parent_get()
       */
      zmw_required_set(zmw_min_get()) ;
      zmw_horizontaly_expanded_set(zmw_cs_horizontal_expand_get()) ;
      zmw_verticaly_expanded_set(zmw_cs_vertical_expand_get()) ;
      if ( zmw_asked_width_get() != ZMW_VALUE_UNDEFINED )
	{
	  zmw_required_width_set(zmw_asked_width_get()) ;
	  zmw_horizontaly_expanded_set(Zmw_False) ; // 10/6/2004
	}
      if ( zmw_asked_height_get() != ZMW_VALUE_UNDEFINED )
	{
	  zmw_required_height_set(zmw_asked_height_get()) ;
	  zmw_verticaly_expanded_set(Zmw_False) ; // 10/6/2004
	}
      if ( zmw_asked_x_get() != ZMW_VALUE_UNDEFINED )
	  zmw_required_x_set(zmw_asked_x_get()) ;
      if ( zmw_asked_y_get() != ZMW_VALUE_UNDEFINED )
	zmw_required_y_set(zmw_asked_y_get()) ;

      /*    if ( zmw_used_by_parent_get() ) */
      zmw_child_cache_set_size(zmw_size_get()) ;
    }

  return(0) ;
}

/*
 * This function is called when the allocated
 * size for the current widget is known.
 * It compute the required size for the children.
 */

int zmw_action_first_pass()
{
  if ( zmw_parent__nb_of_children_max_get() != 0 )
    {
      /*
       * We know about sensitivity AFTER the first pass on the parent
       *   zmw_button("quit") ;
       *   if ( zmw_activated() ) ....
       */
      if ( !zmw_sensitive_get() )
	zmw_sensitived_set(Zmw_False) ;

      if ( zmw_invisible_get() )
	{
	  return(0) ;
	}
    }
  
  zmw_event_in_rectangle_set(zmw_event_in()) ;
  zmw_event_in_children_set(zmw_event_in_rectangle_get()) ;

  zmw_state_push() ;
  zmw_action_set(zmw_action_compute_required_size) ;
  return(1) ;
}

void zmw_debug_trace()
{
  zmw_printf("%s %s (%s/%d)%s%s%s\n"
	     , zmw_type_get()
	     , zmw_action_name()
	     , zmw_action_name_fct()+11
	     , zmw_call_number_get()
	     , zmw_event_to_process() ? " EtP" : ""
	     , zmw_child_number_get()>=0 && zmw_subaction_get() == Zmw_Input_Event && zmw_event_in_rectangle_get() ? " EIR" : ""
	     , zmw_child_number_get()>=0 && zmw_subaction_get()  == Zmw_Input_Event && zmw_event_in_children_get()  ? " EIC" : ""
	     ) ;
}

void zmw_stack_print()
{
  Zmw_State *s ;

  zmw_printf("current name=(%s)\n", zmw_name_full) ;
  zmw_debug_trace() ;
  zmw_printf("Call stack\n") ;
  for(s = zmw_zmw_table_get()+1 ; s <= zmw_state_get() ; s++)
    zmw_printf("%s:%d %s\n", s->u.file, s->u.line, s->u.type) ;
}

int zmw_action_draw()
{
  ZMW_EXTERNAL_HANDLING ;

  switch ( (*zmw_call_number_get_ptr())++ )
    {
    case 0:
      zmw_subaction_set(Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing) ;
      return(zmw_action_first_pass()) ;

    case 1:
      zmw_subaction_set(Zmw_Post_Drawing) ;

      if ( (zmw_debug_get() & Zmw_Debug_Draw_Cross)
	   && zmw_event_in_rectangle_get()
	   && *zmw_window_get() )
	{
	  zmw_cross_draw() ;
	}

      zmw_state_push() ;
      return(1) ;

    case 2:
      break ;
    }
  return(0) ;
}

/*
 *
 */
int zmw_action_dispatch_accelerator()
{
  ZMW_EXTERNAL_HANDLING ;

  if ( (*zmw_call_number_get_ptr())++ == 0 )
    {
      /* The following block is commented, speed up : 10% in CPU time.
       * The block was here certainly to hide a bug that was removed.
       */
      /* To remove random activation */
      /*
      for(i=0; i<zmw_state_get()->u.nb_of_children_max; i++)
	{
	  ZMW_CHILDREN[i].activated = Zmw_False ;
	  ZMW_CHILDREN[i].changed = Zmw_False ;
	}
      */
      zmw_subaction_set(Zmw_Nothing) ;
      zmw_state_push() ;
      return(1) ;
    }
  return(0) ;    
}

/*
 *
 */

int zmw_action_dispatch_event()
{
  ZMW_EXTERNAL_HANDLING ;
  /*
   * Update child_activated of its parent
   */
  if ( !zmw_is_widget_top() && (zmw_children_activated_get() || zmw_activated_get()) )
    {
      zmw_parent__children_activated_set(Zmw_True);
    }
  /*
   * Stop widget tree traversal if a widget has been activated.
   * If it is not done, size in cache are no more valid.
   * I think the tree traversal should be stoped, it has
   * no more any meaning because size are incorrect.
   */
  if ( zmw_activated_get() || zmw_changed_get() )
    {
      zmw_event_remove() ;  
    }
  if ( zmw_zmw_event_removed_get() )
    {
      (*zmw_call_number_get_ptr())++ ;
      return 0 ;
    }
  
  switch ( (*zmw_call_number_get_ptr())++ )
    {
    case 0:
      zmw_subaction_set(Zmw_Compute_Children_Allocated_Size) ;
      return zmw_action_first_pass() ;
    case 1:
      zmw_subaction_set(Zmw_Input_Event) ;
      zmw_state_push() ;
      return 1 ;
    }
  /*
   * Start widget debugging on Ctrl-F2 press
   */
  if ( zmw_zmw_event_get()->type == GDK_KEY_PRESS
       && ( zmw_zmw_event_get()->key.state & GDK_CONTROL_MASK )
       &&  zmw_zmw_event_get()->key.keyval == GDK_F2
       && zmw_event_in_rectangle_get()
       )
    {
      zmw_name_register(zmw_zmw_widget_to_trace_get()) ;
      zmw_event_remove() ;
    }

  return 0 ;
}


/*
 *
 */

int zmw_action_search()
{
  ZMW_EXTERNAL_HANDLING ;
  
  if ( ! zmw_name_registered(zmw_zmw_found_get()) )
  switch ( (*zmw_call_number_get_ptr())++ )
    {
    case 0:
      zmw_subaction_set(Zmw_Compute_Children_Allocated_Size) ;
      return(zmw_action_first_pass()) ;
    case 1:
      zmw_subaction_set(Zmw_Nothing) ;
      zmw_state_push() ;
      return(1) ;
    case 2:
      if ( zmw_event_in_rectangle_get() )
	{
	  zmw_name_register(zmw_zmw_found_get()) ;
	  zmw_zmw_focus_with_cursor_set(zmw_focus_get()) ;
	}
      break ;
    }
  else
    (*zmw_call_number_get_ptr())++ ;
  return(0) ;
}


/*
 *
 */

int zmw_action_clean()
{
  ZMW_EXTERNAL_HANDLING ;

  switch ( (*zmw_call_number_get_ptr())++ )
    {
    case 0:
      zmw_subaction_set(Zmw_Compute_Children_Allocated_Size) ;
      return(zmw_action_first_pass()) ;
    case 1:
      zmw_subaction_set(Zmw_Clean) ;
      zmw_state_push() ;
      return(1) ;
    }
  return(0) ;
}


