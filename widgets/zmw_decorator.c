/*
  ZMW: A Zero Memory Widget Library
  Copyright (C) 2003  Thierry EXCOFFIER, Université Claude Bernard, LIRIS

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
#include <stdarg.h>

void zmw_alignement_horizontal_make(Zmw_Size *s, int border_width)
{
  int free_space ;

  s->allocated.x = ZMW_SIZE_ALLOCATED.x + border_width ;
  s->allocated.width = ZMW_SIZE_ALLOCATED.width	- 2 * border_width ;

  if ( ! s->horizontal_expand )
    {
      free_space = ZMW_SIZE_ALLOCATED.width - 2 * border_width
	- s->required.width ;
      s->allocated.width -= free_space ;
      switch(s->horizontal_alignment )
	    {
	    case -1:
	      break ;
	    case 0:
	      s->allocated.x += free_space / 2 ;
	      break ;
	    case 1:
	      s->allocated.x += free_space ;
	      break ;
	    }
    }
}

void zmw_alignement_vertical_make(Zmw_Size *s, int border_width)
{
  int free_space ;

  s->allocated.y = ZMW_SIZE_ALLOCATED.y + border_width ;
  s->allocated.height = ZMW_SIZE_ALLOCATED.height - 2 * border_width ;

  if ( ! s->vertical_expand )
    {
      free_space = ZMW_SIZE_ALLOCATED.height - 2 * border_width
	- s->required.height ;
      s->allocated.height -= free_space ;
      switch(s->vertical_alignment )
	    {
	    case -1:
	      break ;
	    case 0:
	      s->allocated.y += free_space / 2 ;
	      break ;
	    case 1:
	      s->allocated.y += free_space ;
	      break ;
	    }
    }
}


static int zmw_decorator_border(int options)
{
  int border_width ;

  border_width = 0 ;
  if ( options & Zmw_Decorator_Focus_Any )
    border_width += ZMW_FOCUS_WIDTH ;
  if ( options & Zmw_Decorator_Border_Any )
    border_width += ZMW_BORDER_WIDTH ;

  return( border_width ) ;
}

void zmw_decorator(int options, ...)
{
  static int nb = 0 ; // Size of the stack of clipping rectangles
  static GdkRectangle rect[10] ; // 10 : FIXME

  int border, border_width ;
  int i ;
  int tx, ty ;
  va_list ap;

  va_start(ap, options);
  if ( options & Zmw_Decorator_Translate )
    {
      tx = va_arg(ap, int) ;
      ty = va_arg(ap, int) ;
    }
  va_end(ap);


  border_width = zmw_decorator_border(options) ;



  switch( ZMW_SUBACTION )
    {
    case Zmw_Compute_Required_Size:
      if ( ZMW_NB_OF_CHILDREN )
	{
	  ZMW_SIZE_MIN = ZMW_CHILDREN[0].required ;
	  ZMW_SIZE_MIN.width += 2 * border_width ;
	  ZMW_SIZE_MIN.height += 2 * border_width ;
	}
      else
	{
	  ZMW_SIZE_MIN.width = 2 * border_width + 6 ;
	  ZMW_SIZE_MIN.height = 2 * border_width + 6 ;
	}

      /* Used for focus on text ? */
      /*
      if ( options & (Zmw_Decorator_Focusable|Zmw_Decorator_Activable)  )
	ZMW_SIZE_SENSIBLE = 1 ;
      */

      break ;

    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:
      border = 0 ;
      if ( ((options & Zmw_Decorator_Focusable) && zmw_focused())
	   || (options & Zmw_Decorator_Border_Focus)
	   )
	border |= Zmw_Border_Draw_Focus ;

      if ( options & Zmw_Decorator_Focusable )
	border |= Zmw_Border_Focusable ;

      if ( options & Zmw_Decorator_Pushable )
	{
	  if ( zmw_selected() )
	    border |= Zmw_Border_In ;
	}
      if ( options & Zmw_Decorator_Border_Relief )
	border |= Zmw_Border_Relief ;
      if ( options & Zmw_Decorator_Border_Solid )
	border |= Zmw_Border_Solid ;
      if ( options & Zmw_Decorator_Border_Embossed )
	border |= Zmw_Border_Embossed ;
      if ( options & Zmw_Decorator_Border_In )
	border |= Zmw_Border_In ;
      if ( options & Zmw_Decorator_Interior )
	border |= Zmw_Border_Background ;
      zmw_border_draw(border) ;


      if ( options & Zmw_Decorator_Clip )
	{
	  if ( nb )
	    {		 
	      rect[nb].x = ZMW_MAX(ZMW_SIZE_ALLOCATED.x + border_width
				   , rect[nb-1].x) ;
	      rect[nb].y = ZMW_MAX(ZMW_SIZE_ALLOCATED.y + border_width
				   , rect[nb-1].y) ;
	      rect[nb].width = ZMW_MIN(ZMW_SIZE_ALLOCATED.x
				       +ZMW_SIZE_ALLOCATED.width
				       - 2*border_width
				       ,rect[nb-1].x + rect[nb-1].width)
		- rect[nb].x ;
	      rect[nb].height = ZMW_MIN(ZMW_SIZE_ALLOCATED.y
					+ ZMW_SIZE_ALLOCATED.height
					- 2*border_width
					,rect[nb-1].y + rect[nb-1].height)
		- rect[nb].y ;	  
	    }
	  else
	    {
	      rect[nb].x = ZMW_SIZE_ALLOCATED.x + border_width ;
	      rect[nb].y = ZMW_SIZE_ALLOCATED.y + border_width ;
	      rect[nb].width = ZMW_SIZE_ALLOCATED.width - 2*border_width ;
	      rect[nb].height = ZMW_SIZE_ALLOCATED.height - 2*border_width ;
	    }

	  zmw_gc_push() ;
	  for(i=0; i<ZMW_TABLE_SIZE(ZMW_GC_COPY_ON_WRITE); i++)
	    {
	      zmw_modify_gc(gdk_gc_set_clip_rectangle(ZMW_GC[i], &rect[nb]);
			    ,i) ;
	    }
	  // zmw_printf("CLIP in %dx%d {%p}\n",rect[nb].width, rect[nb].height,ZMW_GC[3]) ;
	  nb++ ;
	}

      /* fall thru */

    case Zmw_Compute_Children_Allocated_Size:
      if ( ZMW_NB_OF_CHILDREN )
	{
	  /* This case if for zmw_viewport, because in this
	   * case the decorator must not put its child where it want
	   */
	  if ( options & Zmw_Decorator_Translate )
	    {
	      ZMW_CHILDREN[0].allocated.x = ZMW_SIZE_ALLOCATED.x + tx ;
	      ZMW_CHILDREN[0].allocated.y = ZMW_SIZE_ALLOCATED.y + ty ;
	      ZMW_CHILDREN[0].allocated.width =ZMW_CHILDREN[0].required.width;
	      ZMW_CHILDREN[0].allocated.height=ZMW_CHILDREN[0].required.height;
	    }
	  else
	    {
	      zmw_alignement_vertical_make(&ZMW_CHILDREN[0], border_width) ;
	      zmw_alignement_horizontal_make(&ZMW_CHILDREN[0], border_width) ;
	    }
	}
      break ;

    case Zmw_Post_Drawing:
      if ( options & Zmw_Decorator_Clip )
	{
	  //	  zmw_printf("CLIP out\n") ;
      	  zmw_gc_pop() ;
          nb-- ;
	}
      break ;

    case Zmw_Input_Event:
      if ( options & Zmw_Decorator_Focusable )
	zmw_focusable() ;
      if ( options & Zmw_Decorator_Activable )
	zmw_activable() ;
      if ( options & Zmw_Decorator_Activable_By_Key )
	if ( zmw_key_pressed() && zmw.event->key.string[0] )
	  zmw.activated = Zmw_True ;
      if ( zmw.activated && (options & Zmw_Decorator_Unpop_On_Activate) )
	zmw_window_unpop_all() ;
      break ;

    default:
      break ;
    }
}
