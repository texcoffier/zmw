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

	  zmw_text("Large content") ; // The page 0 label
	  ZMW(zmw_box_horizontal())   // The page 0 content
	    for(i=0; i<7; i++)
	      zmw_text("Large") ;

	  ZMW(zmw_box_vertical())  // The page 1 label is a box
	    {
	      zmw_text("Very") ;
	      zmw_text("tall") ;
	      zmw_text("content") ;
	    }
	  ZMW(zmw_box_vertical()) // The page 1 content
	    for(i=0; i<5; i++)
	      zmw_text("Tall") ;

	  // The page 2 label is an image
	  zmw_image_from_file("smallcontent.png") ;
	  // The page 2 content is not centered because
	  // it is not in a box.
	  zmw_text("Small content") ;
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_run(notebook) ;
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
