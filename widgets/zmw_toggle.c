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

#include "zmw.h"


void zmw_toggle_(int *value, int bits)
{
  switch( ZMW_SUBACTION )
    {
    case Zmw_Compute_Required_Size:
      ZMW_SIZE_MIN.width = 16 ;
      ZMW_SIZE_MIN.height = 16 ;
      break ;

    case Zmw_Post_Drawing:
      zmw_border_draw(Zmw_Border_Relief
			 | (zmw_focused() ? Zmw_Border_Draw_Focus : 0)
			 | Zmw_Border_Focusable
			 | Zmw_Border_Background
			 | ((((*value&bits)==bits) ^ zmw_selected()) ? Zmw_Border_In : 0)
			 ) ;
      if ( !ZMW_SIZE_SENSIBLE )
	zmw_cross_draw() ;
      break ;
    case Zmw_Input_Event:

      zmw_focusable() ;
      zmw_activable() ;

      if ( zmw_key_string() )
	{
	  zmw.activated = 1 ;
	  zmw_event_remove() ;
	}
  
      break ;
    default:
      break ;
    }
}

void zmw_toggle_bits(int *value, int bits)
{
  ZMW( zmw_toggle_(value, bits) ) { }
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
  ZMW( zmw_box_horizontal() )
    {
      zmw_horizontal_expand(0) ;
      zmw_vertical_expand(0) ;

      zmw_toggle_bits(value, bits) ;

      zmw_text_activable(label) ;
      if ( zmw_activated() )
	{
	  *value ^= bits ;
	}
    }
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
  ZMW( zmw_box_horizontal() )
    {
      zmw_horizontal_expand(0) ;
      zmw_vertical_expand(0) ;

      zmw_radio(value, number) ;

      zmw_text_activable(label) ;
      if ( zmw_activated() )
	{
	  *value = number ;
	}
    }
}


