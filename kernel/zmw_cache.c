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

#include "zmw/zmw.h"


static int global_cache_test = 0 ;
static int global_cache_miss = 0 ;


static struct cached_data {
  int nb ;
  int index ;
  Zmw_Size r ;
  char *name ;
} *global_cache_table = NULL ;

static int global_cache_size = 0 ; 

#define ZMW_PRINTF if (0) zmw_printf

void zmw_cache_init(int size)
{
  int i ;

  ZMW_PRINTF("Cache init : %d\n", size) ;
  if ( size != global_cache_size )
    {
      ZMW_REALLOC(global_cache_table, size) ;
      for(i=global_cache_size; i<size; i++)
	{
	  global_cache_table[i].index = 0 ;
	  global_cache_table[i].name = NULL ;
	}
      global_cache_size = size ;

    }
}

void zmw_cache_free()
{
  int i ;

  if ( global_cache_size )
    {
      fprintf(stderr, "Size cache miss=%g%%\n"
	      , 100*(global_cache_miss/(float)global_cache_test)
	      ) ;

      for(i=0; i<global_cache_size; i++)
	if ( global_cache_table[i].name )
	  ZMW_FREE(global_cache_table[i].name) ;

      free(global_cache_table) ;
    }
}

/*
 * Returns 0 if the size is in the cache
 */

int zmw_cache_get_size_real(Zmw_Size *r)
{
  struct cached_data *cd ;

  ZMW_PRINTF("get size from cache of : [%d]%s\n", ZMW_INDEX, zmw_name_full) ;

  if ( global_cache_size )
    {
      global_cache_test++ ;
      cd = &global_cache_table[ZMW_INDEX%global_cache_size] ;

      if ( cd->index == ZMW_INDEX )
	{
	  *r = cd->r ;
	  if ( zmw.debug & Zmw_Debug_Cache_Fast )
	    if ( !zmw.event_removed && cd->name && strcmp(cd->name, zmw_name_full) )
	      {
		zmw_debug_trace() ;
		zmw_printf("real name = %s\n", zmw_name_full) ;
		zmw_printf("index = %d\n", ZMW_INDEX) ;
		zmw_printf("stored name = %s\n", cd->name) ;
	      }
	  ZMW_PRINTF("Size in cache : %s\n", zmw_size_string(r)) ;
	  return(0) ;
	}
     global_cache_miss++ ;
    }
  ZMW_PRINTF("Size NOT in cache\n") ;
  return(1) ;
}

int zmw_cache_get_size(Zmw_Size *r)
{
  if ( zmw.debug & Zmw_Debug_Cache_Slow )
    return 1 ;

  return zmw_cache_get_size_real(r) ;
}

void zmw_cache_set_size_real(const Zmw_Size *r)
{
  struct cached_data *cd ;

  if ( global_cache_size )
    {
      cd = &global_cache_table[ZMW_INDEX % global_cache_size] ;
      
      cd->r = *r ;
      cd->index = ZMW_INDEX ;
      ZMW_PRINTF("zmw_cache_set_size_real [%d]%s : %s\n",
		 ZMW_INDEX, zmw_name_full, zmw_size_string(r)) ;
      if ( zmw.debug & Zmw_Debug_Cache_Fast )
	{
	  ZMW_PRINTF("Update name\n") ;
	  if ( cd->name )
	    free(cd->name) ;
	  cd->name = strdup(zmw_name_full) ;
	}
      else
	cd->name = NULL ; // unecessary ??
    }
}

static void zmw_cache_check(const Zmw_Size *s)
{
  Zmw_Size ss ;
  char *p ;

  if ( zmw_cache_get_size_real(&ss) )
    zmw_cache_set_size_real(s) ;
  else
    {
      /*
      if ( !s->used_to_compute_parent_size )
	return ;
      */
      p = NULL ;
      if ( s->index != ss.index )
	p = "index" ;
      if ( memcmp( &s->min, &ss.min, sizeof(s->min) ) )
	p = "min size" ;
      if ( memcmp( &s->required, &ss.required, sizeof(s->required) ) )
	p = "required size" ;
      if ( p )
	{
	  if ( zmw.event_removed )
	    {
	      static int hide = 0 ;

	      if ( !hide )
		{
		  zmw_printf("Warning: size change while dispatching an event\n") ;
		  hide = 1 ;
		}
	      return ;
	    }
	  zmw_debug_trace() ;
	  zmw_printf("%s\n", zmw_name_full) ;
	  zmw_printf("[%d] Problems in cache_check : %s\n", ZMW_INDEX, p) ;
	  zmw_printf("Stored = %s !\n",zmw_size_string(&ss)) ;
	  zmw_printf("Comput = %s !\n",zmw_size_string(s)) ;
	  if ( !s->used_to_compute_parent_size )
	    zmw_printf("Not used to compute size !\n") ;
	  exit(1) ;
	}
    }		
}

void zmw_cache_set_size(const Zmw_Size *r)
{
  ZMW_PRINTF("set size of [%d]%s to %s\n", ZMW_INDEX, zmw_name_full, zmw_size_string(r)) ;

  if ( zmw.debug & Zmw_Debug_Cache_Slow )
    zmw_cache_check(r) ;
  else
    zmw_cache_set_size_real(r) ;
}
