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


void zmw_filechooser(Zmw_Boolean *visible
		     , char **filename, const char *title
		     , const char *message)
{
  int end_of_dir ;
  int i, nb, size, width, height ;
  char type ;
  struct dirent **d ;
  int err ;
  Zmw_Boolean activated ;
  Zmw_Boolean take_focus ;
  char buf[999] ;
  static Zmw_Boolean old_visible = Zmw_False ;// XXX only one file chooser at a time...
  static Zmw_Float_0_1 x, y ; // XXX only one file chooser at a time...


  take_focus = Zmw_False ;

  if ( *visible != old_visible )
    {
      old_visible = *visible ;
      if ( *visible )
	take_focus = Zmw_True ;
    }

  if ( ! *visible )
    return ;
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
   * If  current dir file name is relative, make it absolute
   */
  /*
    if ( (*filename)[0] != '/' )
    {
    getcwd(buf, sizeof(buf)) ;
    strcat(buf, "/") ;
    strcat(buf, *filename) ;
    *filename = strdup(buf) ;
    }
  */
  /*
   * Read directory content
   */  
  end_of_dir = -1 ;
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
  activated = Zmw_False ;

  width = ZMW_SIZE_ASKED.width ;
  height = ZMW_SIZE_ASKED.height ;

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

      ZMW(zmw_box_vertical())
	{
	  zmw_vertical_expand(Zmw_True) ;
	  zmw_horizontal_expand(Zmw_True) ;
	  ZMW(zmw_viewport_with_scrollbar(&x, &y))
	    {
	      ZMW(zmw_table(3))
		{
		  zmw_text("Type") ;
		  zmw_text("Name") ;
		  zmw_text("Size") ;
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
		      zmw_text(buf) ;

		      zmw_text(d[i]->d_name) ;
		      if ( zmw_button_pressed() )
			{
			  zmw_select_file(filename, d[i]->d_name, end_of_dir) ;
			  zmw_event_remove() ;
			  x = y = 0 ;
			}
		      if ( size >= 0 && type == 'f' )
		    	{
			  sprintf(buf, "%d", size) ;
			  zmw_text(buf) ;
		    	}
		      else
		    	zmw_text("") ;
			     
		    }
		}
	    }
	  zmw_vertical_expand(0) ;
	  zmw_text_editable(filename) ;
	  if ( take_focus )
	    zmw_name_register(ZMW_FOCUS) ;
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
	      zmw_text("Problem with the directory:") ;
	      zmw_text( strerror(err) ) ;
	    }
	  zmw_horizontal_alignment(0) ;
	  ZMW(zmw_box_horizontal())
	    {	      
	      zmw_button("Cancel") ;
	      if ( zmw_activated() )
		*visible = Zmw_False ;
	      zmw_button(message) ;
	      if ( zmw_activated() )
		{
		  activated = Zmw_True ;
		  *visible = Zmw_False ;
		}
	    }
	}
    }
  for(i=0; i<nb; i++)
    free(d[i]) ;
  if ( nb >= 0 )
    free(d) ;

  if ( activated )
    ZMW_SIZE_ACTIVATED = Zmw_True ;
}



