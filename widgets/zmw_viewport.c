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


#define P 10

void zmw_viewport(int x, int y)
{
  int i ;
  GdkRectangle r ;
  static GdkGC *save[P][ZMW_TABLE_SIZE(ZMW_GC_COPY_ON_WRITE)] ;
  static GdkRectangle rect[P] ;
  static int p = 0 ;

  switch( ZMW_SUBACTION )
    {
    case Zmw_Compute_Required_Size:
      ZMW_SIZE_MIN.width = 100 ;
      ZMW_SIZE_MIN.height = 100 ;
      break ;

    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:
      r.x = ZMW_SIZE_ALLOCATED.x + ZMW_BORDER_WIDTH ;
      r.y = ZMW_SIZE_ALLOCATED.y + ZMW_BORDER_WIDTH ;
      r.width = ZMW_SIZE_ALLOCATED.width - 2*ZMW_BORDER_WIDTH ;
      r.height = ZMW_SIZE_ALLOCATED.height - 2*ZMW_BORDER_WIDTH ;

      if ( p )
	{
	  r.x = ZMW_MAX(r.x, rect[p-1].x) ;
	  r.y = ZMW_MAX(r.y, rect[p-1].y) ;
	  r.width = ZMW_MIN(r.x+r.width,rect[p-1].x+rect[p-1].width) - r.x ;
	  r.height = ZMW_MIN(r.y+r.height,rect[p-1].y+rect[p-1].height) - r.y ;
	}
      rect[p] = r ;

      for(i=0; i<ZMW_TABLE_SIZE(ZMW_GC_COPY_ON_WRITE); i++)
	{
	  save[p][i] = ZMW_GC[i] ;
	  ZMW_GC[i] = gdk_gc_new(ZMW_WINDOW) ;
	  gdk_gc_copy(ZMW_GC[i], save[p][i]) ;
	  gdk_gc_set_clip_rectangle(ZMW_GC[i], &r) ;
	}
      p++ ;

    case Zmw_Compute_Children_Allocated_Size:
      for(i=0; i<ZMW_NB_OF_CHILDREN; i++)
	{
	  if ( !ZMW_CHILDREN[i].used_to_compute_parent_size )
	    continue ;
	  
	  ZMW_CHILDREN[i].allocated = ZMW_CHILDREN[i].required ;
	  ZMW_CHILDREN[i].allocated.x = ZMW_SIZE_ALLOCATED.x - x ;
	  ZMW_CHILDREN[i].allocated.y = ZMW_SIZE_ALLOCATED.y - y ;
	}
      break ;
    case Zmw_Post_Drawing:
      p-- ;
      for(i=0; i<ZMW_TABLE_SIZE(ZMW_GC_COPY_ON_WRITE); i++)
	{
	  gdk_gc_destroy(ZMW_GC[i]) ;
	  ZMW_GC[i] = save[p][i] ;
	}

      zmw_border_draw(Zmw_Border_Relief) ;
      break ;
    default:
      break ;
    }
}

/*
 * Should save the state value for expand
 */
void zmw_viewport_with_scrollbar_general(float *x, float *y
					 , Zmw_Boolean *needed)
{
  int width, height, new_needed ;

  width = height = 0 ; /* To avoid a compiler warning */

  ZMW_EXTERNAL_RESTART ;

  ZMW(zmw_box_vertical())
    {
      if ( needed == NULL || *needed )
	{
	  zmw_horizontal_expand(1) ;
	  zmw_vertical_expand(0) ;
	  zmw_text("Horizontal scroll bar") ;
	  zmw_scrollbar_horizontal(x) ;
	  zmw_text("Vertical scroll bar") ;
	  zmw_scrollbar_horizontal(y) ;
	}
      zmw_name("viewport") ;
      zmw_vertical_expand(1) ;
      ZMW(zmw_viewport(*x*(width-ZMW_SIZE_ALLOCATED.width)
		       ,*y*(height-ZMW_SIZE_ALLOCATED.height)))
	{
	  ZMW_EXTERNAL ;
	  width = ZMW_SIZE_REQUIRED.width ;
	  height = ZMW_SIZE_REQUIRED.height ;
	}
      if ( needed && zmw_drawing() )
	{
	  new_needed = ( width >= ZMW_SIZE_ALLOCATED.width
			 || height >= ZMW_SIZE_ALLOCATED.height ) ;
	  if ( new_needed != *needed )
	    zmw_need_repaint() ;
	  *needed = new_needed ;
	}
    }

  ZMW_EXTERNAL_STOP ;
}

void zmw_viewport_with_scrollbar(float *x, float *y)
{
  zmw_viewport_with_scrollbar_general(x, y, NULL) ;
}

void zmw_viewport_with_scrollbar_if_needed(float *x, float *y
					   , Zmw_Boolean *needed)
{
  zmw_viewport_with_scrollbar_general(x, y, needed) ;
}

