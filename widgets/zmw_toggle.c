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

int zmw_toggle_bits(int value, int bits)
{
  ZMW(zmw_decorator(Zmw_Decorator_Interior
		    | Zmw_Decorator_Border_Relief
		    | Zmw_Decorator_Focusable
		    | Zmw_Decorator_Pushable
		    | Zmw_Decorator_Activable
		    | Zmw_Decorator_Activable_By_Key
		    | ( (value & bits) ? Zmw_Decorator_Border_In : 0 ) 
		    )
      )
    {
    }
  if ( !ZMW_SENSIBLE && zmw_drawing() )
    zmw_cross_draw() ;
  if ( zmw_activated() )
    {
      value ^= bits ;
    }
  return value ;
}

void zmw_toggle_bits_int(int *value, int bits)
{
  *value = zmw_toggle_bits(*value, bits) ;
}

void zmw_toggle_bits_char(char *value, int bits)
{
  *value = zmw_toggle_bits(*value, bits) ;
}

int zmw_toggle(int value)
{
  return zmw_toggle_bits(value, 1) ;
}

void zmw_toggle_int(int *value)
{
  *value = zmw_toggle_bits(*value, 1) ;
}

void zmw_toggle_char(char *value)
{
  *value = zmw_toggle_bits(*value, 1) ;
}

int zmw_toggle_bits_with_label(int value, int bits, const char *label)
{
  Zmw_Boolean a ;

  a = 0 ;
  ZMW( zmw_box_horizontal_activable() )
    {
      zmw_horizontal_expand(0) ;
      zmw_vertical_expand(0) ;

      value = zmw_toggle_bits(value, bits) ;
      a = zmw_activated() ;

      zmw_text(label) ;
    }
  if ( zmw_activated() )
    {
      value ^= bits ;
      zmw_window_unpop_all() ;
    }
  if ( a )
    zmw.activated = Zmw_True ;

  return value ;
}

void zmw_toggle_bits_int_with_label(int *value, int bits, const char *label)
{
  *value = zmw_toggle_bits_with_label(*value, bits, label) ;
}

void zmw_toggle_bits_char_with_label(char *value, int bits, const char *label)
{
  *value = zmw_toggle_bits_with_label(*value, bits, label) ;
}

int zmw_toggle_with_label(int value, const char *label)
{
  return zmw_toggle_bits_with_label(value, 1, label) ;
}

void zmw_toggle_int_with_label(int *value, const char *label)
{
  *value = zmw_toggle_bits_with_label(*value, 1, label) ;
}

void zmw_toggle_char_with_label(char *value, const char *label)
{
  *value = zmw_toggle_bits_with_label(*value, 1, label) ;
}


void zmw_radio(int *value, int number)
{
  int v ;

  v = ( *value == number ) ; 
  zmw_toggle_int( &v ) ;
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
      zmw_window_unpop_all() ;
    }
}
