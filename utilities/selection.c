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

static char *global_text = NULL ;

char *zmw_get_selection(const char *source, const char *content
			, GdkWindow *ww)
{
	Window xw, w_req ;
	int format;
	unsigned long len, bytes_left, dummy;
	unsigned char *data;
	Atom type, sel, a_type ;
	static char *last = NULL ;
	XEvent e ;
	GList *tlw ;
	

	sel = XInternAtom(GDK_DISPLAY(), source, 0) ;
	w_req = XGetSelectionOwner (GDK_DISPLAY(), sel)  ;
	if ( w_req == None )
		{
		return("") ;
		}

 	for( tlw = gdk_window_get_toplevels() ; tlw ; tlw = g_list_next(tlw) )
           {
           	if ( w_req == GDK_WINDOW_XWINDOW(tlw->data) )
			return( global_text ) ;
           }

	xw = GDK_WINDOW_XWINDOW(ww) ;

	a_type = XInternAtom(GDK_DISPLAY(), content, 0) ;
	
	XConvertSelection (GDK_DISPLAY(), sel
				, a_type, a_type
				, xw, CurrentTime-1);
	do
		{
		XWindowEvent(GDK_DISPLAY(), xw, PropertyChangeMask ,&e) ;
		if ( e.xproperty.atom != a_type  )
			{
				data = XGetAtomName(GDK_DISPLAY()
						, e.xproperty.atom) ;
				fprintf(stderr, "<== %s\n", data) ;
				XFree(data) ;
			}
		}
	while( e.xproperty.atom != a_type
		 && e.xproperty.state != PropertyNewValue) ;


	XGetWindowProperty (GDK_DISPLAY(), xw, 
				a_type,
				0, 0,	  	  // offset - len
				0, 	 	  // Delete 0==FALSE
				AnyPropertyType,  //flag
				&type,		  // return type
				&format,	  // return format
				&len, &bytes_left,  //that 
				&data);
				
	if ( bytes_left )
		{
		    if ( XGetWindowProperty (GDK_DISPLAY(), xw, 
				a_type, 0,bytes_left,0,
				AnyPropertyType, &type,&format,
				&len, &dummy, &data)
			== Success)
			{
			if ( last )
				XFree(last) ;
			last = data ;
			return( last ) ;
			}
		}
	
	return("") ;
}


void zmw_set_selection(const char *source, const char *content
			, GdkWindow *ww, char *t)
{
	if ( global_text )
		free(global_text) ;
	global_text = t ;
	
	XSetSelectionOwner (GDK_DISPLAY()
			, XInternAtom(GDK_DISPLAY(), source, 0)
			, GDK_WINDOW_XWINDOW(ww) , CurrentTime);
}

 
int zmw_handle_selection(GdkEvent *e)
{
   GdkEventSelection *s ;	
   

   if ( global_text == NULL )
   	return(0) ;

   if ( e->type == GDK_SELECTION_CLEAR )
	return(1) ;

   if (e->type != GDK_SELECTION_REQUEST)
   	return(0) ;

   s = &e->selection ;

   if (s->target == GDK_TARGET_STRING)
	{
	XChangeProperty (GDK_DISPLAY(),
			 s->requestor,
#if GLIB_MAJOR_VERSION > 1
			 (int)s->property,
#else
			 s->property,
#endif
			 XA_STRING,
			 8,
			 PropModeReplace,
		         global_text,
			 strlen(global_text)
			 );
	}
	else
		s->property = 0 ;

 gdk_selection_send_notify (s->requestor,
                                    s->selection,
                                    s->target,
                                    s->property ,
                                    s->time
                                    );	

gdk_flush() ;
return(0) ;
}


