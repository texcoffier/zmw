/*
    ZMW: A Zero Memory Widget Library
    Copyright (C) 2002-2004 Thierry EXCOFFIER, Université Claude Bernard, LIRIS

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
#include "zmw/zmw_private.h" /* This include is only here for speed up */

int zmw_check_button_bits(int value, int bits, int options)
{
  ZMW(zmw_decorator(Zmw_Decorator_Interior
		    | Zmw_Decorator_Border_Relief
		    | options
		    | Zmw_Decorator_Pushable
		    | Zmw_Decorator_Activable
		    | Zmw_Decorator_Activable_By_Key
		    | ( (value & bits) ? Zmw_Decorator_Border_In : 0 ) 
		    )
      )
    {
    }
  if ( !zmw_sensitive_get() && zmw_drawing() )
    zmw_cross_draw() ;
  if ( zmw_activated() )
    {
      value ^= bits ;
      zmw_event_remove() ; // XXX
      zmw_zmw_event_removed_set(Zmw_True) ; // XXX
    }
  return value ;
}

void zmw_check_button_bits_int(int *value, int bits)
{
  *value = zmw_check_button_bits(*value, bits, Zmw_Decorator_Focusable) ;
}

void zmw_check_button_bits_char(char *value, int bits)
{
  *value = zmw_check_button_bits(*value, bits, Zmw_Decorator_Focusable) ;
}

int zmw_check_button(int value)
{
  return zmw_check_button_bits(value, 1, Zmw_Decorator_Focusable) ;
}

void zmw_check_button_int(int *value)
{
  *value = zmw_check_button_bits(*value, 1, Zmw_Decorator_Focusable) ;
}

void zmw_check_button_char(char *value)
{
  *value = zmw_check_button_bits(*value, 1, Zmw_Decorator_Focusable) ;
}

int zmw_check_button_bits_with_label(int value, int bits, const char *label)
{
  Zmw_Boolean a ;


  a = 0 ;
  ZMW( zmw_hbox_activable() )
    {
      zmw_horizontal_expand(Zmw_False) ;
      zmw_vertical_expand(Zmw_False) ;

      value = zmw_check_button_bits(value, bits, 0) ;
      a = zmw_activated() ;
      zmw_label(label) ;
    }
  if ( zmw_activated() )
    {
      value ^= bits ;
      zmw_window_unpop_all() ;
    }
  if ( a )
    {
      zmw_activated_set(Zmw_True) ;
      zmw_window_unpop_all() ;
    }

  return value ;
}

void zmw_check_button_bits_int_with_label(int *value, int bits, const char *label)
{
  *value = zmw_check_button_bits_with_label(*value, bits, label) ;
}

void zmw_check_button_bits_char_with_label(char *value, int bits, const char *label)
{
  *value = zmw_check_button_bits_with_label(*value, bits, label) ;
}

int zmw_check_button_with_label(int value, const char *label)
{
  return zmw_check_button_bits_with_label(value, 1, label) ;
}

void zmw_check_button_int_with_label(int *value, const char *label)
{
  *value = zmw_check_button_bits_with_label(*value, 1, label) ;
}

void zmw_check_button_char_with_label(char *value, const char *label)
{
  *value = zmw_check_button_bits_with_label(*value, 1, label) ;
}


void zmw_radio_with_option(int *value, int number, int option)
{
  int v ;

  v = zmw_check_button_bits(*value == number, 1, option) ;
  if ( v )
    *value = number ;
}

void zmw_radio_button(int *value, int number)
{
  zmw_radio_with_option(value, number, Zmw_Decorator_Focusable) ;
}


void zmw_radio_button_with_label(int *value, int number, const char *label)
{
  ZMW( zmw_hbox_activable() )
    {
      zmw_horizontal_expand(0) ;
      zmw_vertical_expand(0) ;

      zmw_radio_with_option(value, number, 0) ;
      zmw_label(label) ;
    }
  if ( zmw_activated() )
    {
      *value = number ;
      zmw_window_unpop_all() ;
    }
}
