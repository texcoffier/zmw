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

void zmw_image_general(GdkPixbuf *pb, int option)
{
  int border, activable, focusable ;

  border = !!(option & Zmw_Option_Border) ;
  focusable = !!(option & Zmw_Option_Focusable) ;
  activable = !!(option & Zmw_Option_Activable) ;

  switch( ZMW_SUBACTION )
    {
    case Zmw_Compute_Required_Size:
      ZMW_SIZE_MIN.width = gdk_pixbuf_get_width(pb) ;
      ZMW_SIZE_MIN.height = gdk_pixbuf_get_height(pb) ;

      ZMW_SIZE_MIN.width  += 2*(ZMW_BORDER_WIDTH+focusable*ZMW_FOCUS_WIDTH) ;
      ZMW_SIZE_MIN.height += 2*(ZMW_BORDER_WIDTH+focusable*ZMW_FOCUS_WIDTH) ;
      break ;

    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:
      zmw_border_draw
	((border                            ? Zmw_Border_Relief : 0)
	 | (focusable                       ? Zmw_Border_Focusable : 0)
	 | (zmw_focused()                   ? Zmw_Border_Draw_Focus : 0)
	 | (border                          ? Zmw_Border_Background : 0)
	 | ((activable && zmw_selected()) ? Zmw_Border_In : 0)
	 ) ;

      gdk_pixbuf_render_to_drawable(pb, ZMW_WINDOW, ZMW_GC[0], 0, 0
				    , ZMW_SIZE_ALLOCATED.x
				    + border * ZMW_BORDER_WIDTH
				    + focusable * ZMW_FOCUS_WIDTH
				    , ZMW_SIZE_ALLOCATED.y
				    + border * ZMW_BORDER_WIDTH
				    + focusable * ZMW_FOCUS_WIDTH
				    , gdk_pixbuf_get_width(pb)
				    , gdk_pixbuf_get_height(pb)
				    , GDK_RGB_DITHER_NORMAL, 0, 0) ;
      break ;

    case Zmw_Input_Event:
      if ( option )
	zmw_text_general(NULL, option) ;
      break ;
    default:
      break ;
    }
}

void zmw_image(GdkPixbuf *pb)
{
  ZMW( zmw_image_general(pb, 0) )
    {
    }
}
void zmw_image_activable(GdkPixbuf *pb)
{
  ZMW( zmw_image_general(pb, Zmw_Option_Focusable|Zmw_Option_Activable|Zmw_Option_Border) )
    {
    }
}

void zmw_image_from_file(const char *filename, GdkPixbuf **pb)
{
  if ( *pb == NULL )
    *pb = gdk_pixbuf_new_from_file(filename) ;
  zmw_image(*pb) ;
}

void zmw_image_from_file_activable(const char *filename, GdkPixbuf **pb)
{
  if ( *pb == NULL )
    *pb = gdk_pixbuf_new_from_file(filename) ;
  zmw_image_activable(*pb) ;
}


void zmw_image_dynamic_from_file(const char *filename, GdkPixbuf **pb, char **old_name)
{
  if ( *old_name )
    {
      if ( strcmp(filename, *old_name) != 0 )
	{
	  free(*old_name) ;
	  *old_name = strdup(filename) ;
	  gdk_pixbuf_finalize(*pb) ;
	  *pb = NULL ;
	}
    }
  else
    *old_name = strdup(filename) ;

  zmw_image_from_file(filename, pb) ;
}

