/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */
void notebook(void)
{
  static int current_page = 0 ; // Give the initial page number
  int i ;

  ZMW(zmw_window("Notebook"))
    {
      ZMW(zmw_notebook(&current_page))
	{
	  zmw_vertical_expand(Zmw_False) ;
	  zmw_horizontal_expand(Zmw_False) ;

	  zmw_button("Large content") ; // The page 0 label
	  ZMW(zmw_hbox())   // The page 0 content
	    for(i=0; i<7; i++)
	      zmw_label("Large") ;

	  ZMW(zmw_vbox_activable())  // The page 1 label is a box
	    {
	      zmw_label("Very") ;
	      zmw_label("tall") ;
	      zmw_label("content") ;
	    }
	  ZMW(zmw_vbox()) // The page 1 content
	    for(i=0; i<5; i++)
	      zmw_label("Tall") ;

	  // The page 2 label is an image
	  zmw_image_from_file_activable("smallcontent.png") ;
	  // The page 2 content is not centered because
	  // it is not in a box.
	  zmw_label("Small content") ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(notebook) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 122 50 # very tall button
zmw_dump_screen 0

zmw_button_click
zmw_dump_screen 1

zmw_move_cursor_to 161 50 # small content button
zmw_button_click
zmw_dump_screen 2



REGRESSION TEST */
