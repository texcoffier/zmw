/*
    ZMW: A Zero Memory Widget Library
    Copyright (C) 2002-2004 Thierry EXCOFFIER, Université Claude Bernard, LIRIS

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

/*
 * This file contains only device independent graphic functions
 */

#include "zmw/zmw.h"

/* Rectangle with relief */
static void rectangle_draw(const Zmw_Rectangle *r, int i
			   , Zmw_Color ul, Zmw_Color rd)
{
  zmw_draw_line(ul
		, r->x+i
		, r->y+i
		, r->x + r->width - i - 1
		, r->y+i
		) ;
  zmw_draw_line(ul
		, r->x+i
		, r->y+i
		, r->x+i
		, r->y + r->height - i - 1
		) ;
  
  zmw_draw_line(rd
		, r->x + r->width - i - 1
		, r->y + r->height - i - 1
		, r->x + r->width - i - 1
		, r->y+i
		) ;
  zmw_draw_line(rd
		, r->x + r->width - i - 1
		, r->y + r->height - i - 1
		, r->x + i
		, r->y + r->height - i - 1
		) ;     
}


void zmw_border_draw_with_rectangle(int options, Zmw_Rectangle *r)
{
  Zmw_Color ul, rd, b ;
  int i ;

  if ( ZMW_ACTION != zmw_action_draw )
    return ;


  if ( options & Zmw_Border_Draw_Focus )
    {
      for(i=0; i<ZMW_FOCUS_WIDTH; i++)
	{
	  zmw_draw_rectangle(Zmw_Color_Foreground, Zmw_False
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

  if ( options & Zmw_Border_Background )
    {
      if ( options & Zmw_Border_In )
	b = Zmw_Color_Background_Pushed ;
      else if ( options & Zmw_Border_Out )
	b = Zmw_Color_Background_Poped ;
      else
	b = Zmw_Color_Background_Normal ;

      zmw_draw_rectangle(b, Zmw_True
			 , r->x // +ZMW_BORDER_WIDTH
			 , r->y // +ZMW_BORDER_WIDTH
			 , r->width // -2*ZMW_BORDER_WIDTH
			 , r->height // -2*ZMW_BORDER_WIDTH
			 ) ;
    }

  if ( options & Zmw_Border_In )
    {
      ul = Zmw_Color_Border_Dark ;
      rd = Zmw_Color_Border_Light ;
    }
  else
    {
      ul = Zmw_Color_Border_Light ;
      rd = Zmw_Color_Border_Dark ;
    }
  
  if( options & Zmw_Border_Solid )
    {
      for(i=0; i<ZMW_BORDER_WIDTH; i++)
	{
	  zmw_draw_rectangle(Zmw_Color_Foreground, Zmw_False
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
}
void zmw_border_draw(int options)
{
  Zmw_Rectangle r ;

  if ( ZMW_ACTION != zmw_action_draw )
    return ;

  r = ZMW_SIZE_ALLOCATED ;
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

static void cross_draw(Zmw_Rectangle *r, Zmw_Color c)
{
  zmw_draw_line(c
		, r->x
		, r->y
		, r->x + r->width - 1
		, r->y + r->height - 1
		) ;
  zmw_draw_line(c
		, r->x + r->width - 1
		, r->y
		, r->x
		, r->y + r->height - 1
		) ;
}

void zmw_cross_draw()
{
  cross_draw(&ZMW_SIZE_ALLOCATED, Zmw_Color_Foreground) ;
}

void zmw_draw_clip_push_inside(const Zmw_Rectangle *r)
{
  Zmw_Rectangle c ;

  c.x = ZMW_MAX(r->x, ZMW_CLIPPING.x) ;
  c.y = ZMW_MAX(r->y, ZMW_CLIPPING.y) ;
  c.width = ZMW_MIN(r->x + r->width, ZMW_CLIPPING.x + ZMW_CLIPPING.width)
    - c.x ;
  c.height= ZMW_MIN(r->y + r->height, ZMW_CLIPPING.y + ZMW_CLIPPING.height)
    - c.y ;
  zmw_draw_clip_push(&c) ;
}

int zmw_rgb_scale_to_int(Zmw_Float_0_1 r, Zmw_Float_0_1 g, Zmw_Float_0_1 b,
			 Zmw_Float_0_1 s)
{
  return zmw_rgb_to_int(r*s, g*s, b*s) ;
}

void zmw_rgb(Zmw_Float_0_1 r, Zmw_Float_0_1 g, Zmw_Float_0_1 b)
{
  zmw_color(Zmw_Color_Foreground       , zmw_rgb_scale_to_int(r,g,b, 0.1)) ;
  zmw_color(Zmw_Color_Border_Dark      , zmw_rgb_scale_to_int(r,g,b, 0.5)) ;
  zmw_color(Zmw_Color_Background_Pushed, zmw_rgb_scale_to_int(r,g,b, 0.9)) ;
  zmw_color(Zmw_Color_Background_Normal, zmw_rgb_to_int(r,g,b)) ;
  zmw_color(Zmw_Color_Background_Poped , zmw_rgb_scale_to_int(r,g,b, 1.2)) ;
  zmw_color(Zmw_Color_Border_Light     , zmw_rgb_scale_to_int(r,g,b, 1.5)) ;
}


