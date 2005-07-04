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
#include "zmw/zmw_private.h" /* Need fast access to "Zmw_State" */

/*
 * Initialise a state after its allocation
 */
void zmw_state_init(Zmw_State *s)
{
  s->u.external_state = Zmw_External_No ;
  s->u.nb_of_children_max = 0 ;
  s->u.children = NULL ;
  s->u.parent_to_child.window = NULL ;
  s->u.parent_to_child.gc = NULL ;
  s->u.menu_state = NULL ;
  // Because zmw_swap_xy swap unitialized values and it bother valgrind
  // zmw_rectangle_void(&s->u.parent_to_child.clipping) ;
}

void zmw_state_push()
{
  int p ;

  p = zmw_state_get() - zmw_zmw_table_get() ;
  if ( p == zmw.zmw_table_depth-1 )
    {
      zmw.zmw_table_depth++ ;
      ZMW_REALLOC(zmw_zmw_table_get(), zmw.zmw_table_depth) ;
      zmw_state_set(&zmw_zmw_table_get()[p]) ;
      zmw_state_init(zmw_state_get()+1) ;
    }
  /*
   * Copy the widget current state to the first child
   */
  if ( zmw_state_get()->u.nb_of_children_max == 0 )
    {
      zmw_state_get()->u.nb_of_children_max = 1 ;
      ZMW_MALLOC(zmw_state_get()->u.children, 1) ;
    }
  if ( zmw_state_get() != zmw_zmw_table_get() )
    zmw_state_get()->u.children[0].current_state = *zmw_current_state_get() ;

  zmw_state_get()[1].i = zmw_state_get()[0].i ;
    
  if ( zmw_call_number_get() <= 1 ) /* 4/6/2004 */
    zmw_nb_of_children_set(0) ;

  /*
   *
   */
  // zmw_printf("state push before GC[3] = %p\n", zmw_gc_get()[3]) ;
  (*zmw_state_get_ptr())++ ;

  zmw_x(ZMW_VALUE_UNDEFINED) ;
  zmw_y(ZMW_VALUE_UNDEFINED) ;
  zmw_width(ZMW_VALUE_UNDEFINED) ;
  zmw_height(ZMW_VALUE_UNDEFINED) ;
#if ZMW_DEBUG_STORE_WIDGET_TYPE == 0
  zmw_type_set("not compiled with ZMW_DEBUG_STORE_WIDGET_TYPE") ;
  zmw_file_set("?") ;
  zmw_line_set(-1) ;
#endif

  zmw_nb_of_children_set(0) ;
  zmw_name_separator_set(-1) ;
  zmw_name_set(zmw_state_get()[-1].u.name + strlen(zmw_state_get()[-1].u.name) + 1) ;
  strcpy(&zmw_name_get()[-1], "/.") ;	/* Doesn't inherit name */
  zmw_name_index_set(zmw_name_get() + 1) ;
  zmw_do_not_execute_pop_set(Zmw_False) ;

  /*
  zmw_menu_state_set(NULL) ; // Because of : zmw_tearoff first in a widget
  zmw_subaction_get() = Zmw_Init ; // Because of : zmw_tearoff first in a widget
  */

  zmw_child_number_set(-1) ;
  // Yes -1 because the current state is stored in the next widget.
  zmw_size_get() = zmw_parent__children_get() - 1 ;

  /*
   * no need to initialize zmw_external_state_get()´´
   * because it is always resetted to the correct value
   * zmw_external_state_set(Zmw_External_No) ;
   */
}

/*
 *
 */

void zmw_state_pop()
{
  if ( zmw_do_not_execute_pop_get() )
    {
      return ;
    }
  
  zmw_name_get()[-1] = '\0' ; // Truncate last child name
  zmw_state_get()-- ;
}

/*
 * XXX FIXME
 * This function needs to be rewrote properly to work
 * on all the possible cases.
 * The hard part are the possible use of EXTERNAL and void in the parents.
 *
 * This function is only used by zmw_window
 *
 * The invariant is that it is called to display a popup window,
 * So it CAN NOT be the first child. We go up until it is not the case
 */
Zmw_Child* zmw_widget_previous_size()
{
  int i ;
  Zmw_Child *s ;
  Zmw_State *state ;
  
  state = zmw_state_get() ;
  while ( state->u.child_number == 0  &&  state != zmw_zmw_table_get()+1 )
    {
      state-- ;
    }
  for(i = state->u.child_number - 1; i >= 0 ; i--)
    {
      s = &state[-1].u.children[i] ;
      if ( s->used_by_parent )
	return(s) ;
    }
  zmw_printf("state.Type: %s\n", state->u.type) ;
  zmw_printf("Level: %d\n", state - zmw_state_get()) ;
  zmw_printf("nb=%d i=%d\n", state->u.nb_of_children, i) ;
  ZMW_ABORT ;
}
