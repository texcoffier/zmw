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


#include <time.h>
#include <errno.h>
#include "book.h"

void tip(Library_GUI *gui, const char *text)
{
  ZMW ( zmw_tip() )
    {
      zmw_rgb(gui->prefs.tip_color[0], gui->prefs.tip_color[1], gui->prefs.tip_color[2]) ;
      ZMW(zmw_window_popup_bottom())
	ZMW(zmw_decorator(Zmw_Decorator_Border_Embossed|Zmw_Decorator_Interior))
	{
	   zmw_label(text) ;
	}
    }
}

void book_filter(Library_GUI *gui, int *start)
{
  zmw_vertical_expand(Zmw_False) ;
  ZMW(zmw_hbox())
    {
      zmw_vertical_alignment(0) ;
      zmw_horizontal_expand(Zmw_False) ;
      zmw_label(_("Filter on all the columns :")) ;
      zmw_vertical_expand(Zmw_True) ;
      zmw_horizontal_expand(Zmw_True) ;
      zmw_entry( &gui->or_filter ) ;
      gui->need_filter |= zmw_changed() ;
      tip(gui, _("Only books containing this filter are displayed")) ;
	

      zmw_horizontal_expand(Zmw_False) ;
      zmw_check_button_int_with_label(&gui->only_borrowed
				, _("Display only borrowed books")) ;
      tip(gui, _("Display book borrowed and not yet given back")) ;
      gui->need_filter |= zmw_activated() ;
      zmw_check_button_int_with_label(&gui->only_modified
				, _("Display only modified books")) ;
      tip(gui, _("If true only modified books are displayed, it disables other filters"));
      gui->need_filter |= zmw_activated() ;
    }
}



void table_header(Library_GUI *gui)
{
  void (*old_sort)(Library*) ;
  int i, j ;
  static int *current = NULL ;

  old_sort = gui->sort ;

  zmw_horizontal_expand(Zmw_True) ;
  zmw_horizontal_alignment(0) ;

  for(i=0; i<Column_Last; i++)
    {
      j = gui->prefs.cols[i].order ;
      if ( gui->prefs.cols[j].visible )
	{
	  ZMW(zmw_hbox())
	    {
	      zmw_horizontal_expand(Zmw_False) ;
	      switch( j )
		{
		case Column_Title:
		  zmw_button( _("Book title") ) ;
		  tip(gui, _("Sort by book title")) ;
		  if ( zmw_activated() )
		    {
		      gui->sort = library_sort_title ;
		    }
		  break ;
	      
		case Column_Author:
		  zmw_label( _("Author") ) ;
		  zmw_button( _("Firstname") ) ;
		  tip(gui, _("Sort by author firstname")) ;
		  if ( zmw_activated() )
		    gui->sort = library_sort_author_firstname ;
		  zmw_button( _("Surname") ) ;
		  tip(gui, _("Sort by author surname")) ;
		  if ( zmw_activated() )
		    gui->sort = library_sort_author_surname ;
		  break ;
	      
		case Column_Borrower:
		  zmw_label( _("Borrower") ) ;
		  zmw_button( _("Firstname") ) ;
		  tip(gui, _("Sort by borrower firstname")) ;
		  if ( zmw_activated() )
		    gui->sort = library_sort_last_borrower_firstname ;
		  zmw_button( _("Surname") ) ;
		  tip(gui, _("Sort by borrower surname")) ;
		  if ( zmw_activated() )
		    gui->sort = library_sort_last_borrower_surname ;
		  break ;

		case Column_Collection:
		  zmw_button( _("Collection") ) ;
		  if ( zmw_activated() )
		    gui->sort = library_sort_collection ;
		  tip(gui, _("Sort by collection")) ;
		  break ;
	      
		case Column_Number:
		  zmw_button( _("Number") ) ;
		  if ( zmw_activated() )
		    gui->sort = library_sort_number ;
		  tip(gui, _("Sort by book number in collection")) ;
		  break ;
	      
		case Column_Rate:
		  zmw_button( _("Rate") ) ;
		  if ( zmw_activated() )
		    gui->sort = library_sort_rate ;
		  tip(gui, _("Sort by book rate")) ;
	      break ;
	      
		case Column_Last:
		  abort() ;
		}
	    }
	  zmw_drag_swap((int*)&gui->prefs.cols[i].order, &current) ;
	  ZMW( zmw_if( zmw_drag_from_running() ) )
	    {
	      ZMW(zmw_window_drag())
		{
		  zmw_label(_(gui->prefs.cols[current ? *current : 0].id)) ;
		}
	    }
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
      j = gui->prefs.cols[i].order ;
      if ( gui->prefs.cols[j].visible )
	{
	  zmw_entry(&gui->prefs.cols[j].filter);
	  gui->need_filter |= zmw_changed() ;
	  sprintf(tmp, _("Enter a filter for %s"), _(gui->prefs.cols[j].id)) ;
	  tip(gui, tmp) ;
	}
    }
}



void title(Library_GUI *gui, int i)
{
  zmw_entry(library_book_title_pointer_get(gui->lib, i)) ;
}

void take_focus(Library_GUI *gui)
{
  if ( gui->take_focus )
    {
      zmw_name_register(zmw_focus_get()) ;
      gui->take_focus = Zmw_False ;
    }
}

void author(Library_GUI *gui, int i)
{
  if ( gui->book == i && gui->action == Author_Name )
    {
      zmw_entry( library_book_author_pointer_get(gui->lib, i) ) ;
      take_focus(gui) ;
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
      ZMW( zmw_menu_without_accelerators() )
	{
	  ZMW(menu_popup(gui->prefs.menu_color, "notitle", Bottom, Zmw_False))
	    {
	      zmw_button(_("Change book author")) ;
	      if ( zmw_activated() )
		{
		  gui->book = i ;
		  gui->action = Author_Name_For_Book ;
		  if ( gui->new_name ) free(gui->new_name) ;
		  gui->new_name
		    = strdup(library_book_author_get(gui->lib, i)) ;
		  gui->take_focus = Zmw_True ;
		}

	      zmw_button(_("Change author name for all the books")) ;
	      if ( zmw_activated() )
		{
		  gui->book = i ;
		  gui->action = Author_Name ;
		  gui->take_focus = Zmw_True ;
		}
	    }
	}
    }
}

void borrowers_menu(Library_GUI *gui, int i)
{
  if ( gui->book == i && gui->action == Borrower_Name )
    {
      zmw_entry( library_book_borrower_pointer_get(gui->lib, i, -1) ) ;
      take_focus(gui) ;
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
      ZMW( zmw_menu_without_accelerators() )
	{
	  ZMW(menu_popup(gui->prefs.menu_color, "notitle", Bottom, Zmw_False))
	    {
	      if ( library_book_borrower_have_it(gui->lib, i, -1) )
		{
		  zmw_button(_("Last borrower give back the book")) ;
		  if ( zmw_activated() )
		    {
		      library_book_borrower_give_back(gui->lib, i) ;
		    }
		}
	      else
		{
		  zmw_button(_("This book is borrowed by...")) ;
		  if ( zmw_activated() )
		    {
		      if ( gui->new_name ) free(gui->new_name) ;
		      gui->new_name = strdup("") ;
		      gui->book = i ;
		      gui->action = Borrower_New ;
		      gui->take_focus = Zmw_True ;
		    }
		}
	      if ( library_book_borrower_get(gui->lib, i, -1)[0] )
		{
		  zmw_button(_("Change borrower name for all books")) ;
		  if ( zmw_activated() )
		    {
		      gui->book = i ;
		      gui->action = Borrower_Name ;
		      gui->take_focus = Zmw_True ;
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
      ZMW(zmw_void())
	{
	  zmw_rgb(gui->prefs.borrowed_color[0]
		  , gui->prefs.borrowed_color[1], gui->prefs.borrowed_color[2]) ;
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
      zmw_entry( library_book_collection_pointer_get(gui->lib, i) ) ;
      take_focus(gui) ;
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
      ZMW( zmw_menu_without_accelerators() )
	{
	  ZMW(menu_popup(gui->prefs.menu_color, "notitle", Bottom, Zmw_False))
	    {
	      zmw_button(_("Change collection")) ;
	      if ( zmw_activated() )
		{
		  if ( gui->new_name ) free(gui->new_name) ;
		  gui->new_name = strdup("") ;
		  gui->book = i ;
		  gui->action = Collection_New ;
		  gui->take_focus = Zmw_True ;
		}

	      zmw_button(_("Change collection name for all books")) ;
	      if ( zmw_activated() )
		{
		  gui->book = i ;
		  gui->action = Collection_Name ;
		  gui->take_focus = Zmw_True ;
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

  sprintf(buf, "[%x]", i) ;
  zmw_name(buf) ;

  for(j=0; j<Column_Last; j++)
    {
      k = gui->prefs.cols[j].order ;

      if ( gui->prefs.cols[k].visible )
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
      k = gui->prefs.cols[j].order ;
      if ( gui->prefs.cols[k].visible )
	{
	  widths[i++] = gui->prefs.cols[k].width ;
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
      k = gui->prefs.cols[j].order ;
      if ( gui->prefs.cols[k].visible )
	{
	  gui->prefs.cols[k].width = widths[i++] ;
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
  ZMW(zmw_void())
    {
      zmw_rgb(gui->prefs.header_color[0], gui->prefs.header_color[1], gui->prefs.header_color[2]) ;
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
      char *t[Column_Last] ;
      int i ;

      for(i=0; i<Column_Last; i++)
	t[i] = gui->prefs.cols[i].filter ;

      gui->start = 0 ;
      library_filter(gui->lib, gui->or_filter, gui->only_borrowed
		     , t, gui->only_modified) ;
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
  /* 14/5/2004 */
  if ( zmw_activated() )
    gui->action = Nothing ;

}


void debug_window(Library_GUI *gui)
{
  int i ;

  ZMW(zmw_window(_("Debug window")))
    {
      ZMW(zmw_vbox())
	{
	  ZMW(zmw_table(4))
	    {
	      zmw_label(_("Name")) ;
	      zmw_label(_("Visible")) ;
	      zmw_label(_("Width")) ;
	      zmw_label(_("Order")) ;
	      for(i=0; i<Column_Last; i++)
		{
		  zmw_label(_(gui->prefs.cols[i].id)) ;
		  zmw_check_button_int(&gui->prefs.cols[i].visible) ;
		  zmw_int_editable(&gui->prefs.cols[i].width) ;
		  zmw_label(_(gui->prefs.cols[gui->prefs.cols[i].order].id));
		}
	    }
	  ZMW(zmw_table(2))
	    {
	      zmw_label(_("Start:")) ;
	      zmw_int(gui->start);
	      zmw_label(_("#Row:")) ;
	      zmw_int(gui->nb) ;
	      zmw_label(_("#Book:")) ;
	      zmw_int(library_book_number(gui->lib)) ;
	    }
	}	  
    }
}

Columns column_name_to_index(ColDef *c, const char *name)
{
  Columns i ;

  for(i=0; i<Column_Last; i++)
    if ( strcmp(name, c[i].id) == 0 )
      return i ;

  ZMW_ABORT ;
}

void set_language(const char *lang)
{
  char *a ;

  setenv ("LANGUAGE", lang, 1);
  setlocale(LC_ALL, "") ;
  a = setlocale(LC_MESSAGES, lang) ;
  if ( a == NULL )
    {
      zmw_printf("locale problem with %s\n", lang) ;
    }
}

void change_language(Library_GUI *gui)
{
  if ( gui->prefs.new_language == NULL )
    return ;

  gui->prefs.language = gui->prefs.new_language ;
  gui->prefs.new_language = NULL ;
  set_language(gui->prefs.language) ;
}

void library_top_level(Library_GUI *gui)
{
  ZMW(zmw_vbox())
    {
      zmw_horizontal_expand(Zmw_True) ;
      zmw_vertical_alignment(-1) ;
      menu_bar(gui) ;
      table(gui) ;
    }
}

static Library_GUI gui =
  {
    {
      {
	{ Column_Title     , 1, 400, gettext_noop("Title"     ), "" , ""},
	{ Column_Author    , 1, 200, gettext_noop("Author"    ), "?", ""},
	{ Column_Borrower  , 1, 200, gettext_noop("Borrower"  ), "" , ""},
	{ Column_Collection, 1, 200, gettext_noop("Collection"), "?", ""},
	{ Column_Number    , 1,  50, gettext_noop("Number"    ), "0", ""},
	{ Column_Rate      , 1,  30, gettext_noop("Rate"      ), "9", ""},
      },
      { 0.75, 0.75, 0.75},
      { 0.75, 0.75, 0   },
      { 0   , 0.75, 0   },
      { 0.75, 0.5 , 0.75},
      { 0.9 , 0.9 , 0.9 },
      .language = "C",
      .new_language = NULL,
    },
    NULL
  } ;


void library()
{
  char buf[999] ;
  int i ;

  if ( gui.lib == NULL )
    {
      bindtextdomain("zmwbook","locale") ;
      textdomain("zmwbook") ;

      gui.filename_save = strdup(".") ;
      gui.filechooser_load = Zmw_False ;
      gui.action = Nothing ;
      gui.new_name = NULL ;
      gui.lib = library_load(gui.filename_load) ;

      for(i=0; i<Column_Last; i++)
	{
	  gui.prefs.cols[i].default_value = strdup(gui.prefs.cols[i].default_value) ;
	  gui.prefs.cols[i].filter        = strdup(gui.prefs.cols[i].filter) ;
	}

      gui.or_filter = strdup("") ;
      gui.only_borrowed = Zmw_False ;
      gui.only_modified = Zmw_False ;
      gui.need_filter = Zmw_True ;
      gui.start = 0 ;
      gui.nb = 10 ;
      gui.debug_window = 0 ;
      gui.error_message[0] = NULL ;
      gui.take_focus = Zmw_False ;

      gui.prefs.language = getenv("LANGUAGE") ? getenv("LANGUAGE") : "C" ;

      prefs_load(&gui, "book-preferences.xml") ;      

      gui.prefs.new_language = gui.prefs.language ;

      change_language(&gui) ;
    }


  zmw_rgb(gui.prefs.standard_color[0], gui.prefs.standard_color[1], gui.prefs.standard_color[2]) ;
  sprintf(buf, _("Library: %s%s"), gui.filename_load
	  , gui.library_modified ? _(" (Modified)") : "") ;
  ZMW(zmw_window(buf))
    {
      library_top_level(&gui) ;
    }

  if ( gui.debug_window )
    debug_window(&gui) ;


  ZMW(gui.error_message[0] = (char*)zmw_message((int)gui.error_message[0],
					 _("Error message for book"),
					 _("Close")) )
    {
      ZMW(zmw_vbox())
	{
	  for(i=0; gui.error_message[i]; i++)
	    zmw_label(gui.error_message[i]) ;
	}
    }

  change_language(&gui) ;
  if ( zmw_state_change_allowed() )
    {
      static int last_update = 0 ;

      if ( last_update != time(NULL) )
	{
	  gui.library_modified = library_modified(gui.lib) ;
	  last_update = time(NULL) ;
	}
    }
}

int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;

  if (argc == 2)
    gui.filename_load = strdup(argv[1]) ;
  else
    gui.filename_load = strdup("./exco.lib") ;

  zmw_main(library) ;
  return 0 ;
}
