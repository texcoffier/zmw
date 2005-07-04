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
#include "zmw/zmw_private.h" // The code must on not depends on this line

#define ZMW_NOTEBOOK_BORDER 1

/*
 * For internal use, in all the functions, the children table
 * is sorted : all the label, all the pages.
 *
 * So existing functions may be used.
 */

/* The returned rectangle take into account padding */
static Zmw_Rectangle zmw_notebook_compute_required_label_size(Zmw_Child *ws)
{
  int i ;
  Zmw_Rectangle save, res ;

  for(i=0;i<zmw_nb_of_children_get(); i++)
    zmw_child__size_set(i, &ws[i*2]) ;

  save = *zmw_min_get() ;
  zmw_hbox_required_size(Zmw_False) ; // Padding taken into account
  res = *zmw_min_get() ;
  zmw_min_set(&save) ;

  return( res ) ;
}
 

static void zmw_notebook_compute_required_size(Zmw_Child *ws)
{
  Zmw_Rectangle r, name, d ;
  int i ;

  name = zmw_notebook_compute_required_label_size(ws) ;

  /*
   * Compute page size
   */
  r = ws[1].min ;
  for(i=0;i<zmw_nb_of_children_get(); i++)
    {
      d = ws[2*i+1].required ;
      zmw_padding_add(&d, ws[2*i+1].current_state.padding_width) ;
      r = zmw_rectangle_max(&r, &d) ;
    }
  /*
   * compute total size
   */
  zmw_min_width_set(ZMW_MAX(name.width, r.width) + 2*ZMW_NOTEBOOK_BORDER) ;
  zmw_min_height_set(name.height + r.height + 2*ZMW_NOTEBOOK_BORDER) ;
}

static void zmw_notebook_children_allocated_size(Zmw_Child *ws)
{
  Zmw_Rectangle r, name ;
  int i ;

  name = zmw_notebook_compute_required_label_size(ws) ;

  r = *zmw_allocated_get() ;
  zmw_allocated_height_set(name.height) ;
  if ( zmw_allocated_width_get() > zmw_min_width_get() )
    zmw_allocated_width_set(zmw_min_width_get()) ;
  zmw_hbox_children_allocated_size(Zmw_False) ;
  zmw_allocated_set(&r) ;
  for(i=0;i<zmw_nb_of_children_get(); i++)
    ws[i*2] = *zmw_child__size_get(i) ;


  for(i=1;i<zmw_nb_of_children_get()*2; i += 2)
    {
      ws[i].allocated.x = zmw_allocated_x_get() + ws[i].current_state.padding_width + ZMW_NOTEBOOK_BORDER ;
      ws[i].allocated.y = zmw_allocated_y_get() + name.height + ws[i].current_state.padding_width + ZMW_NOTEBOOK_BORDER ;
      ws[i].allocated.width = zmw_allocated_width_get() - 2*ws[i].current_state.padding_width - 2 * ZMW_NOTEBOOK_BORDER ;
      ws[i].allocated.height = zmw_allocated_height_get() - name.height - 2*ws[i].current_state.padding_width - 2 * ZMW_NOTEBOOK_BORDER ;
      
    }
 
}

void zmw_notebook_draw(int *page)
{
  int i ;
  int label_height ;

  label_height = 0 ;
  for(i=0; i<zmw_nb_of_children_get(); i+=2)
    label_height = ZMW_MAX(label_height
			   , zmw_child__allocated_height_get(i)
			   + zmw_child__padding_width_get(i)
			   ) ;

  if ( *page )
    // Draw the labels background for the labels left to the current page
    zmw_draw_rectangle(Zmw_Color_Background_Pushed
		       , Zmw_True
		       , zmw_allocated_x_get()
		       , zmw_allocated_y_get()
		       , zmw_child__allocated_x_get(2**page)
		       - zmw_child__allocated_x_get(0)
		       , label_height + zmw_child__padding_width_get(0)
		       ) ;
  if ( 2 * *page != zmw_nb_of_children_get()-2 )
    // Draw the labels background for the labels right to the current page
    zmw_draw_rectangle(Zmw_Color_Background_Pushed
		       , Zmw_True
		       , zmw_child__allocated_x_get(2 * *page+2)
		       - zmw_child__padding_width_get(2 * *page+2)
		       , zmw_allocated_y_get()
		       , zmw_child__allocated_x_get(zmw_nb_of_children_get()-2)
		       - zmw_child__allocated_x_get(2 * *page+2)
		       + zmw_child__allocated_width_get(zmw_nb_of_children_get()-2)
		       + 2*zmw_child__padding_width_get(zmw_nb_of_children_get()-2)
		       , label_height + zmw_child__padding_width_get(0)
		       ) ;

  for(i=0; i<zmw_nb_of_children_get(); i+=2)
    {
      if ( 2 * *page != i )
	{
	  /* Bottom label horizontal line */
	  zmw_draw_line(Zmw_Color_Foreground
			, zmw_child__allocated_x_get(i)
			+ zmw_child__padding_width_get(i)
			+ zmw_child__allocated_width_get(i)
			, zmw_allocated_y_get()
			+ label_height
			+ zmw_child__padding_width_get(0)
			, zmw_child__allocated_x_get(i)
			- zmw_child__padding_width_get(i)
			, zmw_allocated_y_get()
			+ label_height
			+ zmw_child__padding_width_get(0)
			) ;
	}
      /* label vertical line */
      zmw_draw_line(Zmw_Color_Foreground
		    , zmw_child__allocated_x_get(i)
		    + zmw_child__padding_width_get(i)
		    + zmw_child__allocated_width_get(i)
		    , zmw_allocated_y_get()
		    , zmw_child__allocated_x_get(i)
		    + zmw_child__padding_width_get(i)
		    + zmw_child__allocated_width_get(i)
		    , zmw_allocated_y_get()
		    + label_height
		    + zmw_child__padding_width_get(0)
		    ) ;
    }

  /* Top above label horizontal line */
  zmw_draw_line(Zmw_Color_Foreground
		, zmw_child__allocated_x_get(0)
		- zmw_child__padding_width_get(0)
		, zmw_allocated_y_get()
		, zmw_child__allocated_x_get(zmw_nb_of_children_get()-2)
		+ zmw_child__padding_width_get(zmw_nb_of_children_get()-2)
		+ zmw_child__allocated_width_get(zmw_nb_of_children_get()-2)
		, zmw_allocated_y_get()
		) ;
  /* Left vertical line */
  zmw_draw_line(Zmw_Color_Foreground
		, zmw_child__allocated_x_get(0)
		- zmw_child__padding_width_get(0)
		, zmw_allocated_y_get()
		, zmw_child__allocated_x_get(0)
		- zmw_child__padding_width_get(0)
		, zmw_allocated_y_get()
		+ zmw_allocated_height_get() - 1
		) ;
  /* Bottom horizontal line */
  zmw_draw_line(Zmw_Color_Foreground
		, zmw_child__allocated_x_get(0)
		- zmw_child__padding_width_get(0)
		, zmw_allocated_y_get()
		+ zmw_allocated_height_get() - 1
		, zmw_child__allocated_x_get(0)
		- zmw_child__padding_width_get(0)
		+ zmw_allocated_width_get()
		, zmw_allocated_y_get()
		+ zmw_allocated_height_get() - 1
		) ;
  /* Right vertical line */
  zmw_draw_line(Zmw_Color_Foreground
		, zmw_child__allocated_x_get(0)
		- zmw_child__padding_width_get(0)
		+ zmw_allocated_width_get() - 1
		, zmw_allocated_y_get()
		+ zmw_allocated_height_get() - 1
		, zmw_child__allocated_x_get(0)
		- zmw_child__padding_width_get(0) - 1
		+ zmw_allocated_width_get()
		, zmw_allocated_y_get()
		+ label_height
		+ zmw_child__padding_width_get(0)
		) ;
  /* Top right under label horizontal line */
  zmw_draw_line(Zmw_Color_Foreground
		, zmw_child__allocated_x_get(0)
		- zmw_child__padding_width_get(0)
		+ zmw_allocated_width_get()
		, zmw_allocated_y_get()
		+ label_height
		+ zmw_child__padding_width_get(0)
		, zmw_child__allocated_x_get(zmw_nb_of_children_get()-2)
		+ zmw_child__padding_width_get(zmw_nb_of_children_get()-2)
		+ zmw_child__allocated_width_get(zmw_nb_of_children_get()-2)
		, zmw_allocated_y_get()
		+ label_height
		+ zmw_child__padding_width_get(0)
		) ;

}

/*
 *
 */
void zmw_notebook(int *page)
{
  static Zmw_Child *ws = NULL ;
  Zmw_Subaction a ;
  int i ;

  a = zmw_subaction_get() ;

  switch( a )
    {
    case Zmw_Compute_Required_Size:
      /*
       * Is this OK ?
       * What is an unsensible notebook ?
       */
      zmw_sensitived_set(Zmw_True) ;

    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:
    case Zmw_Compute_Children_Allocated_Size:

      for(i=0; i<zmw_nb_of_children_get(); i += 2)
	zmw_child__sensitived_set(i, Zmw_True) ;


      ZMW_REALLOC(ws, zmw_nb_of_children_get()) ;
      memcpy(ws, zmw_children_get(), sizeof(*ws)*zmw_nb_of_children_get()) ;
      *zmw_nb_of_children_get_ptr() /= 2 ;

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

      *zmw_nb_of_children_get_ptr() *= 2 ;
      memcpy(zmw_children_get(), ws, sizeof(*ws)*zmw_nb_of_children_get()) ;


      if ( a == Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing )
	zmw_notebook_draw(page) ;

      break ;
    case Zmw_Input_Event:

      for(i=0; i<zmw_nb_of_children_get(); i += 2)
	{
	  if ( zmw_child__activated_get(i) )
	    *page = i/2 ;
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
  for(i=0; i<zmw_nb_of_children_get(); i += 2)
    {
      if ( 2 * *page != i)
	zmw_child__invisible_set(i+1, Zmw_True) ;
    }
  
}
