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

// #include <gdk/gdkkeysyms.h>
#include "zmw/zmw.h"


void zmw_button_name(const char *text
		     , GdkModifierType state, int character, char *buf)
{
  sprintf(buf, "%s   %s%s%s%s%s%c"
	  , text
	  , (state & GDK_SHIFT_MASK) ? "Shift+" : ""
	  , (state & GDK_LOCK_MASK) ? "Lock+" : ""
	  , (state & GDK_CONTROL_MASK) ? "Control+" : ""
	  , (state & GDK_MOD1_MASK) ? "Meta+" : ""
	  , (state & GDK_MOD2_MASK) ? "Alt+" : ""
	  , character
	  ) ;
}	

static int global_zmw_accelerators_nb = 0 ;
static int global_zmw_accelerators_nb_max = 0 ;
static char **global_zmw_accelerators = NULL ;
 
void zmw_accelerator_init()
{
  global_zmw_accelerators_nb = 0 ;
}
static void zmw_accelerator_add(const char *text)
{
  int i ;
	
  if ( global_zmw_accelerators_nb == global_zmw_accelerators_nb_max )
    {
      global_zmw_accelerators_nb_max = global_zmw_accelerators_nb_max*2 + 10 ;
      ZMW_REALLOC(global_zmw_accelerators, global_zmw_accelerators_nb_max) ;
      for(i=global_zmw_accelerators_nb; i<global_zmw_accelerators_nb_max; i++)
	global_zmw_accelerators[i] = NULL ;
    }
  ZMW_REALLOC(global_zmw_accelerators[global_zmw_accelerators_nb],
	      strlen(text)+1) ;
  strcpy(global_zmw_accelerators[global_zmw_accelerators_nb], text) ;
  global_zmw_accelerators_nb++ ;
}

void zmw_button(const char *text)
{
  ZMW(zmw_decorator(  Zmw_Decorator_Interior
		    | Zmw_Decorator_Border_Relief
		    | Zmw_Decorator_Focusable
		    | Zmw_Decorator_Activable
		    | Zmw_Decorator_Activable_By_Key
		    | Zmw_Decorator_Pushable
		    | Zmw_Decorator_Unpop_On_Activate
		    )
      )
    {
      zmw_vertical_expand(Zmw_False) ;
      zmw_horizontal_expand(Zmw_False) ;
      ZMW(zmw_text_simple((char**)&text, Zmw_False, Zmw_True, NULL, NULL))
	{
	}
    }
}

void zmw_button_general(const char *text
			, GdkModifierType state, int character
			, Zmw_Boolean display
			)
{
  char buf[strlen(text)+20] ;

  zmw_button_name(text, state, character, buf) ;
  if ( ZMW_ACTION == zmw_action_dispatch_accelerator )
    zmw_accelerator_add(buf) ;      

  if ( display )
    zmw_button(buf) ;
  else
    zmw_button(text) ;

  if ( !zmw.activated && zmw_accelerator(state, character) )
    zmw.activated = 1 ;
}

void zmw_button_with_hidden_accelerator(const char *text
					, GdkModifierType state, int character
					)
{
  zmw_button_general(text, state, character, Zmw_False) ;
}

void zmw_button_with_accelerator(const char *text
				 , GdkModifierType state, int character
				 )
{
  zmw_button_general(text, state, character, Zmw_True) ;
}



void zmw_accelerators_window(const char *filter)
{
  int i ;
  int save_auto_resize ;
	
  if ( filter )
    {
      for(i=0;i<global_zmw_accelerators_nb; i++)
	{
	  if ( strstr(global_zmw_accelerators[i], filter) )
	    break ;
	}
      if ( i == global_zmw_accelerators_nb )
	return ;
    }
	
  save_auto_resize = ZMW_AUTO_RESIZE ;
  ZMW_AUTO_RESIZE = Zmw_True ;	
  ZMW(zmw_window_drag())
    {
      ZMW(zmw_box_vertical())
	{
	  if ( filter )
	    {
	      zmw_text("Accelerators filtered by:") ;
	      zmw_text(filter) ;
	    }
	  else
	    zmw_text("Accelerators") ;
	  zmw_text("-----------------------") ;
	  for(i=0;i<global_zmw_accelerators_nb; i++)
	    {
	      if ( !filter 
		   || (filter
		       && strstr(global_zmw_accelerators[i], filter)
		       )
		   )
		{
		  zmw_text(global_zmw_accelerators[i]) ;
		}
	    }
	}
    }
  ZMW_AUTO_RESIZE = save_auto_resize ;
}

/*
 ******************************************************************************
 * tearoff
 ******************************************************************************
 */

void zmw_tearoff_with_detached_and_id(int *detached, GdkWindow **w)
{
  if ( zmw_window_detached(detached, Zmw_Detached_Up) )
    zmw_button("<<--- attach ---") ;
  else
    zmw_button("--- detach --->>") ;
  if ( zmw_activated() )
    {
      gdk_window_destroy(ZMW_WINDOW) ;
      if ( w )
	{
	  *w = NULL ;
	}
      else
	{
	  zmw_name_unregister_value_by_pointer("WindowID", ZMW_WINDOW) ;
	}
      zmw_window_detached_toggle(detached, Zmw_Detached_Up) ;
      zmw_event_remove() ;
    }
}

void zmw_tearoff()
{
  zmw_tearoff_with_detached_and_id(NULL, NULL) ;
}

void zmw_tearoff_with_detached(int *detached)
{
  zmw_tearoff_with_detached_and_id(detached, NULL) ;
}
