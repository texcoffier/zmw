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

#define PRINTF if(0) zmw_printf

char* zmw_name_copy()
{
  return( strdup( zmw_name_full ) ) ;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 */




static Zmw_Name *global_registered[100] ;
static int global_nb_registered = 0 ;

/*
 * Return True on find.
 * The index returned the insertion position.
 * If the name is found, the result is the minimum index in
 * case of multiple answers
 */

static Zmw_Boolean zmw_name_find(const char *name, int *index)
{
  int min, max, middle, cmp ;

  min = 0 ;
  max = global_nb_registered - 1 ;
  while( min <= max )
    {
      middle = (min+max)/2 ;
      cmp = strcmp( global_registered[middle]->name, name ) ;
      
      if ( cmp == 0 )
	{
	  while( middle>0
		 && strcmp( global_registered[middle-1]->name, name) == 0 )
	    middle-- ;
	  *index = middle ;
	  PRINTF("Name find: return true\n", name) ;
	  return(Zmw_True) ;
	}
      if ( cmp < 0 )
	min = middle + 1 ;
      else
	max = middle - 1 ;
    }
  *index = min ;
  PRINTF("Name find: return false\n", name) ;
  return(Zmw_False) ;
}


void zmw_name_register(Zmw_Name *n)
{
  int i ;

  PRINTF("Name_register: %s\n", zmw_name_full) ;
  zmw_name_unregister(n) ;

  n->name = strdup(zmw_name_full) ;
  n->index = 0 ;
  zmw_name_find(n->name, &i) ;
  global_nb_registered++ ;
  memmove(&global_registered[i+1]
	  , &global_registered[i]
	  , (global_nb_registered-i-1)*sizeof(global_registered[0])
	  ) ;
  global_registered[i] = n ;  
}

void zmw_name_unregister(Zmw_Name *n)
{
  int i ;

  if ( n->name )
    {
      PRINTF("Name_unregister: %s\n", n->name) ;
      free( n->name ) ;
      n->name = NULL ;
      n->index = 0 ;

      for(i=0 ;i<global_nb_registered; i++)
	if ( global_registered[i] == n )
	  {
	    memmove(&global_registered[i]
		    , &global_registered[i+1]
		    , (global_nb_registered-i-1)*sizeof(global_registered[0])
		    ) ;
	    global_nb_registered-- ;
	    break ;
	  }
    }
}

/*
 * Should be optimized using a name tree
 */

void zmw_name_update()
{
  int i ;
  const char *name ;

  if ( global_nb_registered )
    {
      name = zmw_name_full ;
      /*
      for(i=0 ;i<global_nb_registered; i++)
	if ( global_registered[i]->index == 0 )
	  {
	    if ( strcmp(global_registered[i]->name, name) == 0 )
	      global_registered[i]->index = ZMW_INDEX ;
	  }
      */
      if ( zmw_name_find(name, &i) )
	{
	  do
	    {
	      global_registered[i]->index = ZMW_INDEX ;
	      PRINTF("Name_update %p(%s) %s ==> %d\n"
		     , global_registered[i]
		     , global_registered[i]->name
		     , name, ZMW_INDEX) ;
	      i++ ;
	    }
	  while( i < global_nb_registered
		 && strcmp(global_registered[i]->name, name) == 0 ) ;
	}
    }
}

void zmw_name_init()
{
  int i ;

  PRINTF("Name_init all indexes to 0\n") ;
  for(i=0 ;i<global_nb_registered; i++)
    global_registered[i]->index = 0 ;
}

/*
 *
 */

/*
int zmw_name_is(Zmw_Name *n)
{
  int r, s ;

  r = n->index == ZMW_INDEX ;
  if ( n->name )
    s = !strcmp(n->name, zmw_name_full(zMw)) ;
  else
    s = 0 ;

  if ( r != s )
    {
      printf("--------------------\n"
	     "n->index = %d    ZMW_INDEX = %d\n", n->index, ZMW_INDEX) ;
      printf("n->name = %s\n", n->name) ;
      printf("name_full = %s\n",zmw_name_full(zMw)) ;
      return(s) ;
    }
  else
    return(r) ;
}
*/

Zmw_Boolean zmw_name_is(const Zmw_Name *n)
{
  PRINTF("Name_is %p %s,%d = %s,%d\n"
	 , n, n->name, n->index, zmw_name_full, ZMW_INDEX) ;
  if ( ZMW_ACTION == zmw_action_compute_required_size )
    {
      if ( n->name )
	return( !strcmp(n->name, zmw_name_full) ) ;
      return(0) ;
    }
  else
    return( n->index == ZMW_INDEX ) ;
}


static Zmw_Boolean zmw_name_string_contains(const Zmw_Name *child)
{
  int length ;
  const char *parent ;

  if ( child->name == NULL )
    return(0) ;
  
  PRINTF("Name_string contains child->name=%s parent=%s\n"
	 , child->name, zmw_name_full) ;

  parent = zmw_name_full ;
  length = strlen(parent) ;
          
  if ( strncmp(parent, child->name, length) == 0 )
    {
      if ( child->name[length] != '.' )
        return(1) ;
    }
  return(0) ;
}

Zmw_Boolean zmw_name_contains(const Zmw_Name *n)
{
  if ( ZMW_ACTION == zmw_action_compute_required_size )
    {
      return( zmw_name_string_contains(n) ) ;
    }
  else
    {
      return(
	     n->index >= ZMW_INDEX
	     && n->index < zMw[-1].u.children[zMw[-1].u.nb_of_children].index
	     ) ;
    }
}

/*
 * This is complete garbage.
 * This code works only if the user doesn't change the name
 * of the next widget.
 */
static Zmw_Boolean zmw_name_next_string_contains(const Zmw_Name *child)
{
  int i ;

  PRINTF("zmw_name_next_string_contains %s\n", child->name) ;

  zmw_increment_index() ;
  i = zmw_name_contains(child) ;
  zmw_decrement_index() ;
  return(i) ;
}

Zmw_Boolean zmw_name_next_contains(const Zmw_Name *n)
{
  if ( zMw[-1].i.action == zmw_action_compute_required_size )
    {
      return( zmw_name_next_string_contains(n) ) ;
    }
  else
    {
      return( zmw_name_next_string_contains(n) ) ;
      return(
	     n->index >= ZMW_INDEX
	     && n->index < zMw[-1].u.children[zMw[-1].u.nb_of_children+1].index
	     ) ;
    }
}


/*
 *
 */

void zmw_name_dump(FILE *f)
{
  int i ;

  fprintf(f, "<H2>Names</H2>\n") ;

  for(i=0 ;i<global_nb_registered; i++)
    fprintf(f, "%d %s %s<BR>\n"
	    , global_registered[i]->index
	    , global_registered[i]->why
	    , global_registered[i]->name
	    ) ;
}


void zmw_name_free()
{
  int i ;

  for(i=0 ;i<global_nb_registered; i++)
    if ( global_registered[i]->name )
      ZMW_FREE(global_registered[i]->name) ;
}







