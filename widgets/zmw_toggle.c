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

#include "zmw/zmw.h"

void zmw_toggle_bits(int *value, int bits)
{
  ZMW(zmw_decorator(Zmw_Decorator_Interior
		    | Zmw_Decorator_Border_Relief
		    | Zmw_Decorator_Focusable
		    | Zmw_Decorator_Pushable
		    | Zmw_Decorator_Activable
		    | Zmw_Decorator_Activable_By_Key
		    | Zmw_Decorator_Unpop_On_Activate
		    | ( (*value & bits) ? Zmw_Decorator_Border_In : 0 ) 
		    )
      )
    {
    }
  if ( !ZMW_SENSIBLE && zmw_drawing() )
    zmw_cross_draw() ;
  if ( zmw_activated() )
    {
      *value ^= bits ;
    }
}

void zmw_toggle(int *value)
{
  zmw_toggle_bits(value, 1) ;
}

void zmw_toggle_bits_with_label(int *value, int bits, const char *label)
{
  Zmw_Boolean a ;

  ZMW( zmw_box_horizontal_activable() )
    {
      zmw_horizontal_expand(0) ;
      zmw_vertical_expand(0) ;

      zmw_toggle_bits(value, bits) ;
      a = zmw_activated() ;

      zmw_text(label) ;
    }
  if ( zmw_activated() )
    {
      *value ^= bits ;
    }
  if ( a )
    zmw.activated = Zmw_True ;

}

void zmw_toggle_with_label(int *value, const char *label)
{
  zmw_toggle_bits_with_label(value, 1, label) ;
}


void zmw_radio(int *value, int number)
{
  int v ;

  v = ( *value == number ) ; 
  zmw_toggle( &v ) ;
  if ( v )
    *value = number ;
}

void zmw_radio_with_label(int *value, int number, const char *label)
{
  ZMW( zmw_box_horizontal_activable() )
    {
      zmw_horizontal_expand(0) ;
      zmw_vertical_expand(0) ;

      zmw_radio(value, number) ;
      zmw_text(label) ;
    }
  if ( zmw_activated() )
    {
      *value = number ;
    }
}
