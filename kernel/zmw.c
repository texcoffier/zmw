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

const char *zmw_action_name()
{
  switch( ZMW_SUBACTION )
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
    case Zmw_Search :
      return("Search") ;
    case Zmw_Accelerator :
      return("Accelerator") ;
    case Zmw_Nothing :
      return("Nothing") ;
    case Zmw_Clean :
      return("Clean") ;
    }
  ZMW_ABORT ;
}

const char* zmw_size_string(Zmw_Size *s)
{
  static char buf[100] ;

  sprintf(buf
	  ,"Ask %dx%d %d,%d Req %dx%d %d %d All %dx%d %d %d%s%s%s"
	  ,s->asked.width
	  ,s->asked.height
	  ,s->asked.x
	  ,s->asked.y
	  ,s->min.width
	  ,s->min.height
	  ,s->min.x
	  ,s->min.y
	  ,s->allocated.width
	  ,s->allocated.height
	  ,s->allocated.x
	  ,s->allocated.y
	  ,s->used_to_compute_parent_size ? "" : " NotUsed"
	  ,s->event_in ? " Event" : ""
	  ,s->invisible ? " Inv" : ""
	  ) ;
  return(buf) ;
}



int zmw_event_in()
{
  /* Because of viewport, the event must be in the upper window */
  if ( zMw[-1].i.window == ZMW_WINDOW  &&  !zMw[-1].i.event_in )
    return(0) ;

  return
    ( zmw.event
      && zmw.event->any.window == ZMW_WINDOW
      && zmw.x >= ZMW_SIZE_ALLOCATED.x - ZMW_PADDING_WIDTH
      && zmw.x
      < ZMW_SIZE_ALLOCATED.x + ZMW_SIZE_ALLOCATED.width + ZMW_PADDING_WIDTH
      && zmw.y >= ZMW_SIZE_ALLOCATED.y - ZMW_PADDING_WIDTH
      && zmw.y
      < ZMW_SIZE_ALLOCATED.y + ZMW_SIZE_ALLOCATED.height + ZMW_PADDING_WIDTH
     ) ;
}


void zmw_state_push()
{
  int i ;

  zMw[1].i = zMw[0].i ;
  

  ZMW_NB_OF_CHILDREN = 0 ;
  /*
   *
   */
  zMw++ ;

  zmw_x(ZMW_VALUE_UNDEFINED) ;
  zmw_y(ZMW_VALUE_UNDEFINED) ;
  zmw_width(ZMW_VALUE_UNDEFINED) ;
  zmw_height(ZMW_VALUE_UNDEFINED) ;

  for(i=0; i<ZMW_TABLE_SIZE(ZMW_GC_COPY_ON_WRITE); i++)
    {
      ZMW_GC_COPY_ON_WRITE[i] = Zmw_True ;
    }
  ZMW_FONT_COPY_ON_WRITE = Zmw_True ;

  ZMW_NAME_SEPARATOR = -1 ;
  ZMW_NAME = zMw[-1].u.name + strlen(zMw[-1].u.name) + 1 ;
  strcpy(&ZMW_NAME[-1], "/?") ;	/* Doesn't inherit name */
  ZMW_NAME_INDEX = ZMW_NAME + 1 ; /* after the '?' */
  ZMW_DO_NOT_EXECUTE_POP = Zmw_False ;
  ZMW_INVISIBLE = Zmw_False ;
  ZMW_SIZE_MIN.x = ZMW_VALUE_UNDEFINED ;
  ZMW_SIZE_MIN.y = ZMW_VALUE_UNDEFINED ;

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
  int i ;

  if ( ZMW_DO_NOT_EXECUTE_POP )
    {
      return ;
    }

  for(i=0; i<ZMW_TABLE_SIZE(ZMW_GC_COPY_ON_WRITE); i++)
    if ( ZMW_GC_COPY_ON_WRITE[i] == Zmw_False )
      gdk_gc_destroy(ZMW_GC[i]) ;
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
  else
    zmw_debug(0) ;
}


void zmw_name(const char *s)
{
  strcpy(ZMW_NAME, s) ;
  ZMW_NAME_INDEX = ZMW_NAME + strlen(ZMW_NAME) ;
  ZMW_NAME_SEPARATOR = -1 ;
}

void zmw_increment_index()
{
  ZMW_INDEX++ ;
  ZMW_NAME_SEPARATOR++ ;
  if ( ZMW_NAME_SEPARATOR > 0)
    sprintf(ZMW_NAME_INDEX, ".%d",  ZMW_NAME_SEPARATOR) ;
}

void zmw_decrement_index()
{
  ZMW_INDEX-- ;
  ZMW_NAME_SEPARATOR-- ;
  if ( ZMW_NAME_SEPARATOR > 0 )
    sprintf(ZMW_NAME_INDEX, ".%d",  ZMW_NAME_SEPARATOR) ;
  else
    ZMW_NAME_INDEX[0] = '\0' ;
}

void zmw_init_widget()
{
  ZMW_CALL_NUMBER = 0 ;
  zmw_increment_index() ;
  ZMW_USED_TO_COMPUTE_PARENT_SIZE = Zmw_True ;
  ZMW_SIZE_SENSIBLE = Zmw_False ;
  ZMW_SUBACTION = Zmw_Init ;
  zmw.activated = Zmw_False ;
  zmw.dragged = Zmw_False ;
  zmw_debug_set() ;
}

/*
 * do not call if ZMW_SIZE is not computed
 */
void zmw_action_do_not_enter()
{
  ZMW_WINDOW = zMw[-1].i.window ; /* Restore window */
  /*  ZMW_CALL_NUMBER-- ; */
  if ( zMw == zmw.zmw_table+1 )
    ZMW_INDEX = zMw[1].i.index ;
  else
    ZMW_INDEX = ZMW_SIZE_INDEX ;
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
    }
  else
    {
      /* The required size of child has been computed */
      /* and so the number of descendants */
      ZMW_SIZE_INDEX = zMw[1].i.index ;

      if ( zmw.event )
	{
	  /*
	   * True if it is the window with the cursor
	   */
	  ZMW_SIZE_EVENT_IN = (ZMW_WINDOW == zmw.event->any.window
			       && zMw[-1].i.window != ZMW_WINDOW
			       ) ;
	  /*
	   * Also true if one of the child contains the window with the cursor
	   */
	  if ( !ZMW_SIZE_EVENT_IN )
	    for(i=0;i<ZMW_NB_OF_CHILDREN;i++)
	      ZMW_SIZE_EVENT_IN |= ZMW_CHILDREN[i].event_in ;
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
  Zmw_Boolean invisible ;

  invisible = Zmw_False ;

  if ( zMw[-1].u.nb_of_children_max != 0 )
    {
      /*
       * Not top level, so size is yet computed
       */
      ZMW_SIZE = zMw[-1].u.children[zMw[-1].u.nb_of_children] ;
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
	  zmw_action_do_not_enter() ;
	  return(0) ;
	}
    }
  
  zmw_state_push() ;
  ZMW_ACTION = zmw_action_compute_required_size ;
  return(1) ;
}

void zmw_action_second_pass()
{
  zMw->u.nb_of_children_0 = zMw->u.nb_of_children ;
  ZMW_EVENT_IN = zmw_event_in() ;
}

void zmw_debug_trace()
{
  zmw_printf("[%d] %s %s (%s/%d)%s\n"
	     , ZMW_INDEX
	     , ZMW_NAME
	     , zmw_action_name(ZMW_SUBACTION)
	     , zmw_action_name_fct(ZMW_ACTION)+11
	     , ZMW_CALL_NUMBER
	     , ZMW_EVENT_IN ? " EI" : ""
	     ) ;
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

      if ( (zmw.debug & Zmw_Debug_Draw_Cross) && ZMW_EVENT_IN && ZMW_WINDOW )
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
  ZMW_EXTERNAL_HANDLING ;

  if ( ZMW_CALL_NUMBER++ == 0 )
    {
      ZMW_SUBACTION = Zmw_Accelerator ;
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

  if ( zmw.event->type == GDK_NOTHING )
    {
      ZMW_CALL_NUMBER++ ;
      zmw_action_do_not_enter() ;
      return(0) ;    
    }

  switch ( ZMW_CALL_NUMBER++ )
    {
    case 0:
      ZMW_SUBACTION = Zmw_Compute_Children_Allocated_Size ;
      return(zmw_action_first_pass()) ;
    case 1:
      zmw_action_second_pass() ;

      ZMW_SUBACTION = Zmw_Input_Event ;

      /*
       * Do not enter if no event to dispatch
       */
      /*
      if ( zmw.event->type == GDK_NOTHING )
	{
	  zmw_action_do_not_enter() ;
	  return(0) ;    
	}
      */
      /*
       * Event are computed for the window with the cursor : first
       * Or the zmw containing the window with the cursor : first
       */
      if ( zMw[-1].u.size.event_in
	   ? ( ( zMw[-1].u.call_number == 2 && !ZMW_SIZE_EVENT_IN )
		|| ( zMw[-1].u.call_number == 3 && ZMW_SIZE_EVENT_IN )
		)
	   : ( zMw[-1].i.event_in
	       && ( ( zMw[-1].u.call_number == 2 && !ZMW_EVENT_IN )
		    || ( zMw[-1].u.call_number == 3 && ZMW_EVENT_IN )
		    )
	       )
	   )
	{
	  if ( ZMW_DEBUG )
	    zmw_printf("Event in masked\n") ;
	  ZMW_EVENT_IN_MASKED = Zmw_True ;
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
      if ( ZMW_EVENT_IN )
	{
	  ZMW_SUBACTION = Zmw_Input_Event ;
	  zmw_state_push() ;
	  return(1) ;
	}
      break ;
    }
  /*
   *
   */
  if ( zmw.event->type == GDK_KEY_PRESS
       && ( zmw.event->key.state & 4 )
       &&  zmw.event->key.keyval == GDK_F2
       && ZMW_EVENT_IN
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

  switch ( ZMW_CALL_NUMBER++ )
    {
    case 0:
      ZMW_SUBACTION = Zmw_Compute_Children_Allocated_Size ;
      return(zmw_action_first_pass()) ;
    case 1:
      ZMW_SUBACTION = Zmw_Search ;
      zmw_action_second_pass() ;
      zmw_state_push() ;
      return(1) ;
    case 2:
      if ( ZMW_EVENT_IN && !zmw_name_registered(&zmw.found) )
	{
	  zmw_name_register(&zmw.found) ;
	}
      break ;
    }
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


