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

/* -*- outline-minor -*- */

#include "zmw/zmw.h"
#include <gdk/gdkkeysyms.h>

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

    fprintf(stderr, "%*s", 4*(zMw-zmw.zmw_table-1), "") ;
    va_start(ap, format);
    i = vfprintf(stderr, format, ap );
    va_end(ap);
    return(i) ;
}

#define ZMW_AC(X) if ( ZMW_ACTION == X ) return( #X )

const char *zmw_action_name_fct()
{
  ZMW_AC(zmw_action_compute_required_size) ;
  ZMW_AC(zmw_action_draw) ;
  ZMW_AC(zmw_action_dispatch_event) ;
  ZMW_AC(zmw_action_dispatch_accelerator) ;
  ZMW_AC(zmw_action_search) ;
  return("zmw_action_???") ;
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
    }
  ZMW_ABORT ;
}
const char *zmw_action_name()
{
  return zmw_action_name_( ZMW_SUBACTION ) ;
}

const char* zmw_size_string(const Zmw_Size *s)
{
  static char buf[200] ;

  sprintf(buf
	  ,"Ask %3dx%-3d %3d,%-3d Min %3dx%-3d %3d,%-3d Req%3dx%-3d %3d,%-3d All%3dx%-3d %3d,%-3d INDEX=%6d %s%s%s%s%s%s%s%s"
	  ,s->asked.width
	  ,s->asked.height
	  ,s->asked.x
	  ,s->asked.y
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
	  ,s->index
	  ,s->horizontal_expand ? " HE" : ""
	  ,s->vertical_expand ? " VE" : ""
	  ,s->horizontal_alignment ? "HA" : ""
	  ,s->vertical_alignment ? "VA" : ""
	  ,s->used_to_compute_parent_size ? " Used" : ""
	  ,s->invisible ? " Inv" : ""
	  ,s->sensible ? " Sens" : ""
	  ,s->pass_through ? " PT" : ""
	  ) ;
  return(buf) ;
}

int zmw_event_in()
{
  if ( 0 )
    {
      zmw_printf("zMw[-1].i.window = %p\n", zMw[-1].i.window) ;
      if ( zmw.event )
	zmw_printf("zmw.event->any.window = %p\n", zmw.event->any.window) ;
      zmw_printf("zmw.x = %d\n", zmw.x) ;
      zmw_printf("ZMW_SIZE_ALLOCATED.x - ZMW_PADDING_WIDTH = %d\n", ZMW_SIZE_ALLOCATED.x - ZMW_PADDING_WIDTH) ;
      zmw_printf("ZMW_SIZE_ALLOCATED.x + ZMW_SIZE_ALLOCATED.width + ZMW_PADDING_WIDTH = %d\n", ZMW_SIZE_ALLOCATED.x + ZMW_SIZE_ALLOCATED.width + ZMW_PADDING_WIDTH) ; 
    }

  /* Because of viewport, the event must be in the upper window (no clip) */
  /* 26/4/2004
  if ( zMw[-1].i.window == ZMW_WINDOW  &&  !zMw[-1].i.event_in )
    return(0) ;
  */

  if ( zMw[-1].i.window == ZMW_WINDOW  &&  !zMw[-1].u.size.event_in_rectangle )
    return(0) ;


  return
    ( zmw.event
      && zmw.event->any.window == *ZMW_WINDOW
      && zmw.x >= ZMW_SIZE_ALLOCATED.x - ZMW_PADDING_WIDTH
      && zmw.x
      < ZMW_SIZE_ALLOCATED.x + ZMW_SIZE_ALLOCATED.width + ZMW_PADDING_WIDTH
      && zmw.y >= ZMW_SIZE_ALLOCATED.y - ZMW_PADDING_WIDTH
      && zmw.y
      < ZMW_SIZE_ALLOCATED.y + ZMW_SIZE_ALLOCATED.height + ZMW_PADDING_WIDTH
     ) ;
}

/*
 * Change of widget.
 * Modify variables to indicate that the current widget change
 */
 
void zmw_widget_change()
{
  if ( zmw.remove_event )
  	{
  		zmw.remove_event = Zmw_False ;
		zmw.event_removed = Zmw_True ;
  	}
}


void zmw_state_push()
{
  zMw[1].i = zMw[0].i ;
  
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



  ZMW_NAME_SEPARATOR = -1 ;
  ZMW_NAME = zMw[-1].u.name + strlen(zMw[-1].u.name) + 1 ;
  strcpy(&ZMW_NAME[-1], "/?") ;	/* Doesn't inherit name */
  ZMW_NAME_INDEX = ZMW_NAME + 1 ; /* after the '?' */
  ZMW_DO_NOT_EXECUTE_POP = Zmw_False ;
  ZMW_INVISIBLE = Zmw_False ;
  ZMW_SIZE_MIN.x = ZMW_VALUE_UNDEFINED ;
  ZMW_SIZE_MIN.y = ZMW_VALUE_UNDEFINED ;

  ZMW_MENU_STATE = NULL ; // Because of : zmw_tearoff first in a widget
  ZMW_SUBACTION = Zmw_Init ; // Because of : zmw_tearoff first in a widget

  ZMW_CHILD_NUMBER = -1 ;

  zmw.index_last = ZMW_INDEX ;

// What about the next line....
//  zmw_widget_change() ;

  /* no need to initialize ZMW_EXTERNAL_STATE */
}

/*
 *
 */

static char **global_fonts_name = NULL ;
static GdkFont **global_fonts = NULL ;

GdkFont *zmw_font_load(const char *name)
{
  int i ;

  if ( global_fonts_name == NULL )
    {
      ZMW_REALLOC(global_fonts_name, 1) ;
      global_fonts_name[0] = NULL ;
    }
  for(i=0; global_fonts_name[i]; i++)
    if ( strcmp(global_fonts_name[i], name) == 0 )
      {
	return( global_fonts[i] ) ;
      }

  ZMW_REALLOC(global_fonts_name, i+2) ;
  global_fonts_name[i] = strdup(name) ;
  global_fonts_name[i+1] = NULL ;

  ZMW_REALLOC(global_fonts, i+2) ;
  global_fonts[i] = gdk_font_load(name) ;

  return( global_fonts[i] ) ;
}

void zmw_font_free()
{
  int i ;

  for(i=0; global_fonts_name[i]; i++)
    {
      gdk_font_unref(global_fonts[i]) ;
      ZMW_FREE(global_fonts_name[i]) ;
    }
  ZMW_FREE(global_fonts) ;
  ZMW_FREE(global_fonts_name) ;
}

	 
/*
 *
 */

void zmw_state_pop()
{
  zmw_widget_change() ;

  if ( ZMW_DO_NOT_EXECUTE_POP )
    {
      return ;
    }
 
  ZMW_NAME[-1] = '\0' ;
  zMw-- ;
}


static void zmw_debug_set()
{
  if ( zmw_name_is(&zmw.widget_to_trace) )
    {
      zmw_debug(1) ;
      zmw_printf("%s\n", zmw_action_name()) ;
    }
  /*
  else
    zmw_debug(0) ;
  */
}


void zmw_name(const char *s)
{
  if ( zmw.next_is_transient )
  {
  	zmw_printf("zmw_name(\"%s\" should not be used on a transient\n",
  			s) ;
  	return ;
  }
  strcpy(ZMW_NAME, s) ;
  ZMW_NAME_INDEX = ZMW_NAME + strlen(ZMW_NAME) ;
  ZMW_NAME_SEPARATOR = -1 ;
}

void zmw_name_full_compute()
{
  if ( 1 || ZMW_NAME_SEPARATOR > 0)
    sprintf(ZMW_NAME_INDEX, ".%d",  ZMW_NAME_SEPARATOR) ;
 else
    ZMW_NAME_INDEX[0] = '\0' ;
    
  if ( zmw.next_is_transient )
  	{
    	sprintf(ZMW_NAME_INDEX + strlen(ZMW_NAME_INDEX), ",%d",  ZMW_TRANSIENT_SEPARATOR) ;
    	zmw.next_is_transient = Zmw_False ;
  	}
}

void zmw_next_widget_could_be_transient()
{
	ZMW_TRANSIENT_SEPARATOR++ ;
}

static void zmw_increment_index()
{
  ZMW_INDEX = zmw.index_last + 1 ;
  zmw.index_last++ ;
  if ( !zmw.next_is_transient )
  	{
	  ZMW_NAME_SEPARATOR++ ;
	  ZMW_TRANSIENT_SEPARATOR = 0 ;
  	}
  zmw_name_full_compute() ;
}
/*
void zmw_decrement_index()
{
  ZMW_INDEX-- ;
  ZMW_NAME_SEPARATOR-- ;
  zmw_name_full_compute() ;
}
*/
void zmw_name_of_the_transient_begin()
{
	// ZMW_TRANSIENT_SEPARATOR++ ;
	zmw.next_is_transient = Zmw_True ;
	zmw_name_full_compute() ;
}

void zmw_name_of_the_transient_end()
{
	// ZMW_TRANSIENT_SEPARATOR-- ;
	zmw_name_full_compute() ;
}

void zmw_init_widget()
{
  if ( zmw.external_do_not_make_init )
    {
      /* The init was yet done by the widget calling the external compose */
      zmw.external_do_not_make_init = Zmw_False ;
      return ;
    }
  /* If the previous widget was activated, remove event */
  if ( ZMW_SIZE_ACTIVATED || ZMW_SIZE_CHANGED )
    zmw_event_remove() ;

  ZMW_SIZE_INDEX = -1000 ; // XXX to debug
  ZMW_CALL_NUMBER = 0 ;
  zmw_increment_index() ;
  ZMW_USED_TO_COMPUTE_PARENT_SIZE = Zmw_True ;
  ZMW_SIZE_SENSIBLE = Zmw_False ;
  ZMW_SUBACTION = Zmw_Init ;
  ZMW_SIZE_ACTIVATED = Zmw_False ;
  ZMW_SIZE_CHILD_ACTIVATED = Zmw_False ;
  ZMW_SIZE_CHANGED = Zmw_False ;
  ZMW_SIZE_FOCUSED = Zmw_False ;
  ZMW_SIZE_PASS_THROUGH = Zmw_False ;
  ZMW_MENU_STATE = NULL ;
  ZMW_CHILD_NUMBER++ ;
  zmw_debug_set() ;
}

/*
 * XXX FIXME
 * This function needs to be rewrote properly to work
 * on all the possible cases.
 * The hard part are the possible use of EXTERNAL and void in the parents.
 */
Zmw_Size* zmw_widget_previous_size()
{
  int i ;
  Zmw_Size *s ;
  Zmw_State *state ;
  
  state = zMw - 1 ;
  while ( ( ! state->u.size.used_to_compute_parent_size
	    || state->u.nb_of_children <= 1
	    )
	  && state != zmw.zmw_table+1
	  )
     state-- ;
  
  
  for(i=2; ; i++)
    {
      if ( state->u.nb_of_children < i ) /* Impossible case */
	{
	  zmw_printf("state.Type: %s\n", state->u.type) ;
	  zmw_printf("Level: %d\n", state - zMw) ;
	  zmw_printf("nb=%d i=%d\n", state->u.nb_of_children, i) ;
	  ZMW_ABORT ;
	}
      s = &state->u.children[state->u.nb_of_children - i ] ;
      if ( s->used_to_compute_parent_size )
	return(s) ;
    }
}

/*
 * We don't want to test event on a transient but on
 * the widget BEFORE the transient.
 * It is useful for tips on popup menu button,
 * or multiple tips
 */
void zmw_remove_transient_name()
{
  char *p ;
  
  p = strchr(ZMW_NAME_INDEX, ',') ;
  if ( p )
  	{
  	*p = '\0' ;
  	ZMW_INDEX -= 2 ; /* was 1 the 6/8/2003 */
  	}
}

/*
 * do not call if ZMW_SIZE is not computed
 */
void zmw_action_do_not_enter()
{
  zmw_window_restore() ;
  /*  ZMW_CALL_NUMBER-- ; */
  /*
  if ( zMw == zmw.zmw_table+1 )
    ZMW_INDEX = zMw[1].i.index ;
  else
    ZMW_INDEX = ZMW_SIZE_INDEX ;
  */
  zmw_remove_transient_name() ;
}


static void zmw_padding_remove(Zmw_Rectangle *r)
{
 r->width  -= 2*ZMW_PADDING_WIDTH ;
 r->height -= 2*ZMW_PADDING_WIDTH ;
 if ( r->x != ZMW_VALUE_UNDEFINED )
   r->x += ZMW_PADDING_WIDTH ;
 if ( r->y != ZMW_VALUE_UNDEFINED )
   r->y += ZMW_PADDING_WIDTH ;
}


int zmw_action_compute_required_size()
{
  int i ;

  ZMW_EXTERNAL_HANDLING ;

  if ( ZMW_CALL_NUMBER++ == 0 /* && ZMW_USED_TO_COMPUTE_PARENT_SIZE */ )
    {
      if ( zmw_cache_get_size(&ZMW_SIZE) )
	{
	  zmw_name_update() ; /* update name registry */
	  ZMW_SUBACTION = Zmw_Compute_Required_Size ;
	  zmw_state_push() ;
	  return(1) ;
	}
      /* So ZMW_SIZE is the cached value */
      zmw.index_last = ZMW_SIZE_INDEX - 1 ; /* -+1 the 3/7/2003 */
    }
  else
    {
      /* The required size of child has been computed */
      /* and so the number of descendants */
      // ZMW_SIZE_INDEX = zMw[1].i.index ;
      ZMW_SIZE_INDEX = zmw.index_last + 1 ; /* -+1 the 3/7/2003 */

      if ( zmw.event )
	{
	  /*
	   * True if it is the window with the cursor
	   */
	  ZMW_SIZE_EVENT_IN_CHILDREN = (*ZMW_WINDOW == zmw.event->any.window
			       && zMw[-1].i.window != ZMW_WINDOW
			       ) ;
	  /*
	   * Also true if one of the child contains the window with the cursor
	   */
	  if ( !ZMW_SIZE_EVENT_IN_CHILDREN )
	    for(i=0;i<ZMW_NB_OF_CHILDREN;i++)
	      ZMW_SIZE_EVENT_IN_CHILDREN |= ZMW_CHILDREN[i].event_in_children ;
	  /*
	   * This one will be computed once the allocated size
	   * for itself is known
	   */
	  ZMW_SIZE_EVENT_IN_RECTANGLE = Zmw_False ;
	}

      /*
       * Asked size set required size
       * These sizes are random if !ZMW_USED_TO_COMPUTE_PARENT_SIZE
       */
      ZMW_SIZE_MIN.width += 2*ZMW_PADDING_WIDTH ;
      ZMW_SIZE_MIN.height += 2*ZMW_PADDING_WIDTH ;
      ZMW_SIZE_REQUIRED = ZMW_SIZE_MIN ;
      ZMW_SIZE_HORIZONTAL_EXPAND = ZMW_HORIZONTAL_EXPAND ;
      ZMW_SIZE_VERTICAL_EXPAND = ZMW_VERTICAL_EXPAND ;
      ZMW_SIZE_HORIZONTAL_ALIGNMENT = ZMW_HORIZONTAL_ALIGNMENT ;
      ZMW_SIZE_VERTICAL_ALIGNMENT = ZMW_VERTICAL_ALIGNMENT ;
      
      if ( ZMW_SIZE_ASKED.width != ZMW_VALUE_UNDEFINED )
	{
	  ZMW_SIZE_REQUIRED.width = ZMW_SIZE_ASKED.width + 2*ZMW_PADDING_WIDTH ;
	  ZMW_SIZE_HORIZONTAL_EXPAND = Zmw_False ;
	}
      if ( ZMW_SIZE_ASKED.height != ZMW_VALUE_UNDEFINED )
	{
	  ZMW_SIZE_REQUIRED.height = ZMW_SIZE_ASKED.height + 2*ZMW_PADDING_WIDTH ;
	  ZMW_SIZE_VERTICAL_EXPAND = Zmw_False ;
	}
      if ( ZMW_SIZE_ASKED.x != ZMW_VALUE_UNDEFINED )
	ZMW_SIZE_REQUIRED.x = ZMW_SIZE_ASKED.x ;
      if ( ZMW_SIZE_ASKED.y != ZMW_VALUE_UNDEFINED )
	ZMW_SIZE_REQUIRED.y = ZMW_SIZE_ASKED.y ;

      ZMW_SIZE_DO_NOT_MAP_WINDOW = Zmw_False ;

      /*    if ( ZMW_USED_TO_COMPUTE_PARENT_SIZE ) */
      zmw_cache_set_size(&ZMW_SIZE) ;
    }



  /*
   * Store informations in father
   */
  if ( zMw[-1].u.nb_of_children == zMw[-1].u.nb_of_children_max )
    {
      zMw[-1].u.nb_of_children_max = 10 + 2*zMw[-1].u.nb_of_children_max  ;
      ZMW_REALLOC(zMw[-1].u.children, zMw[-1].u.nb_of_children_max) ;
    }

  zMw[-1].u.children[zMw[-1].u.nb_of_children] = ZMW_SIZE ;
  zMw[-1].u.nb_of_children++ ;

  zmw_action_do_not_enter() ;
  return(0) ;
}

/*
 * The type must do :
 *   0 : Init
 *   1 : Compute allocated size for children and pre-drawing
 *   2 : Drawing
 */

int zmw_action_first_pass()
{
  if ( zMw[-1].u.nb_of_children_max != 0 )
    {
      /*
       * Not top level, so size is yet computed
       */
      ZMW_SIZE = zMw[-1].u.children[ZMW_CHILD_NUMBER] ;
      /*
       * We know about sensitivity AFTER the first pass on the parent
       *   zmw_button("quit") ;
       *   if ( zmw_activated() ) ....
       */
      if ( !ZMW_SENSIBLE )
	ZMW_SIZE_SENSIBLE = 0 ;

      zmw_padding_remove(&ZMW_SIZE_ALLOCATED) ;
      zmw_padding_remove(&ZMW_SIZE_MIN) ;
      zmw_padding_remove(&ZMW_SIZE_REQUIRED) ;

      zMw[-1].u.nb_of_children++ ;

      if ( ZMW_INVISIBLE )
	{
          zmw.index_last = ZMW_SIZE_INDEX - 1 ;  /* -+1 the 3/7/2003 */
	  zmw_action_do_not_enter() ;
	  return(0) ;
	}
    }
  
  zmw_state_push() ;
  ZMW_ACTION = zmw_action_compute_required_size ;
  return(1) ;
}

void zmw_action_second_pass_()
{
  if ( zMw[-1].u.nb_of_children )
    zMw[-1].u.children[ZMW_CHILD_NUMBER].event_in_rectangle
      = ZMW_SIZE_EVENT_IN_RECTANGLE ;

}
	

void zmw_action_second_pass()
{
  zMw->u.nb_of_children_0 = zMw->u.nb_of_children ;
  ZMW_SIZE_EVENT_IN_RECTANGLE = zmw_event_in() ;

  zmw_action_second_pass_() ;  
}

static void zmw_debug_children(Zmw_State *z)
{
  int i ;
  fprintf(stderr, "nbchild=%d nbchild0=%d"
	  , z->u.nb_of_children
	  , z->u.nb_of_children_0
	  ) ;
  for(i=0; i<z->u.nb_of_children_0; i++)
    {
      if ( i == z->u.nb_of_children )
	fprintf(stderr, " *") ;
      fprintf(stderr, " %d", z->u.children[i].index) ;
    }
  fprintf(stderr, "\n") ;
}

void zmw_debug_trace()
{
  zmw_printf("[%d] %s %s %s (%s/%d)%s%s%s\n"
	     , ZMW_INDEX
	     , ZMW_NAME
	     , ZMW_TYPE
	     , zmw_action_name()
	     , zmw_action_name_fct()+11
	     , ZMW_CALL_NUMBER
	     , zmw_event_to_process() ? " EtP" : ""
	     , ZMW_SIZE_EVENT_IN_RECTANGLE ? " EIR" : ""
	     , ZMW_SIZE_EVENT_IN_CHILDREN ? " EIC" : ""
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
      zmw_action_second_pass() ;
      ZMW_SUBACTION = Zmw_Post_Drawing ;

      if ( (zmw.debug & Zmw_Debug_Draw_Cross)
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
  zmw_action_do_not_enter() ;
  return(0) ;
}

/*
 *
 */
int zmw_action_dispatch_accelerator()
{
  int i ;

  ZMW_EXTERNAL_HANDLING ;

  if ( ZMW_CALL_NUMBER++ == 0 )
    {
      /* To remove random activation */
      for(i=0; i<zMw->u.nb_of_children_max; i++)
	{
	  ZMW_CHILDREN[i].activated = Zmw_False ;
	  ZMW_CHILDREN[i].changed = Zmw_False ;
	}

      ZMW_SUBACTION = Zmw_Nothing ;
      zmw_state_push() ;
      return(1) ;
    }
  zmw_action_do_not_enter() ;
  return(0) ;    
}

/*
 *
 */

int zmw_action_dispatch_event()
{
  ZMW_EXTERNAL_HANDLING ;

  if ( ZMW_SIZE_ACTIVATED || ZMW_SIZE_CHANGED )
    {
      zmw_event_remove() ;  
    }  

  /*
   * Stop widget tree traversal if a widget has been activated.
   * If it is not done, size in cache are no more valid
   */
  /*
  if ( zmw.event->type == GDK_NOTHING || zmw.remove_event )
    {
      ZMW_CALL_NUMBER++ ;
      zmw_action_do_not_enter() ;
      zMw[-1].u.nb_of_children = zMw[-1].u.nb_of_children_0 ;

      if ( ZMW_SIZE_ACTIVATED && zMw[-1].u.nb_of_children )
	{
	  zMw[-1].u.children[ZMW_CHILD_NUMBER].child_activated = Zmw_True;
	  zMw[-1].u.size.child_activated = Zmw_True;
	}
      return(0) ;
    }
  */

  switch ( ZMW_CALL_NUMBER++ )
    {
    case 0:
      ZMW_SUBACTION = Zmw_Compute_Children_Allocated_Size ;
      return(zmw_action_first_pass()) ;
    case 1:
      zmw_action_second_pass() ;

      ZMW_SUBACTION = Zmw_Input_Event ;
      /*
       * Event are computed for the window with the cursor : first
       * Or the widget containing the window with the cursor : first
       */
      if ( zMw[-1].u.size.event_in_children
           ? ( ( zMw[-1].u.call_number == 2 && !ZMW_SIZE_EVENT_IN_CHILDREN )
                || ( zMw[-1].u.call_number == 3 && ZMW_SIZE_EVENT_IN_CHILDREN )
                )
           : ( zMw[-1].u.size.event_in_rectangle
               && ( ( zMw[-1].u.call_number == 2 && !ZMW_SIZE_EVENT_IN_RECTANGLE )
                    || ( zMw[-1].u.call_number == 3 && ZMW_SIZE_EVENT_IN_RECTANGLE )
                    )
               )
           )
	{
	  if ( ZMW_DEBUG )
	    zmw_printf("Event in masked\n") ;
	  ZMW_EVENT_IN_MASKED = Zmw_True ;
          zmw.index_last = ZMW_SIZE_INDEX - 1 ;  /* -+1 the 3/7/2003 */
	}
      else
	{
	  if ( ZMW_DEBUG )
	    zmw_printf("Event in not masked\n") ;
	  ZMW_EVENT_IN_MASKED = Zmw_False ;
	  zmw_state_push() ;
	  return(1) ;
	}
      break ;

    case 2:
      if ( ZMW_SIZE_EVENT_IN_RECTANGLE && !zmw_activated() && !ZMW_SIZE_CHILD_ACTIVATED && !zmw.event_removed && !ZMW_SIZE_CHANGED )
	{
	  ZMW_SUBACTION = Zmw_Input_Event ;
	  zmw_state_push() ;
	  return(1) ;
	}
      break ;
    }
  /*
   * So the event dispatching for ourself is terminated.
   * We update the father with local information
   */
  if ( ZMW_SIZE_ACTIVATED )
    {
      zMw[-1].u.children[ZMW_CHILD_NUMBER].activated = Zmw_True;
      ZMW_SIZE_CHILD_ACTIVATED = Zmw_True ;
    }
  
  if ( ZMW_SIZE_CHILD_ACTIVATED && zMw[-1].u.nb_of_children )
    {
      zMw[-1].u.children[ZMW_CHILD_NUMBER].child_activated = Zmw_True;
      zMw[-1].u.size.child_activated = Zmw_True;
    }

  /*
   *
   */
  if ( zmw.event->type == GDK_KEY_PRESS
       && ( zmw.event->key.state & 4 )
       &&  zmw.event->key.keyval == GDK_F2
       && ZMW_SIZE_EVENT_IN_RECTANGLE
       )
    {
      zmw_name_register(&zmw.widget_to_trace) ;
      zmw_event_remove() ;
    }

  zmw_action_do_not_enter() ;

  return(0) ;
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
      zmw_action_second_pass() ;
      zmw_state_push() ;
      return(1) ;
    case 2:
      if ( ZMW_SIZE_EVENT_IN_RECTANGLE )
	{
	  zmw_name_register(&zmw.found) ;
	}
      break ;
    }
  else
  	ZMW_CALL_NUMBER++ ;
  zmw_action_do_not_enter() ;
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
      zmw_action_second_pass() ;
      zmw_state_push() ;
      return(1) ;
    }
  zmw_action_do_not_enter() ;
  return(0) ;
}


