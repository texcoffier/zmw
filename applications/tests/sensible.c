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


void test_sensible(const char *title)
{
  static int button=1, text=1, toggle=1, box=1, self=1 ;
  static int a_toggle=0, a_toggle2=0 ;
  static char *a_text = NULL, *a_text2 = NULL ;

  if ( a_text == NULL )
    {
      a_text = strdup("Edit here") ;
      a_text2 = strdup("Edit here also") ;
    }

  ZMW(zmw_window(title))
    {
      zmw_border_width(2) ;
      zmw_padding_width(4) ;
      ZMW(zmw_vbox())
	{
	  ZMW(zmw_vbox())
	    {
	      zmw_check_button_int_with_label(&button, "Next button is sensible") ;
	      zmw_button("A button") ;
	      if ( button && zmw_activated() )
		fprintf(stderr, "button is activated\n") ;
	    }
	  zmw_border_embossed_in_draw() ;
	      

	  ZMW(zmw_vbox())
	    {
	      zmw_check_button_int_with_label(&toggle, "Next toggle is sensible (doesn't work)") ;
	      zmw_check_button_int(&a_toggle) ;
	      if ( toggle && zmw_activated() )
		fprintf(stderr, "toggle change\n") ;
	    }
	  zmw_border_embossed_in_draw() ;

	  
	  ZMW(zmw_vbox())
	    {
	      zmw_check_button_int_with_label(&text, "Next text is sensible (doesn't work)") ;
	      zmw_entry(&a_text) ;
	      if ( text && zmw_activated() )
		fprintf(stderr, "text is activated\n") ;
	    }
	  zmw_border_embossed_in_draw() ;

	  ZMW(zmw_vbox())
	    {
	      zmw_check_button_int_with_label(&box, "Next box is sensible") ;

	      zmw_sensitive(box) ;
	      ZMW(zmw_vbox())
		{
		  zmw_button("A button in the box") ;
		  if ( zmw_activated() )
		    fprintf(stderr, "Button in box activated\n") ;
		  zmw_check_button_int(&a_toggle2) ;
		  if ( zmw_activated() )
		fprintf(stderr, "Toggle in box change\n") ;
		  zmw_entry(&a_text2) ;
		  if ( zmw_activated() )
		    fprintf(stderr, "Text in box validated\n") ;
		}
	      zmw_border_embossed_in_draw() ;
	    }
	  zmw_border_embossed_in_draw() ;

	  zmw_sensitive(self) ;
	  zmw_check_button_int_with_label(&self, "This toggle is sensible") ;
	}
    }
}
