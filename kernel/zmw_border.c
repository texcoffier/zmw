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

static void rectangle_draw(const Zmw_Rectangle *r, int i, GdkGC *ul, GdkGC *rd)
{
  gdk_draw_line(ZMW_WINDOW, ul
		, r->x+i
		, r->y+i
		, r->x + r->width - i - 1
		, r->y+i
		) ;
  gdk_draw_line(ZMW_WINDOW, ul
		, r->x+i
		, r->y+i
		, r->x+i
		, r->y + r->height - i - 1
		) ;
  
  gdk_draw_line(ZMW_WINDOW, rd
		, r->x + r->width - i - 1
		, r->y + r->height - i - 1
		, r->x + r->width - i - 1
		, r->y+i
		) ;
  gdk_draw_line(ZMW_WINDOW, rd
		, r->x + r->width - i - 1
		, r->y + r->height - i - 1
		, r->x + i
		, r->y + r->height - i - 1
		) ;     
}


void zmw_border_draw_with_rectangle(int options, Zmw_Rectangle *r)
{
  GdkGC *ul, *rd, *b ;
  int i ;

  if ( ZMW_ACTION != zmw_action_draw )
    return ;

  if ( options & Zmw_Border_Draw_Focus )
    {
      for(i=0; i<ZMW_FOCUS_WIDTH; i++)
	{
	  gdk_draw_rectangle(ZMW_WINDOW, ZMW_GC[ZMW_FOREGROUND], 0
			     , r->x + i
			     , r->y + i
			     , r->width - 1 - 2*i
			     , r->height - 1 - 2*i
			     ) ;
	}
    }
  
  if ( options & Zmw_Border_Focusable )
    {
      r->x += ZMW_FOCUS_WIDTH ;
      r->y += ZMW_FOCUS_WIDTH ;
      r->width -= 2 * ZMW_FOCUS_WIDTH ;
      r->height -= 2 * ZMW_FOCUS_WIDTH ;
    }

  if ( options & Zmw_Border_In )
    {
      ul = ZMW_GC[ZMW_BORDER_DARK] ;
      rd = ZMW_GC[ZMW_BORDER_LIGHT] ;
    }
  else
    {
      ul = ZMW_GC[ZMW_BORDER_LIGHT] ;
      rd = ZMW_GC[ZMW_BORDER_DARK] ;
    }
  
  if( options & Zmw_Border_Solid )
    {
      for(i=0; i<ZMW_BORDER_WIDTH; i++)
	{
	  gdk_draw_rectangle(ZMW_WINDOW, ZMW_GC[ZMW_FOREGROUND], 0
			     , r->x + i
			     , r->y + i
			     , r->width - 1 - 2*i
			     , r->height - 1 - 2*i
			     ) ;
	}
    }
  else if ( options & Zmw_Border_Relief )
    {
      for(i=0; i<ZMW_BORDER_WIDTH; i++)
	rectangle_draw(r, i, ul, rd) ;
    }
  else if ( options & Zmw_Border_Embossed )
    {
      for(i=0; i<ZMW_BORDER_WIDTH/2; i++)
	rectangle_draw(r, i, ul, rd) ;
      for(; i<ZMW_BORDER_WIDTH; i++)
	rectangle_draw(r, i, rd, ul) ;
    }

  if ( options & Zmw_Border_Background )
    {
      if ( options & Zmw_Border_In )
	b = ZMW_GC[ZMW_BACKGROUND_PUSHED] ;
      else
	b = ZMW_GC[ZMW_BACKGROUND_POPED] ;

      gdk_draw_rectangle(ZMW_WINDOW, b, 1
			 , r->x+ZMW_BORDER_WIDTH
			 , r->y+ZMW_BORDER_WIDTH
			 , r->width-2*ZMW_BORDER_WIDTH
			 , r->height-2*ZMW_BORDER_WIDTH
			 ) ;
    }
}
void zmw_border_draw(int options)
{
  Zmw_Rectangle r ;

  if ( ZMW_ACTION != zmw_action_draw )
    return ;

  r = ZMW_SIZE_ALLOCATED ;
  /*
  r.x -= ZMW_PADDING_WIDTH ;
  r.y -= ZMW_PADDING_WIDTH ;
  r.width += 2 * ZMW_PADDING_WIDTH ;
  r.height += 2 * ZMW_PADDING_WIDTH ;
  */
  zmw_border_draw_with_rectangle(options, &r) ;
}

void zmw_border_solid_draw()
{
  zmw_border_draw(Zmw_Border_Solid) ;
}
void zmw_border_relief_in_draw()
{
  zmw_border_draw(Zmw_Border_Relief_In) ;
}
void zmw_border_relief_out_draw()
{
  zmw_border_draw(Zmw_Border_Relief_Out) ;
}
void zmw_border_embossed_in_draw()
{
  zmw_border_draw(Zmw_Border_Embossed_In) ;
}
void zmw_border_embossed_out_draw()
{
  zmw_border_draw(Zmw_Border_Embossed_Out) ;
}

static void cross_draw(Zmw_Rectangle *r, GdkGC *gc)
{
  gdk_draw_line(ZMW_WINDOW, gc
		, r->x
		, r->y
		, r->x + r->width - 1
		, r->y + r->height - 1
		) ;
  gdk_draw_line(ZMW_WINDOW, gc
		, r->x + r->width - 1
		, r->y
		, r->x
		, r->y + r->height - 1
		) ;
}

void zmw_cross_draw()
{
  cross_draw(&ZMW_SIZE_ALLOCATED, ZMW_GC[ZMW_FOREGROUND]) ;
}
