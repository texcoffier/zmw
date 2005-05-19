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
#include <gdk/gdkkeysyms.h>


#define ZMW_PRINTF if(0) zmw_printf

/*
 *
 * Draw Zmw :
 *   - Compute required size for children and itself
 *   - Compute allocated size for children
 */


Zmw zmw ;

int zmw_printf(const char *format, ...)
{
    va_list ap;
    int i ;

    fprintf(stderr, "%s        ", zmw_name_full) ;
    va_start(ap, format);
    i = vfprintf(stderr, format, ap );
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
  return zmw_action_name_fct_(ZMW_ACTION) ;
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
  return zmw_action_name_( ZMW_SUBACTION ) ;
}

const char* zmw_size_string(const Zmw_Child *s)
{
  static char buf[200] ;

  sprintf(buf
	  ,"Min %3dx%-3d %3d,%-3d Req%3dx%-3d %3d,%-3d All%3dx%-3d %3d,%-3d INDEX=%6d %s%s%s%s%s%s%s%s"
	  ,s->min.width
	  ,s->min.height
	  ,s->min.x
	  ,s->min.y
	  ,s->required.width
	  ,s->required.height
	  ,s->required.x
	  ,s->required.y
	  ,s->allocated.width
	  ,s->allocated.height
	  ,s->allocated.x
	  ,s->allocated.y
	  ,s->hash
	  ,s->current_state.horizontal_expand ? " HE" : ""
	  ,s->current_state.vertical_expand ? " VE" : ""
	  ,s->current_state.horizontal_alignment ? "HA" : ""
	  ,s->current_state.vertical_alignment ? "VA" : ""
	  ,s->used_to_compute_parent_size ? " Used" : ""
	  ,s->invisible ? " Inv" : ""
	  ,s->sensible ? " Sens" : ""
	  ,s->pass_through ? " PT" : ""
	  ) ;
  return(buf) ;
}

int zmw_event_in()
{
  /* If there is no event : no inside a widget */
  if ( zmw.event == NULL )
    return Zmw_False ;

  /* A top level can be a zmw_if so there is no window */
  if ( ZMW_WINDOW == NULL )
    return Zmw_False ;

  /* Because of viewport, the event must be in the upper window (no clip) */
  if ( zMw[-1].u.parent_to_child.window == ZMW_WINDOW
       && ! ZMW_PARENT_SIZE.event_in_rectangle )
    return Zmw_False ;

  /* The current window should be window receiving the event */
  // if ( zmw.event->any.window != *ZMW_WINDOW )
  if ( zmw.window != *ZMW_WINDOW )
    return Zmw_False ;

  return
    ( 
      zmw.x >= ZMW_SIZE_ALLOCATED.x - ZMW_SIZE_PADDING_WIDTH
      && zmw.x
       < ZMW_SIZE_ALLOCATED.x + ZMW_SIZE_ALLOCATED.width + ZMW_SIZE_PADDING_WIDTH
      && zmw.y >= ZMW_SIZE_ALLOCATED.y - ZMW_SIZE_PADDING_WIDTH
      && zmw.y
      < ZMW_SIZE_ALLOCATED.y + ZMW_SIZE_ALLOCATED.height + ZMW_SIZE_PADDING_WIDTH
     ) ;
}

/*
 * The parent manage the focus of its children
 */

void zmw_focus(Zmw_Name *focus)
{
  ZMW_FOCUS = focus ;
  ZMW_FOCUS->value = (void*) ( ZMW_FOCUS == zmw.focus ) ;
}



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
  zmw_rectangle_void(&s->u.parent_to_child.clipping) ;
}

void zmw_state_push()
{
  int p ;

  ZMW_PRINTF("state_push\n") ;

  p = zMw - zmw.zmw_table ;
  if ( p == zmw.zmw_table_depth-1 )
    {
      ZMW_PRINTF("enlarge heap\n") ;
      zmw.zmw_table_depth++ ;
      ZMW_REALLOC(zmw.zmw_table, zmw.zmw_table_depth) ;
      zMw = &zmw.zmw_table[p] ;
      zmw_state_init(zMw+1) ;
    }
  /*
   * Copy the widget current state to the first child
   */
  if ( zMw->u.nb_of_children_max == 0 )
    {
      zMw->u.nb_of_children_max = 1 ;
      ZMW_MALLOC(ZMW_CHILDREN, 1) ;
    }
  if ( zMw != zmw.zmw_table ) // Only for the very first push (only one time)
    ZMW_CHILDREN[0].current_state = ZMW_SIZE.current_state ;

  zMw[1].i = zMw[0].i ;
    
  if ( ZMW_CALL_NUMBER <= 1 ) /* 4/6/2004 */
    ZMW_NB_OF_CHILDREN = 0 ;

  /*
   *
   */
  // zmw_printf("state push before GC[3] = %p\n", ZMW_GC[3]) ;
  zMw++ ;

  zmw_x(ZMW_VALUE_UNDEFINED) ;
  zmw_y(ZMW_VALUE_UNDEFINED) ;
  zmw_width(ZMW_VALUE_UNDEFINED) ;
  zmw_height(ZMW_VALUE_UNDEFINED) ;
#if ZMW_DEBUG_STORE_WIDGET_TYPE == 0
  ZMW_TYPE = "not compiled with ZMW_DEBUG_STORE_WIDGET_TYPE=1" ;
  ZMW_FILE = "?" ;
  ZMW_LINE = -1 ;
#endif

  ZMW_NB_OF_CHILDREN = 0 ;
  ZMW_NAME_SEPARATOR = -1 ;
  ZMW_NAME = zMw[-1].u.name + strlen(zMw[-1].u.name) + 1 ;
  strcpy(&ZMW_NAME[-1], "/.") ;	/* Doesn't inherit name */
  ZMW_NAME_INDEX = ZMW_NAME + 1 ;
  ZMW_DO_NOT_EXECUTE_POP = Zmw_False ;

  /*
  ZMW_MENU_STATE = NULL ; // Because of : zmw_tearoff first in a widget
  ZMW_SUBACTION = Zmw_Init ; // Because of : zmw_tearoff first in a widget
  */

  ZMW_CHILD_NUMBER = -1 ;

  /*
   * no need to initialize ZMW_EXTERNAL_STATE´´
   * because it is always resetted to the correct value
   * ZMW_EXTERNAL_STATE = Zmw_External_No ;
   */
}

/*
 *
 */

	 
/*
 *
 */

void zmw_state_pop()
{
  ZMW_PRINTF("state_pop\n") ;

  if ( ZMW_DO_NOT_EXECUTE_POP )
    {
      return ;
    }
  
  ZMW_NAME[-1] = '\0' ; // Truncate last child name
  zMw-- ;
}

void zmw_name(const char *s)
{
  strcpy(ZMW_NAME, s) ;
  ZMW_NAME_INDEX = ZMW_NAME + strlen(ZMW_NAME) ;
  *ZMW_NAME_INDEX++ = '.' ;
  ZMW_NAME_SEPARATOR = -1 ;
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

Zmw_Font_Family zmw_font_family_get(const char *family)
{
  Zmw_Font_Family i ;

  for(i=0; i<zmw.nb_font_families; i++)
    if ( strcmp(family, zmw.font_families[i]) == 0 )
      return i ;

  zmw.nb_font_families++ ;

  ZMW_REALLOC(zmw.font_families, zmw.nb_font_families) ;
  zmw.font_families[i] = strdup(family) ;
  return i ;
}  

Zmw_Boolean zmw_font_description_equals(const Zmw_Font_Description *f1
					, const Zmw_Font_Description *f2)
{
  return f1->size == f2->size
    && f1->weight == f2->weight
    && f1->style == f2->style
    && f1->family == f2->family ;
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
  ZMW_PRINTF("init_widget parent callnumber %d do not make init: %d\n", zMw[-1].u.call_number, zmw.external_do_not_make_init) ;

  if ( zmw.external_do_not_make_init )
    {
      /* The init was yet done by the widget calling the external compose */
      zmw.external_do_not_make_init = Zmw_False ;
      return ;
    }
  /* If the previous widget was activated, remove event */
  if ( ZMW_CHILD_NUMBER != -1 && (ZMW_SIZE_ACTIVATED || ZMW_SIZE_CHANGED) )
    {
      zmw_event_remove() ;
    }

  /* Inherit data from parent */
  zMw->u.parent_to_child = zMw[-1].u.parent_to_child ;

  /* Init some variables */
  ZMW_CALL_NUMBER = 0 ;
  ZMW_CHILD_NUMBER++ ;
  zMw->u.size = &zMw[-1].u.children[ZMW_CHILD_NUMBER] ; //for fast access
  ZMW_SUBACTION = Zmw_Init ;
  ZMW_MENU_STATE = NULL ;

  /* Append separator to the current name */
  ZMW_NAME_SEPARATOR++ ;
  sprintf(ZMW_NAME_INDEX, "%d",  ZMW_NAME_SEPARATOR) ;


  /* First pass on the children, initialize their data */
  if ( zMw[-1].u.call_number <= 1  )
    {
      /* Go here only once.
       * if zMw[-1].u.call_number == 0 for top level widgets
       *                               it will never be 1 (no loop)
       *    zMw[-1].u.call_number == 1 for other widgets
       *                               it can't be 0
       *
       * There is one element reserved for spare.
       * It is used to store the current state.
       */
      if ( zMw[-1].u.nb_of_children + 1 >= zMw[-1].u.nb_of_children_max )
	{
	  zMw[-1].u.nb_of_children_max = 10 + 2*zMw[-1].u.nb_of_children_max  ;
	  ZMW_REALLOC(zMw[-1].u.children, zMw[-1].u.nb_of_children_max) ;
	  zMw->u.size = &zMw[-1].u.children[ZMW_CHILD_NUMBER] ;
	}
      zMw[-1].u.nb_of_children++ ;
      
      if ( ZMW_WIDGET_TOP )
	{
	  ZMW_SIZE_HASH = zmw_hash(0, ZMW_NAME-1) ;
	}
      else
	{
	  ZMW_SIZE_HASH = zmw_hash(ZMW_PARENT_SIZE.hash, ZMW_NAME-1) ;
	}
      ZMW_SIZE_MIN.x                  = ZMW_VALUE_UNDEFINED ;
      ZMW_SIZE_MIN.y                  = ZMW_VALUE_UNDEFINED ;
      ZMW_USED_TO_COMPUTE_PARENT_SIZE = Zmw_True ;
      ZMW_SIZE_EVENT_IN_RECTANGLE     = Zmw_False ;
      ZMW_SIZE_EVENT_IN_CHILDREN      = Zmw_False ;
      ZMW_SIZE_INVISIBLE              = Zmw_False ;
      ZMW_SIZE_SENSIBLE               = Zmw_False ;
      ZMW_SIZE_DO_NOT_MAP_WINDOW      = Zmw_False ;
      ZMW_SIZE_ACTIVATED              = Zmw_False ;
      ZMW_SIZE_CHANGED 		      = Zmw_False ;
      ZMW_SIZE_CHILD_ACTIVATED        = Zmw_False ;
      ZMW_SIZE_TIP_VISIBLE            = Zmw_False ;
      ZMW_SIZE_FOCUSED 		      = Zmw_False ;
      ZMW_SIZE_PASS_THROUGH           = Zmw_False ;
      /*
       * Copy the widget current state to the next widget
       */
      ZMW_SIZE_CURRENT_STATE = ZMW_SIZE.current_state ;
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
	     ZMW_WIDGET_TOP
	     || (ZMW_ACTION == zmw_action_dispatch_event
		 && zmw.event_removed )
	     || ZMW_ACTION == zmw_action_clean
	     || ZMW_SIZE_HASH == zmw_hash(ZMW_PARENT_SIZE.hash, ZMW_NAME-1) ) ;
  /*
   * Debug widget
   */
  if ( zmw_name_is(&zmw.widget_to_trace) )
    {
      zmw_debug(1) ;
      zmw_printf("%s\n", zmw_action_name()) ;
    }

  ZMW_PRINTF("init_widget fin child_number=%d\n", ZMW_CHILD_NUMBER) ;
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
  
  state = zMw ;
  while ( state->u.child_number == 0  &&  state != zmw.zmw_table+1 )
    {
      state-- ;
    }
  for(i = state->u.child_number - 1; i >= 0 ; i--)
    {
      s = &state[-1].u.children[i] ;
      if ( s->used_to_compute_parent_size )
	return(s) ;
    }
  zmw_printf("state.Type: %s\n", state->u.type) ;
  zmw_printf("Level: %d\n", state - zMw) ;
  zmw_printf("nb=%d i=%d\n", state->u.nb_of_children, i) ;
  ZMW_ABORT ;
}


int zmw_action_compute_required_size()
{
  int i ;

  ZMW_PRINTF("compute_required call=%d child=%d\n", ZMW_CALL_NUMBER, ZMW_CHILD_NUMBER) ;

  ZMW_EXTERNAL_HANDLING ;

  if ( ZMW_CALL_NUMBER++ == 0 /* && ZMW_USED_TO_COMPUTE_PARENT_SIZE */ )
    {
      if ( zmw_cache_get_size(&ZMW_SIZE) )
	{
	  ZMW_SUBACTION = Zmw_Compute_Required_Size ;
	  zmw_state_push() ;
	  return(1) ;
	}
      /* So ZMW_SIZE is the cached value */
    }
  else
    {
      if ( zmw.event )
	{
	  /*
	   * True if it is the window with the cursor.
	   * Not yes its content.
	   * It will be added in zmw_second_pass
	   */
	  ZMW_SIZE_EVENT_IN_CHILDREN |= (*ZMW_WINDOW == zmw.window
			      && zMw[-1].u.parent_to_child.window != ZMW_WINDOW
			       ) ;
	  /*
	   * Also true if one of the child contains the window with the cursor
	   */
	  if ( !ZMW_SIZE_EVENT_IN_CHILDREN )
	    for(i=0;i<ZMW_NB_OF_CHILDREN;i++)
	      ZMW_SIZE_EVENT_IN_CHILDREN |= ZMW_CHILDREN[i].event_in_children ;
	}

      /*
       * Asked size set required size
       * These sizes are random if !ZMW_USED_TO_COMPUTE_PARENT_SIZE
       */
      ZMW_SIZE_REQUIRED = ZMW_SIZE_MIN ;
      ZMW_SIZE_HORIZONTAL_EXPAND = ZMW_HORIZONTAL_EXPAND ;
      ZMW_SIZE_VERTICAL_EXPAND = ZMW_VERTICAL_EXPAND ;
      if ( ZMW_ASKED.width != ZMW_VALUE_UNDEFINED )
	{
	  ZMW_SIZE_REQUIRED.width = ZMW_ASKED.width ;
	  ZMW_SIZE_HORIZONTAL_EXPAND = Zmw_False ; // 10/6/2004
	}
      if ( ZMW_ASKED.height != ZMW_VALUE_UNDEFINED )
	{
	  ZMW_SIZE_REQUIRED.height = ZMW_ASKED.height ;
	  ZMW_SIZE_VERTICAL_EXPAND = Zmw_False ; // 10/6/2004
	}
      if ( ZMW_ASKED.x != ZMW_VALUE_UNDEFINED )
	  ZMW_SIZE_REQUIRED.x = ZMW_ASKED.x ;
      if ( ZMW_ASKED.y != ZMW_VALUE_UNDEFINED )
	ZMW_SIZE_REQUIRED.y = ZMW_ASKED.y ;

      /*    if ( ZMW_USED_TO_COMPUTE_PARENT_SIZE ) */
      zmw_cache_set_size(&ZMW_SIZE) ;
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
  if ( zMw[-1].u.nb_of_children_max != 0 )
    {
      /*
       * We know about sensitivity AFTER the first pass on the parent
       *   zmw_button("quit") ;
       *   if ( zmw_activated() ) ....
       */
      if ( !ZMW_SENSIBLE )
	ZMW_SIZE_SENSIBLE = Zmw_False ;

      if ( ZMW_SIZE_INVISIBLE )
	{
	  return(0) ;
	}
    }
  
  ZMW_SIZE_EVENT_IN_RECTANGLE = zmw_event_in() ;
  ZMW_SIZE_EVENT_IN_CHILDREN = ZMW_SIZE_EVENT_IN_RECTANGLE ;

  zmw_state_push() ;
  ZMW_ACTION = zmw_action_compute_required_size ;
  return(1) ;
}

static void zmw_debug_children(Zmw_State *z)
{
  int i ;
  fprintf(stderr, "nbchild=%d", z->u.nb_of_children) ;
  for(i=0; i<z->u.nb_of_children; i++)
    {
      if ( i == z->u.nb_of_children )
	fprintf(stderr, " *") ;
      fprintf(stderr, " %d", z->u.children[i].hash) ;
    }
  fprintf(stderr, "\n") ;
}

void zmw_debug_trace()
{
  zmw_printf("%s %s (%s/%d)%s%s%s\n"
	     , ZMW_TYPE
	     , zmw_action_name()
	     , zmw_action_name_fct()+11
	     , ZMW_CALL_NUMBER
	     , zmw_event_to_process() ? " EtP" : ""
	     , ZMW_CHILD_NUMBER>=0 && ZMW_SUBACTION == Zmw_Input_Event && ZMW_SIZE_EVENT_IN_RECTANGLE ? " EIR" : ""
	     , ZMW_CHILD_NUMBER>=0 && ZMW_SUBACTION  == Zmw_Input_Event && ZMW_SIZE_EVENT_IN_CHILDREN  ? " EIC" : ""
	     ) ;
  if ( 0 )
  	{
	  zmw_debug_children(zMw) ;
	  fprintf(stderr, "PARENT CHIDREN :") ;
	  zmw_debug_children(zMw-1) ;
  	}
}

void zmw_stack_print()
{
  Zmw_State *s ;

  zmw_printf("current name=(%s)\n", zmw_name_full) ;
  zmw_debug_trace() ;
  zmw_printf("Call stack\n") ;
  for(s = zmw.zmw_table+1 ; s <= zMw ; s++)
    zmw_printf("%s:%d %s\n", s->u.file, s->u.line, s->u.type) ;
}

int zmw_action_draw()
{
  ZMW_EXTERNAL_HANDLING ;

  switch ( ZMW_CALL_NUMBER++ )
    {
    case 0:
      ZMW_SUBACTION = Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing ;
      return(zmw_action_first_pass()) ;

    case 1:
      ZMW_SUBACTION = Zmw_Post_Drawing ;

      if ( (ZMW_DEBUG & Zmw_Debug_Draw_Cross)
	   && ZMW_SIZE_EVENT_IN_RECTANGLE
	   && *ZMW_WINDOW )
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

  if ( ZMW_CALL_NUMBER++ == 0 )
    {
      /* The following block is commented, speed up : 10% in CPU time.
       * The block was here certainly to hide a bug that was removed.
       */
      /* To remove random activation */
      /*
      for(i=0; i<zMw->u.nb_of_children_max; i++)
	{
	  ZMW_CHILDREN[i].activated = Zmw_False ;
	  ZMW_CHILDREN[i].changed = Zmw_False ;
	}
      */
      ZMW_SUBACTION = Zmw_Nothing ;
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
  if ( !ZMW_WIDGET_TOP && (ZMW_SIZE_CHILD_ACTIVATED || ZMW_SIZE_ACTIVATED) )
    {
      ZMW_PARENT_SIZE.child_activated = Zmw_True;
    }
  /*
   * Stop widget tree traversal if a widget has been activated.
   * If it is not done, size in cache are no more valid.
   * I think the tree traversal should be stoped, it has
   * no more any meaning because size are incorrect.
   */
  if ( ZMW_SIZE_ACTIVATED || ZMW_SIZE_CHANGED )
    {
      zmw_event_remove() ;  
    }
  if ( zmw.event_removed )
    {
      ZMW_CALL_NUMBER++ ;
      return 0 ;
    }
  
  switch ( ZMW_CALL_NUMBER++ )
    {
    case 0:
      ZMW_SUBACTION = Zmw_Compute_Children_Allocated_Size ;
      return zmw_action_first_pass() ;
    case 1:
      ZMW_SUBACTION = Zmw_Input_Event ;
      zmw_state_push() ;
      return 1 ;
    }
  /*
   * Start widget debugging on Ctrl-F2 press
   */
  if ( zmw.event->type == GDK_KEY_PRESS
       && ( zmw.event->key.state & GDK_CONTROL_MASK )
       &&  zmw.event->key.keyval == GDK_F2
       && ZMW_SIZE_EVENT_IN_RECTANGLE
       )
    {
      zmw_name_register(&zmw.widget_to_trace) ;
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
  
  if ( ! zmw_name_registered(&zmw.found) )
  switch ( ZMW_CALL_NUMBER++ )
    {
    case 0:
      ZMW_SUBACTION = Zmw_Compute_Children_Allocated_Size ;
      return(zmw_action_first_pass()) ;
    case 1:
      ZMW_SUBACTION = Zmw_Nothing ;
      zmw_state_push() ;
      return(1) ;
    case 2:
      if ( ZMW_SIZE_EVENT_IN_RECTANGLE )
	{
	  zmw_name_register(&zmw.found) ;
	  zmw.focus = ZMW_FOCUS ;
	}
      break ;
    }
  else
    ZMW_CALL_NUMBER++ ;
  return(0) ;
}


/*
 *
 */

int zmw_action_clean()
{
  ZMW_EXTERNAL_HANDLING ;

  switch ( ZMW_CALL_NUMBER++ )
    {
    case 0:
      ZMW_SUBACTION = Zmw_Compute_Children_Allocated_Size ;
      return(zmw_action_first_pass()) ;
    case 1:
      ZMW_SUBACTION = Zmw_Clean ;
      zmw_state_push() ;
      return(1) ;
    }
  return(0) ;
}


