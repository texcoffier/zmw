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
#include "test_trace.c"
#include "many_widgets.c"
#include "notebook.c"

static int what ;

void main_sb()
{
  if ( zmw_action_get() == zmw_action_draw )
  	{
	    *zmw_debug_get_ptr() |= Zmw_Debug_Trace ;
  	}

  switch(what)
    {
      case 0:
	test_trace("Trace") ;
	break ;
      case 1:
	many_widgets("Trace_many") ;
	break ;
      case 2:
	test_notebook("Notebook") ;
	break ;
      case 3:
	test_trace_2("Trace") ;
	break ;
    }

  if ( zmw_debug_get() )
    exit(0) ;
}

int main(int argc, char **argv)
{
  what = atoi(argv[1]) ;
  zmw_init(&argc, &argv) ;
  zmw_main(main_sb) ;
  return(0) ;
}
