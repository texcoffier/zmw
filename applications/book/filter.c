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

void library_filter(Library *lib, const char *filter, int only_borrowed
		    , char **filters, int only_modified)
{
  int i, j, e, ok ;
  char *filter_lower, *filters_lower[Column_Last] ;
  char *lower[Column_Last], tmp[20] ;

  filter_lower = lower_case(filter) ;
  for(i=0; i<Column_Last; i++)
    filters_lower[i] = lower_case(filters[i]) ;

  lib->filtered_number = 0 ;


  for(i=0; i<lib->books_number; i++)
    {
      if ( only_modified )
	{
	  ok = (lib->books[i].checksum
		!= library_book_checksum(lib, &lib->books[i] )) ;
	}
      else
	{
	  if ( only_borrowed
	       && ( lib->books[i].borrowers_number == 0
		    || lib->books[i].borrowers[lib->books[i].borrowers_number-1] > 0)
	       )
	    continue ;

	  lower[Column_Title] = lower_case(lib->books[i].title) ;
	  lower[Column_Author] = lower_case(lib->authors.strings[lib->books[i].author_index]) ;
	  lower[Column_Collection] = lower_case(lib->collections.strings[lib->books[i].collection_index]) ;
	  sprintf(tmp, "%d", lib->books[i].rate) ;
	  lower[Column_Rate] = strdup(tmp) ;
	  sprintf(tmp, "%d", lib->books[i].number) ;
	  lower[Column_Number] = strdup(tmp) ;
	  
	  e = lib->books[i].borrowers_number ;
	  if ( e )
	    {
	      lower[Column_Borrower] = lower_case(library_borrower_name(lib, lib->books[i].borrowers[e-1])) ;
	    }
	  else
	    {
	      lower[Column_Borrower] = strdup("") ;
	    }
	  
	  ok = (filter_lower[0] == '\0') ;
	  if ( !ok )
	    for(j=0; j<Column_Last; j++)
	      if ( strstr(lower[j], filter_lower) != NULL )
		{
		  ok = 1 ;
		  break ;
		}
	  if ( ok )
	    for(j=0; j<Column_Last; j++)
	      if ( filters_lower[j][0]
		   && strstr(lower[j], filters_lower[j]) == NULL )
		{
		  ok = 0 ;
		  break ;
		}

	  for(j=0; j<Column_Last; j++)
	    free(lower[j]) ;
	}
      if ( ok )
	{	  
	  lib->filtered_books[lib->filtered_number++] = &lib->books[i] ;
	}
    }
  free(filter_lower) ;
  for(i=0; i<Column_Last; i++)
    free(filters_lower[i]) ;

}
