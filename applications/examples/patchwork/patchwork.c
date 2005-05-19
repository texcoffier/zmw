/* DO NOT DISPLAY */
#include "zmw/zmw.h"
/* DO NOT DISPLAY */

void box(const char *title)
{
  ZMW_EXTERNAL_RESTART ;

  ZMW(zmw_decorator(Zmw_Decorator_Border_Embossed))
    {
      ZMW(zmw_vbox())
	{
	  zmw_vertical_expand(Zmw_False) ;
	  ZMW(zmw_decorator(Zmw_Decorator_Border_Solid))
	    {
	      zmw_horizontal_expand(Zmw_False) ;
	      zmw_color(Zmw_Color_Foreground, 0xFF0000) ;
	      zmw_label(title) ;
	    }
	  ZMW_EXTERNAL ;
	}
    }

  ZMW_EXTERNAL_STOP ;
}

void patchwork(void)
{
  static char *text = NULL ;
  static int toggle = 0, radio = 1, page = 1 ;
  static float x=0.5, y=0.2, x_size=0.3, y_size=0.4, x2=0.1, y2=0.1 ;
  static int widths[] = { 40,50,70,60 } ;
  const int nb_widths = sizeof(widths)/sizeof(widths[0]) ;
  static int start = 1560654, nb = 5 ;
  static int border_width = 2, focus_width = 2, padding_width = 2 ;
  int i, j ;
  char buf[99] ;

  if ( text == NULL )
    text = strdup("Edit me!") ;

  zmw_horizontal_expand(Zmw_True) ;
  zmw_vertical_expand(Zmw_True) ;
  zmw_padding_width(padding_width) ;
  zmw_border_width(border_width) ;
  zmw_focus_width(focus_width) ;
  zmw_vertical_alignment(-1) ;
  zmw_horizontal_alignment(0) ;
  ZMW(zmw_window("Patchwork"))
    {
      zmw_width(640-4) ;
      zmw_height(480-4) ;
      ZMW(zmw_vbox())
	{
	  ZMW(zmw_hbox())
	    {
	      ZMW(box("zmw_label"))
		zmw_label("zmw_label") ;
	      ZMW(box("zmw_entry"))
		zmw_entry(&text) ;
	      ZMW(box("zmw_button"))
		zmw_button("not tested") ;
	      ZMW(box("zmw_button"))
		{
		  zmw_button("tested") ;
		  if ( zmw_activated() )
		    printf("zmw_button activated\n") ;
		}
	    }
	  ZMW(zmw_hbox())
	    {
	      ZMW(box("zmw_button_with_accelerator"))
		zmw_button_with_accelerator("zmw_button_with_accelerator"
					    , GDK_CONTROL_MASK, 'S') ;

	      ZMW(box("zmw_check_button_with_label"))
		zmw_check_button_int_with_label(&toggle, "label") ;

	      ZMW(box("zmw_radio_button"))
		{
		  ZMW(zmw_hbox())
		    {
		      zmw_vertical_expand(Zmw_False) ;
		      zmw_horizontal_expand(Zmw_False) ;
		      zmw_radio_button(&radio, 0) ;
		      zmw_radio_button(&radio, 1) ;
		      zmw_radio_button(&radio, 2) ;
		    }
		}
	    }
	  ZMW(zmw_hbox())
	    {
	      ZMW(box("zmw_notebook"))
		{
		  zmw_vertical_expand(Zmw_True) ;
		  ZMW(zmw_notebook(&page))
		    {
		      ZMW(zmw_vbox_activable())
			{
			  zmw_horizontal_alignment(0) ;
			  zmw_label("Page") ;
			  zmw_label("1") ;
			}
		      zmw_button("Page 1 content") ;
		      zmw_label("Page 2") ;
		      ZMW(zmw_vbox_activable())
			{
			  zmw_label("Page label: any kind of widget") ;
			  zmw_label("Page content: any kind of widget") ;
			}
		      ZMW(zmw_hbox_activable())
			{
			  zmw_label("P 3") ;
			  zmw_entry(&text) ;
			}
		      zmw_label("Page 3") ;
		    }
		}

	      ZMW(box("zmw_scrollbar2"))
		{
		  zmw_horizontal_expand(Zmw_True) ;
		  zmw_vertical_expand(Zmw_True) ;
		  zmw_scrollbar2(&x, x_size, &y, y_size) ; 
		  if ( zmw_activated() )
		    printf("x=%g y=%g\n", x, y) ;
		}
	      
	      ZMW(box("zmw_viewport_with_scrollbar"))
		{
		  zmw_horizontal_expand(Zmw_True) ;
		  zmw_vertical_expand(Zmw_True) ;
		  ZMW(zmw_viewport_with_scrollbar(&x2, &y2))
		    {
		      ZMW(zmw_vbox())
			for(i=0; i<10; i++)
			  zmw_label("A very very very long text to be scrolled") ;
		    }
		  if ( zmw_activated() )
		    printf("x2=%g y2=%g\n", x2, y2) ;
		}
	    }
	  ZMW(zmw_hbox())
	    {
	      ZMW(box("zmw_image_from_file"))
		ZMW(zmw_decorator(0))
		{
		  zmw_horizontal_expand(Zmw_False) ;
		  zmw_image_from_file("../message/warning.png") ;
		}

	      ZMW(box("zmw_table_with_widths"))
		{
		  ZMW(zmw_vbox())
		    {
		      ZMW(zmw_table_with_widths(nb_widths, widths))
			{
			  for(j=0;j<3;j++)
			    for(i=0; i<nb_widths; i++)
			      {
				sprintf(buf, "[%d,%d]", i, j) ;
			    zmw_label(buf) ;
			      }
			}
		      if ( zmw_activated() )
			printf("Table width change\n") ;
		      ZMW(zmw_hbox())
			{
			  zmw_label("widths:") ;
			  for(i=0;i<nb_widths;i++)
			    zmw_int(widths[i]) ;
			}
		    }			
		}
	      ZMW(box("zmw_scrolled_view"))
		{
		  zmw_vertical_expand(Zmw_True) ;
		  ZMW(zmw_scrolled_view(&start, &nb, 5000000))
		    {
		      ZMW(zmw_vbox())
			{
			  zmw_vertical_expand(Zmw_False) ;
			  for(i=start; i<start+nb; i++)
			    {
			      zmw_int(i) ;
			    }
			}
		    }
		  if ( zmw_activated() )
		    printf("start=%d nb=%d\n", start, nb) ;
		}
	    }
	  ZMW(box("zmw_decorator"))
	    {
	      ZMW(zmw_hbox())
		{
		  zmw_vertical_expand(Zmw_False) ;
		  zmw_horizontal_expand(Zmw_False) ;
		  ZMW(zmw_decorator(Zmw_Decorator_Border_Relief))
		    zmw_label("Relief") ;
		  ZMW(zmw_decorator(Zmw_Decorator_Border_Relief
				    | Zmw_Decorator_Border_In))
		    zmw_label("+ In") ;
		  ZMW(zmw_decorator(Zmw_Decorator_Border_Relief
				    | Zmw_Decorator_Border_In
				    | Zmw_Decorator_Interior))
		    zmw_label("+ Interior") ;
		  ZMW(zmw_decorator(Zmw_Decorator_Border_Relief
				    | Zmw_Decorator_Interior))
		    zmw_label("- In") ;
		  ZMW(zmw_decorator(Zmw_Decorator_Border_Solid))
		    zmw_label("Solid") ;
		  ZMW(zmw_decorator(Zmw_Decorator_Border_Embossed))
		    zmw_label("Embossed") ;
		  ZMW(zmw_decorator(Zmw_Decorator_Border_Embossed
				    | Zmw_Decorator_Border_In
				    ))
		    zmw_label("+ In") ;
		  ZMW(zmw_decorator(Zmw_Decorator_Border_Embossed
				    | Zmw_Decorator_Activable
				    | Zmw_Decorator_Pushable
				    | Zmw_Decorator_Focusable
				    | Zmw_Decorator_Interior
				    ))
		    zmw_label("Emboss.+Activab.+Focusab.+Pushab.+Interior") ;
		  if ( zmw_activated() )
		    printf("decorator activated\n") ;
		}
	    }
	  ZMW(zmw_hbox())
	    {
	      ZMW(box("zmw_border_width"))
		zmw_int_editable(&border_width) ;

	      ZMW(box("zmw_focus_width"))
		zmw_int_editable(&focus_width) ;

	      ZMW(box("zmw_padding_width"))
		zmw_int_editable(&padding_width) ;
	    }
	}
    }
}
/* DO NOT DISPLAY */
int main(int argc, char *argv[])
{
  zmw_init(&argc, &argv) ;
  zmw_main(patchwork) ;
  return 0 ;
}
/* DO NOT DISPLAY */
/* REGRESSION TEST

zmw_move_cursor_to 26 36
zmw_dump_screen 0




REGRESSION TEST */
