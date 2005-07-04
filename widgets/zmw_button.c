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
#include "zmw/zmw_private.h" /* This include is only here for speed up */


void zmw_button_name(const char *text
		     , GdkModifierType state, int character, char *buf)
{
  sprintf(buf, "%s   %s%s%s%s%s%c"
	  , text
	  , (state & GDK_SHIFT_MASK) ? "Shift+" : ""
	  , (state & GDK_LOCK_MASK) ? "Lock+" : ""
	  , (state & GDK_CONTROL_MASK) ? "Control+" : ""
	  , (state & GDK_MOD1_MASK) ? "Alt+" : ""
	  , (state & GDK_MOD2_MASK) ? "Meta+" : ""
	  , character
	  ) ;
}	

static int global_zmw_accelerators_nb = 0 ;
static int global_zmw_accelerators_nb_max = 0 ;
static struct
{
  char *button ;
  GdkModifierType state ;
} *global_zmw_accelerators = NULL ;
 
void zmw_accelerator_init()
{
  global_zmw_accelerators_nb = 0 ;
}
static void zmw_accelerator_add(const char *text, GdkModifierType state)
{
  int i ;
	
  if ( global_zmw_accelerators_nb == global_zmw_accelerators_nb_max )
    {
      global_zmw_accelerators_nb_max = global_zmw_accelerators_nb_max*2 + 10 ;
      ZMW_REALLOC(global_zmw_accelerators, global_zmw_accelerators_nb_max) ;
      for(i=global_zmw_accelerators_nb; i<global_zmw_accelerators_nb_max; i++)
	global_zmw_accelerators[i].button = NULL ;
    }
  ZMW_REALLOC(global_zmw_accelerators[global_zmw_accelerators_nb].button,
	      strlen(text)+1) ;
  strcpy(global_zmw_accelerators[global_zmw_accelerators_nb].button, text) ;
  global_zmw_accelerators[global_zmw_accelerators_nb].state = state ;
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
		    | Zmw_Decorator_Feedback
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
  if ( zmw_action_get() == zmw_action_dispatch_accelerator )
    zmw_accelerator_add(buf, state) ;      

  if ( display )
    zmw_button(buf) ;
  else
    zmw_button(text) ;

  if ( !zmw_activated_get() && zmw_accelerator(state, character) )
    {
      zmw_activated_set(Zmw_True) ;
    }
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



void zmw_accelerators_window(GdkModifierType filter)
{
  int i ;
  int save_auto_resize ;
	
  if ( filter )
    {
      for(i=0;i<global_zmw_accelerators_nb; i++)
	{
	  if ( (global_zmw_accelerators[i].state & filter) == filter )
	    break ;
	}
      if ( i == global_zmw_accelerators_nb )
	return ;
    }
	
  save_auto_resize = zmw_auto_resize_get() ;
  zmw_auto_resize_set(Zmw_True) ;	
  ZMW(zmw_window_drag())
    {
      ZMW(zmw_vbox())
	{
	  zmw_label("Accelerators\n"
		    "-----------------------\n"
		    "Move focus: Ctrl+Cursor\n"
		    ) ;
	  for(i=0;i<global_zmw_accelerators_nb; i++)
	    {
	      if ( (global_zmw_accelerators[i].state & filter) == filter )
		{
		  zmw_label(global_zmw_accelerators[i].button) ;
		}
	    }
	}
    }
  zmw_auto_resize_set(save_auto_resize) ;
}

/*
 ******************************************************************************
 * tearoff
 ******************************************************************************
 */

void zmw_tearoff()
{
  if ( zmw_window_is_detached() )
    zmw_button("<<--- attach ---") ;
  else
    zmw_button("--- detach --->>") ;
  if ( zmw_activated() )
    {
      gdk_window_destroy(*zmw_window_get()) ;
      zmw_zmw_window_set(NULL) ;
      *zmw_window_get() = NULL ;
      zmw_window_detached_toggle() ;
    }
}

