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

static Library *global_lib ;



int library_compare_author_surname(const void *a, const void *b)
{
  const Book **aa = (void*)a ;
  const Book **bb = (void*)b ;

  return strcmp_caseless(surname(global_lib->authors.strings[(*aa)->author_index])
			 ,surname(global_lib->authors.strings[(*bb)->author_index])
			 )
    ;
}

void library_sort_author_surname(Library *lib)
{
  global_lib = lib ;
  qsort(lib->filtered_books, lib->filtered_number
	, sizeof(*lib->filtered_books), library_compare_author_surname) ;
}



int library_compare_author_firstname(const void *a, const void *b)
{
  const Book **aa = (void*)a ;
  const Book **bb = (void*)b ;

  return strcmp_caseless(global_lib->authors.strings[(*aa)->author_index]
			 ,global_lib->authors.strings[(*bb)->author_index]
			 )
    ;
}

void library_sort_author_firstname(Library *lib)
{
  global_lib = lib ;
  qsort(lib->filtered_books, lib->filtered_number
	, sizeof(*lib->filtered_books), library_compare_author_firstname) ;
}



int library_compare_title(const void *a, const void *b)
{
  const Book **aa = (void*)a ;
  const Book **bb = (void*)b ;

  return strcmp_caseless((*aa)->title, (*bb)->title) ;
}

void library_sort_title(Library *lib)
{
  qsort(lib->filtered_books, lib->filtered_number
	, sizeof(*lib->filtered_books), library_compare_title) ;
}


int library_compare_collection(const void *a, const void *b)
{
  const Book **aa = (void*)a ;
  const Book **bb = (void*)b ;

  return strcmp_caseless(global_lib->collections.strings[(*aa)->collection_index]
		       , global_lib->collections.strings[(*bb)->collection_index]) ;
}

void library_sort_collection(Library *lib)
{
  global_lib = lib ;
  qsort(lib->filtered_books, lib->filtered_number
	, sizeof(*lib->filtered_books), library_compare_collection) ;
}

int library_compare_rate(const void *a, const void *b)
{
  const Book **aa = (void*)a ;
  const Book **bb = (void*)b ;

  return (*aa)->rate - (*bb)->rate ;
}

void library_sort_rate(Library *lib)
{
  qsort(lib->filtered_books, lib->filtered_number
	, sizeof(*lib->filtered_books), library_compare_rate) ;
}


int library_compare_number(const void *a, const void *b)
{
  const Book **aa = (void*)a ;
  const Book **bb = (void*)b ;

  return (*aa)->number - (*bb)->number ;
}

void library_sort_number(Library *lib)
{
  qsort(lib->filtered_books, lib->filtered_number
	, sizeof(*lib->filtered_books), library_compare_number) ;
}



static const char *last_borrower_name(Library *lib, const Book **book)
{
  return library_book_borrower_get(lib, book - (const Book **)lib->filtered_books, -1) ;
}

int library_compare_last_borrower_firstname(const void *a, const void *b)
{
  const Book **aa = (void*)a ;
  const Book **bb = (void*)b ;

  return strcmp_caseless(last_borrower_name(global_lib, aa)
			 , last_borrower_name(global_lib, bb)) ;
}

void library_sort_last_borrower_firstname(Library *lib)
{
  global_lib = lib ;
  qsort(lib->filtered_books, lib->filtered_number
	, sizeof(*lib->filtered_books), library_compare_last_borrower_firstname) ;
}

int library_compare_last_borrower_surname(const void *a, const void *b)
{
  const Book **aa = (void*)a ;
  const Book **bb = (void*)b ;

  return strcmp_caseless(surname(last_borrower_name(global_lib, aa))
			 , surname(last_borrower_name(global_lib, bb))) ;
}

void library_sort_last_borrower_surname(Library *lib)
{
  global_lib = lib ;
  qsort(lib->filtered_books, lib->filtered_number
	, sizeof(*lib->filtered_books), library_compare_last_borrower_surname) ;
}
