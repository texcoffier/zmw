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


void editable_text(const char *title)
{
  static char *text = NULL ;

  if ( text == NULL )
    text = strdup("Type text here") ;

  zmw_auto_resize(1) ;
  ZMW(zmw_window(title))
    {
      zmw_entry(&text) ;
    }
}

void editable_text_2(const char *title)
{
  static char *text1 = NULL, *text2 = NULL ;
  static int cursor, start ;

  if ( text1 == NULL )
  {
    text1 = strdup("abcdefghijklmnopqrstuvwxyz") ;
    text2 = strdup("ABCDEFGHIJKLMNOPQRSTUVWXYZ") ;
  }
  zmw_auto_resize(1) ;
  ZMW(zmw_window(title))
    {
    	ZMW(zmw_vbox())
    	{
      zmw_entry_with_cursor_and_start(&text1, &cursor, &start) ;
      zmw_entry_with_cursor_and_start(&text2, &cursor, &start) ;
    	}
    }
  if ( strlen(text1) < strlen(text2) )
  	{
  		text2[strlen(text1)] = '\0' ;
  	}
  if ( strlen(text1) > strlen(text2) )
  	{
  		text1[strlen(text2)] = '\0' ;
  	}
}
