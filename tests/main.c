/*
    ZMW: A Zero Memory Widget Library
    Copyright (C) 2002-2003  Thierry EXCOFFIER, LIRIS

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

#include "zmw.h"

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


void page_test(const char *label, struct menu *tests, int auto_resize)
{
  int i ;
  zmw_text(label) ;

  ZMW(zmw_box_vertical())
    {
      zmw_vertical_alignment(0) ;
      
      for(i=0; tests[i].text; i++)
	{
	  zmw_toggle_with_label(&tests[i].on_screen, tests[i].text) ;
	  if ( tests[i].on_screen )
	    {
	      zmw_name(tests[i].text) ;
	      zmw_auto_resize(auto_resize) ;
	      (*tests[i].fct)(tests[i].text) ;
	    }
	}
    }
}

void page_option(void (**box)(), int *border_width, int *padding_width
		 , char **font_name, int *auto_resize)
{
  zmw_text("Option") ;

  ZMW(zmw_box_vertical())
    {
      zmw_vertical_alignment(0) ;

      zmw_text("Draw box") ;
      zmw_radio_with_label((int*)box,(int)NULL
			   , "No box") ;
      zmw_radio_with_label((int*)box,(int)zmw_border_solid_draw
			   , "Solid Box") ;
      zmw_radio_with_label((int*)box,(int)zmw_border_relief_out_draw
			   , "Relief Out Box") ;
      zmw_radio_with_label((int*)box,(int)zmw_border_relief_in_draw
			   , "Relief In Box") ;
      zmw_radio_with_label((int*)box,(int)zmw_border_embossed_in_draw
			   ,"Embossed in Box") ;
      zmw_radio_with_label((int*)box,(int)zmw_border_embossed_out_draw
			   , "Embossed out Box") ;

      zmw_text("Border width") ;
      zmw_radio_with_label(border_width, 1, "1 pixel") ;
      zmw_radio_with_label(border_width, 2, "2 pixels") ;
      zmw_radio_with_label(border_width, 4, "4 pixels") ;
      
      zmw_text("Padding width") ;
      zmw_radio_with_label(padding_width, 0, "0 pixel") ;
      zmw_radio_with_label(padding_width, 1, "1 pixel") ;
      zmw_radio_with_label(padding_width, 2, "2 pixels") ;
      zmw_radio_with_label(padding_width, 4, "4 pixels") ;
      
      zmw_text("Font") ;
      zmw_radio_with_label((int*)font_name, (int)"6x13", "6x13") ;
      zmw_radio_with_label((int*)font_name, (int)"7x14", "7x14") ;
      
      zmw_text("Misc") ;
      zmw_toggle_with_label(auto_resize, "Window auto resize") ;
    }
}

static char *test_name = NULL ;

void main_menu()
{
  static GdkWindow *w = NULL ;
  static void (*box)() = NULL ;
  static int border_width = 2 ;
  static int padding_width = 1 ;
  static char *font_name = "6x13" ;
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
      { NULL }
    } ;
  static struct menu input[] =
    {
      { "Editable text"               , editable_text                    , 0 },
      { "Button"                      , test_button                      , 0 },
      { "Focus on 2 widgets"          , test_focus2                      , 0 },
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
      { "Basic menu"                  , test_basic_menu                  , 0 },
      { NULL }
    } ;
  static struct menu *menus[] = { paper, input, output, debug, NULL } ;

  int i, j ;


  zmw_debug(0) ;
  zmw_name("Window") ;
  zmw_border_width(border_width) ;
  zmw_font(font_name) ;
  zmw_auto_resize(auto_resize) ;
  zmw_vertical_alignment(-1) ;
  zmw_horizontal_alignment(-1) ;
  zmw_horizontal_expand(0) ;
  zmw_vertical_expand(0) ;

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


  ZMW(zmw_window(&w, "Test ZMW Testbed"))
    {  
      zmw_padding_width(4) ;
      ZMW(zmw_box_vertical())
	{
	  ZMW(zmw_box_vertical())
	    {
	      zmw_padding_width(padding_width) ;
	      ZMW(zmw_box_vertical())
		{
		  zmw_horizontal_alignment(0) ;
		  zmw_text("Welcome to the ZMW testbed") ;
		  zmw_horizontal_alignment(-1) ;
		  ZMW(zmw_notebook(&page))
		    {
		      page_test("Paper examples", paper, auto_resize) ;
		      page_test("Output", output, auto_resize) ;
		      page_test("Input", input, auto_resize) ;
		      page_test("Debug", debug, auto_resize) ;
		      page_option(&box, &border_width, &padding_width
				  , &font_name, &auto_resize) ;
		    }
		  zmw_horizontal_alignment(0) ;
		  ZMW(zmw_box_horizontal())
		    {
		      zmw_button("Quit") ;
		      if ( zmw_activated() )
			{
			  printf("Bye bye\n") ;
			  zmw_exit(0) ;
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
			  for(j=0; menus[j]; j++)
			    for(i=0; menus[j][i].text; i++)
			      menus[j][i].on_screen = 1 ;
			}
		    }
		}
	    }
	  if ( box )
	    (*box)() ;
	}
    }
}



int main(int argc, char **argv)
{
  if ( argc > 1 )
    {
      test_name = argv[argc-1] ;
      argc-- ;
      argv++ ;
    }

  zmw_run(&argc, &argv, main_menu) ;
  return(0) ;
}
