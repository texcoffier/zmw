/*
  ZMW: A Zero Memory Widget Library
  Copyright (C) 2003-2005 Thierry EXCOFFIER

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

typedef unsigned long long Hash_Key ;

typedef struct
{
  int collection_index ;
  int number ;
  int author_index ;
  char *title ;
  int rate ;
  int borrowers_number ;
  int *borrowers ;
  Hash_Key checksum ; // To see if the book was modified
} Book ;

typedef struct
{
  int number ;
  char **strings ;
  Hash_Key checksum ;
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

typedef enum
  {
    Column_Title, Column_Author, Column_Borrower
    , Column_Collection, Column_Number, Column_Rate
    , Column_Last
  } Columns ;


/* utilities.c */
char *line_read(FILE *file) ;
char *lower_case(const char *t) ;
int strcmp_caseless(const char *a, const char *b) ;
const char *surname(const char *a) ;
Hash_Key hash_key(Hash_Key start, const char *string) ;

/* strings.c */
void strings_read(FILE *file, Strings *s) ;
void strings_write(FILE *file, const Strings *s) ;
void strings_free(Strings *s) ;
int strings_add(Strings *s, char *newv) ;
int strings_search(const char *pin, Strings *straw, Valued *index, int nb);
Hash_Key strings_checksum(Strings *s) ;
int strings_changed(Strings *s) ;
void strings_reset_checksum(Strings *s) ;

/* loadsave.c */
Hash_Key library_book_checksum(Library *lib, Book *b) ;
Library* library_load(const char *filename) ;
void library_free(Library *lib) ;
void library_save(Library *lib, const char *filename) ;

/* filter.c */
void library_filter(Library *lib, const char *filter, int only_borrowed
		    , char **filters, int only_modified) ;

/* sort.c */
void library_sort_author_surname(Library *lib) ;
void library_sort_author_firstname(Library *lib) ;
void library_sort_title(Library *lib) ;
void library_sort_last_borrower_surname(Library *lib) ;
void library_sort_last_borrower_firstname(Library *lib) ;
void library_sort_collection(Library *lib) ;
void library_sort_number(Library *lib) ;
void library_sort_rate(Library *lib) ;

/* library.c */
int         library_modified                   (Library*) ;
int         library_book_number                (Library*) ;

void        library_book_new                   (Library*, char **values) ;
void        library_book_clone                 (Library *lib, int i) ;
char**      library_borrower_pointer_name      (Library*, int borrower_id) ;
const char* library_borrower_name              (Library*, int borrower_id) ;

int         library_book_modified              (Library*, int id) ;

const char* library_book_title_get             (Library*, int id) ;
char**      library_book_title_pointer_get     (Library*, int id) ;

const char* library_book_author_get	       (Library*, int id) ;
void        library_book_author_set	       (Library*, int id, int author);
char**      library_book_author_pointer_get    (Library*, int id) ;

const char* library_book_collection_get        (Library*, int id) ;
char**      library_book_collection_pointer_get(Library*, int id) ;
void        library_book_collection_new(Library *lib, int index, int collection_index) ;
int         library_book_number_get          	(Library*, int id) ;
int         library_book_rate_get            	(Library*, int id) ;
int*        library_book_number_pointer_get  	(Library *lib, int index) ;
int*        library_book_rate_pointer_get    	(Library *lib, int index) ;

/* If borrower==-1 it is the last */
int         library_book_borrower_number     	(Library*, int id) ;
char**      library_book_borrower_pointer_get	(Library*, int id, int borrower) ;
void        library_book_borrower_give_back  	(Library*, int id)  ;
const char* library_book_borrower_get        	(Library*, int id, int borrower) ;
void        library_book_borrower_new        	(Library*, int id, int borrower) ;
int         library_book_borrower_have_it    	(Library*, int id, int borrower) ;

#endif
