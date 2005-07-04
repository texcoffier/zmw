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

The table contains pointers on Zmw_Name lists with
the same hash key.



About memory usage.

Zmw_Name.why  is never allocated by malloc, so no to be freed.
Zmw_Name.name is always allocated by malloc
Zmw_Name      is allocated if it is not a Registration

*/

#include <ctype.h>
#include "zmw/zmw.h"
#include "zmw/zmw_private.h"

#define PRINTF if(0) fprintf(stderr,"%-25s ", __FUNCTION__),zmw_printf

/*
 ******************************************************************************
 *
 ******************************************************************************
 */

static Zmw_Name **global_registered = NULL ;
static int global_nb_registered = 0 ;
static int global_nb_name = 0 ;

/*
 * Initialize or enlarge the hash table
 */
static void zmw_name_init()
{
  int i, j, old_nb ;
  Zmw_Name **n, *move ;

  old_nb = global_nb_registered ;
  global_nb_registered = 1.5*global_nb_registered + 13 ;

  PRINTF("global_nb_registered = %d\n", global_nb_registered) ;

  ZMW_REALLOC(global_registered, global_nb_registered) ;
  for(i=old_nb; i<global_nb_registered; i++)
    global_registered[i] = NULL ;

  for(i=0; i<old_nb; i++)
    {
      for(n = &global_registered[i] ; *n ; )
	{
	  j = (*n)->hash % global_nb_registered ;
	  if ( i != j )
	    {
	      /* remove */
	      move = *n ;
	      *n = move->next ;
	      /* add */
	      move->next = global_registered[j] ;
	      global_registered[j] = move ;
	    }
	  else
	    {
	      n = &(*n)->next ;
	    }
	}
    }
}

static Zmw_Boolean zmw_name_equals(const Zmw_Name *n, Zmw_Hash h, const char *name)
{
  PRINTF("%d %s %d %s\n", n->hash, n->name,h,name) ; 
  if ( n->hash == h )
    return strcmp(n->name, name) == 0 ;

  return Zmw_False ;
}

static Zmw_Hash zmw_name_hash(const char *name)
{
  PRINTF("name=%s\n", name) ;
  if ( name == zmw_name_full && zmw_child_number_get() >= 0 )
    return zmw_hash_key_get() ;
  else
    return zmw_hash(0, name) ;
}

/*
 * Returns a pointer to the first Zmw_Name in the list to be ok.
 * It is a pointer in order to know where to add first list item
 */

static Zmw_Name** zmw_name_find_with_hash(const char *name
					  , const char *why
					  , Zmw_Hash h
					  )
{
  Zmw_Name **n ;

  PRINTF("name=%s why=%s hash=%d\n", name, why, h) ;

  if ( global_registered == NULL )
    zmw_name_init() ;
  
  for( n = & global_registered[ h % global_nb_registered ] ;
       *n ;
       n = &(*n)->next
       )
    {
      if ( zmw_name_equals(*n, h, name) && strcmp(why, (*n)->why) == 0 )
	return n ;
    }
  return n ;
}

static Zmw_Name** zmw_name_find(const char *name, const char *why)
{
  PRINTF("name=%s why=%s\n", name, why) ;
  return zmw_name_find_with_hash(name, why, zmw_name_hash(name)) ;
}

/*
 * After the remove, "n" points on the next
 */
static void zmw_name_remove(Zmw_Name **n)
{
  Zmw_Name *to_delete ;

  PRINTF("hash=%d name=%s why=%s\n", (*n)->hash, (*n)->name, (*n)->why) ;

  to_delete = *n ;
  *n = to_delete->next ;

  ZMW_FREE(to_delete->name) ;
  if ( to_delete->type != Zmw_Is_A_Registration )
    ZMW_FREE(to_delete) ;

  global_nb_name-- ;
}

static void zmw_name_insert(Zmw_Name **where, Zmw_Name *n)
{
  PRINTF("hash=%d name=%s why=%s\n", n->hash, n->name, n->why) ;

  n->next = *where ;
  *where = n ;
  global_nb_name++ ;

  if ( global_nb_name > 5 * global_nb_registered )
    zmw_name_init() ;
}

/*
 * Current widget is equal to "n" ?
 */
Zmw_Boolean zmw_name_is(const Zmw_Name *n)
{
  if ( n->name == NULL )
    return( Zmw_False ) ;

  return zmw_name_equals(n, zmw_hash_key_get(), zmw_name_full) ;
}
/*
 * Current widget is inside "n" ?
 */
Zmw_Boolean zmw_name_is_inside(const Zmw_Name *n)
{
  int len ;

  if ( n->name == NULL )
    return(Zmw_False) ;

  len = strlen(n->name) ;
  return( strncmp(n->name, zmw_name_full, len) == 0
	  && ( zmw_name_full[len]=='\0' || zmw_name_full[len]=='/' )
	  ) ;
}
/*
 * Current widget contains "n" ?
 */
Zmw_Boolean zmw_name_contains(const Zmw_Name *n)
{
  int length ;
  const char *parent ;

  if ( n->name == NULL )
    return(Zmw_False) ;
  
  parent = zmw_name_full ;
  length = strlen(parent) ;
          
  if ( strncmp(parent, n->name, length) == 0 )
    {
      if ( n->name[length] == '\0' || n->name[length] == '/' )
      {
       return(1) ;
      }
    }
  return(0) ;
}

/*
 * Used for http or in window debugging
 */

const char* zmw_name_type(Zmw_Name_Type nt)
{
  static char *type[] =
    {
      "Registration", "Resource String", "Resource Int", "Resource Pointer"
    } ;
  return( type[nt] ) ;
}

const char* zmw_name_value(Zmw_Name *n)
{
  static char buf[99] ;

  switch( n->type )
    {
    case Zmw_Is_A_Registration:
      return("") ;
      break ;
    case Zmw_Is_A_Resource_String:
      return( (char*)n->value ) ;
      break ;
    case Zmw_Is_A_Resource_Int:
      sprintf(buf, "%d", (int)n->value) ;
      break ;
    case Zmw_Is_A_Resource_Pointer:
      sprintf(buf, "%p", n->value) ;
      break ;
    }
  return( buf ) ;
}

void zmw_name_dump(FILE *f)
{
  int i ;

  fprintf(f, 
	  "<H2>Names</H2>\n"
	  "<TABLE BORDER>\n"
	  "<TR><TH>Ptr</TH><TH>Index</TH><TH>Name</TH><TH>Why</TH><TH>Type</TH><TH>Value</TH></TR>\n"
	  ) ;

  for(i=0 ;i<global_nb_registered; i++)
    {
      fprintf(f, "<TR><TD>%p</TD><TD>%d</TD><TD>%s</TD><TD>%s</TD><TD>%s</TD><TD>%s</TD></TR>\n"
	      , global_registered[i]
	      , global_registered[i]->hash
	      , global_registered[i]->name
	      , global_registered[i]->why
	      , zmw_name_type(global_registered[i]->type)
	      , zmw_name_value(global_registered[i])
	      ) ;
    }
  fprintf(f, "</TABLE>\n") ;
}

void zmw_name_initialize(Zmw_Name **name, char *why)
{
  if ( *name == NULL )
    {
      ZMW_MALLOC_0(*name, 1) ;
      (*name)->why = why ;
    }
}


void zmw_name_debug_window()
{
  int i ;
  char buf[999] ;
  static int display_name_state = 0 ;

  ZMW( zmw_vbox() )
    {
      zmw_check_button_int_with_label(&display_name_state, "Ressources and Registration") ;
      if ( display_name_state )
	{
	  ZMW( zmw_vbox() )
	    {
	      zmw_font_size(6) ;
	      zmw_font_family("courier") ;
	      for(i=0 ;i<global_nb_registered; i++)
		{
		  if ( global_registered[i] )
		    {
		      sprintf(buf, "%10p %10u %20s %16s %10s %s"
			      , global_registered[i]
			      , global_registered[i]->hash
			      , global_registered[i]->why
			      , zmw_name_type(global_registered[i]->type)
			      , zmw_name_value(global_registered[i])
			      , global_registered[i]->name
			      ) ;
		      zmw_label(buf) ;
		    }
		}
	    }
	}
    }
  zmw_border_embossed_in_draw() ;
}

/*
 * Free all the ressources registered.
 * It is usefull in order to find leaks
 */
void zmw_name_free()
{
  int i ;

  for(i=0 ;i<global_nb_registered; i++)
    {
      while(global_registered[i])
	zmw_name_remove(&global_registered[i]) ;
    }
  ZMW_FREE(global_registered) ;
  if ( global_nb_name )
    {
      zmw_printf("%d name in resource database not freed\n") ;
    }
}

/*
 * It is the unique function adding a ressource
 */
Zmw_Name* zmw_name_register_value(const char *name, const char *why, void *value,
			     Zmw_Name_Type nt)
{
  Zmw_Name **where, *n ;
  
  if ( name == NULL )
  	return NULL ;

  where = zmw_name_find(name, why) ;

  if ( *where == NULL )
    {
      if ( nt != Zmw_Is_A_Registration )
	{
	  ZMW_MALLOC(n, 1) ;
	  n->why = why ;
	}
      else
	{
	  n = value ;
	}
      n->type = nt ;
      n->name = strdup(name) ;
      n->hash = zmw_name_hash(name) ;
      zmw_name_insert(where, n) ;
      where = &n ;
    }
  (*where)->value = value ;
  return *where ;
}

void *zmw_name_get_pointer_on_resource_with_name_and_type_and_default(const char *name, const char *why, Zmw_Name_Type nt, void *default_value)
{
  Zmw_Name **where ;

  where = zmw_name_find(name, why) ;

  if ( *where )
    return & (*where)->value ;
  else
    return & zmw_name_register_value(name, why, default_value, nt)->value ;
}

void *zmw_name_get_pointer_on_resource_with_name_and_type(const char *name, const char *why, Zmw_Name_Type nt)
{
  return zmw_name_get_pointer_on_resource_with_name_and_type_and_default
    (name, why, nt, NULL) ;
}

int *zmw_name_get_pointer_on_int_resource_with_name(const char *name, const char *why)
{
  return zmw_name_get_pointer_on_resource_with_name_and_type(name, why, Zmw_Is_A_Resource_Int) ;
}

int *zmw_name_get_pointer_on_int_resource(const char *why)
{
  return zmw_name_get_pointer_on_int_resource_with_name(zmw_name_full, why) ;
}


Zmw_Boolean zmw_name_get_value_pointer_with_name(const char *name, const char *why, void **value)
{
  Zmw_Name **where ;

  where = zmw_name_find(name, why) ;
  if ( *where )
    {
      *value = (*where)->value ;
      return( Zmw_True ) ;
    }
  return( Zmw_False ) ;
}  

Zmw_Boolean zmw_name_get_value_pointer(const char *why, void **value)
{
  return( zmw_name_get_value_pointer_with_name(zmw_name_full, why, value) ) ;
}  

Zmw_Boolean zmw_name_get_value_int(const char *why, int *value)
{
  return( zmw_name_get_value_pointer(why, (void**)value) ) ;
}

Zmw_Boolean zmw_name_get_value_int_with_name(const char *name, const char *why, int *value)
{
  return( zmw_name_get_value_pointer_with_name(name, why, (void**)value) ) ;
}

void zmw_name_set_value_pointer_with_name(const char *name, const char *why, void *value)
{
  zmw_name_register_value(name, why, value,
			  Zmw_Is_A_Resource_Pointer) ;
}  
void zmw_name_set_value_pointer(const char *why, void *value)
{
	zmw_name_set_value_pointer_with_name(zmw_name_full, why, value) ;
}  

void zmw_name_set_value_int(const char *why, int value)
{
  zmw_name_set_value_int_with_name(zmw_name_full, why, value ) ;
}  
void zmw_name_set_value_int_with_name(const char *name, const char *why, int value)
{
  zmw_name_register_value(name, why, (void*)value,
			  Zmw_Is_A_Resource_Int) ;
}  

void zmw_name_unregister_value(const char *name, const char *why)
{
  Zmw_Name **where ;

  where = zmw_name_find(name, why) ;
  if ( *where )
    {
      ZMW_ASSERT ( (*where)->type != Zmw_Is_A_Registration ) ;
      zmw_name_remove(where) ;
    }
}

void zmw_name_unregister_value_by_pointer(const char *why, void *p)
{
  int index ;
  Zmw_Name **n ;

  for(index=0 ; index < global_nb_registered; index++)
    for(n = &global_registered[index]; *n; )
      {
	if ( (*n)->value == p
	     && (*n)->type == Zmw_Is_A_Resource_Pointer
	     && strcmp((*n)->why, why) == 0
	     )
	  {
	    zmw_name_remove(n) ;
	  }
	else
	  {
	    *n = (*n)->next ;
	  }
      }
}

/*
 *
 */

void zmw_name_register_with_name(Zmw_Name *n, const char *name)
{
  zmw_name_unregister(n) ;
  zmw_name_register_value(name, n->why, n, Zmw_Is_A_Registration) ;
}

void zmw_name_register(Zmw_Name *n)
{
  zmw_name_register_with_name(n, zmw_name_full) ;
}

#undef zmw_name_registered
char* zmw_name_registered(const Zmw_Name *n)
{
  return n->name ;
}

void zmw_name_unregister(Zmw_Name *n)
{
  Zmw_Name **where ;

  if ( n->name == NULL )
    return ;

  ZMW_ASSERT ( n->type == Zmw_Is_A_Registration ) ;

  for( where = & global_registered[ n->hash % global_nb_registered ] ;
       *where ;
       where = &(*where)->next
       )
    if ( n == *where )
      {
	zmw_name_remove(where) ;
	return ;
      }
  ZMW_ABORT ;
}

/*
 * If *pointer_value is not NULL, nothing is done
 * Else, it is set to local value and initialized if it does not exists,
 * or retrieved from resource if it exists.
 */

void zmw_resource_get(void **pointer_value, const char *resource
		      , void *default_value, Zmw_Name_Type nt)
{
  if ( *pointer_value == NULL )
    {
      *pointer_value
	= zmw_name_get_pointer_on_resource_with_name_and_type_and_default
	( zmw_name_full, resource, nt, default_value) ;  
    }
}

void zmw_resource_int_get(int **pointer_value, const char *resource
		      , int default_value)
{
  zmw_resource_get((void**)pointer_value, resource, (void*)default_value
		   , Zmw_Is_A_Resource_Int) ;
}


void zmw_resource_pointer_get(void **pointer_value, const char *resource
		      , void *default_value)
{
  zmw_resource_get((void**)pointer_value, resource, default_value
		   , Zmw_Is_A_Resource_Pointer) ;
}
