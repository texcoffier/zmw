/*
    ZMW: A Zero Memory Widget Library
    Copyright (C) 2002-2005 Thierry EXCOFFIER, Université Claude Bernard, LIRIS

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

#include "zmw/zmw.h"
#include "zmw/zmw_private.h" /* For Zmw_Font_Description */


Zmw_Font_Family zmw_font_family_index(const char *family)
{
  Zmw_Font_Family i ;

  for(i=0; i<zmw.nb_font_families; i++)
    if ( strcmp(family, zmw.font_families[i]) == 0 )
      return i ;

  zmw.nb_font_families++ ;

  ZMW_REALLOC(zmw.font_families, zmw.nb_font_families) ;
  zmw.font_families[i] = strdup(family) ;
  return i ;
}  

Zmw_Boolean zmw_font_description_equals(const Zmw_Font_Description *f1
					, const Zmw_Font_Description *f2)
{
  return f1->size == f2->size
    && f1->weight == f2->weight
    && f1->style == f2->style
    && f1->family == f2->family ;
}
