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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *line_read(FILE *file)
{
  char lig[999] ;

  if ( fgets(lig, sizeof(lig), file) == NULL )
    ABORT ;
  lig[strlen(lig)-1] = '\0' ;
  return strdup(lig) ;
}


void library_free(Library *lib)
{
  int i ;

  for(i=0; i < lib->books_number; i++)
    {
      free( lib->books[i].title ) ;
      free( lib->books[i].borrowers ) ;
    }
  free(lib->books) ;
  free(lib->filtered_books) ;

  strings_free(&lib->collections) ;
  strings_free(&lib->authors) ;
  strings_free(&lib->borrowers) ;

  free(lib) ;
}


Library* library_load(const char *filename)
{
  FILE *file ;
  int i,k ;
  Library *lib ;

  file = fopen(filename , "r" ) ;
  if ( file == NULL )
    {
      perror(filename) ;
      exit(1) ;
    }

  MALLOC(lib, 1) ;

  if ( fscanf( file , "%d\n" , &lib->books_number ) != 1 )
    ABORT ;

  strings_read(file, &lib->collections) ;
  strings_read(file, &lib->authors) ;
  strings_read(file, &lib->borrowers) ;

  MALLOC(lib->books, lib->books_number) ;
  MALLOC(lib->filtered_books, lib->books_number) ;

  for(i=0; i < lib->books_number; i++)
    {
      if ( fscanf(file, "%d %d %d %d %d"
		  ,&lib->books[i].collection_index
		  ,&lib->books[i].number
		  ,&lib->books[i].author_index
		  ,&lib->books[i].interest
		  ,&lib->books[i].borrowers_number
		  )
	   != 5 )
	{
	  fprintf(stderr, "i=%d\n", i) ;
	  ABORT ;
	}
      MALLOC(lib->books[i].borrowers, lib->books[i].borrowers_number) ;
      for(k=0; k < lib->books[i].borrowers_number ; k++)
	{
	  if ( fscanf(file, " %d", &lib->books[i].borrowers[k]) != 1 )
	    ABORT ;
	}
      free(line_read(file)) ;
      lib->books[i].title = line_read(file) ;
    }
  fclose(file) ;

  library_filter(lib, "", 0) ;

  return lib ;
}

/* XXX FIXME: Should erase old file only if the new file
 * is created without problems
 */

void library_save(const Library* lib, const char *filename)
{
  FILE *file ;
  int i,k ;

  file = fopen(filename , "w" ) ;
  if ( file == NULL )
    {
      perror(filename) ;
      exit(1) ;
    }

  fprintf(file , "%d\n" , lib->books_number ) ;

  strings_write(file, &lib->collections) ;
  strings_write(file, &lib->authors) ;
  strings_write(file, &lib->borrowers) ;

  for(i=0; i < lib->books_number; i++)
    {
      fprintf(file, "%d %d %d %d %d"
	    ,lib->books[i].collection_index
	    ,lib->books[i].number
	    ,lib->books[i].author_index
	    ,lib->books[i].interest
	    ,lib->books[i].borrowers_number
	    ) ;
      for(k=0; k < lib->books[i].borrowers_number ; k++)
	{
	  fprintf(file, " %d", lib->books[i].borrowers[k]) ;
	}
      fprintf(file, "\n%s\n", lib->books[i].title) ;
    }
  fclose(file) ;
}
