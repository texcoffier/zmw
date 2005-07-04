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


void test_focus(const char *title)
{
  static Zmw_Name *focus=NULL, *focus2=NULL, *focus3=NULL ;
  static char *text[12] = { NULL } ;
  static int t0 = 0, t1 = 0, t2 = 0, t3 = 0 ;

  int i ;

  /* These initialization can go before zmw_main */
  zmw_name_initialize(&focus, "Focus") ;
  zmw_name_initialize(&focus2, "Focus2") ;
  zmw_name_initialize(&focus3, "Focus3") ;

  if ( text[0] == NULL )
    {
      for(i=0; i<ZMW_TABLE_SIZE(text); i++)
	text[i] = strdup("String") ;
    }

  ZMW(zmw_window(title))
    {
      zmw_padding_width(3) ;
      zmw_horizontal_expand(1) ;
      ZMW(zmw_vbox())
	{
	  zmw_entry(&text[9]) ;
	  zmw_name("Group 1") ;
	  ZMW(zmw_vbox())
	    {
	      zmw_focus(focus) ;

	      zmw_label("Focus Group 1") ;
	      zmw_entry(&text[0]) ;
	      zmw_check_button_int(&t0) ;
	      zmw_entry(&text[2]) ;
	    }
	  zmw_border_embossed_in_draw() ;
	  zmw_check_button_int(&t2) ;
	  zmw_name("Group 2") ;
	  ZMW(zmw_vbox())
	    {
	      zmw_focus(focus2) ;

	      zmw_label("Focus Group 2") ;
	      zmw_entry(&text[3]) ;
	      zmw_check_button_int(&t1) ;

	      zmw_name("Group 3") ;
	      ZMW(zmw_vbox())
		{
		  zmw_focus(focus3) ;

		  zmw_label("Focus Group 3") ;
		  zmw_entry(&text[6]) ;
		  zmw_check_button_int(&t3) ;
		  zmw_entry(&text[8]) ;
		}
	      zmw_border_embossed_in_draw() ;
	      zmw_entry(&text[5]) ;
	    }
	  zmw_border_embossed_in_draw() ;
	  zmw_entry(&text[11]) ;
	}
    }
}

void test_focus2(const char *title)
{
  static Zmw_Name *focus = NULL ;
  static char *text1 = NULL, *text2 ;

  if ( text1 == NULL )
    {
      zmw_name_initialize(&focus, "Focus test") ;
      text1 = strdup("Text 1") ;
      text2 = strdup("Text 2") ;
    }

  ZMW(zmw_window(title))
    {
      zmw_name("Box Vertical") ;
      ZMW(zmw_vbox())
	{
	  zmw_focus(focus) ;
	  zmw_name("Text1") ;
	  zmw_entry(&text1) ;
	  zmw_name("Text2") ;
	  zmw_entry(&text2) ;
	}
    }
}


void test_focus3(const char *title)
{
  static Zmw_Name *focus = NULL, *focus2 = NULL ;
  static char *text1 = NULL, *text2 ;

  if ( text1 == NULL )
    {
      zmw_name_initialize(&focus, "Focus test") ;
      zmw_name_initialize(&focus2, "Focus test 2") ;
      text1 = strdup("Text 1") ;
      text2 = strdup("Text 2") ;
    }

  ZMW(zmw_window(title))
    {
      zmw_name("Box Vertical") ;
      ZMW(zmw_vbox())
	{
	  zmw_name("Box 1") ;
	  ZMW(zmw_vbox())
	    {
	      zmw_focus(focus) ;
	      zmw_name("Text1") ;
	      zmw_entry(&text1) ;
	      zmw_name("Text2") ;
	      zmw_entry(&text2) ;
	    }

	  zmw_name("Box 2") ;
	  ZMW(zmw_vbox())
	    {
	      zmw_focus(focus2) ;
	      zmw_name("Text3") ;
	      zmw_entry(&text1) ;
	      zmw_name("Text4") ;
	      zmw_entry(&text2) ;
	    }
	}
    }
}

void test_focus4(const char *title)
{
  static Zmw_Name *focus = NULL ;
  static char *text1 = NULL, *text2 ;

  if ( text1 == NULL )
    {
      zmw_name_initialize(&focus, "Focus test 4") ;
      text1 = strdup("Text 1") ;
      text2 = strdup("Text 2") ;
    }

  ZMW(zmw_window(title))
    {
      ZMW(zmw_vbox())
	{
	  zmw_label("This test doesn't work") ;
	  zmw_label("See test_focus4 in zmw.c") ;
	}
    }

  zmw_auto_resize(Zmw_True) ;

  ZMW(zmw_window(title))
    {
	      zmw_name("Text1") ;
	      zmw_entry(&text1) ;
    }

  ZMW(zmw_window(title))
    {
	      zmw_focus(focus) ;
	      zmw_name("Text2") ;
	      zmw_entry(&text2) ;
    }


}
