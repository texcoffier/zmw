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

int library_book_number(Library *lib)
{
  return lib->filtered_number ;
}

const char* library_book_title_get(Library *lib, int index)
{
  return lib->filtered_books[index]->title ;
}

char** library_book_title_pointer_get(Library *lib, int index)
{
  return &lib->filtered_books[index]->title ;
}

const char* library_book_author_get(Library *lib, int index)
{
  return lib->authors.strings[lib->filtered_books[index]->author_index] ;
}

void library_book_author_set(Library *lib, int index, int author)
{
  lib->filtered_books[index]->author_index = author ;
}

char** library_book_author_pointer_get(Library *lib, int index)
{
  return &lib->authors.strings[lib->filtered_books[index]->author_index] ;
}

const char* library_book_collection_get(Library *lib, int index)
{
  return lib->collections.strings[lib->filtered_books[index]->collection_index] ;
}

char** library_borrower_pointer_name(Library *lib, int borrower_index)
{
  if ( borrower_index < 0 )
    borrower_index = -borrower_index - 1 ;
  else
    borrower_index-- ;
  ASSERT(borrower_index < lib->borrowers.number ) ;
  return &lib->borrowers.strings[borrower_index] ;
}

const char* library_borrower_name(Library *lib, int borrower_index)
{
  return *library_borrower_pointer_name(lib, borrower_index) ;
}

int library_book_borrower_number(Library *lib, int index)
{
  return lib->filtered_books[index]->borrowers_number ;
}

char** library_book_borrower_pointer_get(Library *lib, int index
					       , int borrower)
{
  borrower = (borrower + lib->filtered_books[index]->borrowers_number)
    % lib->filtered_books[index]->borrowers_number ;

  return library_borrower_pointer_name(lib, lib->filtered_books[index]->borrowers[borrower]) ;
}

void library_book_borrower_give_back(Library *lib, int index)
{
  int borrower ;

  borrower = lib->filtered_books[index]->borrowers_number - 1 ;
  ASSERT(lib->filtered_books[index]->borrowers[borrower] < 0) ;
  lib->filtered_books[index]->borrowers[borrower] *= -1 ;
}

const char* library_book_borrower_get(Library *lib, int index
					       , int borrower)
{
  if ( lib->filtered_books[index]->borrowers_number == 0 )
    return "" ;
  return *library_book_borrower_pointer_get(lib, index, borrower) ;
}

void library_book_borrower_new(Library *lib, int index
					       , int borrower)
{
  lib->filtered_books[index]->borrowers_number++ ;
  REALLOC(lib->filtered_books[index]->borrowers,
	  lib->filtered_books[index]->borrowers_number) ;

  lib->filtered_books[index]->borrowers
    [lib->filtered_books[index]->borrowers_number-1]
    = borrower ;
}

int library_book_borrower_have_it(Library *lib, int index, int borrower)
{
  if ( lib->filtered_books[index]->borrowers_number == 0 )
    return 0 ;

  borrower = (borrower + lib->filtered_books[index]->borrowers_number)
    % lib->filtered_books[index]->borrowers_number ;

  return lib->filtered_books[index]->borrowers[borrower] <= 0 ;
}


