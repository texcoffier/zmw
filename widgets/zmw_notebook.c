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
 ****************************************************************************
 * BUG : This certainely not work correctly if the padding
 * is not the same for all the notebook elements
 ****************************************************************************
 */
#include "zmw/zmw.h"

/*
 * For internal use, in all the functions, the children table
 * is sorted : all the label, all the pages.
 *
 * So existing functions may be used.
 */

/* The returned rectangle take into account padding */
static Zmw_Rectangle zmw_notebook_compute_required_label_size(Zmw_Size *ws)
{
  int i ;
  Zmw_Rectangle save, res ;

  for(i=0;i<ZMW_NB_OF_CHILDREN; i++)
    ZMW_CHILDREN[i] = ws[i*2] ;

  save = ZMW_SIZE_MIN ;
  zmw_box_horizontal_required_size() ; // Padding taken into account
  res = ZMW_SIZE_MIN ;
  ZMW_SIZE_MIN = save ;

  return( res ) ;
}
 

static void zmw_notebook_compute_required_size(Zmw_Size *ws)
{
  Zmw_Rectangle r, name, d ;
  int i ;

  name = zmw_notebook_compute_required_label_size(ws) ;

  /*
   * Compute page size
   */
  r = ws[1].min ;
  for(i=0;i<ZMW_NB_OF_CHILDREN; i++)
    {
      d = ws[2*i+1].required ;
      zmw_padding_add(&d, ws[2*i+1].current_state.padding_width) ;
      r = zmw_rectangle_max(&r, &d) ;
    }
  /*
   * compute total size
   */
  ZMW_SIZE_MIN.width = ZMW_MAX(name.width, r.width) ;
  ZMW_SIZE_MIN.height = name.height + r.height ;
}

static void zmw_notebook_children_allocated_size(Zmw_Size *ws)
{
  Zmw_Rectangle r, name ;
  int i ;

  name = zmw_notebook_compute_required_label_size(ws) ;

  r = ZMW_SIZE_ALLOCATED ;
  ZMW_SIZE_ALLOCATED.height = name.height ;
  if ( ZMW_SIZE_ALLOCATED.width > ZMW_SIZE_MIN.width )
    ZMW_SIZE_ALLOCATED.width = ZMW_SIZE_MIN.width ;
  zmw_box_horizontal_children_allocated_size() ;
  ZMW_SIZE_ALLOCATED = r ;
  for(i=0;i<ZMW_NB_OF_CHILDREN; i++)
    ws[i*2] = ZMW_CHILDREN[i] ;


  for(i=1;i<ZMW_NB_OF_CHILDREN*2; i += 2)
    {
      ws[i].allocated.x = ZMW_SIZE_ALLOCATED.x + ws[i].current_state.padding_width ;
      ws[i].allocated.y = ZMW_SIZE_ALLOCATED.y + name.height + ws[i].current_state.padding_width ;
      ws[i].allocated.width = ZMW_SIZE_ALLOCATED.width - 2*ws[i].current_state.padding_width ;
      ws[i].allocated.height = ZMW_SIZE_ALLOCATED.height - name.height - 2*ws[i].current_state.padding_width ;
      
    }
 
}

void zmw_notebook_draw(int *page)
{
  int i ;
  int label_height ;

  label_height = 0 ;
  for(i=0; i<ZMW_NB_OF_CHILDREN; i+=2)
    label_height = ZMW_MAX(label_height
		       , ZMW_CHILDREN[i].allocated.height
			 + ZMW_CHILDREN[i].current_state.padding_width
			   ) ;

  if ( *page )
    // Draw the labels background for the labels left to the current page
    zmw_draw_rectangle(Zmw_Color_Background_Pushed
		       , Zmw_True
		       , ZMW_SIZE_ALLOCATED.x
		       , ZMW_SIZE_ALLOCATED.y
		       , ZMW_CHILDREN[2**page].allocated.x
		       - ZMW_CHILDREN[0].allocated.x
		       , label_height + ZMW_CHILDREN[0].current_state.padding_width
		       ) ;
  if ( 2 * *page != ZMW_NB_OF_CHILDREN-2 )
    // Draw the labels background for the labels right to the current page
    zmw_draw_rectangle(Zmw_Color_Background_Pushed
		       , Zmw_True
		       , ZMW_CHILDREN[2 * *page+2].allocated.x - ZMW_CHILDREN[2 * *page+2].current_state.padding_width
		       , ZMW_SIZE_ALLOCATED.y
		       , ZMW_CHILDREN[ZMW_NB_OF_CHILDREN-2].allocated.x
		       - ZMW_CHILDREN[2 * *page+2].allocated.x
		       + ZMW_CHILDREN[ZMW_NB_OF_CHILDREN-2].allocated.width
		       + 2*ZMW_CHILDREN[ZMW_NB_OF_CHILDREN-2].current_state.padding_width
		       , label_height + ZMW_CHILDREN[0].current_state.padding_width
		       ) ;

  for(i=0; i<ZMW_NB_OF_CHILDREN; i+=2)
    {
      if ( 2 * *page != i )
	{
	  /* Bottom label horizontal line */
	  zmw_draw_line(Zmw_Color_Foreground
			, ZMW_CHILDREN[i].allocated.x + ZMW_CHILDREN[i].current_state.padding_width
			+ ZMW_CHILDREN[i].allocated.width
			, ZMW_SIZE_ALLOCATED.y + label_height + ZMW_CHILDREN[0].current_state.padding_width
			, ZMW_CHILDREN[i].allocated.x - ZMW_CHILDREN[i].current_state.padding_width
			, ZMW_SIZE_ALLOCATED.y + label_height + ZMW_CHILDREN[0].current_state.padding_width
			) ;
	}
      /* label vertical line */
      zmw_draw_line(Zmw_Color_Foreground
		    , ZMW_CHILDREN[i].allocated.x + ZMW_CHILDREN[i].current_state.padding_width
		    + ZMW_CHILDREN[i].allocated.width
		    , ZMW_SIZE_ALLOCATED.y
		    , ZMW_CHILDREN[i].allocated.x + ZMW_CHILDREN[i].current_state.padding_width
		    + ZMW_CHILDREN[i].allocated.width
		    , ZMW_SIZE_ALLOCATED.y + label_height + ZMW_CHILDREN[0].current_state.padding_width
		    ) ;
    }

  /* Top above label horizontal line */
  zmw_draw_line(Zmw_Color_Foreground
		, ZMW_CHILDREN[0].allocated.x - ZMW_CHILDREN[0].current_state.padding_width
		, ZMW_SIZE_ALLOCATED.y
		, ZMW_CHILDREN[ZMW_NB_OF_CHILDREN-2].allocated.x + ZMW_CHILDREN[ZMW_NB_OF_CHILDREN-2].current_state.padding_width
		+ ZMW_CHILDREN[ZMW_NB_OF_CHILDREN-2].allocated.width
		, ZMW_SIZE_ALLOCATED.y
		) ;
  /* Left vertical line */
  zmw_draw_line(Zmw_Color_Foreground
		, ZMW_CHILDREN[0].allocated.x - ZMW_CHILDREN[0].current_state.padding_width
		, ZMW_SIZE_ALLOCATED.y
		, ZMW_CHILDREN[0].allocated.x - ZMW_CHILDREN[0].current_state.padding_width
		, ZMW_SIZE_ALLOCATED.y
		+ ZMW_SIZE_ALLOCATED.height - 1
		) ;
  /* Bottom horizontal line */
  zmw_draw_line(Zmw_Color_Foreground
		, ZMW_CHILDREN[0].allocated.x - ZMW_CHILDREN[0].current_state.padding_width
		, ZMW_SIZE_ALLOCATED.y
		+ ZMW_SIZE_ALLOCATED.height - 1
		, ZMW_CHILDREN[0].allocated.x - ZMW_CHILDREN[0].current_state.padding_width
		+ ZMW_SIZE_ALLOCATED.width
		, ZMW_SIZE_ALLOCATED.y + ZMW_SIZE_ALLOCATED.height - 1
		) ;
  /* Right vertical line */
  zmw_draw_line(Zmw_Color_Foreground
		, ZMW_CHILDREN[0].allocated.x - ZMW_CHILDREN[0].current_state.padding_width
		+ ZMW_SIZE_ALLOCATED.width
		, ZMW_SIZE_ALLOCATED.y
		+ ZMW_SIZE_ALLOCATED.height - 1
		, ZMW_CHILDREN[0].allocated.x - ZMW_CHILDREN[0].current_state.padding_width
		+ ZMW_SIZE_ALLOCATED.width
		, ZMW_SIZE_ALLOCATED.y + label_height + ZMW_CHILDREN[0].current_state.padding_width
		) ;
  /* Top right under label horizontal line */
  zmw_draw_line(Zmw_Color_Foreground
		, ZMW_CHILDREN[0].allocated.x - ZMW_CHILDREN[0].current_state.padding_width
		+ ZMW_SIZE_ALLOCATED.width
		, ZMW_SIZE_ALLOCATED.y + label_height + ZMW_CHILDREN[0].current_state.padding_width
		, ZMW_CHILDREN[ZMW_NB_OF_CHILDREN-2].allocated.x + ZMW_CHILDREN[ZMW_NB_OF_CHILDREN-2].current_state.padding_width
		+ ZMW_CHILDREN[ZMW_NB_OF_CHILDREN-2].allocated.width
		, ZMW_SIZE_ALLOCATED.y + label_height + ZMW_CHILDREN[0].current_state.padding_width
		) ;

}

/*
 *
 */
void zmw_notebook(int *page)
{
  static Zmw_Size *ws = NULL ;
  Zmw_Subaction a ;
  int i ;
  int label_height ;

  a = ZMW_SUBACTION ;
  switch( a )
    {
    case Zmw_Compute_Required_Size:
      /*
       * Is this OK ?
       * What is an unsensible notebook ?
       */
      ZMW_SIZE_SENSIBLE = 1 ;

    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:
    case Zmw_Compute_Children_Allocated_Size:

      ZMW_REALLOC(ws, ZMW_NB_OF_CHILDREN) ;
      memcpy(ws, ZMW_CHILDREN, sizeof(*ws)*ZMW_NB_OF_CHILDREN) ;
      ZMW_NB_OF_CHILDREN /= 2 ;

      switch(a)
	{
	case Zmw_Compute_Required_Size:
	  zmw_notebook_compute_required_size(ws) ;
	  break ;
	case Zmw_Compute_Children_Allocated_Size:
	case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:
	  zmw_notebook_children_allocated_size(ws) ;
	  break ;
	default:
	  ZMW_ABORT ;
	}

      ZMW_NB_OF_CHILDREN *= 2 ;
      memcpy(ZMW_CHILDREN, ws, sizeof(*ws)*ZMW_NB_OF_CHILDREN) ;

      if ( a == Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing )
	zmw_notebook_draw(page) ;

      break ;
    case Zmw_Input_Event:

      if ( zmw_button_pressed() )
	{
	  label_height = 0 ;
	  for(i=0; i<ZMW_NB_OF_CHILDREN; i+=2)
	    label_height = ZMW_MAX(label_height
			       , ZMW_CHILDREN[i].allocated.height) ;
	  for(i=0; i<ZMW_NB_OF_CHILDREN; i += 2)
	    {
	      if (
		  zmw.event->any.window == *ZMW_WINDOW
		  && zmw.x >= ZMW_CHILDREN[i].allocated.x - ZMW_CHILDREN[i].current_state.padding_width
		  && zmw.x < ZMW_CHILDREN[i].allocated.x + ZMW_CHILDREN[i].current_state.padding_width
		             + ZMW_CHILDREN[i].allocated.width
		  && zmw.y >= ZMW_SIZE_ALLOCATED.y
		  && zmw.y  < ZMW_SIZE_ALLOCATED.y + label_height
		  )
		*page = i/2 ;
	    }
	  zmw_need_repaint() ;
	  zmw_event_remove() ;
	}
      break ;
    case Zmw_Clean:
      ZMW_FREE(ws) ;
      break ;
    default:
      break ;
    }
  /*
   * Invisibilise the children except one
   */
  for(i=0; i<ZMW_NB_OF_CHILDREN; i += 2)
    {
      if ( 2 * *page != i)
	ZMW_CHILDREN[i+1].invisible = 1 ;
    }
  
}
