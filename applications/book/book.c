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
  Zmw_Boolean need_sort, need_filter ;

  int book ;
  enum { Nothing, Author_Name_For_Book, Author_Name
	 , Borrower_Name, Borrower_New
	 , Collection_Name, Collection_New }  action ;

  Columns order[Column_Last] ;
  int visible[Column_Last] ;
  int width[Column_Last] ;
  char *name[Column_Last] ;
  char *filter[Column_Last] ;
  char *default_value[Column_Last] ;

  char *or_filter ;
  int only_borrowed ;
  int only_modified ;

  int debug_window ;
  int start ;
  int nb ;

  float standard_color[3], tip_color[3], header_color[3] ;
} Library_GUI ;
  
void tip(Library_GUI *gui, const char *text)
{
  if ( zmw_tip_visible() )
    ZMW(zmw_window_popup_bottom())
    {
      zmw_rgb(gui->tip_color[0], gui->tip_color[1], gui->tip_color[2]) ;
      ZMW(zmw_decorator(Zmw_Decorator_Border_Embossed|Zmw_Decorator_Interior))
	zmw_text(text) ;
    }
}

void book_filter(Library_GUI *gui, int *start)
{
  zmw_vertical_expand(Zmw_False) ;
  ZMW(zmw_box_horizontal())
    {
      zmw_vertical_alignment(0) ;
      zmw_horizontal_expand(Zmw_False) ;
      zmw_text("Filter on all the columns :") ;
      zmw_vertical_expand(Zmw_True) ;
      zmw_horizontal_expand(Zmw_True) ;
      zmw_text_editable( &gui->or_filter ) ;
      gui->need_filter |= zmw_changed() ;
      tip(gui, "Only books containing this filter are displayed") ;
	

      zmw_horizontal_expand(Zmw_False) ;
      zmw_toggle_int_with_label(&gui->only_borrowed, "Display only borrowed books") ;
      tip(gui, "Display book borrowed and not yet given back") ;
      gui->need_filter |= zmw_activated() ;
      zmw_toggle_int_with_label(&gui->only_modified, "Display only modified books") ;
      tip(gui, "If true only modified books are displayed, it disables other filters");
      gui->need_filter |= zmw_activated() ;
    }
}



void table_header(Library_GUI *gui)
{
  void (*old_sort)(Library*) ;
  int i, j ;
  static int current = -1, old_current=0 ;

  old_sort = gui->sort ;

  zmw_horizontal_expand(Zmw_True) ;
  zmw_horizontal_alignment(0) ;

  for(i=0; i<Column_Last; i++)
    {
      j = gui->order[i] ;
      if ( gui->visible[j] )
	{
	  switch( j )
	    {
	    case Column_Title:
	      ZMW(zmw_box_horizontal())
		{
		  zmw_horizontal_expand(Zmw_False) ;
		  zmw_button( "Book title" ) ;
		  tip(gui, "Sort by book title") ;
		  if ( zmw_activated() )
		    gui->sort = library_sort_title ;
		}
	      break ;
	      
	    case Column_Author:
	      ZMW(zmw_box_horizontal())
		{
		  zmw_horizontal_expand(Zmw_False) ;
		  zmw_text( "Author" ) ;
		  zmw_button( "Firstname" ) ;
		  tip(gui, "Sort by author firstname") ;
		  if ( zmw_activated() )
		    gui->sort = library_sort_author_firstname ;
		  zmw_button( "Surname" ) ;
		  tip(gui, "Sort by author surname") ;
		  if ( zmw_activated() )
		    gui->sort = library_sort_author_surname ;
		}
	      break ;
	      
	    case Column_Borrower:
	      ZMW(zmw_box_horizontal())
		{
		  zmw_horizontal_expand(Zmw_False) ;
		  zmw_text( "Borrower" ) ;
		  zmw_button( "Firstname" ) ;
		  tip(gui, "Sort by borrower firstname") ;
		  if ( zmw_activated() )
		    gui->sort = library_sort_last_borrower_firstname ;
		  zmw_button( "Surname" ) ;
		  tip(gui, "Sort by borrower surname") ;
		  if ( zmw_activated() )
		    gui->sort = library_sort_last_borrower_surname ;
		}
	      break ;

	    case Column_Collection:
	      ZMW(zmw_box_horizontal())
		{
		  zmw_horizontal_expand(Zmw_False) ;
		  zmw_button( "Collection" ) ;
		  if ( zmw_activated() )
		    gui->sort = library_sort_collection ;
		  tip(gui, "Sort by collection") ;
		}
	      break ;
	      
	    case Column_Number:
	      ZMW(zmw_box_horizontal())
		{
		  zmw_horizontal_expand(Zmw_False) ;
		  zmw_button( "Number" ) ;
		  if ( zmw_activated() )
		    gui->sort = library_sort_number ;
		  tip(gui, "Sort by book number in collection") ;
		}
	      break ;
	      
	    case Column_Rate:
	      ZMW(zmw_box_horizontal())
		{
		  zmw_horizontal_expand(Zmw_False) ;
		  zmw_button( "Rate" ) ;
		  if ( zmw_activated() )
		    gui->sort = library_sort_rate ;
		  tip(gui, "Sort by book rate") ;
		}
	      break ;
	      
	    case Column_Last:
	      abort() ;
	    }
	  if ( zmw_drag_swap(i, (int*)gui->order, &current, &old_current) )
	    ZMW(zmw_window_drag())
	      zmw_text(gui->name[gui->order[current]]) ;
	}
    }

  if ( old_sort != gui->sort )
    gui->need_sort = Zmw_True ;
}

void table_filter(Library_GUI *gui)
{
  int i, j ;
  char tmp[99] ;

  zmw_horizontal_expand(Zmw_True) ;
  zmw_horizontal_alignment(0) ;
  zmw_height(18) ;

  for(i=0; i<Column_Last; i++)
    {
      j = gui->order[i] ;
      if ( gui->visible[j] )
	{
	  zmw_text_editable(&gui->filter[j]);
	  gui->need_filter |= zmw_changed() ;
	  sprintf(tmp, "Enter a filter for %s", gui->name[j]) ;
	  tip(gui, tmp) ;
	}
    }
}



void title(Library_GUI *gui, int i)
{
  zmw_text_editable( library_book_title_pointer_get(gui->lib, i) ) ;
}

int menu_approximation(Library_GUI *gui, Strings *choices)
{
  static int nb = 0 ;
  static Valued v[10] ;
  int j, result ;

  result = 0 ;

  zmw_text_editable(&gui->new_name) ;
  if ( zmw_changed() || nb == 0 )
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
	  zmw_color(Zmw_Color_Foreground, 0xFF0000) ;
	  borrowers_menu(gui, i) ;
	}
    }
  else
    borrowers_menu(gui, i) ;
}

void collection(Library_GUI *gui, int i)
{
  if ( gui->book == i && gui->action == Collection_Name )
    {
      zmw_text_editable( library_book_collection_pointer_get(gui->lib, i) ) ;
    }
  else if ( gui->book == i && gui->action == Collection_New )
    {
      if ( menu_approximation(gui, &gui->lib->collections) )
	{
	  library_book_collection_new(gui->lib, i, gui->selected) ;
	}
    }
  else
    {
      zmw_button( library_book_collection_get(gui->lib, i) ) ;
      if ( zmw_window_is_popped() )
	{
	  ZMW(zmw_window_popup_bottom())
	    {
	      ZMW(zmw_box_vertical())
		{
		  zmw_button("Change collection name for all books") ;
		  if ( zmw_activated() )
		    {
		      gui->book = i ;
		      gui->action = Collection_Name ;
		    }

		  zmw_button("Change collection") ;
		  if ( zmw_activated() )
		    {
		      gui->new_name[0] = '\0' ;
		      gui->book = i ;
		      gui->action = Collection_New ;
		    }
		}
	    }
	}
    }
}

void number(Library_GUI *gui, int i)
{
  zmw_int_editable( library_book_number_pointer_get(gui->lib, i) ) ;
}

void rate(Library_GUI *gui, int i)
{
  zmw_int_editable( library_book_rate_pointer_get(gui->lib, i) ) ;
}

void table_row(Library_GUI *gui, int i)
{
  char buf[9] ;
  int j, k ;

  sprintf(buf, "%x", i) ;
  zmw_name(buf) ;

  for(j=0; j<Column_Last; j++)
    {
      k = gui->order[j] ;

      if ( gui->visible[k] )
	{
	  switch( k )
	    {
	    case Column_Title:      title     (gui, i) ; break ;
	    case Column_Author:     author    (gui, i) ; break ;
	    case Column_Borrower:   borrowers (gui, i) ; break ;
	    case Column_Collection: collection(gui, i) ; break ;
	    case Column_Number:     number    (gui, i) ; break ;
	    case Column_Rate:       rate      (gui, i) ; break ;
	    case Column_Last:       abort() ;
	    }
	}
    }
}

void widths_get(Library_GUI *gui, int *widths, int *number_of_columns)
{
  int i, j, k ;

  i = 0 ;
  for(j=0; j<Column_Last; j++)
    {
      k = gui->order[j] ;
      if ( gui->visible[k] )
	{
	  widths[i++] = gui->width[k] ;
	}
    }
  *number_of_columns = i ;
}

void widths_set(Library_GUI *gui, int *widths)
{
  int i, j, k ;

  i = 0 ;
  for(j=0; j<Column_Last; j++)
    {
      k = gui->order[j] ;
      if ( gui->visible[k] )
	{
	  gui->width[k] = widths[i++] ;
	}
    }
}



void table(Library_GUI *gui)
{
  int i ;
  int widths[Column_Last] ;
  int number_of_columns ;

  book_filter(gui, &gui->start) ;

  zmw_border_width(1) ;
  zmw_focus_width(1) ;

  widths_get(gui, widths, &number_of_columns) ;

  zmw_padding_width(1) ;
  zmw_vertical_expand(Zmw_False) ;
  ZMW(zmw_box_horizontal())
    {
      zmw_rgb(gui->header_color[0], gui->header_color[1], gui->header_color[2]) ;
      ZMW(zmw_decorator(Zmw_Decorator_Interior))
	{
	  ZMW(zmw_table_with_widths(number_of_columns, widths))
	    {
	      widths_set(gui, widths) ; // Here because header can change order
	      table_filter(gui) ;
	      table_header(gui) ;
	    }
	}
    }
  if ( gui->need_filter )
    {
      gui->start = 0 ;
      library_filter(gui->lib, gui->or_filter, gui->only_borrowed
		     ,gui->filter, gui->only_modified) ;
      gui->need_filter = Zmw_False ;
      gui->need_sort = Zmw_True ;
    }
  if ( gui->need_sort && gui->sort )
    {
      (*gui->sort)(gui->lib) ;
      gui->need_sort = Zmw_False ;
      gui->action = Nothing ;
    }

  zmw_vertical_expand(Zmw_True) ;
  zmw_padding_width(1) ;
  ZMW(zmw_scrolled_view_with_columns(&gui->start, &gui->nb
				     , library_book_number(gui->lib)
				     , number_of_columns))
    {
      ZMW(zmw_table_with_widths(number_of_columns, widths))
	{
	  zmw_horizontal_expand(Zmw_True) ;
	  zmw_horizontal_alignment(-1) ;

	  for(i = gui->start;
	      i < gui->start + gui->nb && i<library_book_number(gui->lib) ;
	      i++)
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
      gui->lib = library_load(gui->filename_load) ;
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

void menu_bar_view(Library_GUI *gui)
{
  int i ;

  zmw_button("View") ;
  if ( zmw_window_is_popped() )
    {
      ZMW(zmw_window_popup_bottom_with_title("View"))
	ZMW(zmw_box_vertical())
	{
	  zmw_tearoff() ;
	  for(i=0; i<Column_Last; i++)
	    zmw_toggle_int_with_label(&gui->visible[i], gui->name[i]) ;
	  zmw_text("") ;
	  zmw_toggle_int_with_label(&gui->debug_window, "Debug window") ;
	}
    }
}

void menu_bar_edit(Library_GUI *gui)
{
  zmw_button("Edit") ;
  if ( zmw_window_is_popped() )
    {
      ZMW(zmw_window_popup_bottom_with_title("Edit"))
	ZMW(zmw_box_vertical())
	{
	  zmw_tearoff() ;
	  zmw_button("Add a new book") ;
	  if ( zmw_activated() )
	    {
	      library_book_new(gui->lib, gui->default_value) ;
	      gui->need_filter = Zmw_True ;
	      gui->only_modified = Zmw_True ;
	    }
	  zmw_horizontal_expand(Zmw_True) ;
	  zmw_text("Standard color:") ;
	  zmw_scrollbar_horizontal(&gui->standard_color[0], 0.1) ;
	  zmw_scrollbar_horizontal(&gui->standard_color[1], 0.1) ;
	  zmw_scrollbar_horizontal(&gui->standard_color[2], 0.1) ;
	  zmw_text("Tip color:") ;
	  zmw_scrollbar_horizontal(&gui->tip_color[0], 0.1) ;
	  zmw_scrollbar_horizontal(&gui->tip_color[1], 0.1) ;
	  zmw_scrollbar_horizontal(&gui->tip_color[2], 0.1) ;
	  zmw_text("Header color:") ;
	  zmw_scrollbar_horizontal(&gui->header_color[0], 0.1) ;
	  zmw_scrollbar_horizontal(&gui->header_color[1], 0.1) ;
	  zmw_scrollbar_horizontal(&gui->header_color[2], 0.1) ;
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
      menu_bar_edit(gui) ;
      menu_bar_view(gui) ;
    }
}

void debug_window(Library_GUI *gui)
{
  int i ;

  ZMW(zmw_window("Debug window"))
    {
      ZMW(zmw_box_vertical())
	{
	  ZMW(zmw_table(4))
	    {
	      zmw_text("Name") ;
	      zmw_text("Visible") ;
	      zmw_text("Width") ;
	      zmw_text("Order") ;
	      for(i=0; i<Column_Last; i++)
		{
		  zmw_text(gui->name[i]) ;
		  zmw_toggle_int(&gui->visible[i]) ;
		  zmw_int_editable(&gui->width[i]) ;
		  zmw_text(gui->name[gui->order[i]]);
		}
	    }
	  ZMW(zmw_box_horizontal()) {zmw_text("Start:"); zmw_int(gui->start); }
	  ZMW(zmw_box_horizontal()) {zmw_text("#Row:"); zmw_int(gui->nb); }
	  ZMW(zmw_box_horizontal()) {zmw_text("#Book:"); zmw_int(library_book_number(gui->lib)); }
	}	  
    }
}

void library()
{
  static Library_GUI gui = { NULL } ;
  char buf[999] ;
  int i ;

  if ( gui.lib == NULL )
    {
      gui.filename_load = strdup("./exco.lib") ;
      gui.filename_save = strdup(".") ;
      gui.filechooser_load = Zmw_False ;
      gui.action = Nothing ;
      gui.new_name = strdup("") ;
      gui.lib = library_load(gui.filename_load) ;

      gui.name[0] = "Column Title" ;
      gui.name[1] = "Column Author" ;
      gui.name[2] = "Column Borrower" ;
      gui.name[3] = "Column Collection" ;
      gui.name[4] = "Column Number" ;
      gui.name[5] = "Column Rate" ;
      gui.order[0] = Column_Title ;
      gui.order[1] = Column_Author ;
      gui.order[2] = Column_Borrower ;
      gui.order[3] = Column_Number ;
      gui.order[4] = Column_Collection ;
      gui.order[5] = Column_Rate ;
      gui.visible[Column_Title] = 1 ;
      gui.visible[Column_Author] = 1 ;
      gui.visible[Column_Borrower] = 1 ;
      gui.visible[Column_Collection] = 1 ;
      gui.visible[Column_Number] = 1 ;
      gui.visible[Column_Rate] = 1 ;
      gui.width[Column_Title] = 400 ;
      gui.width[Column_Author] = 200 ;
      gui.width[Column_Borrower] = 200 ;
      gui.width[Column_Collection] = 200 ;
      gui.width[Column_Number] = 50 ;
      gui.width[Column_Rate] = 30 ;
      gui.default_value[0] = strdup("") ;
      gui.default_value[1] = strdup("?") ;
      gui.default_value[2] = NULL ;
      gui.default_value[3] = strdup("?") ;
      gui.default_value[4] = strdup("0") ;
      gui.default_value[5] = strdup("9") ;

      for(i=0; i<Column_Last; i++)
	gui.filter[i] = strdup("") ;

      gui.or_filter = strdup("") ;
      gui.only_borrowed = Zmw_False ;
      gui.only_modified = Zmw_False ;
      gui.need_filter = Zmw_True ;

      gui.standard_color[0] = 0.75 ;
      gui.standard_color[1] = 0.75 ;
      gui.standard_color[2] = 0.75 ;
      gui.tip_color[0] = 0.75 ;
      gui.tip_color[1] = 0.75 ;
      gui.tip_color[2] = 0 ;
      gui.header_color[0] = 0 ;
      gui.header_color[1] = 0.75 ;
      gui.header_color[2] = 0 ;


      gui.start = 0 ;
      gui.nb = 10 ;

      gui.debug_window = 0 ;
    }

  zmw_rgb(gui.standard_color[0], gui.standard_color[1], gui.standard_color[2]) ;
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
  if ( gui.debug_window )
    debug_window(&gui) ;

  if ( zmw_state_change_allowed() )
    gui.library_modified = library_modified(gui.lib) ;
}

int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_run(library) ;
  return 0 ;
}
