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
#include <search.h>

static int file_opened(const char *name)
{
  static int f = 0 ;
  ENTRY item, *answer ;

  if ( f == 0 )
    {
      hcreate(1000000) ;
      f = 1 ;
    }
  item.key = (char*)name ;
  answer = hsearch(item, FIND) ;
  return( answer ? (int)answer->data : 0 ) ;
}

static void files_rec(const char *name)
{
  int opened, old_opened ;
  ENTRY item ;

  opened = file_opened(name) ;
  old_opened = opened ;
  zmw_toggle_with_label(&opened, name) ;
  if ( opened != old_opened )
    {
      item.key = (char*)name ;
      item.data = opened ;
      hsearch(item, ENTER) ;
    }
  if ( opened )
    {
    }
}

void demo_file_tree()
{
  static GdkWindow *w = NULL ;

  zmw_name("File manager") ;
  ZMW(zmw_compute_window_size)
    {
      zmw_window(&w) ;
      zmw_name("Stack") ;
      ZMW(zmw_box_vertical)
	{
	  files_rec("/") ;
	}
    }
}
