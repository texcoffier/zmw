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

#ifndef zmw_cache_init

#define ZMW_DEBUG_CACHE 0

static struct cached_data {
  int nb ;
  int index ;
  Zmw_Size r ;
#if ZMW_DEBUG_CACHE
  Zmw_Name name ;
#endif
} *global_cache_table = NULL ;

static int global_cache_size = 0 ; 

void zmw_cache_init(int size)
{
  if ( size != global_cache_size )
    {
      ZMW_REALLOC(global_cache_table, size) ;
      global_cache_size = size ;
    }
}
    


int zmw_cache_get_size(Zmw_Size *r)
{
  struct cached_data *cd ;

  if ( global_cache_size )
    {
      cd = &global_cache_table[ZMW_INDEX%global_cache_size] ;

      if ( cd->index == ZMW_INDEX )
	{
	  *r = cd->r ;
#if ZMW_DEBUG_CACHE
	  if ( !zmw_name_is(cd->name) )
	    {
	      zmw_printf("real name = %s\n", zmw_name_full(zMw)) ;
	      zmw_printf("index = %d\n", ZMW_INDEX) ;
	      zmw_printf("stored name = %s\n", cd->name) ;
	    }
#endif
	  return(0) ;
	}
    }
  return(1) ;
}

void zmw_cache_set_size(const Zmw_Size *r)
{
  struct cached_data *cd ;

  if ( global_cache_size )
    {
      cd = &global_cache_table[ZMW_INDEX % global_cache_size] ;
      
      cd->r = *r ;
      cd->index = ZMW_INDEX ;
#if ZMW_DEBUG_CACHE
      cd->name = zmw_name_copy() ;
#endif
    }
}

#endif
