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

/*
 *
 */

static Zmw_Name global_zmw_drag_from = ZMW_NAME_UNREGISTERED("Drag From") ;
static Zmw_Name global_zmw_drag_to = ZMW_NAME_UNREGISTERED("Drag To") ;
static char * global_zmw_drag_data = NULL ;
static enum Zmw_Drag global_zmw_drag_accepted ;

static char *global_wd[] =
{   "Zmw_Drag_No"
  , "Zmw_Drag_Begin"
  , "Zmw_Drag_Accepted"
  , "Zmw_Drag_Refused"
  , "Zmw_Drag_Running"
  , "Zmw_Drag_Ending"
} ;

void zmw_drag_debug(FILE *f)
{
  fprintf(f, "action  : %p\n", ZMW_ACTION) ;
  fprintf(f, "from    : %s\n", zmw_name_registered(&global_zmw_drag_from)) ;
  fprintf(f, "to      : %s\n", zmw_name_registered(&global_zmw_drag_to)) ;
  fprintf(f, "accepted: %s\n", global_wd[global_zmw_drag_accepted]) ;
  fprintf(f, "data    : %s\n", global_zmw_drag_data) ;
}

void zmw_drag_full_cancel()
{
  global_zmw_drag_accepted = Zmw_Drag_Refused ;

  zmw_name_unregister(&global_zmw_drag_from) ;
  zmw_name_unregister(&global_zmw_drag_to) ;

  if ( global_zmw_drag_data )
       free(global_zmw_drag_data) ;
  global_zmw_drag_data = NULL ;
}

void zmw_drag_cancel()
{
  if ( global_zmw_drag_accepted == Zmw_Drag_No )
    {
      global_zmw_drag_accepted = Zmw_Drag_Refused ;
      zmw_drag_debug(stderr) ;
    }
}

void zmw_drag_clear_to_widget()
{
  zmw_name_unregister(&global_zmw_drag_to) ;
}


/*
 * Return information to the dragged zmw about the drag
 *
 * Zmw_Drag_No :    The zmw is not dragged
 * Zmw_Drag_Begin :
 *                     Must call "zmw_drag_from_data_set" to set
 *                     the data to be dragged is duplicated
 * Zmw_Drag_Running :
 *                     You MUST display the drag window
 * Zmw_Drag_Accepted:
 *                     You do what you want
 * Zmw_Drag_Refused:
 *                     You do what you want
 */
enum Zmw_Drag zmw_drag_from_state()
{
  enum Zmw_Drag ret ;

  zmw_widget_is_tested() ;

  zmw_increment_index() ;

  ret = Zmw_Drag_No ;

  if ( ZMW_ACTION == zmw_action_dispatch_event
       && global_zmw_drag_accepted != Zmw_Drag_No
       && zmw_name_is(&global_zmw_drag_from)
       )
    {
      ret = global_zmw_drag_accepted ;
      zmw_drag_full_cancel() ;
    }
  else
    {
      /*
      if( ZMW_ACTION == zmw_action_dispatch_event )
	  zmw_printf("CN=%d EI=%d EIM=%d SS=%d\n"
		     ,ZMW_CALL_NUMBER 
		     ,ZMW_EVENT_IN
		     ,ZMW_EVENT_IN_MASKED
		     ,ZMW_SIZE_SENSIBLE
		     ) ;
      */
      if ( zmw_event_to_process() )
	{
	  if ( !zmw_name_registered(&global_zmw_drag_from) && zmw_button_pressed() )
	    {
	      zmw_name_register(&global_zmw_drag_from) ;
	      ret = Zmw_Drag_Begin ;
	      global_zmw_drag_accepted = Zmw_Drag_No ;
	      zmw_event_remove() ;
	    }
	}
      else
	{
	  if ( zmw_name_is(&global_zmw_drag_from) )
	    {
	      zmw_decrement_index() ;
	      ret = Zmw_Drag_Running ;
	    }
	}
    }

  return( ret ) ;  
}

/*
 * If the drag begin, the dragged zmw must set this information
 */
void zmw_drag_from_data_set(const char *data)
{
  if ( global_zmw_drag_data )
    abort() ;
  global_zmw_drag_data = strdup(data) ;
}

/*
 * Call this function if you can accept drop.
 * Return :
 *     Zmw_Drag_No:
 *              No drag for you
 *     Zmw_Drag_Running:
 *              The function "zmw_drag_to_accept" must be called
 *     Zmw_Drag_Ending:
 *              The data was released on you
 *              The function "zmw_drag_to_accept" must be called
 */

enum Zmw_Drag zmw_drag_to_state()
{
  enum Zmw_Drag ret ;

  zmw_widget_is_tested() ;

  ret = Zmw_Drag_No ;

  if ( global_zmw_drag_accepted == Zmw_Drag_No )
    {
      if ( zmw_event_to_process() )
	{
	  if ( zmw_name_registered(&global_zmw_drag_from) )
	    {
	      zmw_name_register(&global_zmw_drag_to) ;
	      
	      if ( zmw_button_released() )
		{
		  ret = Zmw_Drag_Ending ;
		  zmw_event_remove() ;
		}
	      else
		ret = Zmw_Drag_Running ;
	    }
	}
      else
	{
	  if ( zmw_name_is(&global_zmw_drag_to) )
		  ret = Zmw_Drag_Running ;
	}
    }
  return(ret) ;
}

void zmw_drag_to_accept(int bool)
{
  if ( bool )
    global_zmw_drag_accepted = Zmw_Drag_Accepted ;
  else
    global_zmw_drag_accepted = Zmw_Drag_Refused ;
}

char *zmw_drag_to_data_get()
{
  return(global_zmw_drag_data) ;
}



void zmw_drag_debug_window()
{
  static int display_drag_state = 0 ;
  char buf[9999] ;
  
  ZMW(zmw_box_vertical())
    {
      zmw_toggle_with_label(&display_drag_state, "drag state") ;
      if ( display_drag_state )
	{
	  sprintf(buf, "FROM=%s", zmw_name_registered(&global_zmw_drag_from)) ;
	  zmw_text(buf) ;
	  sprintf(buf, "TO=%s", zmw_name_registered(&global_zmw_drag_to)) ;
	  zmw_text(buf) ;
	  if ( global_zmw_drag_data )
	    {
	      sprintf(buf, "DATA=%20s", global_zmw_drag_data) ;
	      zmw_text(buf) ;
	    }
	  if ( zmw_name_registered(&global_zmw_drag_from)
	       && zmw_name_registered(&global_zmw_drag_to) )
	    {
	      sprintf(buf,"ACCEPT=%s",global_wd[global_zmw_drag_accepted]) ;
	      zmw_text(buf) ;
	    }
	}
    }
  zmw_border_embossed_in_draw() ;
}
