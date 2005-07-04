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

/* Bug : Some bad padding inserted .... */

#include "zmw/zmw.h"
#include "zmw/zmw_private.h" // The code must on not depends on this line

#define ZMW_TABLE_VERTICAL_WIDTH 1 // vertical line separator

int * zmw_table_col_width(int nb_cols, int *computed_col_width)
{
  int col, i ;
  static int *widths = NULL ;
  static int max_col = 0 ;
  
  if ( computed_col_width == NULL )
    {
      if ( nb_cols > max_col )
	{
	  max_col = nb_cols ;
	  ZMW_REALLOC(widths, max_col) ;
	}
      computed_col_width = widths ;

      for(col=0; col<nb_cols; col++)
	{
	  computed_col_width[col] = zmw_child__required_width_get(col) + 2*zmw_child__padding_width_get(col) ;
	  for(i = col + nb_cols; i<zmw_nb_of_children_get(); i += nb_cols)	    
	    if ( zmw_child__required_width_get(i) + 2*zmw_child__padding_width_get(i) > computed_col_width[col] )
	      computed_col_width[col] =  zmw_child__required_width_get(i) + 2*zmw_child__padding_width_get(i) ;
	}

    }

  return( computed_col_width ) ;
}

int *zmw_table_row_height(int nb_cols, int *nb_rows)
{
  static int *height = NULL ;
  int row, i, pos ;
  

  *nb_rows = (zmw_nb_of_children_get() + nb_cols - 1) / nb_cols ;
  ZMW_REALLOC(height, *nb_rows) ;

  pos = 0 ;
  for(row=0; row<*nb_rows; row++)
    {
      height[row] = zmw_child__required_height_get(pos) + 2*zmw_child__padding_width_get(pos) ;
      pos++ ;
      for(i = 1; i < nb_cols && pos <zmw_nb_of_children_get(); i++, pos++)
	if ( zmw_child__required_height_get(pos) + 2*zmw_child__padding_width_get(pos) > height[row] )
	  height[row] = zmw_child__required_height_get(pos) + 2*zmw_child__padding_width_get(pos) ;
    }
  return( height ) ;
}

void zmw_table_compute_required_size(int nb_cols, int *col_width)
{
  int i, nb_rows ;
  int *width, *height ;

  width = zmw_table_col_width(nb_cols, col_width) ;
  height = zmw_table_row_height(nb_cols, &nb_rows) ;
  
  zmw_min_width_set(0) ;
  for(i=0; i<nb_cols; i++)
    *zmw_min_width_get_ptr() += width[i] ;
  *zmw_min_width_get_ptr() += (nb_cols+1)*ZMW_TABLE_VERTICAL_WIDTH ;
  
  zmw_min_height_set(0) ;
  for(i=0; i<nb_rows; i++)
    *zmw_min_height_get_ptr() += height[i] ;
}

void zmw_table_compute_allocated_size(int nb_cols, int *col_width, int **wi, int **he)
{
  int i, nb_rows ;
  int *width, *height ;
  int x, y, w, h ;

  *wi = width = zmw_table_col_width(nb_cols, col_width) ;
  *he = height = zmw_table_row_height(nb_cols, &nb_rows) ;

  if ( zmw_nb_of_children_get() == 0 )
    return ;

  x = zmw_allocated_x_get() + ZMW_TABLE_VERTICAL_WIDTH + zmw_child__padding_width_get(0) ;
  y = zmw_allocated_y_get()  + zmw_child__padding_width_get(0) ;

  for(i=0; i<zmw_nb_of_children_get(); i++)
    {
      zmw_child__allocated_x_set(i,x) ;
      zmw_child__allocated_y_set(i,y) ;
      w = width[i%nb_cols] - 2*zmw_child__padding_width_get(i) ;
      h = height[i/nb_cols] - 2*zmw_child__padding_width_get(i) ;
      if ( zmw_child__horizontaly_expanded_get(i) )
	zmw_child__allocated_width_set(i,w);
      else
	{
	  if ( zmw_child__required_width_get(i) < w )
	    {
	      zmw_child__allocated_width_set(i,zmw_child__required_width_get(i)) ;
	      switch(zmw_child__horizontal_alignment_get(i))
		{
	      	case 0: 
		  *zmw_child__allocated_x_get_ptr(i) +=
		    ( w - zmw_child__required_width_get(i) ) / 2;
		  break ;
	      	case 1:
		  *zmw_child__allocated_x_get_ptr(i) +=
		    w - zmw_child__required_width_get(i) ;
		}
	    }
	  else
	    zmw_child__allocated_width_set(i,w) ;
	}

      if ( zmw_child__verticaly_expanded_get(i) )
	zmw_child__allocated_height_set(i,h) ;
      else
	{
	  if ( zmw_child__required_height_get(i) < h )
	    {
	      zmw_child__allocated_height_set(i,zmw_child__required_height_get(i)) ;
	      switch(zmw_child__vertical_alignment_get(i))
		{
	      	case 0: 
		  *zmw_child__allocated_y_get_ptr(i) +=
		    ( h - zmw_child__required_height_get(i) ) / 2;
		  break ;
	      	case 1:
		  *zmw_child__allocated_y_get_ptr(i) +=
		    h - zmw_child__required_height_get(i) ;
		}
	    }
	  else
	    zmw_child__allocated_height_set(i,h) ;
	}

      
      x += width[i%nb_cols] + ZMW_TABLE_VERTICAL_WIDTH ;
      if ( (i+1)%nb_cols == 0 )
      	{
	  x = zmw_allocated_x_get() + ZMW_TABLE_VERTICAL_WIDTH + zmw_child__padding_width_get(0) ;
	  y += height[i/nb_cols] ;
      	}
    }
}

static Zmw_Child *global_children = NULL ;
static int global_nb_children ;

void zmw_children_remove_not_used()
{
  int i, j ;
  Zmw_Child *s ;
  static int size = 0 ;

  if ( zmw_nb_of_children_get() > size )
    {
      size = zmw_nb_of_children_get() ;
      ZMW_REALLOC(global_children, size) ;
    }
  s = zmw_children_get() ;
  zmw_children_set(global_children) ;
  global_children = s ;
  for(i=0, j=0; i<zmw_nb_of_children_get(); i++)
    if ( global_children[i].used_by_parent )
      zmw_children_get()[j++] = global_children[i] ;
			
  global_nb_children = zmw_nb_of_children_get() ;
  zmw_nb_of_children_set(j) ;
}

void zmw_children_restore_not_used()
{
  Zmw_Child *s ;
  int i, j ;
	
  s = zmw_children_get() ;
  zmw_children_set(global_children) ;
  global_children = s ;
  zmw_nb_of_children_set(global_nb_children) ;	

  for(i=0, j=0; i<zmw_nb_of_children_get(); i++)
    if ( zmw_child__used_by_parent_get(i) )
      zmw_child__allocated_set(i, &global_children[j++].allocated) ;

}

int zmw_search_column(int nb_cols, int *col_width)
{
  int i, x ;
	
  x = zmw_allocated_x_get() + ZMW_TABLE_VERTICAL_WIDTH ;
  for(i=0; i<nb_cols; i++)
    {
      x += col_width[i] + ZMW_TABLE_VERTICAL_WIDTH ;
      if ( ZMW_ABS(x - zmw_zmw_x_get()) < 5 )
	{
	  zmw_printf("x=%d zmw_zmw_x_set(%d i)=%d\n", x, zmw_zmw_x_get(), i) ;
	  return(i) ;	  
	}
    }
  return(-1) ;
}

int zmw_search_row(int nb_cols, int *nb_rows)
{
  int i, y ;
  int *height ;
	
  height = zmw_table_row_height(nb_cols, nb_rows) ;

  y = zmw_allocated_y_get() ;
  for(i=0; i<*nb_rows; i++ )
    {
      if ( zmw_zmw_y_get() > y && zmw_zmw_y_get() < y + height[i] )
	    return(i) ;
      y += height[i] ;
    }
  return(-1) ;  
}

void zmw_table_draw(int nb_cols, int *width, int *height, Zmw_Boolean *selection)
{
  int i, x, y ;
   
  if ( selection )
    {
      y = zmw_allocated_y_get() ;
      for(i=0; i<zmw_nb_of_children_get(); i += nb_cols)
   	{
	  if ( selection[i/nb_cols] )
	    zmw_draw_rectangle(Zmw_Color_Background_Pushed, Zmw_True
			       , zmw_allocated_x_get()
			       , y
			       , zmw_allocated_width_get()
			       , height[i/nb_cols]
			       ) ;
   			
	  y += height[i/nb_cols] ;
   	}
    }
/*
 * Vertical column separator
 */
  x = zmw_allocated_x_get() ;
  for(i=0; i<=nb_cols; i++)
    {
      zmw_draw_line(Zmw_Color_Foreground
		    , x /* + 1 */
		    , zmw_allocated_y_get()
		    , x /* + 1 */
		    , zmw_allocated_y_get() + zmw_allocated_height_get()
		    ) ;
      if ( i != nb_cols )
	x += width[i] + ZMW_TABLE_VERTICAL_WIDTH ;
    }

}

void zmw_table_with_widths_and_selection(int nb_cols, int *col_width
					 , Zmw_Boolean *selection, Zmw_Boolean unique)
{
  static int dragged = -1, x, width_orig ;
  int *width, *height, row, i, nb_rows ;
	
  switch( zmw_subaction_get() )
    {
    case Zmw_Compute_Required_Size:
      zmw_children_remove_not_used() ;
      zmw_table_compute_required_size(nb_cols, col_width) ;
      zmw_children_restore_not_used() ;
      if ( col_width )
	zmw_sensitived_set(1) ;
      break ;

    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:
    case Zmw_Compute_Children_Allocated_Size:
      zmw_children_remove_not_used() ;
      zmw_table_compute_allocated_size(nb_cols, col_width, &width, &height) ;
      if ( zmw_subaction_get() == Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing )
      		zmw_table_draw(nb_cols, width, height, selection) ;
      zmw_children_restore_not_used() ;
      break ;
      
    case Zmw_Input_Event:

      zmw_activable() ;

      if ( dragged >= 0 && zmw_button_released_anywhere() )
	{
	  dragged = -1 ;
	  zmw_event_remove() ;
	  break ;
	}

      if ( zmw_activated() )
	{
	  if ( dragged == -1 && selection )
	    {
	      zmw_children_remove_not_used() ;
	      row = zmw_search_row(nb_cols, &nb_rows) ;
	      zmw_children_restore_not_used() ;
	      if ( row != -1 )
	        {
	        if ( unique )
	            {
	            	if ( ! selection[row] )
	            	{
	            	for(i=0; i<nb_rows; i++)
	            		selection[i] = Zmw_False ;
	            	}
	            }
	        else
		    selection[row] ^= Zmw_True ;
	        }
	    }
	  dragged = -1 ;
	}

      if ( col_width )
 	{
	  if ( zmw_selected() )
	    {
	      if ( dragged == -1 )
		{
		  zmw_children_remove_not_used() ;
		  dragged = zmw_search_column(nb_cols, col_width) ;
		  zmw_children_restore_not_used() ;
		  if ( dragged != -1 )
		    {
		      x = zmw_zmw_x_get() ;
		      width_orig = col_width[dragged] ;
		    }
		}
	      else
		{
		  zmw_activated_set(Zmw_True) ;
		  col_width[dragged] = width_orig + (zmw_zmw_x_get() - x) ;
		  zmw_event_remove() ;
		}
	    }
 	}
      break ;
    default:
      break ;
    }
}

void zmw_table(int nb_cols)
{
  zmw_table_with_widths_and_selection(nb_cols, NULL, NULL, Zmw_False) ;
}
void zmw_table_with_widths(int nb_cols, int *widths)
{
  zmw_table_with_widths_and_selection(nb_cols, widths, NULL, Zmw_False) ;
}


