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
/*
 * Créé le Sun Oct 12 15:22:18 2003 par Thierry EXCOFFIER
 */

#ifndef _HOME_EXCO_HOME_SRC_BIB2_APP_BIBLIO_H
#define _HOME_EXCO_HOME_SRC_BIB2_APP_BIBLIO_H

#include <stdio.h>
#include <stdlib.h>

#define MALLOC(P,N) P = malloc((N) * sizeof(*(P)))
#define REALLOC(P,N) P = realloc(P, (N) * sizeof(*(P)))
#define FREE(P) ZMW1( free(P) ; (P)=NULL ; )
#define TABLE_SIZE(T) (sizeof(T)/sizeof((T)[0]))
#define HERE fprintf(stderr,"%s:%s:%d\n", __FILE__, __FUNCTION__, __LINE__) 
#define ABORT do { HERE ; abort() ; } while(0)
#ifndef ABS
#define ABS(X) ( (X)<0 ? -(X) : (X) )
#endif
#define ASSERT(X) do{if(!(X)) { HERE ; fprintf(stderr, "%s\n", #X) ; abort() ; } } while(0)

typedef struct
{
  int collection_index ;
  int number ;
  int author_index ;
  char *title ;
  int interest ;
  int borrowers_number ;
  int *borrowers ;
} Book ;

typedef struct
{
  int number ;
  char **strings ;
} Strings ;

typedef struct
{
  int books_number ; 
  Book *books ;
  Strings collections ;
  Strings authors ;
  Strings borrowers ;

  int filtered_number ;
  Book **filtered_books ;
} Library ;

typedef struct
{
  int index ;
  int value ;
} Valued ;


char *lower_case(const char *t) ;
int strcmp_caseless(const char *a, const char *b) ;
const char *surname(const char *a) ;

void strings_read(FILE *file, Strings *s) ;
void strings_write(FILE *file, const Strings *s) ;
void strings_free(Strings *s) ;
int strings_add(Strings *s, char *newv) ;
int strings_search(const char *pin, Strings *straw, Valued *index, int nb);

char *line_read(FILE *file) ;
Library* library_load(const char *filename) ;
void library_free(Library *lib) ;
void library_save(const Library *lib, const char *filename) ;

void library_filter(Library *lib, const char *filter, int only_borrowed) ;

void library_sort_author_surname(Library *lib) ;
void library_sort_author_firstname(Library *lib) ;
void library_sort_title(Library *lib) ;
void library_sort_last_borrower_surname(Library *lib) ;
void library_sort_last_borrower_firstname(Library *lib) ;


int         library_book_number              (Library*) ;

char**      library_borrower_pointer_name    (Library*, int borrower_id) ;
const char* library_borrower_name            (Library*, int borrower_id) ;

const char* library_book_title_get           (Library*, int id) ;
char**      library_book_title_pointer_get   (Library*, int id) ;

const char* library_book_author_get	     (Library*, int id) ;
void        library_book_author_set	     (Library*, int id, int author);
char**      library_book_author_pointer_get  (Library*, int id) ;

const char* library_book_collection_get      (Library*, int id) ;

/* If borrower==-1 it is the last */
int         library_book_borrower_number     (Library*, int id) ;
char**      library_book_borrower_pointer_get(Library*, int id, int borrower) ;
void        library_book_borrower_give_back  (Library*, int id)  ;
const char* library_book_borrower_get        (Library*, int id, int borrower) ;
void        library_book_borrower_new        (Library*, int id, int borrower) ;
int         library_book_borrower_have_it    (Library*, int id, int borrower) ;

#endif
