/*
  ZMW: A Zero Memory Widget Library
  Copyright (C) 2003 Thierry EXCOFFIER

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

#include "library.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void library_filter(Library *lib, const char *filter, int only_borrowed)
{
  int i, e ;
  char *filter_lower ;
  char *title_lower, *author_lower, *borrower_lower ;

  filter_lower = lower_case(filter) ;
  lib->filtered_number = 0 ;
  for(i=0; i<lib->books_number; i++)
    {
      if ( only_borrowed
	   && ( lib->books[i].borrowers_number == 0
		|| lib->books[i].borrowers[lib->books[i].borrowers_number-1] > 0)
	   )
	continue ;

      title_lower = lower_case(lib->books[i].title) ;
      author_lower = lower_case(lib->authors.strings[lib->books[i].author_index]) ;

      e = lib->books[i].borrowers_number ;
      if ( e )
	{
	  borrower_lower = lower_case(library_borrower_name(lib, lib->books[i].borrowers[e-1])) ;
	}
      else
	{
	  borrower_lower = strdup("") ;
	}
      
      if ( filter_lower[0] == '\0'
	   || strstr(title_lower, filter_lower) != NULL
	   || strstr(author_lower, filter_lower) != NULL
	   || strstr(borrower_lower, filter_lower) != NULL
	   )
	lib->filtered_books[lib->filtered_number++] = &lib->books[i] ;

      free(title_lower) ;
      free(author_lower) ;
    }
  free(filter_lower) ;
}
