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
	  computed_col_width[col] = ZMW_CHILDREN[col].required.width + 2*ZMW_CHILDREN[col].current_state.padding_width ;
	  for(i = col + nb_cols; i<ZMW_NB_OF_CHILDREN; i += nb_cols)	    
	    if ( ZMW_CHILDREN[i].required.width + 2*ZMW_CHILDREN[i].current_state.padding_width > computed_col_width[col] )
	      computed_col_width[col] =  ZMW_CHILDREN[i].required.width + 2*ZMW_CHILDREN[i].current_state.padding_width ;
	}

    }

  return( computed_col_width ) ;
}

int *zmw_table_row_height(int nb_cols, int *nb_rows)
{
  static int *height = NULL ;
  int row, i, pos ;
  

  *nb_rows = (ZMW_NB_OF_CHILDREN + nb_cols - 1) / nb_cols ;
  ZMW_REALLOC(height, *nb_rows) ;

  pos = 0 ;
  for(row=0; row<*nb_rows; row++)
    {
      height[row] = ZMW_CHILDREN[pos].required.height + 2*ZMW_CHILDREN[pos].current_state.padding_width ;
      pos++ ;
      for(i = 1; i < nb_cols && pos <ZMW_NB_OF_CHILDREN; i++, pos++)
	if ( ZMW_CHILDREN[pos].required.height + 2*ZMW_CHILDREN[pos].current_state.padding_width > height[row] )
	  height[row] = ZMW_CHILDREN[pos].required.height + 2*ZMW_CHILDREN[pos].current_state.padding_width ;
    }
  return( height ) ;
}

void zmw_table_compute_required_size(int nb_cols, int *col_width)
{
  int i, nb_rows ;
  int *width, *height ;

  width = zmw_table_col_width(nb_cols, col_width) ;
  height = zmw_table_row_height(nb_cols, &nb_rows) ;
  
  ZMW_SIZE_MIN.width = 0 ;
  for(i=0; i<nb_cols; i++)
    ZMW_SIZE_MIN.width += width[i] ;
  ZMW_SIZE_MIN.width += (nb_cols+1)*ZMW_TABLE_VERTICAL_WIDTH ;
  
  ZMW_SIZE_MIN.height = 0 ;
  for(i=0; i<nb_rows; i++)
    ZMW_SIZE_MIN.height += height[i] ;
}

void zmw_table_compute_allocated_size(int nb_cols, int *col_width, int **wi, int **he)
{
  int i, nb_rows ;
  int *width, *height ;
  int x, y, w, h ;

  *wi = width = zmw_table_col_width(nb_cols, col_width) ;
  *he = height = zmw_table_row_height(nb_cols, &nb_rows) ;

  if ( ZMW_NB_OF_CHILDREN == 0 )
    return ;

  x = ZMW_SIZE_ALLOCATED.x + ZMW_TABLE_VERTICAL_WIDTH + ZMW_CHILDREN[0].current_state.padding_width ;
  y = ZMW_SIZE_ALLOCATED.y  + ZMW_CHILDREN[0].current_state.padding_width ;

  for(i=0; i<ZMW_NB_OF_CHILDREN; i++)
    {
      ZMW_CHILDREN[i].allocated.x = x ;
      ZMW_CHILDREN[i].allocated.y = y ;
      w = width[i%nb_cols] - 2*ZMW_CHILDREN[i].current_state.padding_width ;
      h = height[i/nb_cols] - 2*ZMW_CHILDREN[i].current_state.padding_width ;
      if ( ZMW_CHILDREN[i].current_state.horizontal_expand )
	ZMW_CHILDREN[i].allocated.width = w;
      else
	{
	  if ( ZMW_CHILDREN[i].required.width < w )
	    {
	      ZMW_CHILDREN[i].allocated.width = ZMW_CHILDREN[i].required.width ;
	      switch(ZMW_CHILDREN[i].current_state.horizontal_alignment)
		{
	      	case 0: 
		  ZMW_CHILDREN[i].allocated.x +=
		    ( w - ZMW_CHILDREN[i].required.width ) / 2;
		  break ;
	      	case 1:
		  ZMW_CHILDREN[i].allocated.x +=
		    w - ZMW_CHILDREN[i].required.width ;
		}
	    }
	  else
	    ZMW_CHILDREN[i].allocated.width = w ;
	}

      if ( ZMW_CHILDREN[i].current_state.vertical_expand )
	ZMW_CHILDREN[i].allocated.height = h ;
      else
	{
	  if ( ZMW_CHILDREN[i].required.height < h )
	    {
	      ZMW_CHILDREN[i].allocated.height = ZMW_CHILDREN[i].required.height ;
	      switch(ZMW_CHILDREN[i].current_state.vertical_alignment)
		{
	      	case 0: 
		  ZMW_CHILDREN[i].allocated.y +=
		    ( h - ZMW_CHILDREN[i].required.height ) / 2;
		  break ;
	      	case 1:
		  ZMW_CHILDREN[i].allocated.y +=
		    h - ZMW_CHILDREN[i].required.height ;
		}
	    }
	  else
	    ZMW_CHILDREN[i].allocated.height = h ;
	}

      
      x += width[i%nb_cols] + ZMW_TABLE_VERTICAL_WIDTH ;
      if ( (i+1)%nb_cols == 0 )
      	{
	  x = ZMW_SIZE_ALLOCATED.x + ZMW_TABLE_VERTICAL_WIDTH + ZMW_CHILDREN[0].current_state.padding_width ;
	  y += height[i/nb_cols] ;
      	}
    }
}

static Zmw_Size *global_children = NULL ;
static int global_nb_children ;

void zmw_children_remove_not_used()
{
  int i, j ;
  Zmw_Size *s ;
  static int size = 0 ;

  if ( ZMW_NB_OF_CHILDREN > size )
    {
      size = ZMW_NB_OF_CHILDREN ;
      ZMW_REALLOC(global_children, size) ;
    }
  s = ZMW_CHILDREN ;
  ZMW_CHILDREN = global_children ;
  global_children = s ;
  for(i=0, j=0; i<ZMW_NB_OF_CHILDREN; i++)
    if ( global_children[i].used_to_compute_parent_size )
      ZMW_CHILDREN[j++] = global_children[i] ;
			
  global_nb_children = ZMW_NB_OF_CHILDREN ;
  ZMW_NB_OF_CHILDREN = j ;
}

void zmw_children_restore_not_used()
{
  Zmw_Size *s ;
  int i, j ;
	
  s = ZMW_CHILDREN ;
  ZMW_CHILDREN = global_children ;
  global_children = s ;
  ZMW_NB_OF_CHILDREN = global_nb_children ;	

  for(i=0, j=0; i<ZMW_NB_OF_CHILDREN; i++)
    if ( ZMW_CHILDREN[i].used_to_compute_parent_size )
      ZMW_CHILDREN[i].allocated = global_children[j++].allocated ;

}

int zmw_search_column(int nb_cols, int *col_width)
{
  int i, x ;
	
  x = ZMW_SIZE_ALLOCATED.x + ZMW_TABLE_VERTICAL_WIDTH ;
  for(i=0; i<nb_cols; i++)
    {
      x += col_width[i] + ZMW_TABLE_VERTICAL_WIDTH ;
      if ( ZMW_ABS(x - zmw.x) < 5 )
	{
	  zmw_printf("x=%d zmw.x=%d i=%d\n", x, zmw.x, i) ;
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

  y = ZMW_SIZE_ALLOCATED.y ;
  for(i=0; i<*nb_rows; i++ )
    {
      if ( zmw.y > y && zmw.y < y + height[i] )
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
      y = ZMW_SIZE_ALLOCATED.y ;
      for(i=0; i<ZMW_NB_OF_CHILDREN; i += nb_cols)
   	{
	  if ( selection[i/nb_cols] )
	    zmw_draw_rectangle(Zmw_Color_Background_Pushed, Zmw_True
			       , ZMW_SIZE_ALLOCATED.x
			       , y
			       , ZMW_SIZE_ALLOCATED.width
			       , height[i/nb_cols]
			       ) ;
   			
	  y += height[i/nb_cols] ;
   	}
    }
/*
 * Vertical column separator
 */
  x = ZMW_SIZE_ALLOCATED.x ;
  for(i=0; i<=nb_cols; i++)
    {
      zmw_draw_line(Zmw_Color_Foreground
		    , x /* + 1 */
		    , ZMW_SIZE_ALLOCATED.y
		    , x /* + 1 */
		    , ZMW_SIZE_ALLOCATED.y + ZMW_SIZE_ALLOCATED.height
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
	
  switch( ZMW_SUBACTION )
    {
    case Zmw_Compute_Required_Size:
      zmw_children_remove_not_used() ;
      zmw_table_compute_required_size(nb_cols, col_width) ;
      zmw_children_restore_not_used() ;
      if ( col_width )
	ZMW_SIZE_SENSIBLE = 1 ;
      break ;

    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:
    case Zmw_Compute_Children_Allocated_Size:
      zmw_children_remove_not_used() ;
      zmw_table_compute_allocated_size(nb_cols, col_width, &width, &height) ;
      if ( ZMW_SUBACTION == Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing )
      		zmw_table_draw(nb_cols, width, height, selection) ;
      zmw_children_restore_not_used() ;
      break ;
      
    case Zmw_Input_Event:

      zmw_activable() ;


      if ( dragged >= 0 && zmw_button_released_anywhere() )
	{
	  dragged = -1 ;
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
		      x = zmw.x ;
		      width_orig = col_width[dragged] ;
		    }
		}
	      else
		{
		  ZMW_SIZE_ACTIVATED = Zmw_True ;
		  col_width[dragged] = width_orig + (zmw.x - x) ;
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


