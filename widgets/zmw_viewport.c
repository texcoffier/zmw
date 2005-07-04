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
#include "zmw/zmw_private.h" /* This include is only here for speed up */


#define P 18

int zmw_scrollbar_2_viewport(Zmw_Float_0_1 sb_pos, int sb_size, int vp_size)
{
  return ( sb_pos * vp_size ) ;
}

Zmw_Float_0_1 zmw_scrollbar_size(int sb_size, int vp_size)
{
  Zmw_Float_0_1 size ;

  sb_size -= 2 * zmw_border_width_get() ;	
  size = sb_size / (float)vp_size ;
  if ( size > 1 )
    size = 1 ;
  return( size ) ;
}

/* Not really useful, it returns always 0, 1 and 2 */

void zmw_viewport_get_children(int *v_sb, int *h_sb, int *vp)
{
  int i ;
  
  for(i=0; i<zmw_nb_of_children_get(); i++)
    if ( zmw_child__used_by_parent_get(i) )
      {
	*v_sb = i++ ;
	for(; i<zmw_nb_of_children_get(); i++)
	  if ( zmw_child__used_by_parent_get(i) )
	    {
	      *h_sb = i++ ;
	      for(; i<zmw_nb_of_children_get(); i++)
		if ( zmw_child__used_by_parent_get(i) )
		  {
		    *vp = i++ ;
		    return ;
		  }
	    }
      }
  ZMW_ABORT ;
}

void zmw_scrollbar_needed(Zmw_Boolean *x, Zmw_Boolean *y,
		int v_sb, int h_sb, int vp)
{
  *x = zmw_allocated_width_get()
    < zmw_child__required_width_get(vp) + 2*zmw_child__padding_width_get(vp) + P ;
  *y = zmw_allocated_height_get()
    < zmw_child__required_height_get(vp) + 2*zmw_child__padding_width_get(vp) + P ;

  if ( ! *x )
    zmw_child__invisible_set(h_sb,Zmw_True) ;
  if ( ! *y )
    zmw_child__invisible_set(v_sb,Zmw_True) ;	
}	
 

void zmw_viewport(Zmw_Float_0_1 x, Zmw_Float_0_1 y, Zmw_Float_0_1 *x_size, Zmw_Float_0_1 *y_size, Zmw_Rectangle *r)
{
  Zmw_Boolean x_sb, y_sb ;
  int v_sb, h_sb, vp ; // Indexes in CHILDREN list

  if ( zmw_nb_of_children_get() == 3 )
    zmw_viewport_get_children(&v_sb, &h_sb, &vp) ;
  else
    {
      if ( zmw_call_number_get() != 0 )
	{
	  zmw_printf("nb=%d\n", zmw_nb_of_children_get()) ;
	  zmw_debug_trace() ;
	}
    }

  switch( zmw_subaction_get() )
    {
    case Zmw_Compute_Required_Size:
      zmw_min_width_set(4*P) ;
      zmw_min_height_set(4*P) ;
      break ;

    case Zmw_Compute_Children_Allocated_Size:
    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:

      zmw_scrollbar_needed(&x_sb, &y_sb, v_sb, h_sb, vp) ;

      if ( zmw_nb_of_children_get() != 3 )
	ZMW_ABORT ;
	

      zmw_child__allocated_x_set(v_sb,zmw_allocated_x_get()
	+ zmw_allocated_width_get()
	+ zmw_child__padding_width_get(v_sb)
	- P) ;
      zmw_child__allocated_y_set(v_sb,zmw_allocated_y_get() + zmw_child__padding_width_get(v_sb)) ;
      zmw_child__allocated_width_set(v_sb,P - 2*zmw_child__padding_width_get(v_sb)) ;
      zmw_child__allocated_height_set(v_sb,zmw_allocated_height_get() - P*x_sb - 2*zmw_child__padding_width_get(v_sb)) ;
      
      zmw_child__allocated_y_set(h_sb,zmw_allocated_y_get()
	+ zmw_allocated_height_get()
	+ zmw_child__padding_width_get(h_sb)
	- P) ;
      zmw_child__allocated_x_set(h_sb,zmw_allocated_x_get() + zmw_child__padding_width_get(h_sb)) ;
      zmw_child__allocated_height_set(h_sb,P - 2*zmw_child__padding_width_get(h_sb)) ;
      zmw_child__allocated_width_set(h_sb,zmw_allocated_width_get() - P*y_sb - 2*zmw_child__padding_width_get(h_sb)) ;

           

      zmw_child__allocated_width_set(vp,zmw_allocated_width_get() - P*y_sb - 2*zmw_child__padding_width_get(vp)) ;
      zmw_child__allocated_height_set(vp,zmw_allocated_height_get() - P*x_sb - 2*zmw_child__padding_width_get(vp)) ;
      zmw_child__allocated_x_set(vp,zmw_allocated_x_get() + zmw_child__padding_width_get(vp)) ;
      zmw_child__allocated_y_set(vp,zmw_allocated_y_get() + zmw_child__padding_width_get(vp)) ;

      *x_size = zmw_scrollbar_size(zmw_allocated_width_get() - P*y_sb
				   , zmw_child__required_width_get(vp) + 2*zmw_child__padding_width_get(vp)) ;
      *y_size = zmw_scrollbar_size(zmw_allocated_height_get() - P*x_sb
				   , zmw_child__required_height_get(vp) + 2*zmw_child__padding_width_get(vp)) ;
      
      r->x =
	- zmw_scrollbar_2_viewport(x
				   ,zmw_child__required_width_get(h_sb) + 2*zmw_child__padding_width_get(h_sb)
				   ,zmw_child__required_width_get(vp) + 2*zmw_child__padding_width_get(vp)) ;
      
      r->y =
	- zmw_scrollbar_2_viewport(y
				   ,zmw_child__required_height_get(v_sb) + 2*zmw_child__padding_width_get(v_sb)
				   ,zmw_child__required_height_get(vp) + 2*zmw_child__padding_width_get(vp)) ;

      break ;
    case Zmw_Input_Event:
      zmw_scrollbar_needed(&x_sb, &y_sb, v_sb, h_sb, vp) ;
      break ;
	
    case Zmw_Post_Drawing:    
      zmw_border_draw(Zmw_Border_Relief) ;
      break ;
    default:
      break ;
    }
}

/*
 * Should save the state value for expand?
 * All these complex things are done because ZMW_EXTERNAL
 * lost all local variables.
 */
void zmw_viewport_with_scrollbar(Zmw_Float_0_1 *x, Zmw_Float_0_1 *y)
{
  static Zmw_Float_0_1 x_size[9], y_size[9] ; /* XXX FIXME */
  static Zmw_Rectangle r[9] ; /* XXX FIXME */
  static int activated[9] ;
  static int depth = 0 ;

  ZMW_EXTERNAL_RESTART ;

  activated[depth] = 0 ;
  ZMW(zmw_viewport(*x, *y, &x_size[depth], &y_size[depth], &r[depth]))
    {
      zmw_name("ZMW_VP_SBV") ;
      zmw_vscrollbar(y, y_size[depth]) ;
      activated[depth] |= zmw_activated() ;
      zmw_name("ZMW_VP_SBH") ;
      zmw_hscrollbar(x, x_size[depth]) ;
      activated[depth] |= zmw_activated() ;

      zmw_name("ZMW_VP_EX") ;
      ZMW(zmw_decorator(Zmw_Decorator_Clip|Zmw_Decorator_Translate, r[depth].x, r[depth].y))
	{
	  depth++ ;
	  ZMW_EXTERNAL ;
	  depth-- ;
	}
    }   
  zmw_activated_set(activated[depth]) ;
  ZMW_EXTERNAL_STOP ;
}

