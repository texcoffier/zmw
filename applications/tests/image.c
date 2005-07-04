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



int something_to_read(FILE *f)
{
  fd_set r, w, x ;
  struct timeval to ;

  FD_ZERO(&r) ;
  FD_ZERO(&w) ;
  FD_ZERO(&x) ;
  FD_SET(fileno(f), &r) ;
  FD_SET(fileno(f), &x) ;

  to.tv_sec = 0 ;
  to.tv_usec = 1 ;

  return( select(fileno(f)+1, &r, &w, &x, &to) ) ;
}

void test_image(const char *title)
{
  static char *old_file_name = NULL ;
  static FILE *images = NULL ;
  static char **image_names = NULL ;
  static int nb_images = 0 ;
  static Zmw_Float_0_1 x=0, y=0 ;
  static int to_display = 0 ;
  char buf[9999] ;
  int i ;


 
  ZMW(zmw_window(title))
    {
      zmw_height(900) ;
      zmw_width(900) ;
      ZMW(zmw_vbox())
	{
	  sprintf(buf, "%d images", nb_images) ;
	  zmw_label(buf) ;
	  zmw_horizontal_expand(1) ;
	  zmw_vertical_expand(1) ;
	  ZMW( zmw_viewport_with_scrollbar(&x, &y) )
	    {
	      ZMW(zmw_vbox())
		{
		  for(i=0; i<nb_images; i++)
		    {
		      zmw_label(image_names[i]) ;
		      if ( zmw_button_pressed() )
			to_display = i ;
		    }		  
		}
	    }
	  zmw_vertical_expand(0) ;
	  if ( image_names )
	      zmw_image_dynamic_from_file(image_names[to_display], &old_file_name) ;
	}
    }

 if ( images == NULL )
    {
      images = popen("find / '(' -name '*.gif' -o -name '*.jpg' -o -name '*.png' ')'"
		     , "r") ;
    }
  if ( zmw_subaction_get() == Zmw_Input_Event && nb_images < 100 )
    while ( something_to_read(images) )
      {
	ZMW_REALLOC(image_names, nb_images+1) ;
	fgets(buf, sizeof(buf), images) ;
	buf[strlen(buf)-1] = '\0' ;
	image_names[nb_images] = strdup(buf) ;
	nb_images++ ;
      }
  
}



