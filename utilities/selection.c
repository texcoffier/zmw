/*
    ZMW: A Zero Memory Widget Library
    Copyright (C) 2003 Thierry EXCOFFIER, Université Claude Bernard, LIRIS

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

#include <X11/Xlib.h> 
#include <X11/Xatom.h>
#include <gdk/gdkx.h>
#include <gdk/gdk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // usleep
#include <locale.h>

#define PRINTF if (0) printf 

/*
 * Function created with copy/paste from :
 * http://www.enfin.com/getweb/X-copy+paste.txt
 *
 *  X Windows Copy-Paste mini HOWTO
 * Stelios Xathakis, <axanth@tee.gr>
 * v0.5, 18 Dec 1998
 */

/* PRIMARY versus CLIPBOARD is not tested
 * So there is only one variable
 */

/*
 * Selections are stored in UTF8
 */

static char *global_text = NULL ;

void zmw_set_locale()
{
  static int initialized = 0 ;

  if ( initialized )
    return ;

  initialized = 1 ;

  if ( strcmp(setlocale(LC_ALL, "" ), "C") )
    return ;

  fprintf(stderr
	  , "You shoult really set a good locale LC_ALL=...\n"
	  "The locale list is given by the command 'locale -a'\n"
	  ) ;

  if ( setlocale(LC_ALL, "fr_FR@euro") )
    return ;
  if ( setlocale(LC_ALL, "fr_FR") )
    return ;
  if ( setlocale(LC_ALL, "fr") )
    return ;
  if ( setlocale(LC_ALL, "french") )
    return ;

  fprintf(stderr, "Can not find any latin1 locale\n") ;
}

char *zmw_get_selection(const char *source, const char *content
			, GdkWindow *ww)
{
  static guchar *last ;
  int format ;
  GdkAtom atom_returned, a_source, a_content  ;

  a_source = gdk_atom_intern(source,0) ;
  a_content = gdk_atom_intern(content,0) ;

  if ( gdk_selection_owner_get(a_source) )
    {
      PRINTF("I am the selection owner\n\n") ;
      return global_text ;
    }
  
  gdk_selection_convert(ww, a_source, a_content, GDK_CURRENT_TIME) ;
  gdk_flush() ;
  usleep(100000) ;

  if ( last )
    g_free(last) ;

  gdk_selection_property_get(ww, &last, &atom_returned, &format) ;
  

  if ( format != 8 )
    {
      PRINTF("Bad format, return ''\n\n") ;
      return "" ;
    }
  if ( atom_returned != a_content )
    {
      PRINTF("No good property (%s), returns ''\n\n",
	     gdk_atom_name(atom_returned)
	     ) ;
      return "" ;
    }

  if ( strcmp(content, "STRING") == 0 )
    {
      char *out ;
      int bytes_read, bytes_written ;
      
      zmw_set_locale() ;
      
      out = g_locale_to_utf8(last, -1, &bytes_read
			     , &bytes_written, NULL) ;
      if ( out )
	{
	  g_free(last) ;
	  last = out ;
	}
    }

  return last ;
}


void zmw_set_selection(const char *source, const char *content
			, GdkWindow *ww, char *t)
{
  if ( global_text )
    free(global_text) ;
  global_text = t ;
  
  PRINTF("Set selection owner source=%s\n", source) ;

  gdk_selection_owner_set(ww, gdk_atom_intern(source,0), GDK_CURRENT_TIME, 1) ;
}

 
int zmw_handle_selection(GdkEvent *e)
{
  GdkEventSelection *s ;	
  char *text_to_send ;
  
  if ( global_text == NULL )
    return(0) ;
  
  if ( e->type == GDK_SELECTION_CLEAR )
    return(1) ;
  
  if (e->type != GDK_SELECTION_REQUEST)
    return(0) ;
  
  s = &e->selection ;

  PRINTF("Handle selection from %d for %s target=%s selection=%s\n"
	 , s->requestor
	 , gdk_atom_name(s->property)
	 , gdk_atom_name(s->target)
	 , gdk_atom_name(s->selection)
	 ) ;
  
  if (s->target == GDK_TARGET_STRING
      || strcmp(gdk_atom_name(s->target), "UTF8_STRING")==0 )
     {
       PRINTF("ChangeProperty req=%d prop=%ld targ=%ld (%s)\n"
	      , s->requestor
	      , gdk_x11_atom_to_xatom(s->property)
	      , gdk_x11_atom_to_xatom(s->target)
	      , global_text
	      ) ;

       if ( s->target == GDK_TARGET_STRING )
	 {
	   int bytes_read, bytes_written ;

	   zmw_set_locale() ;
	   text_to_send = g_locale_from_utf8(global_text, -1, &bytes_read
					   , &bytes_written, NULL) ;
	   if ( text_to_send == NULL )
	     text_to_send = global_text ;
	 }
       else
	 text_to_send = global_text ;

       /*
       gdk_property_change(s->requestor,
			   s->property,
			   s->target,
			   8,
			   GDK_PROP_MODE_REPLACE,
			   text_to_send,
			   strlen(text_to_send)
			  ) ;
       */
		       
       XChangeProperty(GDK_DISPLAY(),
		       s->requestor,
		       gdk_x11_atom_to_xatom(s->property),
		       gdk_x11_atom_to_xatom(s->target),
		       8,
		       PropModeReplace,
		       text_to_send,
		       strlen(text_to_send)
		      ) ;
     }
   else
     s->property = 0 ;
   
   gdk_selection_send_notify (s->requestor,
			      s->selection,
			      s->target,
			      s->property ,
			      s->time
			     ) ;	
   
   gdk_flush() ;
   return 0 ;
}


