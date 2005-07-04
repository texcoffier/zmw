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

#include "zmw/zmw.h"
#include "zmw/zmw_private.h" /* This include is only here for speed up */

#define PRINTF if(0) zmw_printf

static void zmw_image_general(GdkPixbuf *pb)
{
  PRINTF("Image general pb=%p\n", pb) ;
  switch( zmw_subaction_get() )
    {
    case Zmw_Compute_Required_Size:
      zmw_min_width_set(gdk_pixbuf_get_width(pb)) ;
      zmw_min_height_set(gdk_pixbuf_get_height(pb)) ;
      break ;

    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:
      PRINTF("win %p %d %d %dx%d\n"
	     , *zmw_window_get()
	     , zmw_allocated_x_get(), zmw_allocated_y_get()
	     , gdk_pixbuf_get_width(pb), gdk_pixbuf_get_height(pb)) ;

      zmw_pixbuf_render_to_drawable(pb, zmw_allocated_x_get()
				    , zmw_allocated_y_get()) ;
      break ;

    default:
      break ;
    }
}

void zmw_image(GdkPixbuf *pb)
{
  ZMW( zmw_image_general(pb) )
    {
    }
}
void zmw_image_activable(GdkPixbuf *pb)
{
  ZMW(zmw_decorator(  Zmw_Decorator_Interior
		    | Zmw_Decorator_Focusable
		    | Zmw_Decorator_Activable
		    )
      )
    ZMW( zmw_image_general(pb) )
    {
    }
}

GdkPixbuf* zmw_pixbuf_get(GdkPixbuf **pb)
{
  GdkPixbuf *pixbuf ;	
	
  if ( pb == NULL )
    {
      pixbuf = NULL ;
      zmw_name_get_value_pointer("Pixbuf", (void*)&pixbuf) ;
    }
  else
    {
      pixbuf = *pb ;
    }
  PRINTF("zmw_pixbuf_get pb=%p pixbuf=%p\n", pb, pixbuf) ;
  return( pixbuf ) ;
}

void zmw_pixbuf_set(GdkPixbuf **pb, GdkPixbuf *pixbuf)
{
  PRINTF("zmw_pixbuf_set pb=%p pixbuf=%p\n", pb, pixbuf) ;
  if ( pb )
    *pb = pixbuf ;
  else
    zmw_name_set_value_pointer("Pixbuf", pixbuf) ; 
}

void zmw_pixbuf_load_and_set(GdkPixbuf **pb, GdkPixbuf **pixbuf, const char *filename)
{
  PRINTF("zmw_pixbuf_load_and_set pb=%p *pixbuf=%p filename=%s\n"
	 , pb, *pixbuf, filename) ;
 if ( *pixbuf == NULL )
   {
#if GLIB_MAJOR_VERSION == 1
     *pixbuf = gdk_pixbuf_new_from_file(filename) ;
#else
     {
       GError *error = NULL ;
       PRINTF("before gdk_pixbuf_new_from_file\n") ;
       *pixbuf = gdk_pixbuf_new_from_file(filename, &error) ;
       if ( error )
	 {
	   PRINTF("error=%s\n", error->message) ;
	   g_error_free(error) ;
	 }
     }
#endif
     zmw_pixbuf_set(pb, *pixbuf) ;
   }
}

void zmw_image_from_file_with_pixbuf(const char *filename, GdkPixbuf **pb)
{
  GdkPixbuf *pixbuf ;	
	
  PRINTF("Image from file with pixbuf pb=%p\n", pb) ;
  pixbuf = zmw_pixbuf_get(pb) ;	
  PRINTF("Image from file with pixbuf pixbuf=%p\n", pixbuf) ;
  zmw_pixbuf_load_and_set(pb, &pixbuf, filename) ;
  zmw_image(pixbuf) ;
}

void zmw_image_from_file(const char *filename)
{
  PRINTF("Image from file\n") ;
  zmw_image_from_file_with_pixbuf(filename, NULL) ;
}


void zmw_image_from_file_activable_with_pixbuf(const char *filename, GdkPixbuf **pb)
{
  GdkPixbuf *pixbuf ;
	
  pixbuf = zmw_pixbuf_get(pb) ;
  zmw_pixbuf_load_and_set(pb, &pixbuf, filename) ;
  zmw_image_activable(pixbuf) ;
}

void zmw_image_from_file_activable(const char *filename)
{
  zmw_image_from_file_activable_with_pixbuf(filename, NULL) ;
}


void zmw_image_dynamic_from_file_with_pixbuf(const char *filename
				, GdkPixbuf **pb, char **old_name)
{
   GdkPixbuf *pixbuf ;
	
   if ( *old_name )
    {
      if ( strcmp(filename, *old_name) != 0 )
	{
	  free(*old_name) ;
	  *old_name = strdup(filename) ;
	  pixbuf = zmw_pixbuf_get(pb) ;
#if GLIB_MAJOR_VERSION == 1
	  gdk_pixbuf_finalize(pixbuf) ;
#else
#endif
	  zmw_pixbuf_set(pb, NULL) ;
	}
    }
  else
    *old_name = strdup(filename) ;

  zmw_image_from_file_with_pixbuf(filename, pb) ;
}

void zmw_image_dynamic_from_file(const char *filename, char **old_name)
{
   zmw_image_dynamic_from_file_with_pixbuf(filename, NULL, old_name) ;
}
