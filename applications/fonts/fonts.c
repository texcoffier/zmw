/*
    ZMW: A Zero Memory Widget Library
    Copyright (C) 2018 Thierry EXCOFFIER, Université Claude Bernard, LIRIS

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

    Contact: Thierry.EXCOFFIER@univ-lyon1.fr
*/

#include "zmw/zmw.h"

char **families ;

void fonts()
{
    static Zmw_Float_0_1 x=0, y=0 ;
    ZMW(zmw_window("fonts"))
    {
        zmw_height(1000) ;
        zmw_width(450) ;
        ZMW(zmw_viewport_with_scrollbar(&x, &y))
            ZMW(zmw_vbox())
                {
                char **f ;
                for(f=families; *f; f++)
                    {
                        zmw_font_family(*f) ;
                        ZMW(zmw_hbox())
                        {
                            zmw_label("ABCD") ;
                            zmw_label(zmw_rectangle_string(zmw_min_get())) ;
                            zmw_label(*f) ;
                        }
                        
                    }
                }
    }
}

int main(int argc, char **argv)
{
  zmw_init(&argc, &argv) ;
  families = zmw_text_family_list() ;
  zmw_main(fonts) ;
  return(0) ;
}
