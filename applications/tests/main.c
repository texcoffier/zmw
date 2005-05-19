/*
    ZMW: A Zero Memory Widget Library
    Copyright (C) 2002-2003 Thierry EXCOFFIER, Université Claude Bernard, LIRIS

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
#include <math.h>
#include <sys/time.h>
#include <ctype.h>

#include "zmw/zmw.h"

#include "hello_world.c"
#include "editable_text.c"
#include "horizontal_box.c"
#include "expand.c"
#include "hor_hor_box.c"
#include "ver_ver_box.c"
#include "ver_hor_box.c"
#include "vertical_box.c"
#include "many_widgets.c"
#include "color.c"
#include "focus.c"
#include "button.c"
#include "menu.c"
#include "notebook.c"
#include "tips.c"
#include "drag_and_drop.c"
#include "toggle.c"
#include "quit.c"
#include "asked.c"
#include "dragger.c"
#include "animation.c"
#include "anchor_box.c"
#include "scrollbar.c"
#include "scrollbar2.c"
#include "viewport.c"
#include "embeded.c"
#include "sensible.c"
#include "state.c"
#include "accel.c"
#include "filechooser.c"
#include "image.c"
#include "compose.c"
#include "message.c"
#include "test_trace.c"
#include "table.c"
#include "cursorin.c"

struct menu
{
  char *text ;
  void (*fct)(const char *title) ;
  int on_screen ;
} ;


/*
 * auto_resize is an option because in some example
 * its value is changed and not restored
 */


void page_test(const char *label, struct menu *tests)
{
  int i ;
  zmw_button(label) ;

  ZMW(zmw_vbox())
    {
      zmw_vertical_alignment(0) ;
      
      for(i=0; tests[i].text; i++)
	  zmw_check_button_int_with_label(&tests[i].on_screen, tests[i].text) ;
    }
}

void page_test_windows(struct menu *tests, int auto_resize)
{
      int i ;
      
      for(i=0; tests[i].text; i++)
	{
	  if ( tests[i].on_screen )
	    {
	      zmw_auto_resize(auto_resize) ;
	      zmw_name(tests[i].text) ;
	      (*tests[i].fct)(tests[i].text) ;
	    }
	}
}

void page_option(int *box, int *border_width, int *padding_width
		 , char **font_name, int *auto_resize, int *size)
{
  zmw_button("Option") ;

  ZMW(zmw_vbox())
    {
      zmw_vertical_alignment(0) ;

      zmw_label("Draw box") ;
      zmw_radio_button_with_label((int*)box,0
			   , "No box") ;
      zmw_radio_button_with_label((int*)box,Zmw_Decorator_Border_Solid
			   , "Solid Box") ;

      zmw_radio_button_with_label((int*)box,Zmw_Decorator_Border_Relief
			   | Zmw_Decorator_Border_Out
			   , "Relief Out Box") ;
      zmw_radio_button_with_label((int*)box,Zmw_Decorator_Border_Relief
			   | Zmw_Decorator_Border_In
			   , "Relief In Box") ;
      zmw_radio_button_with_label((int*)box,Zmw_Decorator_Border_Relief
			   | Zmw_Decorator_Border_In
			   ,"Embossed in Box") ;
      zmw_radio_button_with_label((int*)box,Zmw_Decorator_Border_Relief
			   | Zmw_Decorator_Border_Out
			   , "Embossed out Box") ;

      zmw_label("Border width in pixel") ;
      ZMW(zmw_hbox())
	{
	  zmw_radio_button_with_label(border_width, 1, "1") ;
	  zmw_radio_button_with_label(border_width, 2, "2") ;
	  zmw_radio_button_with_label(border_width, 4, "4") ;
	  zmw_radio_button_with_label(border_width, 6, "6") ;
	  zmw_radio_button_with_label(border_width, 8, "8") ;
	}
      
      zmw_label("Padding width in pixel") ;
      ZMW(zmw_hbox())
	{
	  zmw_radio_button_with_label(padding_width, 0, "0") ;
	  zmw_radio_button_with_label(padding_width, 1, "1") ;
	  zmw_radio_button_with_label(padding_width, 2, "2") ;
	  zmw_radio_button_with_label(padding_width, 4, "4") ;
	}
      
      zmw_label("Font family") ;
      ZMW(zmw_hbox())
	{
	  static char *fonts[] = { "fixed", "clean",
				   "sans", "monospace", NULL } ;
	  int i ;

	  for(i=0; fonts[i]; i++)
	    {
	      zmw_font_family( fonts[i] ) ;
	      zmw_radio_button_with_label((int*)font_name, (int)fonts[i], fonts[i]) ;
	    }
	}
      zmw_label("Font size") ;
      ZMW(zmw_hbox())
	{
	  static char *sizes[] = { "8", "10", "12", "16", NULL } ;
	  int i ;

	  for(i=0; sizes[i]; i++)
	    {
	      zmw_font_size( atoi(sizes[i]) ) ;
	      zmw_radio_button_with_label(size, atoi(sizes[i]), sizes[i]) ;
	    }
	}
      
      
      zmw_label("Misc") ;
      zmw_check_button_int_with_label(auto_resize, "Window auto resize") ;
    }
}

static char *test_name = NULL ;

void main_menu()
{
  static int box = 0 ;
  static int border_width = 2 ;
  static int padding_width = 1 ;
  static char *font_family = "fixed" ;
  static int auto_resize = 0 ;
  static int page = 0 ;
  static struct menu output[] =
    {
      { "Horizontal box"              , horizontal_box                   , 0 },
      { "Horizontal box of horizontal", hor_hor_box                      , 0 },
      { "Vertical box of vertical"    , ver_ver_box                      , 0 },
      { "Vertical box of horizontal"  , ver_hor_box                      , 0 },
      { "Box expansion"               , expand                           , 0 },
      { "Simple expansion"            , test_expand                      , 0 },
      { "Color and current state"     , test_color                       , 0 },
      { "Notebook"                    , test_notebook                    , 0 },
      { "Setting the widget X,Y,W,H"  , test_asked                       , 0 },
      { "Animation"                   , test_animation                   , 0 },
      { "Embeded macros"              , test_embeded                     , 0 },
      { "Viewport with scrollbar"     , test_viewport                    , 0 },
      { "Window with many widget"     , many_widgets                     , 0 },
      { "Image"                       , test_image                       , 0 },
      { "Trace"                       , test_trace                       , 0 },
      { "Table"                       , test_table                       , 0 },
      { "Table with width"            , test_table_width                 , 0 },
      { NULL }
    } ;
  static struct menu input[] =
    {
      { "Editable text"               , editable_text                    , 0 },
      { "Editable text (coupled)"     , editable_text_2                  , 0 },
      { "Button"                      , test_button                      , 0 },
      { "Multiple focus box"          , test_focus                       , 0 },
      { "Hierarchical menu"           , test_menu                        , 0 },
      { "Widget sensitivity"          , test_sensible                    , 0 },
      { "Drag and Drop"               , test_drag_and_drop               , 0 },
      { "Tips (help bubble)"          , test_tips                        , 0 },
      { "Accelerator"                 , test_accel                       , 0 },
      { "File Chooser"                , test_filechooser                 , 0 },
      { "Dragger 1D"                  , test_dragger                     , 0 },
      { "Dragger 2D"                  , test_anchor_box                  , 0 },
      { "Scrollbar"                   , test_scrollbar                   , 0 },
      { "Scrollbar 2"                 , test_scrollbar2                  , 0 },
      { "Cursor in"                   , test_cursorin                    , 0 },
      { NULL }
    } ;
  static struct menu paper[] =
    {
      { "Hello world"                 , hello_world                      , 0 },
      { "Hello world in two lines"    , vertical_box                     , 0 },
      { "Toggle list"                 , test_toggle                      , 0 },
      { "A quit button"               , test_quit                        , 0 },
      { "Current state"               , test_state                       , 0 },
      { "Compose"                     , compose                          , 0 },
      { "Message"                     , test_message                     , 0 },
      { NULL }
    } ;
  static struct menu debug[] =
    {
      { "Minimal menu"                , test_minimal_menu                , 0 },
      { "Basic menu"                  , test_basic_menu                  , 0 },
      { "Basic menu with tip"         , test_basic_menu2                 , 0 },
      { "Focus on 2 widgets"          , test_focus2                      , 0 },
      { "Focus on 4 widgets"          , test_focus3                      , 0 },
      { "Focus on 2 windows"          , test_focus4                      , 0 },
      { "Test Tearoff simple"         , test_menu_tearoff_simple         , 0 },

       { NULL }
    } ;
  static struct menu *menus[] = { paper, output, input, debug, NULL } ;
  static int font_size = 10 ;

  int i, j ;


  zmw_name("Window") ;
  zmw_border_width(border_width) ;
  zmw_padding_width(padding_width) ;
  zmw_font_family(font_family) ;
  zmw_font_size(font_size) ;
  zmw_auto_resize(auto_resize) ;
  zmw_vertical_alignment(-1) ;
  zmw_horizontal_alignment(-1) ;
  zmw_horizontal_expand(Zmw_False) ;
  zmw_vertical_expand(Zmw_False) ;

  if ( test_name )
    {
      for(j=0; menus[j]; j++)
	for(i=0; menus[j][i].text; i++)
	  if ( strcmp(menus[j][i].text, test_name) == 0 )
	    {
	      zmw_name(menus[j][i].text) ;

	      (*menus[j][i].fct)(menus[j][i].text) ;
	      return ;
	    }
    }


  ZMW(zmw_window("Test ZMW Testbed"))
    {  
      ZMW(zmw_decorator(box))
	{
      zmw_horizontal_expand(Zmw_True) ;
      zmw_vertical_expand(Zmw_True) ;
      ZMW(zmw_vbox())
	{
	  ZMW(zmw_vbox())
	    {
	      zmw_padding_width(padding_width) ;
	      ZMW(zmw_vbox())
		{
		  zmw_horizontal_alignment(0) ;
		  zmw_label("Welcome to the ZMW testbed") ;

		  zmw_horizontal_alignment(-1) ;
		  ZMW(zmw_notebook(&page))
		    {
		      zmw_horizontal_expand(Zmw_False) ;
		      zmw_vertical_expand(Zmw_False) ;
		      page_test("Paper examples", paper) ;
		      page_test("Output", output) ;
		      page_test("Input", input) ;
		      page_test("Debug", debug) ;
		      page_option(&box, &border_width, &padding_width
				  , &font_family, &auto_resize, &font_size
				  ) ;
		    }
		  zmw_horizontal_alignment(0) ;
		  zmw_horizontal_expand(Zmw_False) ;
		  zmw_vertical_expand(Zmw_False) ;
		  ZMW(zmw_hbox())
		    {
		      zmw_button_with_hidden_accelerator("Quit", GDK_CONTROL_MASK, 'Q') ;
		      if ( zmw_activated() || zmw_accelerator(GDK_CONTROL_MASK,'C') )
			{
			  printf("Bye bye\n") ;
			  zmw_main_quit(0) ;
			}
		      zmw_button("Close windows") ;
		      if ( zmw_activated() )
			{
			  for(j=0; menus[j]; j++)
			    for(i=0; menus[j][i].text; i++)
			      menus[j][i].on_screen = 0 ;
			}
		      zmw_button("Open windows") ;
		      if ( zmw_activated() )
			{
			    for(i=0; menus[page][i].text; i++)
			      menus[page][i].on_screen = 1 ;
			}
		    }
		}
	    }
	}
	}
    }
  page_test_windows(paper, auto_resize) ;
  page_test_windows(output, auto_resize) ;
  page_test_windows(input, auto_resize) ;
  page_test_windows(debug, auto_resize) ;

}



int main(int argc, char **argv)
{
  zmw_init(&argc, &argv) ;

  if ( argc > 1 )
    test_name = argv[1] ;

  zmw_main(main_menu) ;
  return(0) ;
}
