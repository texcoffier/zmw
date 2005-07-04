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

#include <errno.h>
#include "book.h"

void menu_popup(float color[3], char *title, Position position
		, Zmw_Boolean tearoff)
{
  ZMW_EXTERNAL_RESTART ;

  ZMW(position == Bottom
      ? (tearoff ? zmw_window_popup_bottom_with_title(title)
	 : zmw_window_popup_bottom())
      : (tearoff ? zmw_window_popup_right_with_title(title)
	 : zmw_window_popup_right())
      )
    {
      zmw_padding_width(3) ;
      zmw_border_width(3) ;
      zmw_rgb(color[0], color[1], color[2]) ;

      ZMW(zmw_decorator(Zmw_Decorator_Border_Solid))
	ZMW(zmw_vbox())
	{
	  zmw_border_width(1) ;
	  zmw_padding_width(1) ;
	  if ( tearoff )
	    zmw_tearoff() ;
	  ZMW_EXTERNAL ;
	}
    }
  ZMW_EXTERNAL_STOP ;
}

static void menu_bar_file(Library_GUI *gui)
{
  zmw_button(_("File")) ;
  ZMW( zmw_menu() )
    {
      ZMW(menu_popup(gui->prefs.menu_color,_("File"), Bottom, Zmw_True))
	{
	  zmw_button_with_accelerator(_("Load"), GDK_CONTROL_MASK, 'L') ;
	  if ( zmw_activated() )
	    gui->filechooser_load = Zmw_True ;
	  zmw_button_with_accelerator(_("Save"), GDK_CONTROL_MASK, 'S') ;
	  if ( zmw_activated() )
	    {
	      library_save(gui->lib, gui->filename_load) ;
	    }
	  zmw_button_with_accelerator(_("Save As"), GDK_CONTROL_MASK, 'W') ;
	  if ( zmw_activated() )
	    gui->filechooser_save_as = Zmw_True ;
	  
	  zmw_button(_("Preference Save")) ;
	  if ( zmw_activated() )
	    prefs_save(gui, "book-preferences.xml") ;

	  zmw_button_with_accelerator(_("Quit"), GDK_CONTROL_MASK, 'Q') ;
	  if ( zmw_activated() )
	    {
	      if ( !gui->library_modified )
		zmw_main_quit(0) ;
	      gui->quit_modified = Zmw_True ;
	    }
	}
    }
  /*
   *
   */
  zmw_file_selection(&gui->filechooser_load, &gui->filename_load
		  , _("Library Load"), _("Library Load")) ;
  if ( zmw_activated() )
    {
      library_free(gui->lib) ;
      gui->lib = library_load(gui->filename_load) ;
      gui->library_modified = Zmw_False ;
    }  
  /*
   *
   */
  zmw_file_selection(&gui->filechooser_save_as, &gui->filename_save
		  , _("Library Save As"), _("Library Save As")) ;
  if ( zmw_activated() )
    {
      library_save(gui->lib, gui->filename_save) ;
    }  
  /*
   *
   */
  if ( gui->quit_modified )
    {
      ZMW(zmw_window(_("Quit confirmation")))
	{
	  ZMW(zmw_hbox())
	    {
	      zmw_button(_("Quit without save")) ;
	      if ( zmw_activated() )
		zmw_main_quit(0) ;
	      zmw_button(_("Save and Quit")) ;
	      if ( zmw_activated() )
		{
		  library_save(gui->lib, gui->filename_load) ;
		  zmw_main_quit(0) ;
		}
	      zmw_button(_("Cancel")) ;
	      if ( zmw_activated() )
		gui->quit_modified = Zmw_False ;
	    }
	}
    }  

}

static void menu_bar_view(Library_GUI *gui)
{
  int i ;

  zmw_button(_("View")) ;
  ZMW( zmw_menu() )
    {
      ZMW(menu_popup(gui->prefs.menu_color,_("View"),Bottom, Zmw_True))
	{
	  for(i=0; i<Column_Last; i++)
	    zmw_check_button_int_with_label(&gui->prefs.cols[i].visible, _(gui->prefs.cols[i].id)) ;
	  zmw_label("") ;
	  zmw_check_button_int_with_label(&gui->debug_window, _("Debug window")) ;
	}
    }
}

static void color_editor(const char *label, float c[3])
{
  zmw_rgb(c[0], c[1], c[2]) ;

  ZMW(zmw_decorator(Zmw_Decorator_Interior))
    {
      ZMW(zmw_vbox())
	{
	  zmw_label(label) ;
	  zmw_hscrollbar(&c[0], 0.1) ;
	  zmw_hscrollbar(&c[1], 0.1) ;
	  zmw_hscrollbar(&c[2], 0.1) ;
	}
    }
}

static void menu_bar_edit_color(Library_GUI *gui)
{
  zmw_button(_("Interface colors")) ;
  ZMW( zmw_menu() )
    {
      ZMW(menu_popup(gui->prefs.menu_color,_("Edit colors"), Right, Zmw_True))
	{
	  zmw_horizontal_expand(Zmw_True) ;
	  zmw_padding_width(0) ;
	  color_editor(_("Standard"), gui->prefs.standard_color) ;
	  color_editor(_("Tip")     , gui->prefs.tip_color) ;
	  color_editor(_("Header")  , gui->prefs.header_color) ;
	  color_editor(_("Borrowed"), gui->prefs.borrowed_color) ;
	  color_editor(_("Menu")    , gui->prefs.menu_color) ;
	}
    }
}

static void menu_bar_edit_default(Library_GUI *gui)
{
  int i ;

  zmw_button(_("New book defaults")) ;
  ZMW( zmw_menu() )
    {
      ZMW(menu_popup(gui->prefs.menu_color,_("New book defaults"), Right, Zmw_True))
	{
	  zmw_horizontal_expand(Zmw_True) ;
	  //	  zmw_padding_width(0) ;
	  
	  ZMW(zmw_table(2))
	    {
	      for(i=0; i<Column_Last; i++)
		{
		  if ( i == Column_Borrower )
		    continue ;
		  zmw_label(_(gui->prefs.cols[i].id)) ;
		  zmw_entry(&gui->prefs.cols[i].default_value) ;
		}
	    }
	}
    }
}

static void menu_bar_edit_language(Library_GUI *gui)
{
  int i, lang ;
  static char *langs[] = { "C", "fr_FR.utf8" } ;
  static char *languages[] = { "English", "Français" } ;

  zmw_button(_("Language")) ;
  ZMW( zmw_menu() )
    {
      ZMW(menu_popup(gui->prefs.menu_color,_("Language"), Right, Zmw_True))
	{
	  lang = 0 ; // Default language
	  for(i=0; i<ZMW_TABLE_SIZE(langs); i++)
	    if ( strcmp(gui->prefs.new_language
			? gui->prefs.new_language
			: gui->prefs.language
			, langs[i]) == 0 )
	      lang = i ;

	  for(i=0; i<ZMW_TABLE_SIZE(langs); i++)
	    zmw_radio_button_with_label(&lang, i, languages[i]) ;

	  if ( strcmp(gui->prefs.language, langs[lang]) )
	    {
	      gui->prefs.new_language = langs[lang] ;
	    }
	}
    }
}

static void menu_bar_edit(Library_GUI *gui)
{
  zmw_name("Edit") ;
  zmw_button(_("Edit")) ;
  zmw_name("EditPopup") ;
  ZMW( zmw_menu() )
    {
      ZMW(menu_popup(gui->prefs.menu_color,_("Edit"), Bottom, Zmw_True))
	{
	  zmw_button_with_accelerator(_("Add a new book"), GDK_CONTROL_MASK, 'N') ;
	  if ( zmw_activated() )
	    {
	      char *t[Column_Last] ;
	      int i ;

	      for(i=0; i<Column_Last; i++)
		t[i] = gui->prefs.cols[i].default_value ;

	      library_book_new(gui->lib, t) ;
	      gui->need_filter = Zmw_True ;
	      gui->only_modified = Zmw_True ;
	    }

	  zmw_button_with_accelerator(_("Clone last book"), GDK_CONTROL_MASK, 'C') ;
	  if ( zmw_activated() )
	    {
	      library_book_clone(gui->lib, -1) ;
	      gui->need_filter = Zmw_True ;
	      gui->only_modified = Zmw_True ;
	    }

	  menu_bar_edit_color(gui) ;
	  menu_bar_edit_default(gui) ;
	  menu_bar_edit_language(gui) ;
	}
    }
}

void semantic_len(Library_GUI *gui)
{
  static int stop = 0 ;
  extern void library_top_level(Library_GUI *gui) ;
  extern void set_language(char*) ;

  zmw_button(_("FRENCH Semantic Len")) ;
  ZMW( zmw_if(stop == 0 && zmw_window_is_popped()) )
    {
      stop = 1 ;

      set_language("fr_FR.utf8") ;

      ZMW(zmw_window_drag())
	{
	  zmw_width(100) ;
	  zmw_height(100) ;
	  ZMW(zmw_decorator(Zmw_Decorator_Border_Solid
			    | Zmw_Decorator_Clip
			    | Zmw_Decorator_Translate
			    , -zmw_zmw_x_get() + 110, -zmw_zmw_y_get() + 112))
	    {
	      library_top_level(gui) ;
	    }
	}

      set_language(gui->prefs.language) ;

      stop = 0 ;
    }
}

void menu_bar(Library_GUI *gui)
{
  zmw_vertical_expand(Zmw_False) ;
  zmw_horizontal_alignment(-1) ;
  zmw_label(_("THIS PROGRAM IS UNFINISHED: DO NOT USE ON YOUR LIBRARY"));
  ZMW(zmw_hbox())
    {
      zmw_horizontal_expand(Zmw_False) ;
      menu_bar_file(gui) ;
      menu_bar_edit(gui) ;
      menu_bar_view(gui) ;
      semantic_len(gui) ;
    }
}

int menu_approximation(Library_GUI *gui, Strings *choices)
{
  static int nb = 0 ;
  static Valued v[10] ;
  int j, result ;

  result = 0 ;

  zmw_entry(&gui->new_name) ;
  take_focus(gui) ;
  if ( zmw_changed() || nb == 0 )
    {
      nb = strings_search(gui->new_name, choices, v, TABLE_SIZE(v)) ;
    }
  ZMW(menu_popup(gui->prefs.menu_color, "notitle", Bottom, Zmw_False))
    {
      if ( strcmp_caseless(gui->new_name, choices->strings[v[0].index]) != 0 )
	{
	  zmw_button(_("Set this as a new name")) ;
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
  if ( result )
    {
      gui->action = Nothing ;			  
      gui->new_name = NULL ;
      nb = 0 ;
    }
  return result ;
}

