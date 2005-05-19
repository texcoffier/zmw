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

#ifndef _HOME_EXCO_HOME_WIDGET_APPLICATIONS_BOOK_BOOK_H
#define _HOME_EXCO_HOME_WIDGET_APPLICATIONS_BOOK_BOOK_H

#include <locale.h>
#include <libintl.h>

#include "zmw/zmw.h"
#include "library.h"

typedef struct {
  Columns order ;
  int visible ;
  int width ;
  char *id ;
  char *default_value ;
  char *filter ;
} ColDef ;  

typedef struct {
  ColDef cols[Column_Last] ;
  float standard_color[3], tip_color[3], header_color[3], borrowed_color[3] ;
  float menu_color[3] ;
  char *language ;
  char *new_language ;
} Prefs ;

typedef struct {
  Prefs prefs ;

  Library *lib ;

  Zmw_Boolean filechooser_load, filechooser_save_as, quit_modified ;
  char *filename_load, *filename_save ;
  Zmw_Boolean library_modified ;
  char *new_name ;
  int selected ;
  Zmw_Boolean take_focus ;

  char *error_message[4] ;

  void (*sort)(Library *lib) ;
  Zmw_Boolean need_sort, need_filter ;

  int book ;
  enum { Nothing, Author_Name_For_Book, Author_Name
	 , Borrower_Name, Borrower_New
	 , Collection_Name, Collection_New }  action ;


  char *or_filter ;
  int only_borrowed ;
  int only_modified ;

  int debug_window ;
  int start ;
  int nb ;
} Library_GUI ;


typedef enum { Bottom, Right } Position ;

/*
 * book.c
 */
Columns column_name_to_index(ColDef *c, const char *name) ;
void take_focus(Library_GUI*) ;
/*
 * book-preferences.c
 */
void prefs_save(Library_GUI *gui, const char *filename) ;
void prefs_load(Library_GUI *gui, const char *filename) ;
/*
 * book-menus.c
 */
void menu_bar(Library_GUI *gui) ;
int menu_approximation(Library_GUI *gui, Strings *choices) ;
void menu_popup(float color[3], char *title, Position position
		, Zmw_Boolean tearoff) ;

#ifndef _
#define _(S) gettext(S)
#define gettext_noop(S) S
#endif


#endif
