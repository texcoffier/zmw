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


#define P 18

int zmw_scrollbar_2_viewport(Zmw_Float_0_1 sb_pos, int sb_size, int vp_size)
{
  return ( sb_pos * vp_size ) ;
}

Zmw_Float_0_1 zmw_scrollbar_size(int sb_size, int vp_size)
{
  Zmw_Float_0_1 size ;

  sb_size -= 2 * ZMW_BORDER_WIDTH ;	
  size = sb_size / (float)vp_size ;
  if ( size > 1 )
    size = 1 ;
  return( size ) ;
}

/* Not really useful, it returns always 0, 1 and 2 */

void zmw_viewport_get_children(int *v_sb, int *h_sb, int *vp)
{
  int i ;
  
  for(i=0; i<ZMW_NB_OF_CHILDREN; i++)
    if ( ZMW_CHILDREN[i].used_to_compute_parent_size )
      {
	*v_sb = i++ ;
	for(; i<ZMW_NB_OF_CHILDREN; i++)
	  if ( ZMW_CHILDREN[i].used_to_compute_parent_size )
	    {
	      *h_sb = i++ ;
	      for(; i<ZMW_NB_OF_CHILDREN; i++)
		if ( ZMW_CHILDREN[i].used_to_compute_parent_size )
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
  *x = ZMW_SIZE_ALLOCATED.width
    < ZMW_CHILDREN[vp].required.width + 2*ZMW_CHILDREN[vp].padding_width + P ;
  *y = ZMW_SIZE_ALLOCATED.height
    < ZMW_CHILDREN[vp].required.height + 2*ZMW_CHILDREN[vp].padding_width + P ;

  if ( ! *x )
    ZMW_CHILDREN[h_sb].invisible = Zmw_True ;
  if ( ! *y )
    ZMW_CHILDREN[v_sb].invisible = Zmw_True ;	
}	
 

void zmw_viewport(Zmw_Float_0_1 x, Zmw_Float_0_1 y, Zmw_Float_0_1 *x_size, Zmw_Float_0_1 *y_size, Zmw_Rectangle *r)
{
  Zmw_Boolean x_sb, y_sb ;
  int v_sb, h_sb, vp ; // Indexes in CHILDREN list

  if ( ZMW_NB_OF_CHILDREN == 3 )
    zmw_viewport_get_children(&v_sb, &h_sb, &vp) ;


  switch( ZMW_SUBACTION )
    {
    case Zmw_Compute_Required_Size:
      ZMW_SIZE_MIN.width = 4*P ;
      ZMW_SIZE_MIN.height = 4*P ;
      break ;

    case Zmw_Compute_Children_Allocated_Size:
    case Zmw_Compute_Children_Allocated_Size_And_Pre_Drawing:

      zmw_scrollbar_needed(&x_sb, &y_sb, v_sb, h_sb, vp) ;

      if ( ZMW_NB_OF_CHILDREN != 3 )
	ZMW_ABORT ;
	

      ZMW_CHILDREN[v_sb].allocated.x = ZMW_SIZE_ALLOCATED.x
	+ ZMW_SIZE_ALLOCATED.width
	+ ZMW_CHILDREN[v_sb].padding_width
	- P ;
      ZMW_CHILDREN[v_sb].allocated.y = ZMW_SIZE_ALLOCATED.y + ZMW_CHILDREN[v_sb].padding_width ;
      ZMW_CHILDREN[v_sb].allocated.width = P - 2*ZMW_CHILDREN[v_sb].padding_width ;
      ZMW_CHILDREN[v_sb].allocated.height = ZMW_SIZE_ALLOCATED.height - P*x_sb - 2*ZMW_CHILDREN[v_sb].padding_width ;
      
      ZMW_CHILDREN[h_sb].allocated.y = ZMW_SIZE_ALLOCATED.y
	+ ZMW_SIZE_ALLOCATED.height
	+ ZMW_CHILDREN[h_sb].padding_width
	- P ;
      ZMW_CHILDREN[h_sb].allocated.x = ZMW_SIZE_ALLOCATED.x + ZMW_CHILDREN[h_sb].padding_width ;
      ZMW_CHILDREN[h_sb].allocated.height = P - 2*ZMW_CHILDREN[h_sb].padding_width ;
      ZMW_CHILDREN[h_sb].allocated.width = ZMW_SIZE_ALLOCATED.width - P*y_sb - 2*ZMW_CHILDREN[h_sb].padding_width ;

           

      ZMW_CHILDREN[vp].allocated.width = ZMW_SIZE_ALLOCATED.width - P*y_sb - 2*ZMW_CHILDREN[vp].padding_width ;
      ZMW_CHILDREN[vp].allocated.height = ZMW_SIZE_ALLOCATED.height - P*x_sb - 2*ZMW_CHILDREN[vp].padding_width ;
      ZMW_CHILDREN[vp].allocated.x = ZMW_SIZE_ALLOCATED.x + ZMW_CHILDREN[vp].padding_width ;
      ZMW_CHILDREN[vp].allocated.y = ZMW_SIZE_ALLOCATED.y + ZMW_CHILDREN[vp].padding_width ;

      *x_size = zmw_scrollbar_size(ZMW_SIZE_ALLOCATED.width - P*y_sb
				   , ZMW_CHILDREN[vp].required.width + 2*ZMW_CHILDREN[vp].padding_width) ;
      *y_size = zmw_scrollbar_size(ZMW_SIZE_ALLOCATED.height - P*x_sb
				   , ZMW_CHILDREN[vp].required.height + 2*ZMW_CHILDREN[vp].padding_width) ;
      
      r->x =
	- zmw_scrollbar_2_viewport(x
				   ,ZMW_CHILDREN[h_sb].required.width + 2*ZMW_CHILDREN[h_sb].padding_width
				   ,ZMW_CHILDREN[vp].required.width + 2*ZMW_CHILDREN[vp].padding_width) ;
      
      r->y =
	- zmw_scrollbar_2_viewport(y
				   ,ZMW_CHILDREN[v_sb].required.height + 2*ZMW_CHILDREN[v_sb].padding_width
				   ,ZMW_CHILDREN[vp].required.height + 2*ZMW_CHILDREN[vp].padding_width) ;

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
 * Should save the state value for expand
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
      zmw_scrollbar_vertical(y, y_size[depth]) ;
      activated[depth] |= zmw_activated() ;
      zmw_name("ZMW_VP_SBH") ;
      zmw_scrollbar_horizontal(x, x_size[depth]) ;
      activated[depth] |= zmw_activated() ;

      zmw_name("ZMW_VP_EX") ;
      ZMW(zmw_decorator(Zmw_Decorator_Clip|Zmw_Decorator_Translate, r[depth].x, r[depth].y))
	{
	  depth++ ;
	  ZMW_EXTERNAL ;
	  depth-- ;
	}
    }   
  ZMW_SIZE_ACTIVATED = activated[depth] ;
  ZMW_EXTERNAL_STOP ;
}

