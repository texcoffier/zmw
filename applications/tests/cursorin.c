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

/* Beware, cursor_enter/leave are not safe.
 * The program state must not be changed when testing these values.
 * So, temporary variables are used and their values
 * are modified when a state change is allowed.
 *
 * It was possible to make safe functions, but it will take much time
 * because it needs a tree traversal for each enter/leave.
 *
 * And this example is stupid because we have direct access
 * to in2_ in3_ by testing ZMW_SIZE_EVENT_IN_RECTANGLE after the widget.
 */
void test_cursorin(const char *title)
{
  static int in1=0, in2=0, in3=0 ;
  static int in2_=0, in3_=0 ;
  char buf[999] ;
	
  ZMW(zmw_window(title))
    {
      ZMW(zmw_vbox())
	{
          sprintf(buf, "In the box: %d, in text A: %d, in text B: %d",
          		in1, in2, in3) ;
          zmw_label(buf) ;

	  if ( in2_ )
	    zmw_label("Text A in") ;
	  else
	    zmw_label("Text A") ;
	  if ( zmw_cursor_enter() )
	  	in2 = 1 ;
	  if ( zmw_cursor_leave() )
	  	in2 = 0 ;

	  if ( in3_ )
	    zmw_label("Text B in") ;
	  else
	    zmw_label("Text B") ;
	  if ( zmw_cursor_leave() )
	  	in3 = 0 ;
	  if ( zmw_cursor_enter() )
	  	in3 = 1 ;
	}
      if ( zmw_cursor_enter() )
	  	in1 = 1 ;
       if ( zmw_cursor_leave() )
	  	in1 = 0 ;
    }

  if (  zmw_state_change_allowed() )
    {
      in2_ = in2 ;
      in3_ = in3 ;
    }
}
