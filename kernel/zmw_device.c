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

#include "zmw/zmw.h"


Zmw_Boolean zmw_draw_set_gc(Zmw_Color c)
{
  GdkColor color ;

  if ( ZMW_ACTION == zmw_action_draw )
    {
      color.pixel = ZMW_COLORS[c] ;
      gdk_gc_set_foreground(ZMW_GC, &color) ;
      return Zmw_True ;
    }
  return Zmw_False ;
}


void zmw_draw_line(Zmw_Color c, int x1, int y1, int x2, int y2)
{
  if ( zmw_draw_set_gc(c) )
    gdk_draw_line(ZMW_WINDOW, ZMW_GC, x1, y1, x2, y2) ;
}

void zmw_draw_rectangle(Zmw_Color c, Zmw_Boolean filled
			, int x, int y, int width, int height)
{
  if ( zmw_draw_set_gc(c) )
    gdk_draw_rectangle(ZMW_WINDOW, ZMW_GC, filled, x, y, width, height) ;
}

void zmw_draw_string(Zmw_Color c, int x, int y, const char *text)
{
  if ( zmw_draw_set_gc(c) )
    gdk_draw_string(ZMW_WINDOW, ZMW_FONT, ZMW_GC, x, y, text) ;
}

void zmw_pixbuf_render_to_drawable(GdkPixbuf *pb, int x, int y)
{
  if ( ZMW_ACTION == zmw_action_draw )
    gdk_pixbuf_render_to_drawable(pb, ZMW_WINDOW, ZMW_GC, 0, 0
				  , x
				  , y
				  , gdk_pixbuf_get_width(pb)
				  , gdk_pixbuf_get_height(pb)
				  , GDK_RGB_DITHER_NORMAL, 0, 0) ;
}



void zmw_draw_clip_push(const Zmw_Rectangle *r)
{
  GdkRectangle clipping ;

  ZMW_CLIPPING = *r ;

  clipping.x = r->x ;
  clipping.y = r->y ;
  clipping.width = r->width ;
  clipping.height = r->height ;
  gdk_gc_set_clip_rectangle(ZMW_GC, &clipping) ;
}

void zmw_draw_clip_pop()
{
  zmw_draw_clip_push(&zMw[-1].i.clipping) ; // also restore ZMW_CLIPPING
}



