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

/*
 *
 */

/* The ! in the comment indicates that you can change
 * the state of the program
 */
typedef enum zmw_drag_from
  { Zmw_Drag_From_No      // You are not dragged
    , Zmw_Drag_From_Begin // ! Start of your drag, call zmw_drag_data_set()
    , Zmw_Drag_From_End   // ! End of your drag, call zmw_drag_is_accepted()
    , Zmw_Drag_From_Running // You MUST display a drag window, you may call zmw_drag_is_accepted()
  } Zmw_Drag_From ;

typedef enum zmw_drag_to
  { Zmw_Drag_To_No_Change  // No state change
    , Zmw_Drag_To_Enter    // ! A drag is now over you, call zmw_drag_to_accept
    , Zmw_Drag_To_Leave    // ! A drag leave you
    , Zmw_Drag_To_End      // ! The drag is deposited on you
  } Zmw_Drag_To ;



static Zmw_Name *global_zmw_drag_from = NULL ;
static Zmw_Name *global_zmw_drag_to = NULL ;
static char * global_zmw_drag_data = NULL ;
static Zmw_Boolean global_zmw_drag_accepted = Zmw_False;
static Zmw_Boolean global_zmw_drag_drop = Zmw_False ;

void zmw_drag_printf(char *format, ...)
{
  va_list ap;

  if ( zmw_debug_get() & Zmw_Debug_Drag )
    {
      fprintf(stderr,"%10s->%-10s %10s acpt=%d dd=%d data=%p cn=%s/%d uw=%d\t"
	      , zmw_name_registered(global_zmw_drag_from)
	      , zmw_name_registered(global_zmw_drag_to)
	      , zmw_name_full
	      , global_zmw_drag_accepted
	      , global_zmw_drag_drop
	      , global_zmw_drag_data
	      , zmw_action_name_fct()+11
	      , zmw_call_number_get()
	      , zmw_use_window_from_button_press_get()
	      ) ;
      // zmw_debug_trace() ;
      
      va_start(ap, format);
      vfprintf(stderr, format, ap) ;
      va_end(ap);
    }
}

#define ZMW_DRAG_RETURN(V) ZMW1(zmw_drag_printf("Returns %s\n", #V) ; return V ; )

void zmw_drag_debug(FILE *f)
{
  fprintf(f, "action  : %p\n", zmw_action_get()) ;
  fprintf(f, "from    : %s\n", zmw_name_registered(global_zmw_drag_from)) ;
  fprintf(f, "to      : %s\n", zmw_name_registered(global_zmw_drag_to)) ;
  fprintf(f, "accepted: %d\n", global_zmw_drag_accepted) ;
  fprintf(f, "data    : %s\n", global_zmw_drag_data) ;
}

void zmw_drag_cancel()
{
  zmw_drag_printf("drag_cancel\n") ;
  if ( zmw_name_registered(global_zmw_drag_from) )
    {
      zmw_drag_printf("drag_cancel in\n") ;
      zmw_name_unregister(global_zmw_drag_from) ;
      zmw_name_unregister(global_zmw_drag_to) ;

      zmw_use_window_from_button_press(Zmw_True) ;
      
      ZMW_FREE(global_zmw_drag_data) ;

      global_zmw_drag_drop = Zmw_False ;
      global_zmw_drag_accepted = Zmw_False ;
      
      //      zmw_need_repaint() ;
      //      zmw_need_dispatch() ;
    }
}

/*
 * Return Information to the dragged zmw about the drag.
 * This function must be called at least once outside
 * the ZMW parameter.
 *
 * This function is not reentrant.
 * It is not needed because it is not possible to drag
 * something into the object being dragged.
 */

Zmw_Drag_From zmw_drag_from_state_compute(Zmw_Boolean in_zmw_parameter)
{
  /*
   * We want to make the test always outside the ZMW parameter.
   * So we memorize the result in a static variable.
   * Attention: drag_from_started/drag_from_stopped mus be called
   * before drag_from_running.
   */
  static Zmw_Name *last = NULL ;
  static Zmw_Drag_From computed ;
  static int last_call_number = -1 ;

  zmw_name_initialize(&last, "private: drag_from") ;

  if ( in_zmw_parameter 
       || (zmw_name_is(last) && zmw_call_number_get() == last_call_number )
       )
    return computed ;
  
  zmw_name_register(last) ;
  last_call_number = zmw_call_number_get() ;
  
 if ( global_zmw_drag_drop
      && zmw_subaction_get() == Zmw_Input_Event
      && zmw_name_is(global_zmw_drag_from)
      )
    {
      /*
       * End of a drag
       */
      zmw_drag_printf("end of drag IN\n") ;
      zmw_event_remove() ;
      computed = Zmw_Drag_From_End ;
      zmw_drag_printf("end of drag OUT\n") ;
    }
 else
   if ( zmw_button_pressed() )
     {
       /*
	* Start of a drag
	*/
       zmw_drag_printf("start of drag IN\n") ;
       ZMW_ASSERT( global_zmw_drag_drop == Zmw_False ) ;
       zmw_name_register(global_zmw_drag_from) ;
       zmw_event_remove() ;
       zmw_need_dispatch() ;
       zmw_need_repaint() ;
       zmw_use_window_from_button_press(Zmw_False) ;
       computed = Zmw_Drag_From_Begin ;
       zmw_drag_printf("start of drag OUT\n") ;
     }
   else if ( zmw_name_is(global_zmw_drag_from) )
     {
       /*
	* Running drag
	*/
       computed = Zmw_Drag_From_Running ;
     }
   else
     {
       /*
	* No drag
	*/
       computed = Zmw_Drag_From_No ;
     }
 
  return computed ; 
}

Zmw_Boolean zmw_drag_from_started()
{
  return zmw_drag_from_state_compute(Zmw_False) == Zmw_Drag_From_Begin ;
}
Zmw_Boolean zmw_drag_from_stopped()
{
  return zmw_drag_from_state_compute(Zmw_False) == Zmw_Drag_From_End ;
}
Zmw_Boolean zmw_drag_from_running()
{
  return zmw_drag_from_state_compute(Zmw_True) == Zmw_Drag_From_Running ;
}



/*
 * If the drag begin, the dragged zmw must set this information
 */
void zmw_drag_data_set(const char *data)
{
  zmw_drag_printf("zmw_drag_data_set IN\n") ;
  if ( global_zmw_drag_data )
    {
      ZMW_HERE ;
      zmw_printf("Where is this bug?\n") ;
      zmw_drag_cancel() ;
    }
  global_zmw_drag_data = strdup(data) ;
  zmw_drag_printf("zmw_drag_data_set OUT\n") ;
}

/*
 * Call this function if you can accept drop.
 *
 * Do not use cursor_enter/leave because I wanted it to be state change safe.
 */

Zmw_Drag_To zmw_drag_to_state_compute()
{
  static Zmw_Name *last = NULL ;
  static Zmw_Drag_To computed ;
  static int last_call_number = -1 ;

  zmw_name_initialize(&last, "private: drag_to") ;

  if ( zmw_subaction_get() != Zmw_Input_Event
       || !zmw_name_registered(global_zmw_drag_from)
       )
    {
      zmw_drag_printf("zmw_drag_to_state_compute : Zmw_Drag_To_No_Change\n") ;
      last_call_number = -1 ;
      return Zmw_Drag_To_No_Change ;
    }

  if ( zmw_name_is(last) && last_call_number == zmw_call_number_get() )
    {
      zmw_drag_printf("zmw_drag_to_state_compute : return cached %d\n",
		      computed) ;
      return computed ;
    }

  last_call_number = zmw_call_number_get() ;
  zmw_name_register(last) ;

  zmw_widget_is_tested() ;
  /*
   *
   */
  if ( !global_zmw_drag_drop && zmw_button_released() )
    {
      // zmw_name_unregister(&global_zmw_drag_to) ; not yet
      zmw_drag_printf("drag to release IN\n") ;
      global_zmw_drag_drop = Zmw_True ;
      zmw_event_remove() ;
      zmw_need_dispatch() ;
      zmw_drag_printf("drag to release OUT\n") ;
      computed = Zmw_Drag_To_End ;
    }
  else
    {
      zmw_drag_printf("%s eir=%d\n", zmw_name_full,zmw_event_in_rectangle_get()) ;

      if ( zmw_event_in_rectangle_get() )
	{
	  /*
	   * zmw_event_in_rectangle_get() value is incorrect on dispatch_event/3
	   * The bug bug should diseapear in the future
	   * when zmw_size_get() will be removed.
	   * It is this bug that give randomness to regression test draganddrop
	   */
	  /* The drag area contains the cursor */
	  if ( zmw_name_is(global_zmw_drag_to) )
	    computed = Zmw_Drag_To_No_Change ;
	  else
	    {
	      if ( zmw_name_contains(global_zmw_drag_to) )
		computed = Zmw_Drag_To_No_Change ;
	      else if ( zmw_name_registered(global_zmw_drag_to) )
		computed = Zmw_Drag_To_No_Change ;
	      else
		{
		  zmw_drag_printf("drag to enter IN\n") ;
		  zmw_event_remove() ;
		  zmw_name_register(global_zmw_drag_to) ;
		  zmw_need_dispatch() ;
		  zmw_drag_printf("drag to enter OUT\n") ;
		  computed = Zmw_Drag_To_Enter ;
		}
	    }
	}
      else
	{
	  
	  /* The drag area does not contains cursor */
	  if ( zmw_name_is(global_zmw_drag_to) )
	    {
	      zmw_drag_printf("drag to leave IN\n") ;
	      zmw_event_remove() ;
	      zmw_name_unregister(global_zmw_drag_to) ;
	      global_zmw_drag_accepted = Zmw_False ;
	      zmw_need_dispatch() ;
	      zmw_drag_printf("drag to leave OUT\n") ;
	      computed = Zmw_Drag_To_Leave ;
	    }
	  else
	    computed = Zmw_Drag_To_No_Change ;
	}
    }
  return computed ;
}

Zmw_Boolean zmw_drag_to_enter()
{
  return zmw_drag_to_state_compute() == Zmw_Drag_To_Enter ;
}

Zmw_Boolean zmw_drag_to_leave()
{
  return zmw_drag_to_state_compute() == Zmw_Drag_To_Leave ;
}

Zmw_Boolean zmw_drag_to_dropped()
{
  return zmw_drag_to_state_compute() == Zmw_Drag_To_End ;
}

void zmw_drag_to_nothing()
{
  if ( zmw_name_registered(global_zmw_drag_to) )
    {
      zmw_name_unregister(global_zmw_drag_to) ;
      zmw_need_dispatch() ;
      // global_zmw_drag_accepted = Zmw_False ;
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
   return( global_zmw_drag_accepted ) ;
}

char *zmw_drag_data_get()
{
  return(global_zmw_drag_data) ;
}

/*
 * "*p"     is the item being displayed
 * "*current" is "NULL" if there is nothing to drag,
 *           or "**current" is the item being dragged
 *
 * old_current is here in order to stop infinite swapping.
 * You can't immediatly swap back the widget.
 */

void zmw_drag_swap(int *p, int **current)
{
  int tmp ;
  static int *old_current = NULL ;
  
  if ( zmw_drag_from_started() )
    {
      *current = p ;
      zmw_drag_data_set("Data on the column") ;
    }
  if (  zmw_drag_from_stopped() )
    {
      *current = NULL ;
    }

  if ( *current != NULL )
    {
      if ( zmw_drag_to_enter() )
	{
	  if ( p != old_current )
	    {
	      tmp = **current ;
	      **current = *p ;
	      *p = tmp ;
	      
	      old_current = *current ;
	      *current = p ;
	      
	      zmw_drag_accept_set(Zmw_True) ;
	    }
	  else
	    old_current = NULL ;
	}
    }
}

/*
 *
 */

void zmw_drag_debug_window()
{
  static int display_drag_state = 0 ;
  char buf[9999] ;
  
  ZMW(zmw_vbox())
    {
      zmw_check_button_int_with_label(&display_drag_state, "drag state") ;
      if ( display_drag_state )
	{
	  sprintf(buf, "FROM=%s", zmw_name_registered(global_zmw_drag_from)) ;
	  zmw_label(buf) ;
	  sprintf(buf, "TO=%s", zmw_name_registered(global_zmw_drag_to)) ;
	  zmw_label(buf) ;
	  sprintf(buf, "accepted=%d", global_zmw_drag_accepted) ;
	  zmw_label(buf) ;
	  sprintf(buf, "dragdrop=%d", global_zmw_drag_drop) ;
	  zmw_label(buf) ;
	  sprintf(buf, "DATA=%20s", global_zmw_drag_data) ;
	  zmw_label(buf) ;
	}
    }
  zmw_border_embossed_in_draw() ;
}

void zmw_drag_initialize()
{
  zmw_name_initialize(&global_zmw_drag_to  , "Drag to"  ) ;
  zmw_name_initialize(&global_zmw_drag_from, "Drag from") ;
}
