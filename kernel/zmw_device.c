/*
    ZMW: A Zero Memory Widget Library
    Copyright (C) 2003-2004 Thierry EXCOFFIER, Université Claude Bernard, LIRIS

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

static GdkVisual *global_visual = NULL ;

int zmw_rgb_to_int(Zmw_Float_0_1 r, Zmw_Float_0_1 g, Zmw_Float_0_1 b)
{
  static int sr, sg, sb ;

  if ( global_visual == NULL )
    {
      global_visual = gdk_rgb_get_visual() ;
      sr = global_visual->red_mask >> global_visual->red_shift ;
      sg = global_visual->green_mask >> global_visual->green_shift ;
      sb = global_visual->blue_mask >> global_visual->blue_shift ;
      if ( 0 )
	zmw_printf("%d %d  %d %d  %d %d\n",
		   global_visual->red_mask, global_visual->red_shift,
		   global_visual->green_mask, global_visual->green_shift,
		   global_visual->blue_mask, global_visual->blue_shift)	 ;
    }

  ZMW_CLAMP(r, 0, 1) ;
  ZMW_CLAMP(g, 0, 1) ;
  ZMW_CLAMP(b, 0, 1) ;
  return ((int)(r*sr) << global_visual->red_shift)
    + ((int)(g*sg) << global_visual->green_shift)
    + ((int)(b*sb) << global_visual->blue_shift)
    ;
}

void zmw_int_to_rgb(int c,Zmw_Float_0_1 *r, Zmw_Float_0_1 *g, Zmw_Float_0_1 *b)
{
  *r = (c&global_visual->red_mask)/(float)global_visual->red_mask ;
  *g = (c&global_visual->green_mask)/(float)global_visual->green_mask ;
  *b = (c&global_visual->blue_mask)/(float)global_visual->blue_mask ;
}

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
    gdk_draw_line(*ZMW_WINDOW, ZMW_GC, x1, y1, x2, y2) ;
}

void zmw_draw_rectangle(Zmw_Color c, Zmw_Boolean filled
			, int x, int y, int width, int height)
{
  if ( zmw_draw_set_gc(c) )
    gdk_draw_rectangle(*ZMW_WINDOW, ZMW_GC, filled, x, y, width, height) ;
}

void zmw_pixbuf_render_to_drawable(GdkPixbuf *pb, int x, int y)
{
  if ( ZMW_ACTION == zmw_action_draw )
    gdk_pixbuf_render_to_drawable(pb, *ZMW_WINDOW, ZMW_GC, 0, 0
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
  zmw_draw_clip_push(&zMw[-1].u.parent_to_child.clipping) ; // also restore ZMW_CLIPPING
}



/*
 *
 */

#include <pango/pango.h>
#include <pango/pangox.h>
#include <gdk/gdkx.h>

static PangoContext *zmw_global_context = NULL ;
static PangoFontDescription *zmw_global_font_description ;
static PangoLayout *zmw_global_layout ;
static const char *zmw_global_text = NULL ;

static void zmw_text_family_list()
{
  int i, nb ;
  PangoFontFamily **families ;

  pango_context_list_families(zmw_global_context, &families, &nb) ;

  for(i=0; i<nb; i++)
    printf("%s\n", pango_font_family_get_name(families[i])) ;

  g_free(families) ;
}

static Zmw_Boolean zmw_global_font_change = Zmw_False ;

static void zmw_text_set_family(const char *family)
{
  if ( strcmp(pango_font_description_get_family(zmw_global_font_description),
	      family) != 0 )
    {
      zmw_global_font_change = Zmw_True ;
      pango_font_description_set_family(zmw_global_font_description, family) ;
    }
}

static void zmw_text_set_weight(int weight)
{
  if ( pango_font_description_get_weight(zmw_global_font_description)
       != weight )
    {
      zmw_global_font_change = Zmw_True ;
      pango_font_description_set_weight(zmw_global_font_description, weight) ;
    }
}

static void zmw_text_set_size(int size)
{
  if ( pango_font_description_get_size(zmw_global_font_description)
       != size )
    {
      zmw_global_font_change = Zmw_True ;
      pango_font_description_set_size(zmw_global_font_description
				      , size*PANGO_SCALE) ;
    }
}

static void zmw_text_set_style(Zmw_Font_Style ts)
{
  static int c[] = { PANGO_STYLE_NORMAL, PANGO_STYLE_ITALIC} ;

  if ( pango_font_description_get_style(zmw_global_font_description)
       != c[ts] )
    {
      zmw_global_font_change = Zmw_True ;
      pango_font_description_set_style(zmw_global_font_description, c[ts]) ;
    }
}

/*
 * This function should be called before any function
 * in order to update Pango state
 */

void zmw_text_set_text(const char *text, const Zmw_Font_Description *fd)
{
  zmw_global_text = text ;

  zmw_text_set_family(fd->family) ;
  zmw_text_set_size(fd->size) ;
  zmw_text_set_weight(fd->weight) ;
  zmw_text_set_style(fd->style) ;

  if ( zmw_global_font_change )
    {
      pango_context_set_font_description(zmw_global_context
					 , zmw_global_font_description);
      zmw_global_font_change = Zmw_False ;
    }

  pango_layout_set_text(zmw_global_layout, text, -1);
}

void zmw_text_init()
{
  if ( zmw_global_context )
    return ;

  zmw_global_context = pango_x_get_context(GDK_DISPLAY());

  pango_context_set_language(zmw_global_context
			     , pango_language_from_string("en_US")
			     );
  pango_context_set_base_dir(zmw_global_context, PANGO_DIRECTION_LTR);

  zmw_global_font_description = pango_font_description_new() ;

  pango_font_description_set_family(zmw_global_font_description, "times") ;
  zmw_text_set_size(10) ;
  zmw_text_set_weight(PANGO_WEIGHT_NORMAL) ;
  zmw_text_set_style(Zmw_Font_Style_Normal) ;

  pango_font_description_set_variant(zmw_global_font_description
				     , PANGO_VARIANT_NORMAL) ;
  pango_font_description_set_stretch(zmw_global_font_description
				     , PANGO_STRETCH_NORMAL) ;

  pango_context_set_font_description(zmw_global_context
				     , zmw_global_font_description);
  zmw_global_layout = pango_layout_new(zmw_global_context) ;

  // zmw_text_family_list() ;
}



void zmw_text_render(Zmw_Color c, int xx, int yy)
{
  GdkDrawable *d ;
  gint x, y ;

  if ( zmw_draw_set_gc(c) )
    {
      gdk_window_get_internal_paint_info(*ZMW_WINDOW, &d, &x, &y) ;
      pango_x_render_layout(GDK_DISPLAY()
			    , GDK_WINDOW_XWINDOW(d)
			    , GDK_GC_XGC(ZMW_GC)
			    , zmw_global_layout
			    , xx
			    , yy
			    ) ;
    }
}

void zmw_text_get_size(int *width, int *height)
{
  pango_layout_get_pixel_size(zmw_global_layout, width, height) ;
}

int zmw_text_xy_to_index(int x, int y)
{
  int index, trailing ;

  if ( pango_layout_xy_to_index(zmw_global_layout
				, x*PANGO_SCALE, y*PANGO_SCALE
				, &index, &trailing) == 0 )
    {
      if ( x < 0 || y < 0 )
	index = 0 ;
      else
	index = strlen(zmw_global_text) ;
    }

  return index ;
}

void zmw_text_get_grapheme_rectangle(int index, Zmw_Rectangle *rect)
{
  PangoRectangle r ;

  pango_layout_index_to_pos(zmw_global_layout, index, &r) ;

  rect->x = r.x/PANGO_SCALE ;
  rect->y = r.y/PANGO_SCALE ;
  rect->width = r.width/PANGO_SCALE ;
  rect->height = r.height/PANGO_SCALE ;
}

int zmw_text_next_char(const char *text, int cursor)
{
   return g_utf8_next_char( &text[cursor] ) - text ; 
}

int zmw_text_previous_char(const char *text, int cursor)
{
  if ( cursor > 0 )
    return g_utf8_prev_char( &text[cursor] ) - text ; 
  else
    return 0 ;
}

int zmw_text_delete_char(char *text, int cursor)
{
  int previous = zmw_text_previous_char(text, cursor) ;

  memmove(&text[previous], &text[cursor], strlen(text) - cursor + 1) ;
  return previous ;
}

void zmw_text_cursor_position(int cursor_pos, Zmw_Rectangle *r)
{
  int previous ;

  previous = zmw_text_previous_char(zmw_global_text, cursor_pos) ;
    
  zmw_text_get_grapheme_rectangle(previous, r) ;

  if ( cursor_pos )
    {
      if ( zmw_global_text[previous] == '\n' )
	{
	  r->x = 0 ;
	  r->y += r->height ;
	}
      else
	{
	  r->x += r->width ;
	}
    }
  r->x-- ;  
}

