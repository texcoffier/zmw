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

#include <ctype.h>
#include "zmw/zmw.h"

static Zmw_Boolean zmw_name_string_contains(const char *child) ;
static Zmw_Boolean zmw_name_next_string_contains(const char *child) ;
static Zmw_Boolean zmw_name_string_is(const Zmw_Name *n) ;
static int zmw_index_of_next_widget() ;
static int zmw_name_last_index_of_next_widget() ;

static int global_name_check = 0 ;
static int global_name_fast = 0 ;

#define PRINTF if(0) zmw_printf

#if ZMW_DEBUG_NAME

void zmw_name_debug_print(const Zmw_Name *n)
{
	 zmw_printf("\rn->index=%d\n"		
		    "ZMW_INDEX=%d\n"		
		    "zmw.index_last=%d\n"       
		    "n->name      =%s\n"	
		    "zmw_name_full=%s\n"	
		    "n->why=%s\n"		
		    "ACTION=%s\n"		
		    "SUBACTION=%s\n"		
		    "ZMW_CALL_NUMBER=%d\n"	
		    "zMw[-1].u.call_number=%d\n" 
		    "name_string_contains=%d\n"	
		    "name_next_string_contains=%d\n"	
		    "name_string_is=%d\n"	
		    "zMw[-1].u.children[zMw[-1].u.nb_of_children+1].index=%d\n" 
		    "index_of_next() = %d\n" 
		    "zMw[-1].u.children[zMw[-1].u.nb_of_children].index=%d\n" 
		    "index_of_next_next() = %d\n" 
		    "zMw[-1].u.nb_of_children=%d\n" 
		    "zMw[-1].u.nb_of_children0=%d\n"
		    "event_type=%s\n" 
		    , n->index			
		    , ZMW_INDEX			
		    , zmw.index_last            
		    , n->name			
		    , zmw_name_full		
		    , n->why			
		    , zmw_action_name_fct()	
		    , zmw_action_name()		
		    , ZMW_CALL_NUMBER		
		    , zMw[-1].u.call_number 
		    , n->name ? zmw_name_string_contains(n->name) : -1 
		    , n->name ? zmw_name_next_string_contains(n->name) : -1 
		    , n->name ? zmw_name_string_is(n) : -1 
		    , zMw[-1].u.children[zMw[-1].u.nb_of_children+1].index 
		    , zmw_name_last_index_of_next_widget()
		    , zMw[-1].u.children[zMw[-1].u.nb_of_children].index 
		    , zmw_index_of_next_widget()
		    , zMw[-1].u.nb_of_children 
		    , zMw[-1].u.nb_of_children_0 
		    , zmw.event->type == GDK_NOTHING ? "NOTHING" : "?"
		    ) ;
}

#define ZMW_NAME_ASSERT(X) ZMW1(					\
    if ( (zmw.debug & Zmw_Debug_Name) && !(X) )			\
       {								\
	 zmw_printf("\nASSERT %s FAILED in %s\n",#X, __FUNCTION__) ;	\
	 zmw_name_debug_print(n) ;					\
       }								\
    )
#else
#define ZMW_NAME_ASSERT(X) 
#endif

char* zmw_name_copy()
{
  return( strdup( zmw_name_full ) ) ;
}

Zmw_Boolean zmw_the_first_pass_is_done(const Zmw_Name *n)
{
	return( n->index && ZMW_CALL_NUMBER > 1 ) ;
}

Zmw_Boolean zmw_the_parent_first_pass_is_done()
{
	return( zMw[-1].u.call_number > 1 ) ;
}



/*
 ******************************************************************************
 *
 ******************************************************************************
 */

static Zmw_Name **global_registered = NULL ;
static int global_nb_registered = 0 ;
static int global_nb_registered_max = 0 ;


/*
 * Return True on find.
 * The index returned the insertion position.
 * If the name is found, the result is the minimum index in
 * case of multiple answers
 */

static Zmw_Boolean zmw_name_find(const char *name, int *index)
{
  int min, max, middle, cmp ;

  //  if ( global_registered == NULL )
  //    return Zmw_False ;
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
	  // PRINTF("Name find: return true\n", name) ;
	  return(Zmw_True) ;
	}
      if ( cmp < 0 )
	min = middle + 1 ;
      else
	max = middle - 1 ;
    }
  *index = min ;
  // PRINTF("Name find: return false\n", name) ;
  return(Zmw_False) ;
}

static Zmw_Boolean zmw_name_search_index(const char *name, const char *why, int *index)
{
  if( zmw_name_find(name, index) )
    {
      for( ;
	   *index < global_nb_registered
	     && strcmp(global_registered[*index]->name, name) == 0
	     ;
	   (*index)++
	   )
	if ( strcmp(why, global_registered[*index]->why) == 0 )
	  return(Zmw_True) ;
    }
  return(Zmw_False) ;
}


static void zmw_name_remove(int index)
{
  memmove(&global_registered[index]
	  , &global_registered[index+1]
	  , (global_nb_registered-index-1)*sizeof(global_registered[0])
	  ) ;
  global_nb_registered-- ;  
}

static void zmw_name_insert(int index, Zmw_Name *n)
{
  if ( global_nb_registered >= global_nb_registered_max )
    {
      global_nb_registered_max = global_nb_registered_max*2 + 10 ;
      ZMW_REALLOC(global_registered, global_nb_registered_max) ;
    }

  global_nb_registered++ ;
  memmove(&global_registered[index+1]
	  , &global_registered[index]
	  , (global_nb_registered-index-1)*sizeof(global_registered[0])
	  ) ;
  global_registered[index] = n ;
  n->index = 0 ;
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

static Zmw_Boolean zmw_name_string_is(const Zmw_Name *n)
{
  return( !strcmp(n->name, zmw_name_full) ) ;
}

Zmw_Boolean zmw_name_is(const Zmw_Name *n)
{
  PRINTF("Name_is %p %s[%d] = %s[%d]\n"
	 , n, n->name, n->index, zmw_name_full, ZMW_INDEX) ;
  if ( n->name == NULL )
    return( Zmw_False ) ;
  global_name_check++ ;
  if ( zmw_the_first_pass_is_done(n) )
    {
      global_name_fast++ ;
      ZMW_NAME_ASSERT( (n->index == ZMW_INDEX) == zmw_name_string_is(n) ) ;
      return( n->index == ZMW_INDEX ) ;
    }
  else
    {
      return( zmw_name_string_is(n) ) ;
    }
}


static Zmw_Boolean zmw_name_string_contains(const char *name)
{
  int length ;
  const char *parent ;

  if ( name == NULL )
    return(Zmw_False) ;
  
  PRINTF("Name_string contains child->name=%s parent=%s\n"
	 , name, zmw_name_full) ;

  parent = zmw_name_full ;
  length = strlen(parent) ;
          
  if ( strncmp(parent, name, length) == 0 )
    {
      if ( name[length] == '\0' || name[length] == '/' )
      {
       return(1) ;
      }
    }
  return(0) ;
}

Zmw_Boolean zmw_name_is_inside(const Zmw_Name *n)
{
  int len ;

  if ( n->name )
    {
      len = strlen(n->name) ;
      return( strncmp(n->name, zmw_name_full, len) == 0
	      && ( zmw_name_full[len]=='\0' || zmw_name_full[len]=='/' )
	      ) ;
    }
  else
    return( Zmw_False ) ;
}

static int zmw_index_of_next_widget_rec(Zmw_State *z)
{
   if ( z->u.nb_of_children < z->u.nb_of_children_0 )
   	return ( z->u.children[z->u.nb_of_children].index ) ;
   	
   if ( z < zmw.zmw_table )
   	ZMW_ABORT ;
   	
   return( zmw_index_of_next_widget_rec(z - 1) ) ;
}

static int zmw_index_of_next_widget()
{
	return( ZMW_SIZE_INDEX ) ;
   return( zmw_index_of_next_widget_rec(zMw-1) ) ;
}


Zmw_Boolean zmw_name_contains(const Zmw_Name *n)
{
  global_name_check++ ;
  if ( zmw_the_first_pass_is_done(n) )
    {
      global_name_fast++ ;
      ZMW_NAME_ASSERT( (n->index >= ZMW_INDEX	
	     && n->index < zmw_index_of_next_widget())
      	 == zmw_name_string_contains(n->name) ) ;
      return(
	     n->index >= ZMW_INDEX
	     && n->index < zmw_index_of_next_widget()
	     ) ;
    }
  else
    {
      return( zmw_name_string_contains(n->name) ) ;
    }
}

/*
 * This is complete garbage.
 * This code works only if the user doesn't change the name
 * of the next widget.
 */ 
 
static Zmw_Boolean zmw_name_next_string_contains(const char *name)
{
  int i ;

  PRINTF("zmw_name_next_string_contains %s\n", name) ;

  zmw_name_of_the_transient_begin() ;
  i = zmw_name_string_contains(name) ;
  zmw_name_of_the_transient_end() ;

  return(i) ;
}


Zmw_Boolean zmw_name_is_my_transient(const char *n)
{
	if ( n == NULL )
		return(Zmw_False );
	return ( strlen(n) > strlen(zmw_name_full)
		&& n[strlen(zmw_name_full)] == ',' ) ;
	
}


static int zmw_name_last_index_of_next_widget()
{
     return( zMw[-1].u.children[zMw[-1].u.nb_of_children].index ) ;


   if ( zMw[-1].u.nb_of_children <= zMw[-1].u.nb_of_children_0 )
     return( zMw[-1].u.children[zMw[-1].u.nb_of_children].index ) ;
   else
     return( 2000000000 ) ; /* Current widget is the last one */

   if ( zMw[-1].u.nb_of_children+1 < zMw[-1].u.nb_of_children_0 )
   	return( zMw[-1].u.children[zMw[-1].u.nb_of_children+1].index ) ;
   return ( zmw_index_of_next_widget_rec(zMw-2) ) ;
}

Zmw_Boolean zmw_name_next_contains(const Zmw_Name *n)
{
  if ( n==NULL || !zmw_name_is_my_transient(n->name) )
	return( Zmw_False ) ;

  global_name_check++ ;	
  if ( zmw_the_parent_first_pass_is_done() && zmw_the_first_pass_is_done(n)
  )
    {
      global_name_fast++ ;
      ZMW_NAME_ASSERT( (n->index >= ZMW_INDEX
	     && n->index < zmw_name_last_index_of_next_widget())
	     	== zmw_name_next_string_contains(n->name) ) ;
      return(
	     n->index >= ZMW_INDEX
	     && n->index < zmw_name_last_index_of_next_widget()
	     ) ;
    }
  else
    {
      return( zmw_name_next_string_contains(n->name) ) ;
    }
}

/*
 * Some name manipulations.
 * To speed up things, the len is given and returned
 */

void zmw_name_cut_last(char *name, int *len)
{
  if ( *len == 0 )
    {
      *len = -1 ;
      return ;
    }
  while( *len && name[*len] != '/' )
    (*len)-- ;
  if ( *len >= 0 )
    name[*len] = '\0' ;
}

Zmw_Boolean zmw_name_is_transient(const char *name, int len)
{
  len-- ;
  while( isdigit(name[len]) )
    len-- ;
  return name[len] == ',' ;
}

/*
 *
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
	      , global_registered[i]->index
	      , global_registered[i]->name
	      , global_registered[i]->why
	      , zmw_name_type(global_registered[i]->type)
	      , zmw_name_value(global_registered[i])
	      ) ;
    }
  fprintf(f, "</TABLE>\n") ;
}

void zmw_name_debug_window()
{
  int i ;
  char buf[999] ;
  static int display_name_state = 0 ;

  ZMW( zmw_box_vertical() )
    {
      zmw_font("5x8") ;
      zmw_toggle_int_with_label(&display_name_state, "Ressources and Registration") ;
      if ( display_name_state )
	{
	  ZMW( zmw_box_vertical() )
	    {
	      for(i=0 ;i<global_nb_registered; i++)
		{
		  sprintf(buf, "%10p %6d %20s %16s %10s %s"
			  , global_registered[i]
			  , global_registered[i]->index
			  , global_registered[i]->why
			  , zmw_name_type(global_registered[i]->type)
			  , zmw_name_value(global_registered[i])
			  , global_registered[i]->name
			  ) ;
		  zmw_text(buf) ;
		}
	    }
	}
    }
  zmw_border_embossed_in_draw() ;
}


void zmw_name_free()
{
  int i ;

  for(i=0 ;i<global_nb_registered; i++)
    if ( global_registered[i]->name )
      ZMW_FREE(global_registered[i]->name) ;
      
  fprintf(stderr, "Fast name access miss=%g%%\n",
  	100*((global_name_check-global_name_fast)/(float)global_name_check)
  		) ;
}

/*
 *
 */

void zmw_name_register_value(const char *name, const char *why, void *value,
			     Zmw_Name_Type nt)
{
  int index ;
  Zmw_Name *n ;
  
  if ( name == NULL )
  	return ;

  if ( ! zmw_name_search_index(name, why, &index) )
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
      zmw_name_insert(index, n) ;
    }
  global_registered[index]->value = value ;
}

Zmw_Name* zmw_name_get_name(const char *name, const char *why)
{
  int index ;

  if ( zmw_name_search_index(name, why, &index) )
    {
      return global_registered[index] ;
    }
  return( NULL ) ;
}  

Zmw_Boolean zmw_name_get_value_pointer_with_name(const char *name, const char *why, void **value)
{
  int index ;

  if ( zmw_name_search_index(name, why, &index) )
    {
      *value = global_registered[index]->value ;
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
  int index ;

  if ( zmw_name_search_index(name, why, &index) )
    {
      ZMW_FREE(global_registered[index]) ;
      zmw_name_remove(index) ;
    }
}

void zmw_name_unregister_value_by_pointer(const char *why, void *p)
{
  int index ;

  for(index=0 ; index < global_nb_registered; index++)
    if ( global_registered[index]->value == p
	 && global_registered[index]->type == Zmw_Is_A_Resource_Pointer
	 && strcmp(global_registered[index]->why, why) == 0
	 )
      {
	zmw_name_remove(index) ;
	index-- ;
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


void zmw_name_unregister(Zmw_Name *n)
{
  int index ;

  if ( n->name )
    {
      if ( zmw_name_search_index(n->name, n->why, &index) )
	{
	  for( ;
	       index < global_nb_registered
		 && strcmp(n->name, global_registered[index]->name) == 0
		 ; index++)
	    if ( global_registered[index] == n )
	      {
		zmw_name_remove(index) ;
		free( n->name ) ;
		n->name = NULL ;
		n->index = 0 ;
		return ;
	      }
	}
      ZMW_ABORT ;
    }
}

/*
 * If *pointer_value is not NULL, nothing is done
 * Else, it is set to local value and initialized if it does not exists,
 * or retrieved from ressource if it exists.
 */

void zmw_resource_int_get(int **pointer_value, Zmw_Resource *r)
{
  if ( *pointer_value == NULL )
    {
      *pointer_value = &r->value ;
      r->value = r->initial_value ;
      if ( r->widget_name == NULL )
	{
	  zmw_name_get_value_int(r->name, &r->value) ;
	  r->widget_name_used_for_get = strdup(zmw_name_full) ;
	}
      else
	{
	  zmw_name_get_value_int_with_name(r->widget_name, r->name, &r->value) ;
	  r->widget_name_used_for_get = NULL ;
	}
      r->old_value = r->value ;
      r->initialized = Zmw_True ;
    }
  else
    r->old_value = **pointer_value ;
  r->type = Zmw_Is_A_Resource_Int ;
}

void zmw_resource_pointer_get(void **pointer_value, Zmw_Resource *r)
{
  zmw_resource_int_get((int**)pointer_value, r) ;
  r->type = Zmw_Is_A_Resource_Pointer ;
}

void zmw_resource_set(Zmw_Resource *r)
{
  if ( r->initialized && r->value != r->old_value )
    {
      if ( r->widget_name_used_for_get )
	{
	  zmw_name_register_value( r->widget_name_used_for_get
				   , r->name
				   , (void*)r->value
				   , r->type
				   ) ;
	  free(r->widget_name_used_for_get) ;
	}
      else
	zmw_name_register_value( r->widget_name
				 , r->name
				 , (void*)r->value
				 , r->type
				 ) ;
    }
}

  
