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

void zmw_filename_normalize(char *name)
{
  int i, j ;

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
	      memmove(&name[0], &name[i+3], strlen(&name[i+3])+1) ;
	    i = j ;
	  }
      }
}

void zmw_filechooser(Zmw_Boolean *visible, GdkWindow **w
		    , char **filename, const char *title
		    , const char *message)
{
  int end_of_dir ;
  int i, j, nb ;
  struct dirent **d ;
  int err ;
  Zmw_Boolean activated ;
  char buf[999] ;

  static float x, y ;
  static Zmw_Boolean needed ;

  if ( ! *visible )
    {
      zmw.activated = 0 ;
      return ;
    }

  if ( *filename == NULL )
    {
      getcwd(buf, sizeof(buf)) ;
      strcat(buf, "/") ;
      *filename = strdup(buf) ;
    }

  end_of_dir = -1 ;
  for(i=0; (*filename)[i]; i++)
    if ( (*filename)[i] == '/' )
      end_of_dir = i ;

  if ( end_of_dir>0 )
    {
      (*filename)[end_of_dir] = '\0' ;
      nb = scandir(*filename, &d, NULL, alphasort) ;
      (*filename)[end_of_dir] = '/' ;
    }
  else
    {
      nb = scandir("/", &d, NULL, alphasort) ;
    }
  err = errno ;
  activated = Zmw_False ;

  ZMW(zmw_window(w, title))
    {
      zmw_height(500) ;
      ZMW(zmw_box_vertical())
	{
	  zmw_vertical_expand(1) ;
	  zmw_horizontal_expand(1) ;
	  ZMW(zmw_viewport_with_scrollbar_if_needed(&x, &y, &needed))
	    {
	      ZMW(zmw_box_vertical())
		{
		  for(i=0; i<nb; i++)
		    {
		      if ( strcmp(d[i]->d_name, "..") )
			{
			  if ( end_of_dir >= 0 )
			    {
			      for(j=end_of_dir+1;
				  (*filename)[j]
				    && d[i]->d_name[j-end_of_dir-1];
				  j++)
				if ((*filename)[j]
				    != d[i]->d_name[j-end_of_dir-1])
				  break ;
			      if ( (*filename)[j] )
				continue ;
			    }
			}
		      if ( strcmp(d[i]->d_name, ".") == 0 )
			continue ;
		      zmw_text(d[i]->d_name) ;
		      if ( zmw_button_pressed() )
			{
			  ZMW_REALLOC(*filename
				      , strlen(*filename)
				      + strlen(d[i]->d_name) + 5) ;
			  if ( end_of_dir >= 0 )
			    strcpy(*filename + end_of_dir + 1, d[i]->d_name) ;
			  else
			    sprintf(*filename, "/%s", d[i]->d_name) ;
			  if ( zmw_is_dir(*filename) )
			  strcat(*filename, "/") ;
			  zmw_filename_normalize(*filename) ;
			  zmw_event_remove() ;
			  x = y = 0 ;
			}
		    }
		}
	    }
	  zmw_vertical_expand(0) ;
	  zmw_width(300) ;
	  zmw_text_editable(filename) ;
	  if ( zmw_activated() )
	    {
	      zmw_filename_normalize(*filename) ;
	      x = y = 0 ;
	    }
	  if ( nb < 0 )
	    {
	      zmw_text("Problem with the directory :") ;
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
    zmw.activated = Zmw_True ;
}



