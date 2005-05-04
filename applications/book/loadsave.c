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

Hash_Key library_book_checksum(Library *lib, Book *b)
{
  Hash_Key cs ;
  int i ;

  cs  =  b->collection_index
    + 1000*(b->number
	    + 1000*(b->author_index
		    + 1000*(b->rate)
		    )
	    )
    ;
  cs = hash_key(cs, b->title) ;
  for(i=0; i<b->borrowers_number; i++)
    cs = cs*532483 + b->borrowers[i] ;

  return cs ;
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
  char *filter[Column_Last] = {"","","","","","" } ;

  file = fopen(filename , "r" ) ;
  if ( file == NULL )
    {
      fprintf(stderr, "Library load\n") ;
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
		  ,&lib->books[i].rate
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
      lib->books[i].checksum = library_book_checksum(lib, &lib->books[i]) ;

    }
  fclose(file) ;

  library_filter(lib, "", 0, filter, 0) ;

  return lib ;
}

/* XXX FIXME: Should erase old file only if the new file
 * is created without problems
 */

void library_save(Library* lib, const char *filename)
{
  FILE *file ;
  int i,k ;

  file = fopen(filename , "w" ) ;
  if ( file == NULL )
    {
      fprintf(stderr, "Library save\n") ;
      perror(filename) ;
      exit(1) ;
    }

  fprintf(file , "%d\n" , lib->books_number ) ;

  strings_write(file, &lib->collections) ;
  strings_write(file, &lib->authors) ;
  strings_write(file, &lib->borrowers) ;
  strings_reset_checksum(&lib->authors) ;
  strings_reset_checksum(&lib->borrowers) ;
  strings_reset_checksum(&lib->collections) ;

  for(i=0; i < lib->books_number; i++)
    {
      fprintf(file, "%d %d %d %d %d"
	    ,lib->books[i].collection_index
	    ,lib->books[i].number
	    ,lib->books[i].author_index
	    ,lib->books[i].rate
	    ,lib->books[i].borrowers_number
	    ) ;
      for(k=0; k < lib->books[i].borrowers_number ; k++)
	{
	  fprintf(file, " %d", lib->books[i].borrowers[k]) ;
	}
      fprintf(file, "\n%s\n", lib->books[i].title) ;
    }
  fclose(file) ;

  for(i=0; i < lib->books_number; i++)
    {
      lib->books[i].checksum = library_book_checksum(lib, &lib->books[i]) ;
    }
}
