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

#include "zmw/zmw.h"
#include "library.h"

typedef struct {
  Library *lib ;

  Zmw_Boolean filechooser_load, filechooser_save_as, quit_modified ;
  char *filename_load, *filename_save ;
  Zmw_Boolean library_modified ;
  char *new_name ;
  int selected ;

  void (*sort)(Library *lib) ;
  Zmw_Boolean need_sort ;

  int book ;
  enum { Nothing, Author_Name_For_Book, Author_Name
	 , Borrower_Name, Borrower_New }  action ;

} Library_GUI ;
  

void book_tip_borrowers(Library *lib, int i)
{
  int j ;

  if ( library_book_borrower_number(lib,i) )
    {
      ZMW(zmw_decorator(Zmw_Decorator_Border_Embossed))
	zmw_text( "Borrowers:" ) ;
      for(j=0; j<library_book_borrower_number(lib,i); j++)
	{
	  if ( library_book_borrower_have_it(lib, i, j) )
	    ZMW(zmw_box_vertical())
	    {
	      zmw_color(ZMW_FOREGROUND, 0xFF0000) ;
	      zmw_text(library_book_borrower_get(lib, i, j)) ;
	    }
	  else
	    zmw_text(library_book_borrower_get(lib, i, j)) ;
	}
    }
}

void book_tip(Library *lib, int i)
{
  if ( zmw_tip_visible() )
    ZMW(zmw_window_popup_bottom())
    {
      ZMW(zmw_decorator(Zmw_Decorator_Border_Embossed))
	ZMW(zmw_box_vertical())
	{
	  ZMW(zmw_decorator(Zmw_Decorator_Border_Embossed))
	    zmw_text( "Collection:" ) ;
	  zmw_text( library_book_collection_get(lib, i) ) ;

	  book_tip_borrowers(lib, i) ;
	}
    }
}

void tip(const char *text)
{
  if ( zmw_tip_visible() )
    ZMW(zmw_window_popup_bottom())
      ZMW(zmw_decorator(Zmw_Decorator_Border_Embossed))
         zmw_text(text) ;
}

void book_filter(Library_GUI *gui, int *start)
{
  static char *filter=NULL ;
  static int only_borrowed = Zmw_False ;
  static Zmw_Boolean compute_filter = Zmw_False ;

  if ( filter == NULL )
    {
      filter = strdup("") ;
    }

  zmw_vertical_expand(Zmw_False) ;
  ZMW(zmw_box_horizontal())
    {
      zmw_vertical_alignment(0) ;
      zmw_horizontal_expand(Zmw_False) ;
      zmw_text("Filter :") ;
      zmw_vertical_expand(Zmw_True) ;
      zmw_horizontal_expand(Zmw_True) ;
      zmw_text_editable( &filter ) ;
      compute_filter |= zmw_activated() ;
      tip("Only books containing this filter are displayed") ;
	

      zmw_horizontal_expand(Zmw_False) ;
      zmw_toggle_with_label(&only_borrowed, "Display only borrowed books") ;
      compute_filter |= zmw_activated() ;
    }

  if ( compute_filter )
    {
      *start = 0 ;
      library_filter(gui->lib, filter, only_borrowed) ;
      compute_filter = Zmw_False ;
      gui->need_sort = Zmw_True ;
    }
  if ( gui->need_sort && gui->sort )
    {
      (*gui->sort)(gui->lib) ;
      gui->need_sort = Zmw_False ;
      gui->action = Nothing ;

    }
}

void table_header(Library_GUI *gui, int *x)
{
  void (*old_sort)(Library*) ;

  old_sort = gui->sort ;

  ZMW(zmw_box_horizontal())
    {
      zmw_y(0) ;
      zmw_x(0) ;
      zmw_horizontal_expand(Zmw_False) ;
      zmw_button( "Book title" ) ;
      tip("Sort by book title") ;
      if ( zmw_activated() )
	gui->sort = library_sort_title ;
      zmw_x(x[0]) ;
      zmw_text( "Author" ) ;
      zmw_x(ZMW_VALUE_UNDEFINED) ;
      zmw_button( "Firstname" ) ;
      tip("Sort by author firstname") ;
      if ( zmw_activated() )
	gui->sort = library_sort_author_firstname ;
      zmw_button( "Surname" ) ;
      tip("Sort by author surname") ;
      if ( zmw_activated() )
	gui->sort = library_sort_author_surname ;
      zmw_x(x[0]+x[1]) ;
      zmw_text( "Borrower" ) ;
      zmw_x(ZMW_VALUE_UNDEFINED) ;
      zmw_button( "Firstname" ) ;
      tip("Sort by borrower firstname") ;
      if ( zmw_activated() )
	gui->sort = library_sort_last_borrower_firstname ;
      zmw_button( "Surname" ) ;
      tip("Sort by borrower surname") ;
      if ( zmw_activated() )
	gui->sort = library_sort_last_borrower_surname ;
    }

  if ( old_sort != gui->sort )
    gui->need_sort = Zmw_True ;
}

void title(Library_GUI *gui, int i)
{
  zmw_text_editable( library_book_title_pointer_get(gui->lib, i) ) ;
  if ( zmw_activated() )
    gui->library_modified = Zmw_True ;
  book_tip(gui->lib, i) ;
}

int menu_approximation(Library_GUI *gui, Strings *choices)
{
  static int nb = 0 ;
  static Valued v[10] ;
  int j, result ;

  result = 0 ;

  zmw_text_editable(&gui->new_name) ;
  if ( zmw_activated() || nb == 0 )
    {
      nb = strings_search(gui->new_name, choices, v, TABLE_SIZE(v)) ;
    }
  ZMW(zmw_window_popup_bottom())
    {
      ZMW(zmw_decorator(Zmw_Decorator_Border_Embossed))
	{
	  ZMW(zmw_box_vertical())
	    {
	      if ( strcmp_caseless(gui->new_name
				   , choices->strings[v[0].index]) != 0 )
		
		{
		  zmw_button("Set this as a new name") ;
		  if ( zmw_activated() )
		    {
		      gui->selected = strings_add(choices, gui->new_name) ;
		      result = 1 ;
		    }
		}
	      for(j=0; j<nb; j++)
		{
		  zmw_button(choices->strings[v[j].index]) ;
		  if ( zmw_activated() )
		    {
		      free(gui->new_name) ;
		      gui->selected = v[j].index ;
		      result = 1 ;
		    }
		}
	    }
	}
    }
  if ( result )
    {
      gui->action = Nothing ;			  
      gui->library_modified = Zmw_True ;
      nb = 0 ;
    }
  return result ;
}

void author(Library_GUI *gui, int i)
{
  if ( gui->book == i && gui->action == Author_Name )
    {
      zmw_text_editable( library_book_author_pointer_get(gui->lib, i) ) ;
    }
  else if ( gui->book == i && gui->action == Author_Name_For_Book )
    {
      if ( menu_approximation(gui, &gui->lib->authors) )
	{
	  library_book_author_set(gui->lib, i, gui->selected) ;
	}
    }
  else
    {
      zmw_button( library_book_author_get(gui->lib, i) ) ;
      if ( zmw_window_is_popped() )
	{
	  ZMW(zmw_window_popup_bottom())
	    {
	      ZMW(zmw_box_vertical())
		{
		  zmw_button("Change book author") ;
		  if ( zmw_activated() )
		    {
		      gui->book = i ;
		      gui->action = Author_Name_For_Book ;
		      free(gui->new_name) ;
		      gui->new_name
			= strdup(library_book_author_get(gui->lib, i)) ;
		    }
		  zmw_button("Change author name for all the books") ;
		  if ( zmw_activated() )
		    {
		      gui->book = i ;
		      gui->action = Author_Name ;
		    }
		}
	    }
	}
      book_tip(gui->lib, i) ;
    }
}

void borrowers_menu(Library_GUI *gui, int i)
{
  if ( gui->book == i && gui->action == Borrower_Name )
    {
      zmw_text_editable( library_book_borrower_pointer_get(gui->lib, i, -1) ) ;
    }
  else if ( gui->book == i && gui->action == Borrower_New )
    {
      if ( menu_approximation(gui, &gui->lib->borrowers) )
	{
	  library_book_borrower_new(gui->lib, i, -gui->selected-1) ;
	}
    }
  else
    {
      zmw_button( library_book_borrower_get(gui->lib, i, -1) ) ;
      book_tip(gui->lib, i) ;
      if ( zmw_window_is_popped() )
	{
	  ZMW(zmw_window_popup_bottom())
	    {
	      ZMW(zmw_box_vertical())
		{
		  if ( library_book_borrower_get(gui->lib, i, -1)[0] )
		    {
		      zmw_button("Change borrower name for all books") ;
		      if ( zmw_activated() )
			{
			  gui->book = i ;
			  gui->action = Borrower_Name ;
			}
		    }
		  if ( library_book_borrower_have_it(gui->lib, i, -1) )
		    {
		      zmw_button("Last borrower give back the book") ;
		      if ( zmw_activated() )
			{
			  library_book_borrower_give_back(gui->lib, i) ;
			  gui->library_modified = Zmw_True ;
			}
		    }
		  else
		    {
		      zmw_button("This book is borrowed by...") ;
		      if ( zmw_activated() )
			{
			  gui->new_name[0] = '\0' ;
			  gui->book = i ;
			  gui->action = Borrower_New ;
			}
		    }
		}
	    }
	}
    }
}

void borrowers(Library_GUI *gui, int i)
{
  if ( library_book_borrower_have_it(gui->lib, i, -1) )
    {
      ZMW(zmw_box_vertical())
	{
	  zmw_color(ZMW_FOREGROUND, 0xFF0000) ;
	  borrowers_menu(gui, i) ;
	}
    }
  else
    borrowers_menu(gui, i) ;
}

void table_row(Library_GUI *gui, int i)
{
  char buf[9] ;

  sprintf(buf, "%x", i) ;
  zmw_name(buf) ;

  title(gui, i) ;
  author(gui, i) ;
  borrowers(gui, i) ;
}


void table(Library_GUI *gui)
{
  int i ;
  static int start=0, nb=10 ;
  static int x[] = { 400, 200, 200 } ;

  book_filter(gui, &start) ;
  table_header(gui, x) ;

  zmw_vertical_expand(Zmw_True) ;
  zmw_padding_width(1) ;
  zmw_border_width(1) ;
  zmw_focus_width(1) ;

  ZMW(zmw_scrolled_view_with_columns(&start, &nb, library_book_number(gui->lib),3 ))
    {
      ZMW(zmw_table_with_widths(3, x))
	{
	  // zmw_vertical_expand(Zmw_False) ;
	  // zmw_height(17) ;

	  for(i=start; i <start+nb && i<library_book_number(gui->lib) ; i++)
	    table_row(gui, i) ;
	}
    }
  if ( zmw_activated() )
    gui->action = Nothing ;
}

void menu_bar_file(Library_GUI *gui)
{
  zmw_button("File") ;
  if ( zmw_window_is_popped() )
    {
      ZMW(zmw_window_popup_bottom_with_title("File"))
	ZMW(zmw_box_vertical())
	{
	  zmw_tearoff() ;
	  zmw_button_with_accelerator("Load", GDK_CONTROL_MASK, 'L') ;
	  if ( zmw_activated() )
	    gui->filechooser_load = Zmw_True ;
	  zmw_button_with_accelerator("Save", GDK_CONTROL_MASK, 'S') ;
	  if ( zmw_activated() )
	    {
	      library_save(gui->lib, gui->filename_load) ;
	      gui->library_modified = Zmw_False ;
	    }
	  zmw_button_with_accelerator("Save As", GDK_CONTROL_MASK, 'W') ;
	  if ( zmw_activated() )
	    gui->filechooser_save_as = Zmw_True ;

	  zmw_button_with_accelerator("Quit", GDK_CONTROL_MASK, 'Q') ;
	  if ( zmw_activated() )
	    {
	      if ( !gui->library_modified )
		zmw_exit(0) ;
	      gui->quit_modified = Zmw_True ;
	    }
	}

    }
  /*
   *
   */
  zmw_filechooser(&gui->filechooser_load, &gui->filename_load
		  , "Library Load", "Library Load") ;
  if ( zmw_activated() )
    {
      library_free(gui->lib) ;
      gui->lib = library_load("FILE/exco.lib") ;
      gui->library_modified = Zmw_False ;
    }  
  /*
   *
   */
  zmw_filechooser(&gui->filechooser_save_as, &gui->filename_save
		  , "Library Save As", "Library Save As") ;
  if ( zmw_activated() )
    {
      library_save(gui->lib, gui->filename_save) ;
    }  
  /*
   *
   */
  if ( gui->quit_modified )
    {
      ZMW(zmw_window("Quit confirmation"))
	{
	  ZMW(zmw_box_horizontal())
	    {
	      zmw_button("Quit without save") ;
	      if ( zmw_activated() )
		zmw_exit(0) ;
	      zmw_button("Save and Quit") ;
	      if ( zmw_activated() )
		{
		  library_save(gui->lib, gui->filename_load) ;
		  zmw_exit(0) ;
		}
	      zmw_button("Cancel") ;
	      if ( zmw_activated() )
		gui->quit_modified = Zmw_False ;
	    }
	}
    }  

}

void menu_bar(Library_GUI *gui)
{
  zmw_vertical_expand(Zmw_False) ;
  zmw_horizontal_alignment(-1) ;
  zmw_text("THIS PROGRAM IS UNFINISHED: DO NOT USE ON YOUR LIBRARY");
  ZMW(zmw_box_horizontal())
    {
      zmw_horizontal_expand(Zmw_False) ;
      menu_bar_file(gui) ;
    }
}

void library()
{
  static Library_GUI gui = { NULL } ;
  char buf[999] ;

  if ( gui.lib == NULL )
    {
      gui.filename_load = strdup("./exco.lib") ;
      gui.filename_save = strdup(".") ;
      gui.filechooser_load = Zmw_False ;
      gui.action = Nothing ;
      gui.new_name = strdup("") ;
      gui.lib = library_load(gui.filename_load) ;
    }

  sprintf(buf, "Library: %s%s", gui.filename_load
	  , gui.library_modified ? " (Modified)" : "") ;
  ZMW(zmw_window(buf))
    {
      ZMW(zmw_box_vertical())
	{
	  zmw_vertical_alignment(-1) ;
	  menu_bar(&gui) ;
	  table(&gui) ;
	}
    }
}

int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_run(library) ;
  return 0 ;
}
