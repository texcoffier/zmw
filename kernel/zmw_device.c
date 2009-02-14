/*
    ZMW: A Zero Memory Widget Library
    Copyright (C) 2003-2005 Thierry EXCOFFIER, Université Claude Bernard, LIRIS

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
#include "zmw/zmw_private.h"

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

  if ( zmw_action_get() == zmw_action_draw )
    {
      color.pixel = zmw_colors_get()[c] ;
      gdk_gc_set_foreground(zmw_gc_get(), &color) ;
      return Zmw_True ;
    }
  return Zmw_False ;
}


void zmw_draw_line(Zmw_Color c, Zmw_Position x1, Zmw_Position y1
		   , Zmw_Position x2, Zmw_Position y2)
{
  if ( zmw_draw_set_gc(c) )
    gdk_draw_line(*zmw_window_get(), zmw_gc_get(), x1, y1, x2, y2) ;
}

void zmw_draw_rectangle(Zmw_Color c, Zmw_Boolean filled
			, Zmw_Position x, Zmw_Position y
			, Zmw_Size width, Zmw_Size height)
{
  if ( zmw_draw_set_gc(c) )
    gdk_draw_rectangle(*zmw_window_get(), zmw_gc_get(), filled, x, y, width, height) ;
}

void zmw_pixbuf_render_to_drawable(GdkPixbuf *pb
				   , Zmw_Position x, Zmw_Position y)
{
  if ( zmw_action_get() == zmw_action_draw )
    gdk_pixbuf_render_to_drawable(pb, *zmw_window_get(), zmw_gc_get(), 0, 0
				  , x
				  , y
				  , gdk_pixbuf_get_width(pb)
				  , gdk_pixbuf_get_height(pb)
				  , GDK_RGB_DITHER_NORMAL, 0, 0) ;
}



void zmw_draw_clip_set()
{
  GdkRectangle clipping ;

  clipping.x = zmw_clipping_get()->x ;
  clipping.y = zmw_clipping_get()->y ;
  clipping.width = zmw_clipping_get()->width ;
  clipping.height = zmw_clipping_get()->height ;
  gdk_gc_set_clip_rectangle(zmw_gc_get(), &clipping) ;
}

/*
 *
 */

#include <pango/pango.h>
#include <pango/pangox.h>
#include <gdk/gdkx.h>
#include <gdk/gdkx.h>

static PangoContext *zmw_g_context = NULL ;
static PangoFontDescription *zmw_g_font_description ;
static PangoLayout *zmw_g_layout ;

static char *zmw_g_text_old = NULL ;
static char *zmw_g_text_new = NULL ;
static Zmw_Font_Description zmw_g_font_description_old = {family: 2005} ;
static Zmw_Font_Description zmw_g_font_description_new ;

static void zmw_text_family_list()
{
  int i, nb ;
  PangoFontFamily **families ;

  pango_context_list_families(zmw_g_context, &families, &nb) ;

  for(i=0; i<nb; i++)
    printf("%s\n", pango_font_family_get_name(families[i])) ;

  g_free(families) ;
}

/* The text is not duplicated */
void zmw_text_set_text(char *text, const Zmw_Font_Description *fd)
{
  zmw_g_text_new = text ;
  zmw_g_font_description_new = *fd ;
}

static void zmw_text_update_values()
{
  int change ;

  change = 0 ;

  if ( zmw_g_font_description_old.family
       != zmw_g_font_description_new.family )
    {
      pango_font_description_set_family(zmw_g_font_description
					, zmw.font_families[zmw_g_font_description_new.family]) ;
      zmw_g_font_description_old.family = zmw_g_font_description_new.family ;
      change = 1 ;
    }

  if ( zmw_g_font_description_old.weight
       != zmw_g_font_description_new.weight )
    {
      pango_font_description_set_weight(zmw_g_font_description
					, zmw_g_font_description_new.weight);
      zmw_g_font_description_old.weight = zmw_g_font_description_new.weight ;
      change = 1 ;
    }

  if ( zmw_g_font_description_old.size
       != zmw_g_font_description_new.size )
    {
      pango_font_description_set_size(zmw_g_font_description
				      , zmw_g_font_description_new.size*PANGO_SCALE) ;
      zmw_g_font_description_old.size = zmw_g_font_description_new.size ;
      change = 1 ;
    }

  if ( zmw_g_font_description_old.style
       != zmw_g_font_description_new.style )
    {
      static int c[] = { PANGO_STYLE_NORMAL, PANGO_STYLE_ITALIC} ;
      pango_font_description_set_style(zmw_g_font_description
				       , c[zmw_g_font_description_new.style]);
      zmw_g_font_description_old.style = zmw_g_font_description_new.style ;
      change = 1 ;
    }
  if ( change )
    pango_context_set_font_description(zmw_g_context
				       , zmw_g_font_description);

  if ( change || zmw_g_text_old == NULL || strcmp(zmw_g_text_old, zmw_g_text_new) )
    {
      pango_layout_set_text(zmw_g_layout, zmw_g_text_new, -1);
      zmw_string_copy(&zmw_g_text_old, zmw_g_text_new) ;
    }

}

unsigned long zmw_hash_key(Zmw_Font_Description *fd, const char * text)
{
  unsigned long k ;
  const char *c ;

  k = fd->family + 100 * (fd->weight + 10 * ( fd->style + 3 * fd->size )) ;
  for(c=text; *c; c++)
    k = k * 1053707 + *c ;

  return k ;
}

/* Use a bigger hash key and get rid of "text" and "fd" ?
*/

static struct hash_cell {
  unsigned long hash_key ;
  char *text ;
  Zmw_Font_Description fd ;
  Zmw_Size width, height ;
} *zmw_g_cache = NULL ;
static int zmw_g_table_size ;


void zmw_text_get_size(Zmw_Size *width, Zmw_Size *height)
{
  unsigned long k ;
  int i ;
  int w, h ;

  if ( zmw_g_table_size == 0 ) /* Work without font size cache */
    {
      zmw_text_update_values() ;
      pango_layout_get_pixel_size(zmw_g_layout, &w, &h) ;
      *width = w ;
      *height = h ;
      return ;
    }
  
  if ( zmw_g_cache == NULL )
    ZMW_MALLOC_0(zmw_g_cache, zmw_g_table_size) ;

  k = zmw_hash_key(&zmw_g_font_description_new, zmw_g_text_new) ;
  i = k % zmw_g_table_size ;
  if ( zmw_g_cache[i].hash_key == k
       && zmw_font_description_equals(&zmw_g_cache[i].fd
				      , &zmw_g_font_description_new)
       )
    {
      *width = zmw_g_cache[i].width ;
      *height = zmw_g_cache[i].height ;
    }
  else
    {
      zmw_text_update_values() ;
      pango_layout_get_pixel_size(zmw_g_layout, &w, &h) ;

      zmw_string_copy(&zmw_g_cache[i].text, zmw_g_text_new) ;

      zmw_g_cache[i].fd = zmw_g_font_description_new ;
      zmw_g_cache[i].width = w ;
      zmw_g_cache[i].height = h ;
      zmw_g_cache[i].hash_key = k ;
      *width = w ;
      *height = h ;
    }
}


void zmw_text_init(int pango_cache)
{
  if ( zmw_g_context )
    return ;

  zmw_g_table_size = pango_cache ;

  // zmw_g_context = pango_x_get_context(GDK_DISPLAY());
  zmw_g_context = gdk_pango_context_get() ;

  pango_context_set_language(zmw_g_context
			     , pango_language_from_string("en_US")
			    ) ;
  pango_context_set_base_dir(zmw_g_context, PANGO_DIRECTION_LTR);

  zmw_g_font_description = pango_font_description_new() ;

  pango_font_description_set_variant(zmw_g_font_description
				     , PANGO_VARIANT_NORMAL) ;
  pango_font_description_set_stretch(zmw_g_font_description
				     , PANGO_STRETCH_NORMAL) ;
  /*
  pango_context_set_font_description(zmw_g_context
				     , zmw_g_font_description);
  */
  zmw_g_layout = pango_layout_new(zmw_g_context) ;

  if ( 0 )
    zmw_text_family_list() ;
}



void zmw_text_exit()
{
  int i ;

  for(i=0; i<zmw_g_table_size; i++)
    free(zmw_g_cache[i].text) ;
  free(zmw_g_cache) ;

  // To be freed ?
  // static PangoContext *zmw_g_context = NULL ;
  // static PangoLayout *zmw_g_layout ;
  pango_font_description_free(zmw_g_font_description) ;
  free(zmw_g_text_old) ;
}



void zmw_text_render(Zmw_Color c, Zmw_Position xx, Zmw_Position yy)
{
  GdkDrawable *d ;
  gint x, y ;

  if ( zmw_draw_set_gc(c) )
    {
      zmw_text_update_values() ;
      gdk_window_get_internal_paint_info(*zmw_window_get(), &d, &x, &y) ;

      gdk_draw_layout(d
      			, zmw_gc_get()
			    , xx
			    , yy
			    , zmw_g_layout
			    ) ;

    }
}




int zmw_text_xy_to_index(Zmw_Position x, Zmw_Position y)
{
  int index, trailing ;

  zmw_text_update_values() ;

  if ( x < 0 )
    x = 0 ;
  if ( y < 0 )
    y = 0 ;

  if ( pango_layout_xy_to_index(zmw_g_layout
				, x*PANGO_SCALE, y*PANGO_SCALE
				, &index, &trailing) )
    return index ;

  if ( pango_layout_xy_to_index(zmw_g_layout
				, 0, y*PANGO_SCALE
				, &index, &trailing) )

    {
      // The cursor is at the right of the text
      while( zmw_g_text_new[index]  &&  zmw_g_text_new[index] != '\n' )
	index++ ;
      return index ;
    }

  // The cursor is under the end of the text
  return strlen(zmw_g_text_new) ;
}

void zmw_text_get_grapheme_rectangle(int index, Zmw_Rectangle *rect)
{
  PangoRectangle r ;

  zmw_text_update_values() ;
  pango_layout_index_to_pos(zmw_g_layout, index, &r) ;

  rect->x = r.x/PANGO_SCALE ;
  rect->y = r.y/PANGO_SCALE ;
  rect->width = r.width/PANGO_SCALE ;
  rect->height = r.height/PANGO_SCALE ;
}

int zmw_text_next_char(int cursor)
{
  if ( zmw_g_text_new[cursor] == '\0' )
    return cursor ;
  return g_utf8_next_char( &zmw_g_text_new[cursor] ) - zmw_g_text_new ; 
}

int zmw_text_previous_char(int cursor)
{
  if ( cursor > 0 )
    return g_utf8_prev_char( &zmw_g_text_new[cursor] ) - zmw_g_text_new ; 
  else
    return 0 ;
}

int zmw_text_delete_char(int cursor)
{
  int previous = zmw_text_previous_char(cursor) ;

  memmove( &zmw_g_text_new[previous]
	 , &zmw_g_text_new[cursor]
         , strlen(zmw_g_text_new) - cursor + 1) ;
  return previous ;
}

void zmw_text_cursor_position(int cursor_pos, Zmw_Rectangle *r)
{
  int previous ;

  previous = zmw_text_previous_char(cursor_pos) ;
    
  zmw_text_get_grapheme_rectangle(previous, r) ;

  if ( cursor_pos )
    {
      if ( zmw_g_text_new[previous] == '\n' )
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

int zmw_text_up_char(int cursor)
{
  Zmw_Rectangle c, r ;
  int i ;

  // Do not move the cursor if there is no line up
  for(i=0; i<cursor-1; i++)
    if ( zmw_g_text_new[i] == '\n' )
      break ;
  if ( i == cursor-1 )
    return cursor ;

  zmw_text_cursor_position(cursor, &c) ;
  zmw_text_get_grapheme_rectangle(cursor, &r) ;
  return zmw_text_xy_to_index(c.x + r.width/2, c.y - r.height) ;
}

int zmw_text_home_char(int cursor)
{
  while(--cursor>0)
    if ( zmw_g_text_new[cursor] == '\n' )
      return cursor + 1 ;

  return 0 ;
}

int zmw_text_end_char(int cursor)
{
  while(zmw_g_text_new[cursor] && zmw_g_text_new[cursor] != '\n' )
    cursor++ ;

  return cursor ;
}

int zmw_text_down_char(int cursor)
{
  Zmw_Rectangle c, r ;
  int i ;

  // Do not move the cursor if there is no line down
  for(i=cursor; zmw_g_text_new[i]; i++)
    if ( zmw_g_text_new[i] == '\n' )
      break ;
  if ( zmw_g_text_new[i] == '\0' )
    return cursor ;

  zmw_text_cursor_position(cursor, &c) ;
  zmw_text_get_grapheme_rectangle(cursor, &r) ;
  return zmw_text_xy_to_index(c.x + r.width/2, c.y + r.height) ;
}
