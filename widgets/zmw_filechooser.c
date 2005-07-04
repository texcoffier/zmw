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

#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

Zmw_Boolean zmw_is_dir(const char *name)
{
  struct stat st ;

  if ( stat(name, &st) )
    {
      return(Zmw_False) ;
    }
  else
    {
      return( S_ISDIR(st.st_mode) ) ;
    }
}

void zmw_file_info(const char *name, char *type, int *size)
{
  struct stat st ;
  
  if ( lstat(name, &st) )
    {
      *type = '?' ;
      *size = -1 ;
    }
  else
    {
      *size = (int)st.st_size ;
    	
      if( S_ISDIR(st.st_mode) )
	*type = 'd' ;
      else if ( S_ISREG(st.st_mode) )
	*type = 'f' ;
      else if ( S_ISLNK(st.st_mode) )
	*type = 'l' ;
      else *type = '?' ;
    }
}

void zmw_filename_normalize(char **pname)
{
  int i, j ;
  char *name ;

  name = *pname ;

  for(i=0; name[i]; i++)
    if ( name[i] == '/' && name[i+1] == '.' )
      {
	if ( name[i+2] == '/' )
	  {
	    memmove(&name[i+1], &name[i+3], strlen(&name[i+3])+1) ;
	    i-- ;
	  }
	else if ( name[i+2] == '.' && name[i+3] == '/' )
	  {
	    for(j=i-1; j>=0; j--)
	      if ( name[j] == '/' )
		break ;
	    if ( j >= 0 )
	      memmove(&name[j], &name[i+3], strlen(&name[i+3])+1) ;
	    else
	      {
		if ( name[0] == '/' )
		  memmove(&name[0], &name[i+3], strlen(&name[i+3])+1) ;
		else
		  {
		    char buf[999] ;
		    if ( getcwd(buf, sizeof(buf)) == NULL )
		      ZMW_ABORT ;
		    *pname = strdup(buf) ;
		    return ;
		  }
	      }
	    i = j ;
	  }
      }
}

Zmw_Boolean zmw_name_filtered(char *asked, char *real)
{
  int j ;
	
  if ( strcmp(real, "..") == 0 )
    return( Zmw_False ) ;
  if ( strcmp(real, ".") == 0 )
    return( Zmw_True ) ;
	
  for(j=0; asked[j] && real[j]; j++)
    if ( asked[j] != real[j])
      return(Zmw_True) ;

  if ( asked[j] )
    return(Zmw_True) ;
		
  return( Zmw_False ) ;
}


void zmw_select_file(char **filename, char *file, int end_of_dir)
{
  ZMW_REALLOC(*filename, strlen(*filename) + strlen(file) + 5) ;
  strcpy(*filename + end_of_dir + 1, file) ;
  strcat(*filename, "/") ;
  if ( ! zmw_is_dir(*filename) )
    (*filename)[strlen(*filename)-1] = '\0' ;
  zmw_filename_normalize(filename) ;
}


void zmw_file_selection(Zmw_Boolean *visible
		     , char **filename, const char *title
		     , const char *message)
{
  int end_of_dir ;
  int i, nb, size, width, height ;
  char type ;
  struct dirent **d ;
  int err ;
  Zmw_Boolean activated ;
  char buf[999] ;
  static Zmw_Boolean old_visible = Zmw_False ;// XXX only one file chooser at a time...
  static Zmw_Float_0_1 x, y ; // XXX only one file chooser at a time...
  static Zmw_Boolean take_focus = Zmw_False ;

  if ( zmw_subaction_get() == Zmw_Input_Event )
    {
      zmw_activated_set(Zmw_False) ;
    }
  activated = Zmw_False ;

  if ( *visible != old_visible )
    {
      old_visible = *visible ;
      if ( *visible )
	{
	  take_focus = Zmw_True ;
	}
    }

  /* These to are here only to remove compiler warning */
  nb = 0 ;
  err = 0 ;
  /* These lines are here (and not in the 'if') to remove compiler warnings */ 
  end_of_dir = -1 ;
  width = zmw_asked_width_get() ;
  height = zmw_asked_height_get() ;

  if ( *visible )
    {
      /*
       * Create current dir file name if NULL filename
       */
      if ( *filename == NULL )
	{
	  getcwd(buf, sizeof(buf)-2) ;
	  strcat(buf, "/") ;
	  *filename = strdup(buf) ;
	}
      /*
       * Read directory content
       */  
      for(i=0; (*filename)[i]; i++)
	if ( (*filename)[i] == '/' )
	  end_of_dir = i ;
      
      switch( end_of_dir )
	{
	case 0:
	  nb = scandir("/", &d, NULL, alphasort) ;
	  break ;
	case -1:
	  nb = scandir(".", &d, NULL, alphasort) ;
	  break ;
	default:
	  (*filename)[end_of_dir] = '\0' ;
	  nb = scandir(*filename, &d, NULL, alphasort) ;
	  (*filename)[end_of_dir] = '/' ;
	}    
      err = errno ;
    }

  ZMW( zmw_if( *visible ) )
    {      
      ZMW(zmw_window(title))
	{
	  if ( height == ZMW_VALUE_UNDEFINED )
	    zmw_height(200) ;
	  else
	    zmw_height(height) ;
	  
	  if ( width == ZMW_VALUE_UNDEFINED )
	    zmw_width(200) ;
	  else
	    zmw_width(width) ;
	  
	  ZMW(zmw_vbox())
	    {
	      zmw_vertical_expand(Zmw_True) ;
	      zmw_horizontal_expand(Zmw_True) ;
	      ZMW(zmw_viewport_with_scrollbar(&x, &y))
		{
		  ZMW(zmw_table(3))
		    {
		      zmw_label("Type") ;
		      zmw_label("Name") ;
		      zmw_label("Size") ;
		      for(i=0; i<nb; i++)
			{
			  if ( zmw_name_filtered(&(*filename)[end_of_dir+1],
						 d[i]->d_name ) )
			    continue ;
			  strncpy(buf, *filename, end_of_dir + 1) ;
			  strcpy(buf + end_of_dir + 1, d[i]->d_name) ;
			  zmw_file_info(buf, &type, &size) ;
			  
			  buf[0] = type ;
			  buf[1] = '\0' ;
			  zmw_label(buf) ;
			  
			  zmw_label(d[i]->d_name) ;
			  if ( zmw_button_pressed() )
			    {
			      zmw_select_file(filename, d[i]->d_name, end_of_dir) ;
			      zmw_event_remove() ;
			      x = y = 0 ;
			    }
			  if ( size >= 0 && type == 'f' )
			    {
			      sprintf(buf, "%d", size) ;
			      zmw_label(buf) ;
			    }
			  else
			    zmw_label("") ;
			  
			}
		    }
		}
	      zmw_vertical_expand(0) ;
	      zmw_entry(filename) ;
	      if ( take_focus )
		{
		  zmw_name_register(zmw_focus_get()) ;
		  take_focus = Zmw_False ;
		}
	      /* XXX Not nice, but effective.
	       * I don't know yet how to get keys
	       * on not focused items.
	       * (but not accelerators)
	       */
	      switch( (*filename)[strlen(*filename)-1] )
		{
		case '\n':
		case '\r':
		  activated = Zmw_True ;
		case '\e':
		  *visible = Zmw_False ;
		  (*filename)[strlen(*filename)-1] = '\0' ;
		  break ;
		}
	      
	      if ( zmw_activated() )
		{
		  zmw_filename_normalize(filename) ;
		  x = y = 0 ;
		}
	      
	      if ( nb < 0 )
		{
		  zmw_label("Problem with the directory:") ;
		  zmw_label( strerror(err) ) ;
		}
	      zmw_horizontal_alignment(0) ;
	      ZMW(zmw_hbox())
		{	      
		  zmw_button("Cancel") ;
		  if ( zmw_activated() )
		    *visible = Zmw_False ;
		  zmw_button(message) ;
		  if ( zmw_activated() )
		    {
		      activated = Zmw_True ;
		      *visible = Zmw_False ;
		      printf("========\n") ;
		    }
		}
	    }
	}
    }
  if ( zmw_subaction_get() == Zmw_Input_Event )
    {
      if ( activated )
	zmw_activated_set(Zmw_True) ;
    }
  if ( *visible )
    {
      for(i=0; i<nb; i++)
	free(d[i]) ;
      if ( nb >= 0 )
	free(d) ;
    }
}



