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


void test_filechooser(const char *title)
{
  static GdkWindow *w = NULL, *ww = NULL ;
  static char *filename_current = NULL ;
  static char *filename_selected = NULL ;
  static Zmw_Boolean choosing_a_filename = 0 ;

  if ( filename_current == NULL )
    {
      filename_current = strdup("/") ;
      filename_selected = strdup("/") ;
    }
    
  ZMW(zmw_window(&w, title))
    {
      ZMW(zmw_box_vertical())
	{
	  zmw_button("Launch File Chooser") ;
	  if ( zmw_activated() )
	    choosing_a_filename = 1 ;
	  zmw_text("File name current :") ;
	  zmw_text(filename_current) ;
	  zmw_text("File name selected :") ;
	  zmw_text(filename_selected) ;
	}

    }

  zmw_filechooser(&choosing_a_filename, &ww
		  , &filename_current, "Choose a filename", "Load file") ;
  if ( zmw_activated() )
    {
      free(filename_selected) ;
      filename_selected = strdup(filename_current) ;
    }
}
