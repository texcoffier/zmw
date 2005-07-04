/*
    ZMW: A Zero Memory Widget Library
    Copyright (C) 2002-2005 Thierry EXCOFFIER, Université Claude Bernard, LIRIS

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
#include "zmw/zmw_private.h" /* Need fast access to "Zmw_Child" */

static int global_cache_test = 0 ;
static int global_cache_miss = 0 ;

static struct cached_data {
  Zmw_Hash hash ;
  Zmw_Child r ;
  char *name ;
  int age ;
} *global_cache_table = NULL ;

static int global_cache_size = 0 ; 
static int global_cache_age = 0 ;

#define ZMW_PRINTF if (0) zmw_printf


const char* zmw_child_string(const Zmw_Child *s)
{
  static char buf[200] ;

  sprintf(buf
	  ,"Min %3dx%-3d %3d,%-3d Req%3dx%-3d %3d,%-3d All%3dx%-3d %3d,%-3d INDEX=%6d %s%s%s%s%s%s%s%s"
	  ,s->min.width
	  ,s->min.height
	  ,s->min.x
	  ,s->min.y
	  ,s->required.width
	  ,s->required.height
	  ,s->required.x
	  ,s->required.y
	  ,s->allocated.width
	  ,s->allocated.height
	  ,s->allocated.x
	  ,s->allocated.y
	  ,s->hash_key
	  ,s->current_state.horizontal_expand ? " HE" : ""
	  ,s->current_state.vertical_expand ? " VE" : ""
	  ,s->current_state.horizontal_alignment ? "HA" : ""
	  ,s->current_state.vertical_alignment ? "VA" : ""
	  ,s->used_by_parent ? " Used" : ""
	  ,s->invisible ? " Inv" : ""
	  ,s->sensitived ? " Sens" : ""
	  ,s->pass_through ? " PT" : ""
	  ) ;
  return(buf) ;
}

/*
 * Make the cache bigger and empty it
 */
void zmw_child_cache_init(int size)
{
  int i ;

  ZMW_PRINTF("Cache init : %d\n", size) ;

  if ( size != global_cache_size )
    {
      ZMW_REALLOC(global_cache_table, size) ;
      for(i=global_cache_size; i<size; i++)
	{
	  global_cache_table[i].name = NULL ;
	  global_cache_table[i].age = 0 ;
	  global_cache_table[i].hash = 0 ;
	}
      global_cache_size = size ;
    }
  global_cache_age++ ;
}

int zmw_child_cache_size()
{
  return global_cache_size ;
}

void zmw_child_cache_free()
{
  int i ;

  if ( global_cache_size )
    {
      fprintf(stderr, "Size cache miss=%g%%\n"
	      , 100*(global_cache_miss/(float)global_cache_test)
	      ) ;
      for(i=0; i<global_cache_size; i++)
	free(global_cache_table[i].name) ;

      free(global_cache_table) ;
    }
}

/*
 * Returns 0 if the size is in the cache
 */
int zmw_child_cache_get_size_real(Zmw_Child *r)
{
  struct cached_data *cd ;

  ZMW_PRINTF("get size from cache of : %s\n", zmw_name_full) ;
  if ( global_cache_size )
    {
      global_cache_test++ ;
      cd = &global_cache_table[zmw_hash_key_get() % global_cache_size] ;

      if ( cd->hash == zmw_hash_key_get()
	   && cd->age == global_cache_age
	   &&  cd->name && strcmp(cd->name, zmw_name_full) == 0 )
	{
	  *r = cd->r ;
	  ZMW_PRINTF("Size in cache : %s\n", zmw_child_string(r)) ;
	  return(0) ;
	}
      global_cache_miss++ ;
    }
  ZMW_PRINTF("Size NOT in cache\n") ;
  return(1) ;
}

int zmw_child_cache_get_size(Zmw_Child *r)
{
  if ( zmw_debug_get() & Zmw_Debug_Cache_Slow )
    return 1 ;

  return zmw_child_cache_get_size_real(r) ;
}

void zmw_child_cache_set_size_real(const Zmw_Child *r)
{
  struct cached_data *cd ;

  if ( global_cache_size )
    {
      cd = &global_cache_table[zmw_hash_key_get() % global_cache_size] ;
      
      cd->r = *r ;
      cd->hash = zmw_hash_key_get() ;
      cd->age = global_cache_age ;
      ZMW_PRINTF("zmw_child_cache_set_size_real %s : %s\n",
		 zmw_name_full, zmw_child_string(r)) ;
      zmw_string_copy(&cd->name, zmw_name_full) ;
    }
}

static void zmw_child_cache_check(const Zmw_Child *s)
{
  Zmw_Child ss ;
  char *p ;

  if ( zmw_child_cache_get_size_real(&ss) )
    zmw_child_cache_set_size_real(s) ;
  else
    {
      if ( !s->used_by_parent && !ss.used_by_parent)
	return ;
      

      p = NULL ;
      if ( s->hash_key != ss.hash_key )
	p = "index" ;
      if ( memcmp( &s->min, &ss.min, sizeof(s->min) ) )
	p = "min size" ;
      if ( memcmp( &s->required, &ss.required, sizeof(s->required) ) )
	p = "required size" ;
      if ( p )
	{
	  if ( zmw_zmw_event_removed_get() )
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
	  zmw_printf("Problems in cache_check : %s\n", p) ;
	  zmw_printf("Stored = %s !\n",zmw_child_string(&ss)) ;
	  zmw_printf("Comput = %s !\n",zmw_child_string(s)) ;
	  if ( !s->used_by_parent )
	    zmw_printf("Not used to compute size !\n") ;
	  exit(1) ;
	}
    }		
}

void zmw_child_cache_set_size(const Zmw_Child *r)
{
  ZMW_PRINTF("set size of %s to %s\n", zmw_name_full, zmw_child_string(r)) ;

  if ( zmw_debug_get() & Zmw_Debug_Cache_Slow )
    zmw_child_cache_check(r) ;
  else
    zmw_child_cache_set_size_real(r) ;
}
